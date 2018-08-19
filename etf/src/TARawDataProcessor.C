///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TARawDataProcessor.C												 //
//   TARawDataProcessor.C -- source file for class TARawDataProcessor				 //
//   Introduction: transform raw binary datafile from PXI crates to rootfile for	 //
// further analysis. The result is a ROOT tree containing channel id and edge timing //
// results, including edge type and number of edges. This is a singleton class.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2018/5/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <unistd.h> // access
#include <cstdlib> // rand
#include <fstream> // fread
#include <iostream>
#include <libgen.h> // basename function
#include <climits>
#include <ctime>

#include "TFile.h"
#include "TTree.h"
#include "readVME.h"

#include "TARawDataProcessor.h"
#include "TACtrlPara.h"
#include "tEntry.h"
#include "TAPopMsg.h"

using std::cout;
using std::endl;
using std::flush;

//#define DEBUG
//#define DEBUG_VME

TARawDataProcessor* TARawDataProcessor::fInstance = nullptr;

TARawDataProcessor::TARawDataProcessor()
	 : fPXIDataFile(""), fVMEDataFile(""), fROOTFile(""){
	fBunchIdMisAlignCnt = 0;
	fEventCnt = 0; fVMEEventCnt = 0;
	fIsCheckBunchId = TACtrlPara::IsCheckBunchIdAlignment();
	fRunId = 0;
	fIndex0 = 0;
	fIndex1 = INT_MAX;
}
TARawDataProcessor *TARawDataProcessor::Instance(){
	if(!fInstance) fInstance = new TARawDataProcessor();
	return fInstance;
}
TARawDataProcessor::~TARawDataProcessor(){}

void TARawDataProcessor::SetDataFileName(const string &name, int runId, bool isPXI){
	// retrieve the file name from a path+name string
	fRunId = runId;
	char tmp[64]; strcpy(tmp, name.c_str());
	sprintf(tmp, "%s_%d", basename(tmp), fRunId);
	if(!strcmp(tmp, ""))
		TAPopMsg::Error("TARawDataProcessor", "SetDataFileName: Input data file is empty");
	if(0 != access(name.c_str(), F_OK))
		TAPopMsg::Error("TARawDataProcessor", "SetDataFileName: %s doesn't exist.\n\tFor data files in the current directory, please add './' in front of the data files' name;\n\tFor data files in ../data/, relative or absolute path to the files is not necessary", name.c_str());
	if(isPXI){
		fPXIDataFile = name;
		fROOTFile = tmp; fROOTFile += ".root";
	}
	else{
		fVMEDataFile = name;
		fVMEROOTFile = tmp; fVMEROOTFile += ".root";
		// fROOTFile stands for VME and/or PXI ROOTFile for users
		if(!strcmp(fROOTFile.c_str(), "")) fROOTFile = fVMEROOTFile;
	}
//	TAPopMsg::Debug("TARawDataProcessor", "SetDataFileName: fROOTFile: %s", fROOTFile.c_str());
}
const char *TARawDataProcessor::GetDataFileName() const{
	if(strcmp(fPXIDataFile.c_str(), "")) return fPXIDataFile.c_str();
	if(strcmp(fVMEDataFile.c_str(), "")) return fVMEDataFile.c_str();
	return "";
}

void TARawDataProcessor::SetPeriod(int index0, int index1){
	if(fIndex0 >= fIndex1)
		TAPopMsg::Error("TARawDataProcessor", "SetPeriod: index0 %d is not smaller than index1 %d", index0, index1);
	fIndex0 = index0; fIndex1 = index1;
}

//////---------------------------- READ OFFLINE-----------------------------------------------------//
inline double rand0_5(){
//	return 0.;
	return rand()*1./RAND_MAX;
} // bin smoothing, import from BUAA code.

// read offline binary data file and store them in a tree and a rootfile.
int TARawDataProcessor::ReadOffline(){
	int staPXI = ReadOfflinePXI(); // readPXI have to be at first to generate a rootfile
	int staVME = ReadOfflineVME(); // so that VME treeData would be stored in the same rootfile
	int sta = 0;
	if(staPXI != 0) sta = staPXI;
	if(staVME != 0) sta = staVME;
	return sta;
}

