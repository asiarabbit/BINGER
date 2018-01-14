///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor.C												 //
//   TAEventProcessor.C -- source file for class TAEventProcessor					 //
//   Introduction: The global event processor class, a singleton class, responsible  //
// for distribution of channel data to the detector hierarchy and the right channel	 //
// objects. After the event is fully assigned, event assembly routine would be		 //
// initiated. It is also this class' duty to provide analysis result output			 //
// interface.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/13.															     //
// Last modified: 2018/1/6, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <libgen.h>
#include <unistd.h>

// ROOT include
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraph2D.h"

#include "TAEventProcessor.h"
#include "TARawDataProcessor.h"
#include "TAParaManager.h"
#include "TAVisual.h"
#include "TAPID.h"
#include "TAUIDParser.h"
#include "TAT0_0.h"
#include "TAT0_1.h"
#include "TASiPMPlaArray.h"
#include "TAMWDCArrayL.h"
#include "TAMWDCArrayR.h"
#include "TASiPMPlaBarrel.h"
#include "TAChannel.h"
#include "TAMWDC.h"
#include "TATOFWall.h"
#include "TAPlaStrip.h"
#include "TAPlaStripData.h"
#include "TAPlaStripPara.h"
#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TAAnodeData.h"
#include "TAMath.h"
#include "tEntry.h"
#include "tTrack.h"
#include "t3DTrkInfo.h"
#include "t3DPIDInfo.h"
#include "TAGPar.h" // Global parameters

using std::cout;
using std::endl;
using std::flush;
using std::setw;

//#define DEBUG
#define GO // do the filling of ROOT objects
//#define VERBOSE // show TAPopMsg::Info() information

TAEventProcessor* TAEventProcessor::fInstance = nullptr;

