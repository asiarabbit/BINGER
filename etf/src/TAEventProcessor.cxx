///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAEventProcessor.cxx																							 //
//   TAEventProcessor.cxx -- source file for class TAEventProcessor									 //
//   Introduction: The global event processor class, a singleton class, responsible  //
// for distribution of channel data to the detector hierarchy and the right channel	 //
// objects. After the event is fully assigned, event assembly routine would be			 //
// initiated. It is also this class' duty to provide analysis result output					 //
// interface.																																				 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.cxxom.																		     //
// Created: 2017/10/13.																													     //
// Last modified: 2018/10/16, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
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
#include "TAMWDCArrayM.h" // derived from TAMWDCArrayR
#include "TASiPMPlaArray.h"
#include "TAMWDCArrayU.h"
#include "TAMWDCArrayD.h"
#include "TAPDCArrayU.h" // derived from TAMWDCArray2
#include "TAPDCArrayD.h"
#include "TAMUSICM.h"
#include "TAMUSICL.h"
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
#include "readVME.h"
#include "TAOpticFiberArray.h"
#include "TAPDCArrayTa4.h"
#include "TATrackTa4.h"

using std::cout;
using std::endl;
using std::flush;
using std::setw;

//#define DEBUG
#define GO // do the filling of ROOT objects
//#define VERBOSE // show TAPopMsg::Info() information
//#define SHOW_ENTRY
//#define SHOW_TRACK // show track projection information

TAEventProcessor *TAEventProcessor::fInstance = nullptr;

