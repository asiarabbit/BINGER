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
// Last modified: 2017/10/30, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <unistd.h> // access
#include <cstdlib> // rand
#include <fstream> // fread
#include <iostream>
#include <libgen.h> // basename function
#include <climits>

#include "TFile.h"
#include "TTree.h"

#include "TARawDataProcessor.h"
#include "TACtrlPara.h"
#include "tEntry.h"
#include "TAPopMsg.h"

using std::cout;
using std::endl;
using std::flush;

TARawDataProcessor* TARawDataProcessor::fInstance = nullptr;

TARawDataProcessor::TARawDataProcessor() : fDataFile(""), fROOTFile(""){
	fBunchIdMisAlignCnt = 0;
	fEventCnt = 0;
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

void TARawDataProcessor::SetDataFileName(const string &name, int runId){
	// retrieve the file name from a path+name string
	char tmp[64]; strcpy(tmp, name.c_str());
	sprintf(tmp, "%s_%d", basename(tmp), runId);
	if(!strcmp(tmp, ""))
		TAPopMsg::Error("TARawDataProcessor", "SetDataFileName: Input data file is empty");
	if(0 != access(name.c_str(), F_OK))
		TAPopMsg::Error("TARawDataProcessor", "SetDataFileName: %s doesn't exist", name.c_str());
	fDataFile = name;
	fROOTFile = tmp; fROOTFile += ".root";
//	TAPopMsg::Debug("TARawDataProcessor", "SetDataFileName: fROOTFile: %s", fROOTFile.c_str());
}
void TARawDataProcessor::SetPeriod(int index0, int index1){
	if(fIndex0 >= fIndex1)
		TAPopMsg::Error("TARawDataProcessor", "SetPeriod: index0 %d is not smaller than index1 %d", index0, index1);
	fIndex0 = index0; fIndex1 = index1;
}

//////---------------------------- READ OFFLINE-----------------------------------------------------//
inline double rand0_5(){ return rand()*1./RAND_MAX; } // bin smoothing, import from BUAA code.

// read offline binary data file and store them in a tree and a rootfile.
int TARawDataProcessor::ReadOffline(){
	if(0 == access(fROOTFile.c_str(), F_OK)) return -1; // file already exists

	static const double H_BLIP = 25. / pow(2., 8.);
	static const double V_BLIP = 25. / pow(2., 10.);

	static const int edge_num_limit = 10;
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
	bool Is_Error = false; // a symbol indicating if the last channel of a fragment, which is supposed to store a group header, is longer than a sizeof(int).

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
	TTree *treeData = new TTree("treeData", ("experiment data-"+fDataFile).c_str());
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
	if(!(fp = fopen(fDataFile.c_str(),"rb"))){
		cout << "File does not exist." << endl;
		exit(1);
	}
	cout << "Data file " << fDataFile << " opened. " << endl;
	fBunchIdMisAlignCnt = 0;
	fEventCnt = 0;
	while(1){
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
				cout << " Framgent Number: " << frag_nu << endl;
				Is_vacant = 1;
			}
		} // end if(frag_nu > 0)
		else{
			cout << "This event has no data!" << endl;
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
						if( (data_ch[k]>>30 & 0x1) == 0){
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
					for(int k = 0; k < nhl && k < 10; k++)
						entry_temp.leadingTime[k] =
							(hl[j][k] +  rand0_5()) * H_BLIP;
					for(int k = 0; k < nht && k < 10; k++)
						entry_temp.trailingTime[k] =
							(ht[j][k] +  rand0_5()) * H_BLIP;
					entry_temp.is_V = false; entry_temp.channelId = ch_id[j];
					entry_temp.nl = nhl; entry_temp.nt = nht;
					entry_temp.index = index;
					entry_temp.bunchId = -1;
					if(index >= fIndex0) treeData->Fill();
				} // end if(nhl >= 1 && nht >= 1)
				else if(nvl >= 1 && nvt >= 0){
					for(int k = 0; k < nvl && k < 10; k++)
						entry_temp.leadingTime[k] =
							(vl[j][k] +  rand0_5()) * V_BLIP;
					for(int k = 0; k < nvt && k < 10; k++)
						entry_temp.trailingTime[k] =
							(vt[j][k] +  rand0_5()) * V_BLIP;
					entry_temp.is_V = true; entry_temp.channelId = ch_id[j];
					entry_temp.nl = nvl; entry_temp.nt = nvt;
					entry_temp.index = index;
					entry_temp.bunchId = -1;
					if(index >= fIndex0) treeData->Fill();
				} // end if(nvl >= 1 && nvt >= 1)

				// initialize edge arrays
				if(nvl > 0) for(int k = 0; k < nvl; k++) vl[j][k] = -1.;
				if(nvt > 0) for(int k = 0; k < nvt; k++) vt[j][k] = -1.;
				if(nhl > 0) for(int k = 0; k < nhl; k++) hl[j][k] = -1.;
				if(nht > 0) for(int k = 0; k < nht; k++) ht[j][k] = -1.;

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
				Is_Error = 1;
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
				if(index >= fIndex0) fBunchIdMisAlignCnt++;
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
		if(index >= fIndex0) treeData->Fill();

		if(frag_nu > 0) delete [] frag_h;

		section_len = -1; Is_vacant = 0; Is_Error = 0;

		cout << "Processing event index  " << index << "\r" << flush;
	} // end while
	
	cout << endl << endl << "Total Event Count: " << endl;
	cout << endl <<  "       \033[1m" << fEventCnt << "\033[0m        " << endl << endl;
	cout << "\033[36;1mBunch ID misalignment count: " << fBunchIdMisAlignCnt << "\033[0m\n";
	if(fBunchIdMisAlignCnt > 0) TAPopMsg::Error("TARawDataProcessor", "ReadOffLine: BunchId misalignment count happend %d times", fBunchIdMisAlignCnt);

	treeData->Write("", TObject::kOverwrite);
	fclose(fp);
	f->Close();
	cout << "\033[33;1m:)\033[0m treeData has been written into " << fROOTFile << ". \033[33;1m:)\033[0m\n\n";
//	cout << "mark 1\n"; getchar(); // DEBUG
	delete f;

	return 0;
} // end function ReadOffline