int TARawDataProcessor::ReadOfflinePXI(){
	if(0 != access(fPXIDataFile.c_str(), F_OK)) return -1; // PXI data file doesn't exist
	if(0 == access(fROOTFile.c_str(), F_OK)){
		TFile *f = new TFile(fROOTFile.c_str());
		TTree *treeData = (TTree*)f->Get("treeData");
		if(treeData){
			TAPopMsg::Info("TARawDataProcessor", "ReadOfflinePXI: %s exist and treeData not nullptr. So this function has been called once. BINGER would use the current ROOTFile and treeData. RawData => ROOT file transformation would be skipped", fROOTFile.c_str());
			
			// retrieve the index of the last event, and assign which to fEventCnt
			int index_t;
			treeData->SetBranchAddress("index", &index_t);
			treeData->GetEntry(treeData->GetEntries()-2); // the last but one entry
			fEventCnt = index_t;
			f->Close(); delete f; f = nullptr;
			return -1; // file already exists
		} // end if treeData is not nullptr
	} // end if fROOTFile exists

	static const double H_BLIP = 25. / pow(2., 8.);
	static const double V_BLIP = 25. / pow(2., 10.);

	static const int edge_num_limit = tEntry::NMAX;
	static const int ch_num_limit_per_frag = 128;
	static const int frag_num_limit_per_event = 100; // used for bunchID checking


	/* struct varibles definition in the raw binary data file */
	struct section_head{
		int index;
		int map_index;
		int data_type:8;
		int fragment_num:24;
	} sec_h;

	struct fragment_head{
		int start_channel;
		int channel_num:30;
		unsigned int channel_full:1;
		unsigned int length_change:1;
		int data_len;
	} *frag_h;


	// section head
	int index; // event index
	int section_len;
	int frag_nu; // fired fragment number in an event
	bool Is_vacant = false; // a symbol indicating if an event is a vacant event
	bool Is_Error = false; // a symbol indicating if the last channel of a fragment, which is supposed to store a group header, is longer than a sizeof(int)
	bool Is_BunchidMisAlign = false; // as the name indicates

	// fragment head
	int start_ch[frag_num_limit_per_event];
	int ch_num[frag_num_limit_per_event];
	int data_len = -1; // real data length, including edges only
	for(int i = 0; i < frag_num_limit_per_event; i++){
		start_ch[i] = -1;
		ch_num[i] = -1;
	} // end for over i


	// data_validity_table
	int va_ch_nu;
	int *ch_id = NULL; // channel id of fired channel(s), to store data_validity_table


	// channel_offset_table
	int *edge_num = NULL;	
	int *offset = NULL; // arrray to store data_offset_table
	bool offset_ed = false; // indicating whether pointer offset has been allocated.

	// edge data
	int *data_ch = NULL; // array to store edge information in a channel
	int *data_ch_last = NULL; // array to store edge information of the last channel in a fragment
	int hl[ch_num_limit_per_frag][edge_num_limit]{}, nhl = 0;
	int ht[ch_num_limit_per_frag][edge_num_limit]{}, nht = 0;
	int vl[ch_num_limit_per_frag][edge_num_limit]{}, nvl = 0;
	int vt[ch_num_limit_per_frag][edge_num_limit]{}, nvt = 0;

	short length_last_ch; // length of the last channel of a fired fragment.
	int bunchID[frag_num_limit_per_event]{};
	int eventID[frag_num_limit_per_event]{};
	
	tEntry entry_temp;
	TFile *f = new TFile(fROOTFile.c_str(), "RECREATE");
	TTree *treeData = new TTree("treeData", ("experiment data-"+fPXIDataFile).c_str());
	treeData->Branch("index", &entry_temp.index, "index/I"); // run id
	treeData->Branch("channelId", &entry_temp.channelId, "channelId/I");
	treeData->Branch("nl", &entry_temp.nl, "nl/I");
	treeData->Branch("nt", &entry_temp.nt, "nt/I");
	treeData->Branch("leadingTime", entry_temp.leadingTime, "leadingTime[nl]/D");
	treeData->Branch("trailingTime", entry_temp.trailingTime, "trailingTime[nt]/D");
	treeData->Branch("is_V", &entry_temp.is_V, "is_V/O");
	treeData->Branch("bunchId", &entry_temp.bunchId, "bunchId/I");

	// open the original binary data file
	FILE *fp;
	if(!(fp = fopen(fPXIDataFile.c_str(),"rb"))){
		cout << "File " << fPXIDataFile << " does not exist." << endl;
		exit(1);
	}
	cout << "\n\nData file " << fPXIDataFile << " opened. " << endl;
	fBunchIdMisAlignCnt = 0;
	fEventCnt = 0;
	while(1){
		section_len = -1;
		Is_vacant = false; Is_Error = false; Is_BunchidMisAlign = false;

//		srand(time(0)); // set random seed
		// read section head
		if(fread(&sec_h, sizeof(section_head), 1, fp) <= 0) break; // read secion_head
		index = sec_h.index; // read section index
		if(index >= fIndex1) break;

		frag_nu = sec_h.fragment_num; // read fired fragment number
		section_len = sizeof(section_head); // increment event length by size of section head
		if(index >= fIndex0) fEventCnt++;
		// Assign member data struct fAskMe
//		fAskMe.index = index; fAskMe.frag_nu = frag_nu; /// Nov. 7, 2016


		// read fragment head(s)
		// allocate proper memory to pointer frag_h with data type fragment_head
		if(frag_nu > 0){
			frag_h = new fragment_head[frag_nu]; 
			if(fread(frag_h, sizeof(fragment_head), frag_nu, fp) <= 0) break; // read all fragment heads at one time
			section_len += frag_nu * sizeof(fragment_head); // increment event length by the length read just now
			// To see if the event is a vacant event, which only has a section head and  a fragment head.
			if(frag_h[0].data_len == 0){
				cout << "This event has no data! frag_h[0].data_len == 0.";
				cout << " frag_nu: " << frag_nu << "\tindex: " << index << endl;
				Is_vacant = 1;
			}
		} // end if(frag_nu > 0)
		else{
			cout << "This event has no data!" << "\tindex: " << index << endl;
			Is_vacant = 1;
		}

////????????$$$$$$$$$$$$$$$ READ DATA ZONE OF THE SECTION WITH CERTAIN BUNCHID $$$$$$$$$$$$$$$$$$$$$$$$$$////////////////////////////////////////////
		if(frag_nu >= frag_num_limit_per_event){
			cout << "\033[31;1mfragment number limit out of range!\0333[0m\n";
			exit(1);
		}
		for(int i = 0; i < frag_nu; i++) if(!Is_vacant){ // circle over fired fragments
			offset_ed = false; // initialize offset_ed.
			data_len = frag_h[i].data_len; // data_len(inluding lengths of possible tables)
			section_len += data_len; // increment event length by the length of fragment data (including table(s) if there are or is any)

			// read data validity table //
//			fAskMe.fragment[i].index = i; /// Nov. 7, 2016
			if(frag_h[i].channel_full == 1){ // all the channels in the fragment are fired.
				va_ch_nu = frag_h[i].channel_num; // valid channel number is the channel number the fragment has.
//				fAskMe.ch_nu += va_ch_nu - 1; /// Nov. 7, 2016
//				fAskMe.fragment[i].va_ch_nu = va_ch_nu - 1; /// Nov. 7, 2016
				ch_id = new int[va_ch_nu];
				for(int j = 0; j < va_ch_nu; j++) ch_id[j] = frag_h[i].start_channel + j; // channel id
			} // end if(frag_h[i].channel_full == 1)
			else { // not all the channels in the fragment are fired, so there is a data_validity_table
				if(fread(&va_ch_nu, sizeof(int), 1, fp) <= 0) break; // read valid channel number				
//				fAskMe.ch_nu += va_ch_nu - 1; /// Nov. 7, 2016
//				fAskMe.fragment[i].va_ch_nu = va_ch_nu - 1;  /// Nov. 7, 2016
				ch_id = new int[va_ch_nu];
				if(fread(ch_id, sizeof(int), va_ch_nu, fp) <= 0) break; // read channel id of every fired channels
				data_len -= (va_ch_nu + 1) * sizeof(int); // data_len represents the length of edge information in a fragment, so the length of any table is subtracted
			} // end else
////////////////////////////////// read channel_offset_table //////////////////////////////////////////////////////////////////////////////////////////

			// extract data_len
			if(frag_h[i].length_change == 1) // channel_offset_table exists
			{
				offset_ed = true;
				data_len -= va_ch_nu * sizeof(int); // data_len represents the length of edge information in a fragment, so the length of any table is subtracted
			}

			// start to read channel_offset_table
			edge_num = new int[va_ch_nu];
			if(!offset_ed){ // no channel_offset_table
				for(int j = 0; j < va_ch_nu; j++){
					edge_num[j] = data_len / (va_ch_nu * 4); // unit: words. One word is four bytes, 32 bits
				}
			}
			else { // A channel_offset_table exists
				// allocate proper memory to pointer offset with data type int
				offset = new int[va_ch_nu];
				if(fread(offset, sizeof(int), va_ch_nu, fp) <= 0) break; // read offset table at a time
				for(int j = 0; j < va_ch_nu - 1; j++) edge_num[j] = (offset[j + 1] - offset[j]) / 4; // obtain edge number of each fired channel in the fragment. The last word of the data of the fragment is a group header, which is not processed here.
			} // end else
			

////////////////////////////////// processing one channel //////////////////////////////////////////////////////////////////////////////////////////////
			// within the circle of fired fragment(s)
			for(int j = 0; j < va_ch_nu - 1; j++)if(edge_num[j] > 0){ // circle over fired channel(s) except the last one which is explained later per fragment.
				// allocate proper memory to pointer data_ch with data type int
				data_ch = new int[edge_num[j]];
				if(fread(data_ch, sizeof(int), edge_num[j], fp) <= 0) break; // read edge information of a channel at a time

				// circle over edge(s) of a certain channel in a certain fragment for a certain event
				nhl = 0; nht = 0; nvl = 0; nvt = 0;
				for(int k = 0; k < edge_num[j]; k++) if(k <= edge_num_limit){ // edge number greater than edge_num_limit are futile and ommited
					if( (data_ch[k]>>31 & 0x1) == 0){ // high
						if( (data_ch[k]>>30 & 0x1) == 0){
							if(frag_h[i].channel_num == 33){ // TOF
								hl[j][nhl] = (data_ch[k] & 0x7FFFF); nhl++;
							}
							else{ // MWDC
								ht[j][nht] = (data_ch[k] & 0x7FFFF); nht++;
							}
						} // end if
						else{
							if(frag_h[i].channel_num == 33){ // TOF
								ht[j][nht] = (data_ch[k] & 0x7FFFF); nht++;
							}
							else{ // MWDC
								hl[j][nhl] = (data_ch[k] & 0x7FFFF); nhl++;
							}
						}
					}
					else{ // very high
						if(0 == (data_ch[k]>>30 & 0x1) ){
							vl[j][nvl] = (data_ch[k] & 0x1FFFFF); // (data_ch[k] & 0x7FFFF) * 4 + (data_ch[k]>>19 & 0x3); // 
							nvl++;
						}
						else{
							vt[j][nvt] = (data_ch[k] & 0x1FFFFF); // (data_ch[k] & 0x7FFFF) * 4 + (data_ch[k]>>19 & 0x3); // 
							nvt++;
						}
					}
				} // end for over edges
				if(nhl >= 1 && nht >= 0){
					for(int k = 0; k < nhl && k < edge_num_limit; k++)
						entry_temp.leadingTime[k] =
							(hl[j][k] +  rand0_5()) * H_BLIP;
					for(int k = 0; k < nht && k < edge_num_limit; k++)
						entry_temp.trailingTime[k] =
							(ht[j][k] +  rand0_5()) * H_BLIP;
					entry_temp.is_V = false; entry_temp.channelId = ch_id[j];
					if(nhl > edge_num_limit || nht > edge_num_limit){
//						TAPopMsg::Warn("TARawDataProcessor", "ReadOfflinePXI: HPTDC edge number is greater than %d: nhl: %d, nht: %d", edge_num_limit, nhl, nht);
					}
					entry_temp.nl = nhl < edge_num_limit ? nhl : edge_num_limit;
					entry_temp.nt = nht < edge_num_limit ? nht : edge_num_limit;
					entry_temp.index = index;
					entry_temp.bunchId = -1;
					if(index >= fIndex0) treeData->Fill();
				} // end if(nhl >= 1 && nht >= 1)
				else if(nvl >= 1 && nvt >= 0){
					for(int k = 0; k < nvl && k < edge_num_limit; k++)
						entry_temp.leadingTime[k] =
							(vl[j][k] +  rand0_5()) * V_BLIP;
					for(int k = 0; k < nvt && k < edge_num_limit; k++)
						entry_temp.trailingTime[k] =
							(vt[j][k] +  rand0_5()) * V_BLIP;
					entry_temp.is_V = true; entry_temp.channelId = ch_id[j];
					entry_temp.nl = nvl; entry_temp.nt = nvt;
					entry_temp.index = index;
					entry_temp.bunchId = -1;
					if(index >= fIndex0) treeData->Fill();
				} // end if(nvl >= 1 && nvt >= 1)

				// initialize edge arrays
				if(nvl > 0) for(int k = 0; k < nvl; k++) vl[j][k] = 0;
				if(nvt > 0) for(int k = 0; k < nvt; k++) vt[j][k] = 0;
				if(nhl > 0) for(int k = 0; k < nhl; k++) hl[j][k] = 0;
				if(nht > 0) for(int k = 0; k < nht; k++) ht[j][k] = 0;

				delete [] data_ch;
			} // circle over fired channel(s) except the last one which is explained below.
////////////////////////////////// process the last channel ///////////////////////////////////////////////////
			// The last channel of every fired fragment only stores a group header defined by HPTDC designed by CERN, containing trigger and event index for the fragment.
			length_last_ch = (data_len - offset[va_ch_nu - 1]) / 4; // words
			if(length_last_ch > 0){
				data_ch_last = new int[length_last_ch];
				if(fread(data_ch_last, sizeof(int), length_last_ch, fp) <= 0) break;
				bunchID[i] = data_ch_last[0] & 0xFFF;
				if(bunchID[i] >= 4096){ // 4096 = 0xFFF
					cout << "bunchId too large!\n";
					cout << "bunchID[i]: " << bunchID[i] << endl;
					getchar();
				}
				eventID[i] = (data_ch_last[0] >> 12) & 0xFFF;
			} // end if(length_last_ch > 0)
			else{

				cout << "data_len: " << data_len << endl;
				cout << "offset[va_ch_nu - 1]: " << offset[va_ch_nu - 1] << endl;
				cout << "va_ch_nu: " << va_ch_nu << endl; getchar();


				cout << "length_last_ch: " << length_last_ch << endl;
				cout << "eventID and bunchID would not be assigned this time." << endl;
				getchar();
			}
			if(length_last_ch != 1){
				Is_Error = true;
				if(Is_Error) TAPopMsg::Info("TARawDataProcessor", "ReadOfflinePXI: length_last_ch is not 1 word... index: %d, frag_num: %d, length_last_ch: %d", index, i, length_last_ch);
//				cout << "event " << index << " fragment " << i << ":" << endl;
//				cout << "length_last_ch(words): " << (data_len - offset[va_ch_nu - 1]) / 4. << endl;
//				cout << "va_ch_nu: " << va_ch_nu << endl;
//				cout << "frag_h[" << i << "].channel_full: " << frag_h[i].channel_full << endl;
			} // end if

			start_ch[i] = frag_h[i].start_channel;
			ch_num[i] = frag_h[i].channel_num; // channel count in a fragment, 33 or 129
//			fAskMe.fragment[i].channel_num = frag_h[i].channel_num; /// Nov. 7, 2016
//			fAskMe.fragment[i].start_channel = frag_h[i].start_channel; /// Nov. 7, 2016
			if(!(ch_num[i] == 33 || ch_num[i] == 129)){
				cout << "Odd... Channel number of one fragment doesn't equal 33 or 129.";
				cout << " ch_num[" << i << "]: " << ch_num[i] << endl;
				getchar(); // DEBUG 
			} // end if(!(ch...)


//			cout << "start to delete..." << endl; getchar();
			delete [] ch_id;
			delete [] edge_num;
			if(offset_ed) delete [] offset;
			delete [] data_ch_last;
//			#include "read/initializePostFragment.C"
		} // end for over fragments
		section_len /= 4;

		// check consistency of bunchIDs of all fragments in one data section.
		for(int ii = 1; ii < frag_nu; ii++){ // print the bunchID and eventID of any fragment whose IDs don't agree with the first fragment's in an event
//			cout << "bunchID[" << ii << "]: " << bunchID[ii] << endl; // DEBUG
//			cout << "eventID[" << ii << "]: " << eventID[ii] << endl; // DEBUG
//			getchar(); // DEBUG
			if(bunchID[0] != bunchID[ii] || eventID[0] != eventID[ii])
			{
				if(index >= fIndex0){
					fBunchIdMisAlignCnt++;
					Is_BunchidMisAlign = true;
				}
				if(this->fIsCheckBunchId){
					cout << "\nEvent " << index << "_______________" << endl;
					for(int i = 0; i < frag_nu; i++){
						cout << "bunchID[" << i << "]: " << bunchID[i];
						cout << "\tstart_ch[" << i << "]: " << start_ch[i] << endl;
					}
					cout << "-----------------" << endl;
					for(int i = 0; i < frag_nu; i++)
						cout << "eventID[" << i << "]: " << eventID[i] << endl;
					getchar();
					break;
				} // end if
			} // end if(bunchID[0] != bunchID[ii] || eventID[0] != eventID[ii])
		} // end for over ii
		// circle over events
		// mark the end of one data section.
		entry_temp.index = -2; // default is -1; so don't confuse with it.
		entry_temp.channelId = section_len; // this place is dispatched for new use.	
		entry_temp.nl = 0;
		entry_temp.nt = 0;
		entry_temp.bunchId = bunchID[0];
		if(Is_BunchidMisAlign) entry_temp.channelId = -2; // marking bunchId misalignment
		if(index >= fIndex0) treeData->Fill();

		if(frag_nu > 0) delete [] frag_h;

		cout << "Processing event index  " << index << "\r" << flush;
	} // end while
	// ensure a normal ending at the end of a data file //
	if(entry_temp.nl != 0){
		entry_temp.index = -2; // default is -1; so don't confuse with it.
		entry_temp.channelId = section_len; // this place is dispatched for new use.	
		entry_temp.nl = 0;
		entry_temp.nt = 0;
		entry_temp.bunchId = bunchID[0];
		if(Is_BunchidMisAlign) entry_temp.channelId = -2; // marking bunchId misalignment
		if(index >= fIndex0) treeData->Fill();
	} // end if

	cout << "\n\n>>>>>>>>>>>>> PXI DATA Total Event Count <<<<<<<<<<<<<<<<<<<:\n" << endl;
	cout << "       \033[1m" << fEventCnt << "\033[0m        \n" << endl;
	cout << "\033[36;1mBunch ID misalignment count: " << fBunchIdMisAlignCnt << "\033[0m\n";
	if(fBunchIdMisAlignCnt > 0) TAPopMsg::Info("TARawDataProcessor", "ReadOffLinePXI: BunchId misalignment count happend %d times", fBunchIdMisAlignCnt);

	cout << "\033[33;1m:)\033[0m treeData has been written into " << fROOTFile << ". \033[33;1m:)\033[0m\n\n";


	// write
	treeData->Write("", TObject::kOverwrite);
	fclose(fp);
	f->Close();
	delete f;
//	delete treeData;
//	cout << "mark 1\n"; getchar(); // DEBUG

	return 0;
} // end function ReadOfflinePXI