TAEventProcessor::TAEventProcessor(const string &datafile, int runId)
		: fIsPID(false), fIsTracking(true), fChkChId(-1),
		fRawDataProcessor(0), fParaManager(0), fCtrlPara(0),
		fVisual(0), fPID(0), fGPar(0), fPDCArrayTa4(0){
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
	if(fPDCArrayTa4){
		delete fPDCArrayTa4; fPDCArrayTa4 = nullptr;
	}
} // end the destructor
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
TAPDCArrayTa4 *TAEventProcessor::GetPDCArrayTa4() const{
	if(!fPDCArrayTa4)
		TAPopMsg::Error("TAEventProcessor", "GetPDCArrayTa4(): pointer is null.");
	return fPDCArrayTa4;
}
bool TAEventProcessor::Is3DTracking() const{
	return GetCtrlPara()->Is3DTracking();
}
void TAEventProcessor::SetConfigExpDir(const string &dir){
	GetCtrlPara()->SetConfigExpDir("../config/"+dir);
}
void TAEventProcessor::SetMagneticIntensity(double B){
	// for extrapolation from B @1000A to other B value
	TAPopMsg::Info("TAEvProsr", "SetMagneticIntensity(): Input Mag is: %f", B);
	GetPID()->SetMagneticIntensity(B);
}
// set magnetic field using MagRecrod txt file generated by the dipole for every 5s
void TAEventProcessor::SetMagPerEvent(unsigned clock){
	static const char *PXIDataFileName = GetRawDataProcessor()->GetPXIDataFileName();
	if(!strcmp("", PXIDataFileName)) return; // PXI data file not exist

	static TTree *tree = nullptr;
	static bool valid = true;
	static float BArr[1][1][9][24][60][12]{}; // y-m-d--h-min-5s
	if(!tree && valid){
		string MagROOTFile = "/magTree.root";
		char tmp[64]; strcpy(tmp, GetCtrlPara()->ConfigExpDir());
		MagROOTFile = tmp + MagROOTFile;
		if(access(MagROOTFile.c_str(), F_OK))
			TAPopMsg::Error("TAEventProcessor", "SetMagPerEvent(): %s does not exist.",
				MagROOTFile.c_str());
		TFile *f = new TFile(MagROOTFile.c_str());
		tree = (TTree*)f->Get("treeMag");
		if(!tree){
			valid = false;
			return;
		}
		double B, T; // magnetic/Tesla; Temperature/C;
		int y, m, d, h, min, s; // time/s
		tree->SetBranchAddress("B", &B);
		tree->SetBranchAddress("T", &T);
		tree->SetBranchAddress("y", &y);
		tree->SetBranchAddress("m", &m);
		tree->SetBranchAddress("d", &d);
		tree->SetBranchAddress("h", &h);
		tree->SetBranchAddress("min", &min);
		tree->SetBranchAddress("s", &s);
		const int n = tree->GetEntries();
		for(int i = 0; i < n; i++){
			tree->GetEntry(i);
			BArr[y-2018][m-7][d-6][h][min][(s+1)/5-1] = B;
		}
//		cout << "Mark 1" << endl; // DEBUG
	} // end outer if
	if(!valid) return;
	static int y0 = -1, m0, d0, h0, min0; // the time the PXI data file was acquired
	static int s0 = 48; // extracted from VME comment

	char tmp[64]; strcpy(tmp, PXIDataFileName);
	if(-1 == y0){
		sscanf(basename(tmp), "%4d%2d%2d_%2d%2d", &y0, &m0, &d0, &h0, &min0);
	}

	// FIXME: has to assume that the data was taken within the same month, and won't last longer than a day //
	int dh = (clock%86400)/3600, dmin = (clock%3600)/60, ds = clock%60;
	int s = s0+ds, min = min0+dmin, h = h0+dh, d = d0;
	min += s/60; s %= 60;
	h += min/60; min %= 60;
	d += h/24; h %= 24;
	double resultB = BArr[y0-2018][m0-7][d-6][h][min][(s+1)/5-1];


//	cout << "y0: " << y0 << "\tm0: " << m0 << "\td0: " << d0 << endl; // DEBUG
//	cout << "h0: " << h0 << "\tmin0: " << min0 << "\ts: " << s << endl; // DEBUG
//	cout << "h0+dh: " << h0+dh << "\tmin0+dmin: " << min0+dmin << "\t(s+1)/5-1: " << (s+1)/5-1 << endl; // DEBUG
//	cout << "resultB: " << resultB << "\tclock: " << clock << endl; getchar(); // DEBUG

	SetMagneticIntensity(-resultB);
} // end of member function SetMagPerEvent
void TAEventProcessor::SetIs3DTracking(bool opt){
	SetIsTracking(true);
	fCtrlPara->SetIs3DTracking(opt);
}
void TAEventProcessor::SetSTRROOTFile(const string &file){
	string STRFile = "/STR/"+file;
	char tmp[64]; strcpy(tmp, GetCtrlPara()->ConfigExpDir());
	STRFile = dirname(tmp) + STRFile;
	GetCtrlPara()->SetSTRROOTFile(STRFile);
}
void TAEventProcessor::SetDataFile(const string &datafile, int runId, bool isPXI){
	if(!strcmp("", datafile.c_str()))
		TAPopMsg::Error("TAEventProcessor", "SetDataFile: Input datafile name is empty");
	const char c = datafile.c_str()[0];
	string name;
	if('/' == c || '.' == c) name = datafile; // data file with its path specified
	else name = "../data/"+datafile;
	GetRawDataProcessor()->SetDataFileName(name, runId, isPXI);
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
	// select an experiment, to direct to a directory containing the exp config parameters
	const char dir[8][64] = {"pion_2017Oct", "beamTest_2016Nov", "C16_Exp_2018_Summer", // 0-1-2
						"tripletDC_P_Ma_Test", "tripletDC_P_Ma_Test_ETF", "OpticFiber", // 3-4-5
							"C16_Exp_2018_July_18O", "C16_Exp_2018_July_16C"}; // 6-7
	const char *sdir = dir[7];
	TAPopMsg::Info("TAEventProcessor", "Configure: selected Exp Config Dir: %s", sdir);
	SetConfigExpDir(sdir);
	// STR_spline.root || STR_stiff.root || STR_aaa900.root
	SetSTRROOTFile("STR_spline.root"); // space-time relations for MWDCs
	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	// read the global parameters array first and only; type: 004
	const short nignore = 4, typeignore[nignore] = {0, 1, 2, 3};
	GetParaManager()->ReadParameters(nignore, typeignore);
	// NOTE THAT THE SUBSCRIPT FOR EACH DETECTOR CANNOT in ANY CIRCUMSTANCES BE ALTERED //
	// note that the detector UID has to be equal to the array detList subscript
	detList[0] = new TAT0_0("T0_0", "T0_0@Mid-RIBLL2", 0); // shutdown: FORBIDDEN
	detList[1] = new TAT0_1("T0_1", "T0_1@End-RIBLL2", 1); // shutdown: FORBIDDEN
//	detList[2] = new TASiPMPlaArray("SiPMPlaArray", "SiPMPlaArray@Post-Target", 2);
//	detList[3] = new TAMWDCArrayL("DCArrayL", "DCArrayL@Post-Magnet", 3);
	detList[4] = new TAMWDCArrayR("DCArrayR", "DCArrayR@Post-Magnet", 4);
//	detList[4] = new TAMWDCArrayM("DCArrayM", "DCArrayM@P.Ma_TEST", 4);
//	detList[5] = new TASiPMPlaBarrel("SiPMPlaBarrel", "SiPMPlaBarrel@Hug-Target", 5);
//	detList[6] = new TAMWDCArrayU("DCArrayU", "DCArrayU@Pre-Target", 6);
//	detList[7] = new TAMWDCArrayD("DCArrayD", "DCArrayD@Post-Target", 7);
	detList[8] = new TAPDCArrayU("PDCArrayU", "PDCArrayU@Pre-Target", 8);
	detList[9] = new TAPDCArrayD("PDCArrayD", "PDCArrayD@Post-Target", 9);
//	detList[10] = new TAMUSICM("MUSICM", "MUSICM@Pre-Target", 10);
//	detList[11] = new TAMUSICL("MUSICL", "MUSICL@Post-Target", 11);
//	detList[12] = new TAT0_1("VETO_0", "VETO_0@Pre-MSUICF", 12);
//	detList[13] = new TAT0_1("VETO_1", "VETO_1@Post-MSUICF", 13);
	detList[14] = new TAT0_0("T0_0_VME0", "T0_0_VME0@Mid-RIBLL2", 14); // for PDCArrU
	detList[15] = new TAT0_1("T0_1_VME0", "T0_1_VME1@End-RIBLL2", 15); // for PDCArrU
	detList[16] = new TAT0_0("T0_0_VME1", "T0_0_VME0@Mid-RIBLL2", 16); // for PDCArrD
	detList[17] = new TAT0_1("T0_1_VME1", "T0_1_VME1@End-RIBLL2", 17); // for PDCArrD
//	detList[18] = new TAMUSICM("Si", "Si@Post-Target", 18);
	detList[19] = new TAOpticFiberArray("OpticFiberArray", "OpticFiberArray", 19);

	for(TADetUnion *&p : detList) if(p) p->Configure(); // build the detectors
	// time start for DCArrU-D is TAT0_1
	TAT0_1 *str_t0_1 = (TAT0_1*)detList[1];
	TAT0_1 *str_t0_1_0 = (TAT0_1*)detList[15]; // v1190 - slot_9
	TAT0_1 *str_t0_1_1 = (TAT0_1*)detList[17]; // v1190 - slot_11
	if(!str_t0_1) TAPopMsg::Error("TAEvProsr", "Configure: T0_1 is nullptr");
	if(detList[3]) ((TAMWDCArray*)detList[3])->SetPlaT0(str_t0_1);
	if(detList[4]) ((TAMWDCArray*)detList[4])->SetPlaT0(str_t0_1);
	if(detList[6]) ((TAMWDCArray2*)detList[6])->SetPlaT0(str_t0_1);
	if(detList[7]) ((TAMWDCArray2*)detList[7])->SetPlaT0(str_t0_1);
	if(detList[8]) ((TAMWDCArray2*)detList[8])->SetPlaT0(str_t0_1_0); // PDCArrU
	if(detList[9]) ((TAMWDCArray2*)detList[9])->SetPlaT0(str_t0_1_1); // PDCArrD // should be 1_1 for beam exp
	// for P. Ma's test
	bool isPMaTest = false;
	if(isPMaTest && detList[4]){
		TATOFWall *tofw = ((TAMWDCArrayM*)detList[4])->GetTOFWall();
		vector<TAPlaStrip *> &stripArr = tofw->GetStripArr();
		if(!stripArr.size()){ // strip array is empty
			tofw->SetNStrip(1);
			stripArr.push_back((TAPlaStrip*)str_t0_1);
		}
		if(!strcmp("tripletDC_P_Ma_Test_ETF", sdir)) str_t0_1->SetIsSingleEnd(true);
	} // end configuration of TOFWall for P. Ma's Test

	// read all the parameters required and assign positiion parameters to every channel and alike
	GetParaManager()->ReadParameters();

	if(IsPID()) GetPID()->Configure();
	// TAVisual::Configure can only be implemented AFTER all the other detectors are created
	GetVisual()->Configure();
	// show some information
	if(TAPopMsg::IsVerbose()){
		if(detList[3]) ((TAMWDCArray*)detList[3])->Info();
		if(detList[4]) ((TAMWDCArray*)detList[4])->Info();
		if(detList[6]) ((TAMWDCArray2*)detList[6])->Info();
		if(detList[7]) ((TAMWDCArray2*)detList[7])->Info();
		if(detList[8]) ((TAMWDCArray2*)detList[8])->Info();
		if(detList[9]) ((TAMWDCArray2*)detList[9])->Info();
	}
	isCalled = true; // has been called
	CheckChannelId();

	fPDCArrayTa4 = TAPDCArrayTa4::Instance();
} // end of member function Configure
// check the the channel with channelId being chId
void TAEventProcessor::CheckChannelId() const{
	if(fChkChId < 0) return;
	static const TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	int uid = GetParaManager()->GetUID(fChkChId);
	if(TAParaManager::UID_DUMMY == uid){ // entry with homeless channel id
		TAPopMsg::Info("TAEventProcessor",
			"CheckChannelId: \033[31mDUMMY_CHANNEL\033[0m");
		return;
	}
	if(TAParaManager::UID_DUMMY == uid){ // entry with uninitialized channel id
		TAPopMsg::Info("TAEventProcessor",
			"CheckChannelId: \033[31mEMPTY_CHANNEL\033[0m");
		return;
	}
	int type[6]{}; TAUIDParser::DNS(type, uid); int detId = type[0]; // resolute UID

	if(detList[detId]) detList[detId]->GetChannel(uid)->Info();
	else TAPopMsg::Warn("TAEventProcessor", "CheckChannelId: Detector List \
element #%d is null.", detId);
} // end of member function CheckChannelId
void TAEventProcessor::BunchIdMisAlignCheck(bool opt) const{
	GetRawDataProcessor()->SetBunchIdCheck(opt);
}