TAEventProcessor::TAEventProcessor(const string &datafile, int runId)
		: fRawDataProcessor(0), fParaManager(0), fCtrlPara(0),
		fVisual(0), fPID(0), fGPar(0), fIsPID(true), fIsTracking(true){
	fEntryList.reserve(100);
	fTrackList.reserve(50);

	fRawDataProcessor = TARawDataProcessor::Instance();
	fParaManager = TAParaManager::Instance();
	fCtrlPara = TACtrlPara::Instance();
	fVisual = TAVisual::Instance();
	fPID = TAPID::Instance();
	fGPar = TAGPar::Instance();
	TAPopMsg::Verbose(false); // TAPopMsg::Silent();
	if(strcmp("", datafile.c_str())) SetDataFile(datafile, runId);
}
TAEventProcessor::~TAEventProcessor(){
	if(fRawDataProcessor){
		delete fRawDataProcessor; fRawDataProcessor = nullptr;
	}
	if(fParaManager){
		delete fParaManager; fParaManager = nullptr;
	}
	if(fCtrlPara){
		delete fCtrlPara; fCtrlPara = nullptr;
	}
	if(fVisual){
		delete fVisual; fVisual = nullptr;
	}
	if(fPID){
		delete fPID; fPID = nullptr;
	}
	if(fGPar){
		delete fGPar; fGPar = nullptr;
	}
}
TAEventProcessor* TAEventProcessor::Instance(const string &datafile, int runId){
	if(!fInstance) fInstance = new TAEventProcessor(datafile, runId);
	return fInstance;
}
TARawDataProcessor *TAEventProcessor::GetRawDataProcessor() const{
	if(!fRawDataProcessor)
		TAPopMsg::Warn("TAEventProcessor", "GetRawDataProcessor: pointer is null.");
	return fRawDataProcessor;
}
TAParaManager *TAEventProcessor::GetParaManager() const{
	if(!fParaManager) TAPopMsg::Warn("TAEventProcessor", "GetParaManager: pointer is null.");
	return fParaManager;
}
TACtrlPara *TAEventProcessor::GetCtrlPara() const{
	if(!fCtrlPara) TAPopMsg::Warn("TAEventProcessor", "GetCtrlPara: pointer is null.");
	return fCtrlPara;
}
TAVisual *TAEventProcessor::GetVisual() const{
	if(!fVisual) TAPopMsg::Error("TAEventProcessor", "GetVisual(): pointer is null.");
	return fVisual;
}
TAPID *TAEventProcessor::GetPID() const{
	if(!fPID) TAPopMsg::Error("TAEventProcessor", "GetPID(): pointer is null.");
	return fPID;
}
TAGPar *TAEventProcessor::GetGPar() const{
	if(!fGPar) TAPopMsg::Error("TAEventProcessor", "GetGPar(): pointer is null.");
	return fGPar;
}
bool TAEventProcessor::Is3DTracking() const{
	return GetCtrlPara()->Is3DTracking();
}
void TAEventProcessor::SetConfigExpDir(const string &dir){
	GetCtrlPara()->SetConfigExpDir("../config/"+dir);
}
void TAEventProcessor::SetMagneticIntensity(double B){
	// for extrapolation from B @1000A to other B value
	GetPID()->SetMagneticIntensity(B);
}
void TAEventProcessor::SetIs3DTracking(bool opt){
	SetIsTracking(true);
	TACtrlPara::Instance()->SetIs3DTracking(opt);
}
void TAEventProcessor::SetSTRROOTFile(const string &file){
	string STRFile = "/STR/"+file;
	char tmp[64]; strcpy(tmp, GetCtrlPara()->ConfigExpDir());
	STRFile = dirname(tmp) + STRFile;
	GetCtrlPara()->SetSTRROOTFile(STRFile);
}
void TAEventProcessor::SetDataFile(const string &datafile, int runId){
	if(!strcmp("", datafile.c_str()))
		TAPopMsg::Error("TAEventProcessor", "SetDataFile: Input datafile name is empty");
	const char c = datafile.c_str()[0];
	if('/' == c || '.' == c) // data file with its path specified
		GetRawDataProcessor()->SetDataFileName(datafile, runId);
	else GetRawDataProcessor()->SetDataFileName("../data/"+datafile, runId);
}
void TAEventProcessor::SetPeriod(int index0, int index1){
	GetRawDataProcessor()->SetPeriod(index0, index1);
}
// create detectors
void TAEventProcessor::Configure(){
	static bool isCalled = false; // make sure that no matter how many times called,
	if(isCalled){ // configure function would only be implemented once
//		TAPopMsg::Warn("TAEventProcessor", "Configurte: has been called once.");
		return;
	}
	// STR_spline.root || STR_stiff.root || STR_aaa900.root
	SetSTRROOTFile("STR_spline.root"); // space-time relations for MWDCs
	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	// read the global parameters array first; type: 004
	const short nignore = 4, typeignore[nignore] = {0, 1, 2, 3};
	GetParaManager()->ReadParameters(nignore, typeignore);
	// note that the detector UID has to be equal to the array detList subscript
	detList[0] = new TAT0_0("T0_0", "T0_0@Mid-RIBLL2", 0);
	detList[1] = new TAT0_1("T0_1", "T0_1@End-RIBLL2", 1);
	detList[2] = new TASiPMPlaArray("SiPMPlaArray", "SiPMPlaArray@Post-Target", 2);
	detList[3] = new TAMWDCArrayL("DCArrayL", "DCArrayL@Post-Magnet", 3);
	detList[4] = new TAMWDCArrayR("DCArrayR", "DCArrayR@Post-Magnet", 4);
	detList[5] = new TASiPMPlaBarrel("SiPMPlaBarrel", "SiPMPlaBarrel@Hug-Target", 5);
	for(TADetUnion *&p : detList) if(p) p->Configure(); // build the detectors

	// read all the parameters required and assign positiion parameters to every channel and alike
	GetParaManager()->ReadParameters();

	// TAVisual::Configure can only be implemented AFTER all the other detectors are created.
	GetVisual()->Configure();
	GetPID()->Configure();
	// show some information
	if(TAPopMsg::IsVerbose()){
		((TAMWDCArray*)detList[3])->Info();
		((TAMWDCArray*)detList[4])->Info();
	}
	isCalled = true; // has been called
}
// assign an event to the detectors by distributing channel data to the matching channel objects
void TAEventProcessor::Assign(){
	for(tEntry *&e : fEntryList) Assign(e);
}
void TAEventProcessor::Assign(tEntry *entry){

	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	int uid = GetParaManager()->GetUID(entry->channelId);
	if(TAParaManager::UID_DUMMY == uid){ // entry with homeless channel id
		strcpy(entry->name, "\033[31mDUMMY_CHANNEL\033[0m");
		return;
	}
	if(2 * TAParaManager::UID_DUMMY == uid){ // entry with homeless channel id
		strcpy(entry->name, "\033[31mEMPTY_CHANNEL\033[0m");
		return;
	}
	int type[6]{}; TAUIDParser::DNS(type, uid); int detId = type[0]; // resolute UID

	if(detList[detId]) detList[detId]->GetChannel(uid)->GetData()->Assign(entry);
	else TAPopMsg::Error("TAEventProcessor", "Assign: Detector List element #%d is null.", detId);
}
// a method dedicated for TAVisual::Fill()
void TAEventProcessor::FillTrack(TGraph *gTrack, TGraph *gTrack_R) const{
	if(!gTrack || !gTrack_R)
		TAPopMsg::Error("TAEventProcessor", "FillTrack: input TGraph pointer is null");
	TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	TAMWDCArray *dcArr[2]{0};
	dcArr[0] = (TAMWDCArray*)detList[3];
	dcArr[1] = (TAMWDCArray*)detList[4];
	for(int i = 2; i--;) dcArr[i]->FillTrack(gTrack, gTrack_R);
}
void TAEventProcessor::Initialize(){
	for(tEntry *&t : fEntryList) if(t){ delete t; t = nullptr; } fEntryList.clear();
	for(tTrack *&t : fTrackList) if(t){ delete t; t = nullptr; } fTrackList.clear();
	
	for(TADetUnion *&det : GetParaManager()->GetDetList()){
		if(det) det->Initialize();
	}
	GetPID()->Initialize();
}