// read VME data into ROOTFile generated by ReadOfflinePXI()
int TARawDataProcessor::ReadOfflineVME(){
	if(!strcmp("", GetVMEDataFileName())) return -1; // VME data file is empty

	TFile *f = new TFile(fROOTFile.c_str(), "UPDATE");
	TTree *treeData_t = (TTree*)f->Get("treeDataVME");
	if(treeData_t){
		TAPopMsg::Info("TARawDataProcessor", "ReadOfflineVME: treeDataVME not null. The function has been called once.");
		f->Close(); delete f; f = nullptr;
		return -1;
	}

	// high resolution mode
	static const double H_BLIP = 25. / pow(2., 8.);
	static const int edge_num_limit = tEntry::NMAX;
	static const int ch_num_limit_per_frag = 128;

	// high resolution leading edge measurements, number of its edges per channel
	int hl[ch_num_limit_per_frag][edge_num_limit]{}, nhl[ch_num_limit_per_frag]{};
	int ht[ch_num_limit_per_frag][edge_num_limit]{}, nht[ch_num_limit_per_frag]{};
	int channelId[ch_num_limit_per_frag]{};

	// define the VME plugin tree - very raw data
	TTree *vme = new TTree("vme", ("Prestine raw VME data-"+fVMEDataFile).c_str());
	tVME_event evt;
	vme->Branch("adc", evt.adc, "adc[32]/I");
	vme->Branch("qdc", evt.qdc[0], "qdc[32]/I");
	vme->Branch("mtdc0", evt.mtdc0, "mtdc0[128][10]/I");
	vme->Branch("mtdc1", evt.mtdc1, "mtdc1[128][10]/I");
	vme->Branch("sca", evt.sca, "sca[16]/I");
	vme->Branch("dsca", evt.dsca, "dsca[16]/I");

	// define the data tree
	tEntry entry_temp;
	TTree *treeDataVME = new TTree("treeDataVME", ("experiment VME data-"+fVMEDataFile).c_str());
	treeDataVME->Branch("index", &entry_temp.index, "index/I"); // run id
	treeDataVME->Branch("channelId", &entry_temp.channelId, "channelId/I");
	treeDataVME->Branch("nl", &entry_temp.nl, "nl/I");
	treeDataVME->Branch("nt", &entry_temp.nt, "nt/I");
	treeDataVME->Branch("leadingTime", entry_temp.leadingTime, "leadingTime[nl]/D");
	treeDataVME->Branch("trailingTime", entry_temp.trailingTime, "trailingTime[nt]/D");
	treeDataVME->Branch("is_V", &entry_temp.is_V, "is_V/O");
	treeDataVME->Branch("bunchId", &entry_temp.bunchId, "bunchId/I");

	// scaler statistics, in case of event matching with PXI data
	unsigned sca[16]{}, psca[16]{}; // psca: previous value of sca
	int dsca[16]{}, nsca = 0; // scaler, trigger count
	TTree *treeSCA = new TTree("treeSCA", ("SCA INFO-"+fVMEDataFile).c_str());
	treeSCA->Branch("index", &entry_temp.index, "index/I"); // event index
	treeSCA->Branch("sca", sca, "sca[16]/i");
	treeSCA->Branch("dsca", dsca, "dsca[16]/I");
	treeSCA->Branch("psca", psca, "psca[16]/i");

	// open the original binary data file
	FILE *fp;
	if(!(fp = fopen(fVMEDataFile.c_str(),"rb"))){
		cout << "VME File " << fVMEDataFile << " does not exist." << endl;
		exit(1);
	}
	cout << "Data file " << fVMEDataFile << " opened. " << endl;
	fVMEEventCnt = 0;

	// read VME binary data file
	const int blkSize = 8192; // size of one block, in sizeof(int)
	int buffer[blkSize]{}; // cache block data
	int pos = 0; // the current pos of reading pointer in a block
	int event_num = 0; // number of events processed
	int block_num = 0; // number of blocks processed
	// blcok header
#ifdef DEBUG_VME
	int blk_index, blk_last_pos, ev_vmeReadCnt; // block index, last pos of data zone and ? (not clear what this is)
#endif
	int blk_ev_num; // event number
	// event header
	int ev_len, ev_index; // event length, event index

	// dump the first block, which contains no valid data but comment
	if(fread(buffer, sizeof(int), blkSize, fp) <= 0){ // reading failure
		cout << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	} // end if
	else{
#define VME_COMMENT
#ifdef VME_COMMENT
		char comment[sizeof(buffer)] = ""; // store the first block
		memcpy(comment, buffer, sizeof(buffer));
		cout << "\n\nVME datafile comment:\n\033[36;1m";
		for(char c : comment) if(c) cout << c;
		cout << "\n\n\033[0m";
//		sleep(5);
//		getchar();
#endif
		memset(buffer, 0, sizeof(buffer)); // abandon the first block
	}
	block_num++;
	
	int index, index0 = -1; // index0: to make the first index to be zero
	while(fread(buffer, sizeof(int), blkSize, fp) > 0){
		// block header - the first 3 words
		blk_ev_num = buffer[1];
		pos = 3;
#ifdef DEBUG_VME
		blk_index = buffer[0];
		blk_last_pos = buffer[2];
		cout << "---------- Block Header -------------\n"; // DEBUG
		cout << "block index: " << blk_index; // DEBUG
		cout << ", block event number: " << blk_ev_num; // DEBUG
		cout << ", block_last_pos: " << blk_last_pos << endl; // DEBUG
		getchar(); // DEBUG
#endif

		// loop over events in the preset block
		for(int i = 0; i < blk_ev_num; i++){
			// event header
			ev_len = buffer[pos];
			ev_index = buffer[pos+1];
			entry_temp.is_V = false;
			if(-1 == index0) index0 = ev_index;
			entry_temp.index = ev_index - index0;
			index = entry_temp.index;
			if(index >= fIndex1) break;
//			cout << "ev_index: " << ev_index << "\tindex0: " << index0 << endl; // DEBUG
//			cout << "entry_temp.index: " << entry_temp.index << endl; getchar(); // DEBUG
			entry_temp.bunchId = 0;
#ifdef DEBUG_VME
			ev_vmeReadCnt = buffer[pos+2];
			cout << "---------- Event Header -------------\n"; // DEBUG
			cout << "event index: " << ev_index; // DEBUG
			cout << ", event length: " << ev_len; // DEBUG
			cout << ", ev_vmeReadCnt: " << ev_vmeReadCnt << endl; // DEBUG
			getchar(); // DEBUG
#endif

			// // // read the data zone of an event // // //
			int id_v1190 = 0, id_v830 = 0; // the No. of v1190 and v830 plugin
			double pileUp = -9999.; // to copy this information from ch-31 to ch-30
			evt.initialize(); // initialize evt structure
			for(int j = 3; j < ev_len; j++){ // loop over channels in an event

				int slot = -1, header = -1; // to identify a channel
				int chData = buffer[pos+j]; // channel data
				slot = (chData>>27) & 0x1F;

				if((5 == slot || 17 == slot) && !id_v830 && !id_v1190){ // QDC v965 and ADC v785
					header = (chData>>24) & 0x7;
					if(2 == header){ // data header
#ifdef DEBUG_VME
						int cnt = (chData >> 8) & 0x3F;
						cout << "Header for v7x5, slot: " << slot; // DEBUG
						cout << ", count: " << cnt << endl; // DEBUG
						getchar(); // DEBUG
#endif
						continue;
					} // end header reading
					if(4 == header){ // data trailer
#ifdef DEBUG_VME
						cout << "Trailer for v7x5, slot: " << slot; // DEBUG
						getchar(); // DEBUG
#endif
						continue;
					} // end trailer reading
					if(0 == header){ // data channel
						int chId = (chData >> 16) & 0x1F;
						entry_temp.nl = 1; entry_temp.nt = 0;
						switch(slot){
							case 5: // QDC v965; slot 5
								entry_temp.channelId = chId + 8401;
								entry_temp.leadingTime[0] = (chData & 0xFFF) +  rand0_5();
								if(chId >= 32) TAPopMsg::Error("TARawDataProcessor", "ReadOfflineVME: abnormal chId for qdc plugin: %d", chId);
								evt.qdc[0][chId] = chData & 0xFFF;
								break;
							case 17: // ADC v785; slot 17
								entry_temp.channelId = chId + 8501;
								entry_temp.leadingTime[0] = (chData & 0xFFF);
								if(31 == chId) pileUp = entry_temp.leadingTime[0] +  rand0_5();
								if(chId >= 32) TAPopMsg::Error("TARawDataProcessor", "ReadOfflineVME: abnormal chId for adc plugin: %d", chId);
								evt.adc[chId] = chData & 0xFFF;
								break;
							default:
								TAPopMsg::Error("TARawDataProcessor", "ReadOfflineVME: abnormal slot number for non-mTDC plugins: slot: %d", slot); break;
						} // end switch
						if(index >= fIndex0) treeDataVME->Fill();
						continue;
					} // end if header == 0
				} // end if QDC or ADC

				///// ->->-> start or end of data zone for certain plugins ->->-> ////
				// global header and global trailer of mTDC plugins
				if((8 == slot || 16 == slot) && !id_v830){
					int geo = chData & 0x1F; // plugin id (slot No. in practical)
					if(8 == slot && (9 == geo || 11 == geo)){ // HPTDC global header
						id_v1190 = geo;
						// initialization for assignment
						for(int &x : channelId) x = -2;
						memset(nhl, 0, sizeof(nhl));
						memset(nht, 0, sizeof(nht));
#ifdef DEBUG_VME
						cout << "HPTDC Global Header for v1190, geo: " << geo << endl; // DEBUG
						cout << "id_v1190: " << id_v1190 << endl; // DEBUG
						getchar(); // DEBUG
#endif
						continue;
					} // end if Global Header
					if(16 == slot && (9 == geo || 11 == geo)){ // global trailer
						if(9 != id_v1190 && 11 != id_v1190)
							TAPopMsg::Error("TARawDataProcessor", "ReadOfflineVME: HPTDC global trailer encountered, but id_v1190 value is abnormal: %d", id_v1190);
#ifdef DEBUG_VME
						cout << "HPTDC Global Trailer for v1190, geo: " << geo << endl; // DEBUG
						cout << "id_v1190: " << id_v1190 << endl; // DEBUG
						getchar(); // DEBUG
#endif
						// assign and fill treeDataVME in global trailer
						for(int jj = 0; jj < 128; jj++) if(channelId[jj] >= 0){
							int chid = channelId[jj]; // chid in a v1190 plugin
							if(chid >= 128) TAPopMsg::Error("TARawDataProcessor", "ReadOfflineVME: abnormal chId for mtdc plugin: %d", chid);
							if(9 == id_v1190) entry_temp.channelId = chid + 8001;
							else if(11 == id_v1190) entry_temp.channelId = chid + 8201;
							if(nhl[chid] > edge_num_limit || nht[chid] > edge_num_limit){
//								TAPopMsg::Warn("TARawDataProcessor", "ReadOfflineVME: HPTDC edge number is greater than %d: nhl: %d, nht: %d", edge_num_limit, nhl[chid], nht[chid]);
							}
							entry_temp.nl = nhl[chid] < edge_num_limit ? nhl[chid] : edge_num_limit;
							entry_temp.nt = nht[chid] < edge_num_limit ? nht[chid] : edge_num_limit;
//							entry_temp.nl = nhl[chid]; // DEBUG
//							entry_temp.nt = nht[chid]; // DEBUG

							for(int k = 0; k < nhl[chid] && k < edge_num_limit; k++){
								entry_temp.leadingTime[k] = 
									(hl[chid][k] +  rand0_5()) * H_BLIP;
								// fill tree - vme with mtdc data //
								// only leading edges are collected
								if(9 == id_v1190) evt.mtdc0[chid][k] = hl[chid][k];
								else if(11 == id_v1190) evt.mtdc1[chid][k] = hl[chid][k];
							} // end for over k
							for(int k = 0; k < nht[chid] && k < edge_num_limit; k++)
								entry_temp.trailingTime[k] = 
									(ht[chid][k] +  rand0_5()) * H_BLIP;
							if(index >= fIndex0) treeDataVME->Fill();

							if(nhl[chid] > 0) for(int k = 0; k < nhl[chid]; k++)
								hl[chid][k] = 0;
							if(nht[chid] > 0) for(int k = 0; k < nht[chid]; k++)
								ht[chid][k] = 0;
						} // end for over chs in a v1190 plugin and if
						
						id_v1190 = 0;
						continue;
					} // end if Global Trailer
				} // end of header and trailer processing
				if(21 == slot && !id_v1190){ // scaler v830 header
					id_v830++;
					nsca = 0;
//					memset(sca, 0, sizeof(sca));
//					memset(psca, 0, sizeof(psca));
//					memset(dsca, 0, sizeof(dsca));
#ifdef DEBUG_VME
					cout << "v830 header encountered" << endl; // DEBUG
					getchar(); // DEBUG
#endif
					continue;
				} // end of if(21 == slot)
				///// <-<-<- start or end of data zone for certain plugins <-<-<- ////
				// HPTDC group header and group trailer //
				if(1 == slot && id_v1190){ // TDC group header
#ifdef DEBUG_VME
					cout << "TDC group header for v1190, id_v1190: " << id_v1190 << endl; // DEBUG
					int bunchId = chData & 0xFFF, eventId = chData>>12 & 0xFFF; // DEBUG
					int TDCId = chData>>24 & 0xF, headMark = chData>>28 & 0xF; // DEBUG
					cout << "bunchId: " << bunchId << "\teventId: " << eventId << endl; // DEBUG
					cout << "TDCId: " << TDCId << "\theadMark: " << headMark << endl; // DEBUG
					getchar(); // DEBUG
#endif
					continue;
				} // end TDC group header
				if(3 == slot && id_v1190 && !id_v830){ // TDC group trailer
#ifdef DEBUG_VME
					int cnt = (chData & 0xFFF) - 2; // exclude the word count of header and trailer
					cout << "TDC group trailer for v1190, id_v1190: " << id_v1190 << endl; // DEBUG
					cout << cnt << " hits." << endl; // DEBUG
					int wordCnt = chData & 0xFFF, eventId = chData>>12 & 0xFFF; // DEBUG
					int TDCId = chData>>24 & 0xF, headMark = chData>>28 & 0xF; // DEBUG
					cout << "wordCnt: " << wordCnt << "\teventId: " << eventId << endl; // DEBUG
					cout << "TDCId: " << TDCId << "\theadMark: " << headMark << endl; // DEBUG
					getchar(); // DEBUG
#endif
					continue;
				} // end TDC group trailer
				///////////////////////////////////////////

				if(0 == slot && id_v1190 && !id_v830){ // HPTDC physical data zone
					int chId = (chData>>19) & 0x7F; // local chId in the plugin
					bool lt = (chData>>26) & 0x1; // leading or trailing edge
					if(0 == lt){ // leading edge
						hl[chId][nhl[chId]] = chData & 0x7FFFF; nhl[chId]++;
						if(channelId[chId] < 0) channelId[chId] = chId;
					} // end if (leading edge)
					else{ // trailing edge
						ht[chId][nht[chId]] = chData & 0x7FFFF; nht[chId]++;
						if(channelId[chId] < 0) channelId[chId] = chId;
					} // end else (trailing edge)
					continue;
				} // end if HPTDC physical data zone

				if(id_v830){
					psca[nsca] = sca[nsca];
					sca[nsca] = chData;
					dsca[nsca] = sca[nsca] - psca[nsca];
					evt.sca[nsca] = sca[nsca];
					evt.dsca[nsca] = dsca[nsca];
					nsca++;
//					cout << "nsca: " << nsca << endl; // DEBUG
					if(0)if(11 == nsca){ // DEBUG
						cout << "index: " << index << endl; // DEBUG
						cout << "psca[11]: " << psca[11] << endl; // DEBUG
						cout << "dsca[11]: " << dsca[11] << "\tsca[11]: " << sca[11] << endl; // DEBUG
						getchar(); // DEBUG
					} // DEBUG
					if(16 == nsca){
						id_v830 = 0;
						if(index >= fIndex0) treeSCA->Fill();

//						cout << "index: " << index << endl; // DEBUG
//						for(int i = 0; i < 16; i++){ // DEBUG
//							cout << "sca[" << i << "]: " << sca[i]; // DEBUG
//							cout << "\tpsca[" << i << "]: " << psca[i]; // DEBUG
//							cout << "\tdsca[" << i << "]: " << dsca[i] << endl; // DEBUG
//						}
//						getchar(); // DEBUG
					} // end if
				} // end if(id_v830)
			} // end loop over channels in an event
			// copy channel data of ch-8532, because two MUSICs share the same pileUp channel
			if(-9999. != pileUp){ // pileUp channel has been fired
				// MUSIC-L
				entry_temp.channelId = 8533;
				entry_temp.nl = 1;
				entry_temp.nt = 0;
				entry_temp.leadingTime[0] = pileUp;
				if(index >= fIndex0) treeDataVME->Fill();
				// Si
				entry_temp.channelId = 8534;
				entry_temp.nl = 1;
				entry_temp.nt = 0;
				entry_temp.leadingTime[0] = pileUp;
				if(index >= fIndex0) treeDataVME->Fill();
			} // end if(-9999. != pileUp)
			
			// mark the end of one event
			entry_temp.index = -2;
			entry_temp.channelId = ev_len;
			entry_temp.nl = 0;
			entry_temp.nt = 0;
			entry_temp.bunchId = 0;
			if(index >= fIndex0) treeDataVME->Fill();
			// // // end of event decoding // // //

			pos += ev_len;
			if(index >= fIndex0) event_num++;

			if(index >= fIndex0) vme->Fill(); // Fill vme pristine data tree
		} // end loop over events in a block
		if(index >= fIndex1) break;
		if(index >= fIndex0) block_num++;
		cout << "Block " << block_num << " processed.\r" << flush;
		memset(buffer, 0, sizeof(buffer)); // initialize block buffer
	} // end while over blocks
	// ensure a normal ending at the end of a data file //
	if(entry_temp.nl != 0){
		entry_temp.index = -2;
		entry_temp.channelId = ev_len;
		entry_temp.nl = 0;
		entry_temp.nt = 0;
		entry_temp.bunchId = 0;
	} // end if

	fVMEEventCnt = event_num;

	cout << "\n\n>>>>>>>>>>>>> VME DATA Total BLOCK Count <<<<<<<<<<<<<<<<<<<\n" << endl;
	cout << "       \033[33;1m" << block_num << "\033[0m        \n" << endl;
	cout << ">>>>>>>>>>>>> VME DATA Total EVENT Count <<<<<<<<<<<<<<<<<<<\n" << endl;
	cout << "       \033[33;1m" << event_num << "\033[0m        \n" << endl;
	if(fEventCnt > 0){
		cout << "------------- EVENT NUMBER DIFFERENCE -------------" << endl;
		cout << "\033[36;1mPXI ev_cnt - VME ev_cnt: \033[31;1m" << fEventCnt - event_num << "\033[0m\n" << endl;
	}
	cout << "\033[45;1m .... .... ... RAW DATA DECODEING HAS BEEN COMPLETED .... .... ... \033[0m\n\n\n" << endl;
	// close the binary file
	fclose(fp);

	treeDataVME->Write("", TObject::kOverwrite);
	treeSCA->Write("", TObject::kOverwrite);
	vme->Write("", TObject::kOverwrite);
	f->Close(); delete f; f = nullptr;
//	delete treeDataVME;

	return 0;
} // end function ReadOfflineVME