// assign an event to the detectors by distributing channel data to the matching channel objects
void TAEventProcessor::Assign(){
	// special treatment for T0_1 with single-end readout
	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	static TAPlaStrip *T0_1 = (TAT0_1*)(detList[1]);
	if(T0_1->IsSingleEnd()){
		static int chId[4]{};
		if(!chId[0]){ // UV-UH-DV-DH
			chId[0] = T0_1->GetUV()->GetPara()->GetChannelId();
			chId[1] = T0_1->GetUH()->GetPara()->GetChannelId();
			chId[2] = T0_1->GetDV()->GetPara()->GetChannelId();
			chId[3] = T0_1->GetDH()->GetPara()->GetChannelId();
		} // end if(!chid[0])
		for(const tEntry *e : fEntryList){
			bool DVF = false, UVF = false; // if D(U)V ch fired
			if(chId[0] == e->channelId){ // U channels are Daq-ed
				UVF = true;
				// fabricate a DV entry stuffed with UV data
				tEntry *eDV = new tEntry(*e);
				eDV->channelId = chId[2]; // change chId to DV's
				fEntryList.push_back(eDV);
			}
			if(chId[2] == e->channelId){ // D channels are Daq-ed
				DVF = true;
				// fabricate a UV entry stuffed with DV data
				tEntry *eUV = new tEntry(*e);
				eUV->channelId = chId[0]; // change chId to UV's
				fEntryList.push_back(eUV);
			}
			if(DVF && UVF)
				TAPopMsg::Error("TAEventProcessor", "Assign(): T0_1 singe-end readout, \
while DV and UV are both fired");
		} // end for over entries
	} // end if(T0_1->IsSingleEnd())

	for(tEntry *&e : fEntryList) Assign(e);

} // end assign
void TAEventProcessor::Assign(tEntry *entry){

	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	int uid = GetParaManager()->GetUID(entry->channelId);
	if(TAParaManager::UID_DUMMY == uid){ // entry with homeless channel id
		strcpy(entry->name, "\033[31mDUMMY_CHANNEL\033[0m");
		return;
	}
	if(TAParaManager::UID_DUMMY * 2 == uid){ // entry with uninitialized channel id
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
	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	static TAMWDCArray *dcArr[2] = {(TAMWDCArray*)detList[3], (TAMWDCArray*)detList[4]};
	static TAMWDCArray2 *dcArr2[2] = {(TAMWDCArray2*)detList[6], (TAMWDCArray2*)detList[7]};
	// static TAMWDCArray2 *pdcArr2[2] = {(TAMWDCArray2*)detList[8], (TAMWDCArray2*)detList[9]};
	static TAPDCArrayTa4 *pdcArrayTa4 = TAPDCArrayTa4::Instance();
	for(int i = 2; i--;){
		if(dcArr[i]) dcArr[i]->FillTrack(gTrack, gTrack_R);
		if(dcArr2[i]) dcArr2[i]->FillTrack(gTrack, gTrack_R);
		// if(pdcArr2[i]) pdcArr2[i]->FillTrack(gTrack, gTrack_R);
	}
	if(pdcArrayTa4) pdcArrayTa4->FillTrack(gTrack, gTrack_R);
} // end member function FillTrack
void TAEventProcessor::Initialize(){
	for(tEntry *&t : fEntryList) if(t){ delete t; t = nullptr; }
	fEntryList.clear();
	for(tTrack *&t : fTrackList) if(t){ delete t; t = nullptr; }
	fTrackList.clear();

	for(TADetUnion *&det : GetParaManager()->GetDetList()){
		if(det) det->Initialize();
	}
	if(IsPID()) GetPID()->Initialize();
	if(fPDCArrayTa4) fPDCArrayTa4->Initialize();
}


// -- Analyze and Run method -- //
// analyze one event after its channels data are assigned to detectors
static const double c0 = TAParaManager::Instance()->GetPhysConst("c0");
void TAEventProcessor::Analyze(){
	if(!IsTracking()) return;

	static TAParaManager::ArrDet_t &detList = GetParaManager()->GetDetList();
	static TAMWDCArrayL *dcArrL = (TAMWDCArrayL*)detList[3];
	static TAMWDCArrayR *dcArrR = (TAMWDCArrayR*)detList[4];
	// XXX: should be changed to R for 16C
//	static TAMWDCArrayR *dcArrR = dynamic_cast<TAMWDCArrayM*>(detList[4]);
//	if(!dcArrR && detList[4]) TAPopMsg::Error("TAEvPsr", "Analyze: DCArrD not TAMWDCArrayM object.");
	static TAMWDCArrayU *dcArrU = (TAMWDCArrayU*)detList[6];
	static TAMWDCArrayD *dcArrD = (TAMWDCArrayD*)detList[7];
	static TAPDCArrayU *pdcArrU = (TAPDCArrayU*)detList[8];
	static TAPDCArrayD *pdcArrD = (TAPDCArrayD*)detList[9];

	// pattern recognition and rough fit for particle tracking
	//  XXX THE FOLLOWING ORDER CANNOT BE MESSED UP WITH XXX  //
	if(dcArrL){ dcArrL->Map(); dcArrL->AssignTracks(fTrackList); }
	if(dcArrR){ dcArrR->Map(); dcArrR->AssignTracks(fTrackList); }
	if(dcArrU){ dcArrU->Map(); dcArrU->AssignTracks(fTrackList); }
	if(dcArrD){ dcArrD->Map(); dcArrD->AssignTracks(fTrackList); }
	if(pdcArrU){ pdcArrU->Map(); pdcArrU->AssignTracks(fTrackList); }
	if(pdcArrD){ pdcArrD->Map(); pdcArrD->AssignTracks(fTrackList); }
	// assign and output beta and index
	int index = GetEntryList()[0]->index;
	int n3DTrkR = 0;
	if(dcArrR) n3DTrkR = dcArrR->GetN3DTrack(); // number of 3D tracks in DCArrR
	for(tTrack *&t: fTrackList){
		t->index = index;
		// rearrange trkid to make it global and unique
		if(0 == t->type/10%10) // MWDCArray L
			if(-1 != t->id) t->id += n3DTrkR;
#ifdef SHOW_TRACK
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
} // end of inline function correctCycleClear
// the overall data analysis routine
// (id0, id1): index range for analysis; secLenLim: event length limit; rawrtfile: raw rootfile
void TAEventProcessor::Run(int id0, int id1, int secLenLim, const string &rawrtfile){
	secLenLim = secLenLim < 2000 ? secLenLim : 2000; // 2000 word (7.8KiB) is the event length limit
	if(id0 >= id1)
		TAPopMsg::Error("TAEventProcessor", "Run: index0 %d is not smaller than index1 %d", id0, id1);
	string rootfile = rawrtfile; // name of the ROOT file storing raw data - treeData
	if(!strcmp(rawrtfile.c_str(), "")){ // no input rootfile
		GetRawDataProcessor()->SetPeriod(id0, id1);
		GetRawDataProcessor()->ReadOffline(); // prepare data file
		rootfile = GetRawDataProcessor()->GetROOTFileName();
		printf("\033[32;1m  Analyzing event#%d to event#%d from datafile  %s and %s\n\033[0m", id0, id1,
			GetRawDataProcessor()->GetPXIDataFileName(), GetRawDataProcessor()->GetVMEDataFileName());
	} // end if(strcmp...)
	else printf("\033[32;1m  Analyzing event#%d to event#%d from rootfile   %s\n\033[0m", id0, id1, rootfile.c_str());
//	return;
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TAEventProcessor", "Run: %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	TTree *treeData[2]{}; // [0-1]: [PXI-VME]
	treeData[0] = (TTree*)f->Get("treeData");
	treeData[1] = (TTree*)f->Get("treeDataVME");
	TTree *treeSCA = (TTree*)f->Get("treeSCA");
	TTree *vme = (TTree*)f->Get("vme");
	if(!treeData[0] && !treeData[1])
		TAPopMsg::Error("TAEventProcessor", "Run: Obtained PXI and VME treeData-s are null pointers");
	tEntry entry_t;
	for(TTree *tree : treeData) if(tree){
		tree->SetBranchAddress("index", &entry_t.index);
		tree->SetBranchAddress("nl", &entry_t.nl);
		tree->SetBranchAddress("nt", &entry_t.nt);
		tree->SetBranchAddress("channelId", &entry_t.channelId);
		tree->SetBranchAddress("leadingTime", entry_t.leadingTime);
		tree->SetBranchAddress("trailingTime", entry_t.trailingTime);
		tree->SetBranchAddress("is_V", &entry_t.is_V);
		tree->SetBranchAddress("bunchId", &entry_t.bunchId);
	} // end loop over tree pointers
	unsigned sca[16];
	if(treeSCA) treeSCA->SetBranchAddress("sca", sca); // scaler data
	tVME_event evt;
	if(vme){
		vme->SetBranchAddress("adc", evt.adc);
		vme->SetBranchAddress("qdc", evt.qdc[0]);
		vme->SetBranchAddress("mtdc0", evt.mtdc0);
		vme->SetBranchAddress("mtdc1", evt.mtdc1);
		vme->SetBranchAddress("sca", evt.sca);
		vme->SetBranchAddress("dsca", evt.dsca);
	} // end if(vme)

	vector<tEntry *> &entry_ls = GetEntryList();
	vector<tTrack *> &track_ls = GetTrackList();
	TAPDCArrayTa4 *pdcArrayTa4 = GetPDCArrayTa4();

	// read rootfile and assembly each event
	int nPXI = 1, nVME = 1; // N of entries in the treeData for both PXI and VME daq systems
	if(treeData[0]) nPXI = treeData[0]->GetEntries();
	if(treeData[1]) nVME = treeData[1]->GetEntries();
	int cntTrk = 0, cnt3DTrk = 0; // ntr: n trk per event; cntTrkY: Y tracks from (P)DCTa
	int cntTrkX = 0, cntTrkY = 0; // X: for DCArrL-R-U-D; Y: for DCArrU-D
	int dcntTrkX = 0, dcntTrkY = 0; // the increment
	int cntaozWrong = 0, cntaoz = 0;
	int entryId[2]{}; // entry id iterators for the two treeData-s, [0-1]: [PXI-VME]
	int index, cntSec = 0;
	// ntr: N of trk in DCArrLR; ntrT: N of trk in DCArrLR+UD
	int ntr = 0, ntrT = 0;
	int n3DtrLs[6]{}, ntrLs[6][3]{}; // total N of TrkProjs; DCArr-L-R-U-D-PDCU-D -- [XUV - XY]
	const int ntrMax = 100; // maximum number of track projections in an event
#ifdef GO
	#include "TAEventProcessor/define_hist.cxx" // define histograms of interest
	#include "TAEventProcessor/define_tree.cxx" // define the track tree
#endif
	mag = pid->GetExtMag();
	cout << std::left;
	cout << setw(10) << "index" << setw(10) << "nEv" << setw(10) << "trkX" << setw(10) << "totTrk";
	cout << setw(10) << "3Dtrk" << setw(10) << "naoz" << setw(10) << "naozBad" << endl;
//	return; // DEBUG
	while(entryId[0] < nPXI && entryId[1] < nVME){
		Initialize(); // clear everything from last data section
		// assign all entries in a sec to fEntryList for processing
		int indext[2] = {-1, -1}, bunchId_PXI = 0; // cache index for PXI and VME
		for(int i = 0; i < 2; i++){ // loop over the two treeData-s
			TTree *tree = treeData[i];
			if(tree){
				while(1){
					entry_t.initialize();
					tree->GetEntry(entryId[i]++);
					// buncId of VME is confined to zero
					if(-2 == entry_t.index && entry_t.bunchId != 0) bunchId_PXI = entry_t.bunchId;
					if(-2 != entry_t.index){ // index == -2 marks end of one data section
						entry_ls.push_back(new tEntry(entry_t));
						indext[i] = entry_t.index;
					} // end if(-2 != entry_t.index)
					else break;
				} // entry assignment for the data section complete
			} // end if(tree)
		} // end for over i
		if(indext[1] >= 0 && indext[0] >= 0 && indext[1] != indext[0]){
			TAPopMsg::Error("TAEvProsr", "Run: PXI and VME index are not consistent.\
 PXI index: %d, VME index: %d", indext[0], indext[1]);
		}
		if(indext[0] >= 0) index = indext[0]; // use pxi index
		else if(indext[1] >= 0) index = indext[1]; // use vme index
		else TAPopMsg::Error("TAEvProsr", "Run: indexes of VME and PXI are both below 0.");

		if(treeSCA) treeSCA->GetEntry(cntSec);
		if(vme) vme->GetEntry(cntSec);
		cntSec++;
		if(0 == entry_ls.size()) continue; // empty event
		// correct time from cycle-clear
		double bunchIdTime = (abs(bunchId_PXI) & 0x7FF) * 25.;
		if(entry_t.bunchId < 0) bunchIdTime *= -1.;
//		cout << "bunchIdTime: " << bunchIdTime << endl; getchar(); // DEBUG
		if(0. != bunchIdTime) for(tEntry *t : entry_ls){
			if(0 == t->bunchId) continue; // VME event
//			t->show(); // DEBUG
			for(double &x : t->leadingTime) correctCycleClear(x, bunchIdTime);
			for(double &x : t->trailingTime) correctCycleClear(x, bunchIdTime);
		}
		if(entry_t.channelId > secLenLim) continue; // index==-2, then channelId stores secLen
		if(entry_t.channelId < 0) continue; // bunchIdMisAlignment happened
		// bool GOTTAYO = false;
		// static const int indexArrSpecial[] = { // 2020 March start again
		// 	855,   3497,  4531,  4851,  5544,  6613,  7046,  7082,  10175, 10278,
		// 	10336, 10734, 10821, 13075, 13131, 15311, 16607, 18184, 18663, 19569,
		// 	19655, 22503, 26499, 27113, 29122, 37057, 38259, 39277, 40944, 43218,
		// 	45121, 45451, 45804, 48547, 48629, 49534, 49872, 50820, 51196, 51840
		// };
		// for(const int t : indexArrSpecial) if(t == index) { GOTTAYO = true; break; }
		if(index != -1 && (index < id0)){ //  && cntSec % 10 == 0 //  || !GOTTAYO
			cout << "Skipping Event index " << index << "\r" << flush;
			continue;
		} // starting from index - id0
		if(index >= id1) break; // ending at index - id1-1
///		cout << "\n\nindex: " << index << endl; // DEBUG
		Assign(); // *** assign entries in fEntryList *** //
#ifdef SHOW_ENTRY
		cout << "\n\nindex: " << index << endl; // DEBUG
		for(auto &t : entry_ls) cout << t->name << "\t" << t->channelId << endl; // DEBUG
		getchar(); // DEBUG
#endif
//		for(auto &t : entry_ls) t->show(); // DEBUG
//		getchar(); // DEBUG
#ifdef GO
		#include "TAEventProcessor/fill_pre.cxx" // fill hists and trees before tracking
#endif
		// *** recognize patterns and assign raw tracks to fTrackList *** //
//		SetMagPerEvent(evt.sca[15]/1000);
//		mag = pid->GetExtMag();
		Analyze();
		// assgin ntr variables
		ntr = 0; ntrT = 0;
		memset(ntrLs, 0, sizeof(ntrLs));
		memset(n3DtrLs, 0, sizeof(n3DtrLs));
		dcntTrkX = 0; dcntTrkY = 0;
		for(tTrack *&t : track_ls){
			const int dcArrId = t->type / 10 % 10, dcType = t->type % 10;
			ntrLs[dcArrId][dcType]++;
			if(0 == dcArrId || 1 == dcArrId) ntr++; // ntr for MWDCArrayL-R
			if(0 == dcType) dcntTrkX++;
			if(1 == dcType && (2 == dcArrId || 3 == dcArrId)) dcntTrkY++;
		} // end for over tracks
		ntr = ntr < ntrMax ? ntr : ntrMax;
#ifdef GO
		#include "TAEventProcessor/fill_post.cxx" // fill hists and trees after tracking
#endif


		// output the tracking and pid result //
		cntTrk += ntrT; cnt3DTrk += n3DtrT;
		cntTrkX += dcntTrkX; cntTrkY += dcntTrkY;
		if(index % 1 == 0){
			cout << setw(10) << index << setw(10) << cntSec << setw(10) << cntTrkX;
			cout << setw(10) << cntTrk << setw(10) << cnt3DTrk;
			cout << setw(10) << cntaoz << setw(10) << cntaozWrong << "\r" << flush;
//			cout << "idx " << index << " nEv/trkX " << cntSec << "/";
//			cout << cntTrk - (cnt3DTrk/3)*2 << " totTrk " << cntTrk << " 3Dtrk " << cnt3DTrk / 3;
//			cout << " naoz " << cntaoz << " naozBad " << cntaozWrong << "\r" << flush; // cntaozWrong
		}
//		cout << endl << "ntrT: " << ntrT << "\tn3DtrT: " << n3DtrT << endl; getchar(); // DEBUG
	} // end while over treeData entries
	cout << std::right;
#ifdef GO
	#include "TAEventProcessor/write.cxx" // write all the ROOT objects necessary
#endif

	cout << "\n\n";
	cout << "Totally \033[33;1m" << cntSec << "\033[0m sections ";
	cout << "\033[1m" << entryId[0] << "\033[0m PXI entries \033[0m ";
	cout << "and \033[1m" << entryId[1] << "\033[0m VME entries \033[1m" << cntTrk;
	cout << "\033[0m tracks and \033[1m" << cnt3DTrk / 3;
	cout << "\033[0m 3D tracks have been processed.\n";
	cout << " cntaoz " << cntaoz << " cntaozWrong " << cntaozWrong << endl;
	cout << "The result has been written in " << rootfile << " Exiting the Run function.\n\n";
	f->Close(); delete f;
//	delete treeTrack;
} // end of member function Run
//#define DEBUG
// correct drift time and refit with the update
void TAEventProcessor::RefineTracks(int &n3Dtr, t3DTrkInfo *trk3DIf,
	const double *tof2, const double *taHitX){
	// XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX //
//	if(!IsPID()) return; // PID is not on, so beta is unavailable				// XXX XXX XXX //
	// XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX //
//		TAPopMsg::Warn("TAEventProcessor", "RefineTracks: PID is not on, so beta is unavailable");
	vector<tTrack *> &tl = GetTrackList();
	const int ntr = tl.size(); if(ntr <= 0) return;

	static TAParaManager::ArrDet_t &detv = GetParaManager()->GetDetList();
	static TAMWDCArray *dcArrV[2] = {(TAMWDCArray*)detv[3], (TAMWDCArray*)detv[4]};
	// identify 3D tracks and start track refinement
	static const int ntrMax = 100, ntr3DMax = ntrMax/3;
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
		TAPopMsg::Error("TAEventProcessor", "RefineTracks: This is odd... track projections of X,\
 U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d, ntr: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2], ntr);
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
		double Ag[nF][3], ag[nF][3], rr[nF]; // rr: drift distance in XUV form
		double anodeId[nF][2]; // [0]: fired anode layer id: 0-17; [1]: nu
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
					TAAnodePara *anoPar = ano->GetAnodePara();
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
					// recover the rough correction of time of flight from DC to TOF wall for a refined one
					trk->t[j] -= fCtrlPara->T_tofDCtoTOFW(uid) - fCtrlPara->T_wireMean(uid);
//					double beta_t[2] = {0.5, 0.6}; // for simulation test only XXX XXX XXX
					dcArr->DriftTimeCorrection(trk->t[j], trk->r[j], anodeId[tmp], trkVec,
						trk->firedStripId, 0.62); // trk->beta beta_t[isDCArrR[jj]]
					rr[tmp] = trk->r[j];
#ifdef DEBUG
					cout << "After correction,\n"; // DEBUG
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
		TAMath::BFGS4(Ag, ag, trkVec, rr, nF); // update trkVec
//		cout << "After correction,\n"; // DEBUG
//		cout << "k1: " << trkVec[0] << "\tk2: " << trkVec[1] << "\tb1: " << trkVec[2] << "\tb2: " << trkVec[3] << endl; // DEBUG

		trk3DIf[jj].initialize(); // initialization

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
		const int it = trkId[jj][0]; // the track id of jj-th 3D track's Xproj
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
//		cout << "0.p2[0]: " << p2[0] << endl; // DEBUG
		p2[0] = trkVec[0] * p2[2] + trkVec[2]; // x = k1 z + b1; // DEBUG
//		cout << "1.p2[0]: " << p2[0] << endl; // DEBUG
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
			for(int k = 0; k < 6; k++){ // loop over six anode layers in the three MWDCs
				if(tl[it]->dcTOT[k] >= GetGPar()->Val(54)){
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
//				cout << "j: " << j << "\tk: " << k << endl; // DEBUG
//				cout << "TOTAvrgtmp: " << TOTAvrgtmp << "\ttl[it]->dcTOT[k]: " << tl[it]->dcTOT[k] << endl; getchar(); // DEBUG
				if(tl[it]->dcTOT[k] >= TOTAvrgtmp*0.6){ // or it is deemed as noise
					trk3DIf[jj].dcTOTAvrg += tl[it]->dcTOT[k]; TOTcnt++;
//					cout << "trk3DIf[jj].dcTOTAvrg: " << trk3DIf[jj].dcTOTAvrg << endl; // DEBUG
//					getchar(); // DEBUG
				}
				else tl[it]->dcTOT[k] = -9999.; // 2018-01-15, slick, rule out noise-like TOTs
			} // end for over k
		} // end for voer XUV
		if(0 == TOTcnt) trk3DIf[jj].dcTOTAvrg = -9999.; // failed
		else{
			trk3DIf[jj].dcTOTAvrg /= TOTcnt; // the updated average
			trk3DIf[jj].dcTOTcnt = TOTcnt;
		}
	} // end for over jj
} // end of member function RefineTracks
// refine PID using the refined 3D track information
void TAEventProcessor::RefinePID(const int n3Dtr, const t3DTrkInfo *trk3DIf, t3DPIDInfo *pid3DIf){
	if(!IsPID() || n3Dtr <= 0) return; // PID is not desired
	TAPID *pid = GetPID();
	for(int i = 0; i < n3Dtr; i++) if(-9999. != trk3DIf[i].taHitX){
		const t3DTrkInfo &t = trk3DIf[i]; t3DPIDInfo &pi = pid3DIf[i];
		double pOut[4] = {t.k1, t.k2, t.b1, t.b2};
		pid->FlyPion(t.tof2, t.taHitX, pOut, t.isDCArrR); // TAPID::kOpt1
		pi.aoz = pid->GetAoZ();
		pi.aozdmin = pid->GetChi();
		pid->GetTargetExitAngle(pi.angTaOut);
		pi.beta2 = pid->GetBeta();
		pi.poz = pid->GetPoZ();
		pi.trkLenT = pid->GetTotalTrackLength();
	}
}