// -- Analyze and Run method -- //
// analyze one event after its channels data are assigned to detectors
static const double c0 = TAParaManager::Instance()->GetPhysConst("c0");
void TAEventProcessor::Analyze(){
	if(!IsTracking()) return;

	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	static TAMWDCArrayL *dcArrL = (TAMWDCArrayL*)detList[3];
	static TAMWDCArrayR *dcArrR = (TAMWDCArrayR*)detList[4];

	// pattern recognition and rough fit for particle tracking
	if(dcArrL){ dcArrL->Map(); dcArrL->AssignTracks(fTrackList); }
	if(dcArrR){
		dcArrR->Map();
		dcArrR->AssignTracks(fTrackList);
	}
	// assign and output beta and index
	int index = GetEntryList()[0]->index;
	const int n3DTrkR = dcArrR->GetN3DTrack(); // number of 3D tracks in DCArrR
	for(tTrack *&t: fTrackList){
		t->index = index;
		// rearrange trkid to make it global and unique
		if(0 == t->type/10%10) // MWDCArray L
			if(-1 != t->id) t->id += n3DTrkR;
#ifdef DEBUG
		t->show(); // DEBUG
#endif
	} // end for over tracks
} // end of member function Analyze
inline void correctCycleClear(double &x, const double bunchIdTime){
//	cout << "0.x: " << x << endl; getchar(); // DEBUG
	if(-9999. != x){
		x -= bunchIdTime;
		if(x < 0.) x += 51200.;
	}
//	cout << "1.x: " << x << endl; getchar(); // DEBUG
}
// the overall data analysis routine
// (id0, id1): index range for analysis; secLenLim: event length limit; rawrtfile: raw rootfile
void TAEventProcessor::Run(int id0, int id1, int secLenLim, const string &rawrtfile){
	secLenLim = secLenLim < 2000 ? secLenLim : 2000; // 2000 word (7.8KiB) is the event length limit
	Configure(); // prepare ETF setup
	if(id0 >= id1)
		TAPopMsg::Error("TAEventProcessor", "Run: index0 %d is not smaller than index1 %d", id0, id1);
	string rootfile = rawrtfile; // name of the ROOT file storing raw data - treeData
	if(!strcmp(rawrtfile.c_str(), "")){ // no input rootfile
		GetRawDataProcessor()->SetPeriod(id0, id1);
		GetRawDataProcessor()->ReadOffline(); // prepare data file
		rootfile = GetRawDataProcessor()->GetROOTFileName();
		printf("\033[32;1m  Analyzing event#%d to event#%d from datafile  %s\n\033[0m", id0, id1, GetRawDataProcessor()->GetDataFileName());
	}
	else printf("\033[32;1m  Analyzing event#%d to event#%d from rootfile   %s\n\033[0m", id0, id1, rootfile.c_str());
//	return;
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TAEventProcessor", "Run: %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	TTree *treeData = (TTree*)f->Get("treeData");
	if(!treeData) TAPopMsg::Error("TAEventProcessor", "Run: Obtained treeData is null pointer");
	tEntry entry_t;
	treeData->SetBranchAddress("index", &entry_t.index);
	treeData->SetBranchAddress("nl", &entry_t.nl);
	treeData->SetBranchAddress("nt", &entry_t.nt);
	treeData->SetBranchAddress("channelId", &entry_t.channelId);
	treeData->SetBranchAddress("leadingTime", entry_t.leadingTime);
	treeData->SetBranchAddress("trailingTime", entry_t.trailingTime);
	treeData->SetBranchAddress("is_V", &entry_t.is_V);
	treeData->SetBranchAddress("bunchId", &entry_t.bunchId);
	vector<tEntry *> &entry_ls = GetEntryList();
	vector<tTrack *> &track_ls = GetTrackList();

	// read rootfile and assembly each event
	const int n = treeData->GetEntries();
	int cntTrk = 0, cnt3DTrk = 0; // ntr: n trk per event
	int cntaozWrong = 0, cntaoz = 0;
	int i = 0, index, cntSec = 0;
	int ntr = 0; // number of track projections in a data section(3*3D track)
	const int ntrMax = 200; // maximum number of track projections in an event
#ifdef GO
	#include "TAEventProcessor/define_hist.C" // define histograms of interest
	#include "TAEventProcessor/define_tree.C" // define the track tree
#endif
	cout << std::left;
	cout << setw(10) << "index" << setw(10) << "nEv" << setw(10) << "trkX" << setw(10) << "totTrk";
	cout << setw(10) << "3Dtrk" << setw(10) << "naoz" << setw(10) << "naozBad" << endl;
	while(i < n){
		Initialize(); // clear everything from last data section
		// assign all entries in a sec to fEntryList for processing
		while(1){
			entry_t.initialize();
			treeData->GetEntry(i++);
			if(-2 != entry_t.index){ // index == -2 marks end of one data section
				entry_ls.push_back(new tEntry(entry_t));
				index = entry_t.index;
			}
			else break;
		} // entry assignment for the data section complete
		if(0 == entry_ls.size()) continue; // empty event
		// correct time from cycle-clear
		double bunchIdTime = (abs(entry_t.bunchId) & 0x7FF) * 25.;
		if(entry_t.bunchId < 0) bunchIdTime *= -1.;
//		cout << "bunchIdTime: " << bunchIdTime << endl; getchar(); // DEBUG
		for(tEntry *t : entry_ls){
//			t->show(); // DEBUG
			for(double &x : t->leadingTime) correctCycleClear(x, bunchIdTime);
			for(double &x : t->trailingTime) correctCycleClear(x, bunchIdTime);
		}
		if(entry_t.channelId > secLenLim) continue; // index==2, then channelId stores secLen
		if(index < id0){
			cout << "Skipping Event index " << index << "\r" << flush;
			continue;
		} // starting from index - id0
		if(index >= id1) break; // ending at index - id1-1
///		cout << "\n\nindex: " << index << endl; // DEBUG
		Assign(); // *** assign entries in fEntryList *** //
//		for(auto &t : entry_ls) cout << t->name << "\t" << t->channelId << endl; getchar(); // DEBUG
//		for(auto &t : entry_ls) t->show(); getchar(); // DEBUG
#ifdef GO
		#include "TAEventProcessor/fill_pre.C" // fill hists and trees before tracking
#endif
		// *** recognize patterns and assign raw tracks to fTrackList *** //
		Analyze();
		ntr = track_ls.size() < ntrMax ? track_ls.size() : ntrMax;
		for(int j = 0; j < ntr; j++){ cntTrk++; if(track_ls[j]->id != -1) cnt3DTrk++; }
#ifdef GO
		#include "TAEventProcessor/fill_post.C" // fill hists and trees after tracking
#endif
		cntSec++;
		if(index % 1 == 0){
			cout << setw(10) << index << setw(10) << cntSec << setw(10) << cntTrk - (cnt3DTrk/3)*2;
			cout << setw(10) << cntTrk << setw(10) << cnt3DTrk / 3;
			cout << setw(10) << cntaoz << setw(10) << cntaozWrong << "\r" << flush;
//			cout << "idx " << index << " nEv/trkX " << cntSec << "/";
//			cout << cntTrk - (cnt3DTrk/3)*2 << " totTrk " << cntTrk << " 3Dtrk " << cnt3DTrk / 3;
//			cout << " naoz " << cntaoz << " naozBad " << cntaozWrong << "\r" << flush; // cntaozWrong
		}
	} // end while over treeData entries
	cout << std::right;
#ifdef GO
	#include "TAEventProcessor/write.C" // write all the ROOT objects necessary
#endif

	cout << "\n\n";
	cout << "Totally \033[33;1m" << cntSec << "\033[0m sections ";
	cout << "\033[1m" << i << "\033[0m entries \033[1m" << cntTrk;
	cout << "\033[0m tracks and \033[1m" << cnt3DTrk / 3;
	cout << "\033[0m 3D tracks have been processed.\n";
	cout << " cntaoz " << cntaoz << " cntaozWrong " << cntaozWrong << endl;
	cout << "Exiting the Run function.\n\n";
	f->Close(); delete f;
//	delete treeTrack;
} // end of member function Run
//#define DEBUG
// correct drift time and refit with the update
void TAEventProcessor::RefineTracks(int &n3Dtr, t3DTrkInfo *trk3DIf, const double *tof2, const double *taHitX){
	// XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX //
//	if(!IsPID()) return; // PID is not on, so beta is unavailable				// XXX XXX XXX //
	// XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX //
//		TAPopMsg::Warn("TAEventProcessor", "RefineTracks: PID is not on, so beta is unavailable");
	vector<tTrack *> &tl = GetTrackList();
	const int ntr = tl.size(); if(ntr <= 0) return;

	static TAParaManager::ArrDet_t &decv = GetParaManager()->GetDetList();
	static TAMWDCArray *dcArrV[2] = {(TAMWDCArray*)decv[3], (TAMWDCArray*)decv[4]};
	// identify 3D tracks and start track refinement
	static const int ntrMax = 200, ntr3DMax = ntrMax/3;
	bool isDCArrR[ntr3DMax]{};
	int n3DtrXUV[3]{};
	int trkId[ntr3DMax][3]; memset(trkId, -1, sizeof(trkId)); // track id [3D track id][XUV]
	// loop over grouped track projections
	for(int j = 0; j < ntr; j++) if(-1 != tl[j]->id){
		for(int k = 0; k < 3; k++){ // loop over X-U-V track types
			if(tl[j]->type%10 == k){
				trkId[tl[j]->id][k] = j;
				n3DtrXUV[k]++;
			}
		} // end loop over X-U-V track types
	} // end for over j and if
	if(n3DtrXUV[0] != n3DtrXUV[1] || n3DtrXUV[0] != n3DtrXUV[2]){
		TAPopMsg::Error("TAEventProcessor", "RefineTracks: This is odd... track projections of X, U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d, ntr: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2], ntr);
	} // end if
	n3Dtr = n3DtrXUV[0];
	// // // ^^^^^^^ circulation over 3-D tracks in one data section ^^^^^^^ // // //
	for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section
		isDCArrR[jj] = bool(tl[trkId[jj][0]]->type/10%10); // 0: L; 1: R
		int nFX = 0, nFU = 0, nFV = 0; // fired anode layers in 
		for(int j = 0; j < 6; j++){ // count effective measurements
			if(tl[trkId[jj][0]]->nu[j] != -1) nFX++;
			if(tl[trkId[jj][1]]->nu[j] != -1) nFU++;
			if(tl[trkId[jj][2]]->nu[j] != -1) nFV++;
		} // end for over j
		const int nF = nFX + nFU + nFV; // number of measured points
		// ag[3]: anode direction vector, Ag[3]: one point in the anode
		double Ag[nF][3]{}, ag[nF][3]{}, rr[nF]{}; // rr: drift distance in XUV form
		double anodeId[nF][2]{}; // [0]: fired anode layer id: 0-17; [1]: nu
		double trkVec[4]{}; // track slope
		TAMWDCArray *dcArr = dcArrV[isDCArrR[jj]];
		const double phiAvrg = dcArr->GetPhiAvrg();
		trkVec[0] = tl[trkId[jj][0]]->k; // k1
		trkVec[2] = tl[trkId[jj][0]]->b; // b1
		trkVec[1] = TAMath::kUV_Y(phiAvrg, tl[trkId[jj][1]]->k, tl[trkId[jj][2]]->k); // k2
		trkVec[3] = TAMath::bUV_Y(phiAvrg, tl[trkId[jj][1]]->k, tl[trkId[jj][2]]->k, tl[trkId[jj][1]]->b, tl[trkId[jj][2]]->b); // b2
	 	int tmp = 0;
 		for(int l = 0; l < 3; l++){ // loop over X-U-V
			tTrack *&trk = tl[trkId[jj][l]];
			for(int j = 0; j < 6; j++){ // loop over 6 anode layers
				if(trk->nu[j] != -1){ // one measure point
					anodeId[tmp][0] = 6*l + j; // fired anode layer id: 0-17
					anodeId[tmp][1] = trk->nu[j]; // nu: anode id in certain layer
					TAAnode *ano = dcArr->GetMWDC(j/2)->GetAnode(l, j%2 + 1, trk->nu[j]);
					TAAnodePara *anoPar = (TAAnodePara*)ano->GetPara();
					anoPar->GetGlobalCenter(Ag[tmp]); anoPar->GetGlobalDirection(ag[tmp]);
					unsigned uid = ano->GetUID();
#ifdef DEBUG
					cout << "j: " << j << endl; // DEBUG
					cout << "Before correction,\n"; // DEBUG
					cout << "trk->t[j]: " << trk->t[j] << endl; // DEBUG
					cout << "trk->r[j]: " << trk->r[j] << endl; // DEBUG
#endif
					// t = T_tof + T_wire + T_drift + T0
					// substract T_wire and T_tof from the time measurement
					trk->t[j] -= TACtrlPara::T_tofDCtoTOFW(uid) - TACtrlPara::T_wireMean(uid); // recover the rough correction of time of flight from DC to TOF wall for a refined one
					double beta_t[2] = {0.5, 0.68}; // for simulation test only XXX XXX XXX
					dcArr->DriftTimeCorrection(trk->t[j], trk->r[j], anodeId[tmp], trkVec, trk->firedStripId, beta_t[isDCArrR[jj]]); // trk->beta
					rr[tmp] = trk->r[j];
#ifdef DEBUG
					cout << "After correction,\n";
					cout << "trk->t[j]: " << trk->t[j] << endl; // DEBUG
					cout << "trk->r[j]: " << trk->r[j] << endl; // DEBUG
					getchar(); // DEBUG
#endif
					tmp++;
				} // end if
			} // end for over j
		} // end for over l

		// fit the track with the new drift time and drift distance // DEBUG
//		cout << "Before correction,\n"; // DEBUG
//		cout << "k1: " << trkVec[0] << "\tk2: " << trkVec[1] << "\tb1: " << trkVec[2] << "\tb2: " << trkVec[3] << endl; // DEBUG
		TAMath::BFGS4(Ag, ag, trkVec, rr, nF);
//		cout << "After correction,\n"; // DEBUG
//		cout << "k1: " << trkVec[0] << "\tk2: " << trkVec[1] << "\tb1: " << trkVec[2] << "\tb2: " << trkVec[3] << endl; // DEBUG
		for(double &x : trk3DIf[jj].chi) x = -9999.;
		tmp = 0; trk3DIf[jj].chi2 = 0.;
		// assign residuals and prepare for the tree filling
		for(int l = 0; l < 3; l++){ // loop over X-U-V
			tTrack *&trk = tl[trkId[jj][l]];
			for(int j = 0; j < 6; j++){ // DC0X1X2-DC1X1X2-DC2X1X2
				if(trk->nu[j] != -1){ // one measure point
					trk3DIf[jj].chi[l*6+j] = TAMath::dSkew(Ag[tmp], ag[tmp], trkVec) - rr[tmp];
					trk3DIf[jj].chi2 += trk3DIf[jj].chi[l*6+j] * trk3DIf[jj].chi[l*6+j];
					tmp++;
//					cout << "chi[" << l*6+j << "]: " << trk3DIf[jj].chi[l*6+j] << endl; // DEBUG
				} // end if
			} // end for over j
		} // end for over l
		const int it = trkId[jj][0];
		trk3DIf[jj].Chi = sqrt(trk3DIf[jj].chi2/(nF-4));
		trk3DIf[jj].k1 = trkVec[0]; trk3DIf[jj].b1 = trkVec[2];
		trk3DIf[jj].k2 = trkVec[1]; trk3DIf[jj].b2 = trkVec[3];
		trk3DIf[jj].isDCArrR = isDCArrR[jj];
		trk3DIf[jj].tof2 = tof2[it];
		trk3DIf[jj].taHitX = taHitX[it];
//		cout << "isDCArrR: " << trk3DIf[jj].isDCArrR << endl; // DEBUG
//		cout << "chi2: " << trk3DIf[jj].chi2 << endl; // DEBUG
//		cout << "Chi: " << trk3DIf[jj].Chi << endl; // DEBUG
//		cout << "tof2: " << trk3DIf[jj].tof2 << endl; getchar(); // DEBUG

		//// calculate TOF hit position ////
		double p2[3]{}; // p2: fired strip projection
		trk3DIf[jj].firedStripId = tl[it]->firedStripId;
		TAPlaStrip *strip = dcArr->GetTOFWall()->GetStrip(tl[it]->firedStripId);
		strip->GetStripPara()->GetGlobalProjection(p2); // retrieve fired strip projection
		p2[1] = trkVec[1] * p2[2] + trkVec[3]; // y = k2 z + b2;
//		p2[0] = trkVec[0] * p2[2] + trkVec[2]; // x = k1 z + b1;
		trk3DIf[jj].TOF_posY_refine = p2[1]+600.; // 600.=1200./2. [0-1-2]: [x-y-z]
		trk3DIf[jj].TOF_posY = strip->GetHitPosition();
//		cout << "TOF_posY: " << trk3DIf[jj].TOF_posY << endl; // DEBUG
//		cout << "TOF_posY_refine: " << trk3DIf[jj].TOF_posY_refine << endl; getchar(); // DEBUG
		//// Get averaged TOT of DC signals ////
		// calculate averaged TOT over all the hit anode layers
		double TOTAvrgtmp = 0.; int TOTcnt = 0;
		// get the average, temporary, for the following filtering
		for(int j = 0; j < 3; j++){ // loop over XUV
			const int it = trkId[jj][j]; // trk id
			for(int k = 0; k < 6; k++){ // loop over six anode layers in the  three MWDCs
				if(tl[it]->dcTOT[k] >= 250.){
					TOTAvrgtmp += tl[it]->dcTOT[k]; TOTcnt++;
				}
			} // end for over k
		} // end for voer XUV
		if(0 == TOTcnt) continue;
		TOTAvrgtmp /= TOTcnt; // the temporary average
		TOTcnt = 0; trk3DIf[jj].dcTOTAvrg = 0.; // initialization for average update
		for(int j = 0; j < 3; j++){ // loop over XUV
			const int it = trkId[jj][j]; // trk id
			for(int k = 0; k < 6; k++){ // loop over six anode layers in the  three MWDCs
				if(tl[it]->dcTOT[k] >= TOTAvrgtmp*0.6){ // or it is deemed as noise
					trk3DIf[jj].dcTOTAvrg += tl[it]->dcTOT[k]; TOTcnt++;
				}
			} // end for over k
		} // end for voer XUV
		if(0 == TOTcnt) trk3DIf[jj].dcTOTAvrg = -9999.; // failed
		else trk3DIf[jj].dcTOTAvrg /= TOTcnt; // the updated average

	} // end for over jj
} // end of member function RefineTracks
// refine PID using the refined 3D track information
void TAEventProcessor::RefinePID(const int n3Dtr, const t3DTrkInfo *trk3DIf, t3DPIDInfo *pid3DIf){
	if(!IsPID() || n3Dtr <= 0) return; // PID is not desired
	TAPID *pid = GetPID();
	for(int i = 0; i < n3Dtr; i++) if(-9999. != trk3DIf[i].taHitX){
		const t3DTrkInfo &t = trk3DIf[i]; t3DPIDInfo &pi = pid3DIf[i];
		double pOut[4] = {t.k1, t.k2, t.b1, t.b2};
		pid->Fly(t.tof2, t.taHitX, pOut, short(t.isDCArrR));
		pi.aoz = pid->GetAoZ();
		pi.aozdmin = pid->GetChi();
		pid->GetTargetExitAngle(pi.angTaOut);
		pi.beta2 = pid->GetBeta();
		pi.poz = pid->GetPoZ();
		pi.trkLenT = pid->GetTotalTrackLength();
	}
}



