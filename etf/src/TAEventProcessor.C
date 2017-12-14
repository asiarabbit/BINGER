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
// Last modified: 2017/12/13, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <libgen.h>

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

//#define DEBUG
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
	SetDataFile(datafile, runId);
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
	const char c = datafile.c_str()[0];
	if('/' == c || '.' == c) // data file with its path specified
		GetRawDataProcessor()->SetDataFileName(datafile, runId);
	else GetRawDataProcessor()->SetDataFileName("../data/"+datafile, runId);
}
void TAEventProcessor::SetPeriod(int index0, int index1){
	GetRawDataProcessor()->SetPeriod(index0, index1);
}
void TAEventProcessor::Configure(){ // create detectors
	SetSTRROOTFile("STR.root"); // space-time relations for MWDCs

	static bool isCalled = false;
	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	if(isCalled){
//		TAPopMsg::Warn("TAEventProcessor", "Configurte: has been called once.");i
		return;
	}
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
	static TAT0_0 *T0_0 = (TAT0_0*)detList[0];
	static TAT0_1 *T0_1 = (TAT0_1*)detList[1];
	static TASiPMPlaArray *sipmArr = (TASiPMPlaArray*)detList[2];
	static TAMWDCArrayL *dcArrL = (TAMWDCArrayL*)detList[3];
	static TAMWDCArrayR *dcArrR = (TAMWDCArrayR*)detList[4];

	// Time of Flight and beam energy measurement
	double beta = -1., t0_0 = -9999., t0_1 = -9999.;
	if(T0_0 && T0_1){
		t0_0 = T0_0->GetTime(); t0_1 = T0_1->GetTime();
		double L = 25.88 * 1000.;
		if(t0_0 != -9999. && t0_1 != -9999.){
			beta = L / (t0_1 - t0_0);
			beta /= c0;
		}
	}
	// pattern recognition and rough fit for particle tracking
	if(dcArrL){ dcArrL->Map(); dcArrL->AssignTracks(fTrackList); }
	if(dcArrR){ dcArrR->Map(); dcArrR->AssignTracks(fTrackList); }

	// assign and output beta and index
	vector<tTrack *> &track_ls = GetTrackList();
	int index = GetEntryList()[0]->index;
	for(tTrack *&t: track_ls){
		t->index = index;
#ifdef DEBUG
		t->show(); // DEBUG
#endif
	}
} // end of member function Analyze
inline void correctCycleClear(double &x, double bunchIdTime){
	if(-9999. != x){
		x -= bunchIdTime;
		if(x < 0.) x += 51200.;
	}
}
// the overall data analysis routine
// (id0, id1): index range for analysis; secLenLim: event length limit; rawrtfile: raw rootfile
void TAEventProcessor::Run(int id0, int id1, int secLenLim, const string &rawrtfile){
	Configure(); // prepare ETF setup
	if(id0 >= id1)
		TAPopMsg::Error("TAEventProcessor", "Run: index0 %d is not smaller than index1 %d", id0, id1);
	string rootfile = rawrtfile; // name of the ROOT file storing raw data - treeData
	if(!strcmp(rawrtfile.c_str(), "")){ // no input rootfile
		GetRawDataProcessor()->SetPeriod(id0, id1);
		GetRawDataProcessor()->ReadOffline(); // prepare data file
		rootfile = GetRawDataProcessor()->GetROOTFileName();
		printf("  Analyzing event#%d to event#%d from datafile  %s\n", id0, id1, GetRawDataProcessor()->GetDataFileName());
	}
	else printf("  Analyzing event#%d to event#%d from rootfile   %s\n", id0, id1, rootfile.c_str());
//	return;
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
	// read rootfile and assembly each event
	#include "TAEventProcessor/define_hist.C" // define histograms of interest
	#include "TAEventProcessor/define_tree.C" // define the track tree

	const int n = treeData->GetEntries();
	int cntTrk = 0, cnt3DTrk = 0;
	int cntaozWrong = 0, cntaoz = 0;
	int i = 0; int cntSec = 0;
	while(i < n){
		Initialize(); // clear everything from last data section.
		// assign all entries in a sec to fEntryList for processing.
		while(1){
			entry_t.initialize();
			treeData->GetEntry(i++);
			if(-2 != entry_t.index){ // index == -2 marks end of one data section
				index = entry_t.index;
				tEntry *pEntry_t = new tEntry(entry_t);
				entry_ls.push_back(pEntry_t);
			}
			else break;
		} // entry assignment for the data section complete
		// correct time from cycle-clear
		const double bunchIdTime = (entry_t.bunchId & 0x7FF) * 25.;
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
		#include "TAEventProcessor/fill_pre.C" // fill hists and trees before tracking
		// *** recognize patterns and assign raw tracks to fTrackList *** //
		Analyze();
		#include "TAEventProcessor/fill_post.C" // fill hists and trees after tracking
		cntSec++;
	} // end while over treeData entries
	#include "TAEventProcessor/write.C" // write all the ROOT objects necessary.

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
// correct drift time and refit with the update
void TAEventProcessor::RefineTracks(int &n3Dtr, t3DTrkInfo *trk3DIf, const double *tof2, const double *taHitX){
	if(!IsPID()) return; // PID is not on, so beta is unavailable
//		TAPopMsg::Warn("TAEventProcessor", "RefineTracks: PID is not on, so beta is unavailable");
	vector<tTrack *> &tl = GetTrackList();
	const int ntr = tl.size(); if(!ntr) return;

	static TAParaManager::ArrDet_t &decv = GetParaManager()->GetDetList();
	static TAMWDCArray *dcArrV[2] = {(TAMWDCArray*)decv[3], (TAMWDCArray*)decv[4]};
	// identify 3D tracks and start track refinement
	static const int ntrMax = 200, ntr3DMax = ntrMax/3;
	bool isDCArrR[ntr3DMax];
	int n3DtrXUV[3]{}, cntTrk = 0, cntSec = 0;
	int trkId[ntr3DMax][3]; memset(trkId, -1, sizeof(trkId)); // track id [3D track id][XUV]
	// loop over grouped track projections
	for(int j = 0; j < ntr; j++) if(tl[j]->id != -1){
		isDCArrR[j] = bool((tl[j]->type/10)%10); // 0: L; 1: R
		for(int k = 0; k < 3; k++){ // loop over X-U-V track types
			if(tl[j]->type%10 == k){
				trkId[tl[j]->id][k] = j;
				n3DtrXUV[k]++;
			}
		} // end loop over X-U-V track types
	} // end for over j and if
	if(n3DtrXUV[0] != n3DtrXUV[1] || n3DtrXUV[0] != n3DtrXUV[2]){
		TAPopMsg::Error("TAT0CaliDCArr", "Refine_DTHisto: This is odd... track projections of X, U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2]);
	} // end if
	n3Dtr = n3DtrXUV[0];
	// // // ^^^^^^^ circulation over 3-D tracks in one data section ^^^^^^^ // // //
	for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section
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
		TAMWDCArray *dcArr = dcArrV[isDCArrR[trkId[jj][0]]];
		double phiAvrg = dcArr->GetPhiAvrg();
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
					// t = T_tof + T_wire + T_drift + T0
					// substract T_wire and T_tof from the time measurement
					trk->t[j] -= TACtrlPara::T_tofDCtoTOFW(uid) - TACtrlPara::T_wireMean(uid); // recover the rough correction of time of flight from DC to TOF wall for a refined one.
					dcArr->DriftTimeCorrection(trk->t[j], trk->r[j], anodeId[tmp], trkVec, trk->firedStripId, trk->beta);
					rr[tmp] = trk->r[j];
					tmp++;
				} // end if
			} // end for over j
		} // end for over l
		// fit the track with the new drift time and drift distance
		TAMath::BFGS4(Ag, ag, trkVec, rr, nF);
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
				} // end if
			} // end for over j
		} // end for over l
		trk3DIf[jj].Chi = sqrt(trk3DIf[jj].chi2/nF);
		trk3DIf[jj].k1 = trkVec[0]; trk3DIf[jj].b1 = trkVec[2];
		trk3DIf[jj].k2 = trkVec[1]; trk3DIf[jj].b2 = trkVec[3];
		trk3DIf[jj].isDCArrR = isDCArrR[jj];
		trk3DIf[jj].tof2 = tof2[trkId[jj][0]];
		trk3DIf[jj].taHitX = taHitX[trkId[jj][0]];
	} // end for over jj
} // end of member function RefineTracks
// refine PID using the refined 3D track information
void TAEventProcessor::RefinePID(const int n3Dtr, const t3DTrkInfo *trk3DIf, t3DPIDInfo *pid3DIf){
	if(!IsPID()) return; // PID is not desired
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



