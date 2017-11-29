///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TACtrlPara.C													     //
//   TACtrlPara.h -- source file for class TACtrlPara							     //
//   Introduction: mainly for holding global parameters used for program control in  //
// tracking precision level and tracking procedures.								 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/7.															     //
// Last modified: 2017/11/20, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TFile.h"

#include <cstring>
#include "TACtrlPara.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TAAnodePara.h"

TACtrlPara *TACtrlPara::kInstance = nullptr;

TACtrlPara *TACtrlPara::Instance(){
	if(!kInstance) kInstance = new TACtrlPara();
	return kInstance;
}
TACtrlPara::~TACtrlPara(){
	if(!kInstance) delete kInstance;
	kInstance = nullptr;
}

bool TACtrlPara::IsCheckBunchIdAlignment(){ return true; }
bool TACtrlPara::IsDriftTimeQtCorrection(){ return false; }
bool TACtrlPara::IsCoarseFit(){ return kIsCoarseFit; }
bool TACtrlPara::Is3DTracking(){ return kIs3DTracking; }
// tolerance window for 3D coincidence test of X U and V track projections
// 5: half a DC cell, given all kinds of errors
double TACtrlPara::Get3DCoincideWindow(){ return 10.; }
double TACtrlPara::D2Thre(){ return 300.; } // for eliminating falsely fired andoes. BINGO unit: mm^2
bool TACtrlPara::TimeThre(double t){ return t > -40. && t < 400.; }
double TACtrlPara::Beta(){ return 0.65; } // central beam energy
	// threshold for chi per dot, to eliminate false combinations. 4.0
double TACtrlPara::ChiThrePD(){ return 7.5; }
int TACtrlPara::Vicinity(){ return 5; } // used in discerning multiple tracks, unit: cell
	// TATrack::kBFGSFit; // kNormalFit: 0; kBFGSFit: 1 kIterFit: 2
int TACtrlPara::FitMethod(){ return 1; }
	// only effective if input fit method is kNormalFit
	// allowed value: -2, -1, 0, 1, 2, 3, with calculation depth increasing. unit: mm^2
int TACtrlPara::Precision(){ return 2; }
	// MWDCArrayR_DC1_U: installation error
double TACtrlPara::DCArrR_DC1UHorizontalDeviation(){ return 4.; }

void TACtrlPara::GetNStripStrayRangeR(double &minR, double &maxR) const{
	minR = kNStripStrayMinR; maxR = kNStripStrayMaxR; // DCArrayR
}
void TACtrlPara::GetNStripStrayRangeL(double &minL, double &maxL) const{
	minL = kNStripStrayMinL; maxL = kNStripStrayMaxL; // DCArrayL
}
double TACtrlPara::DsquareThresholdPerDot(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TACtrlPara", "GetTOFWallStripDelay: Not an MWDC array");
	static double d2Thre[2] = {200., 200.};
	return d2Thre[type[0] - 3];
}
// calculate the minmum deviation of a track off the fired strips in a TOF wall
bool TACtrlPara::TOFWallStripStrayTest(double strayMin, unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid);
	int detId = type[0];
	if(3 == detId) // MWDC array L
		return strayMin > kNStripStrayMinL && strayMin < kNStripStrayMaxL;
	else if(4 == detId) // MWDC array R
		return strayMin > kNStripStrayMinR && strayMin < kNStripStrayMaxR;
	else{
		TAPopMsg::Error("TACtrlPara", "TOFWallStripStrayTest: invalid detId: %d", detId);
		return false;
	}
}
double TACtrlPara::T_tofDCtoTOFW(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC array");
	if(type[1] >= 3)
		TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC");

	// DC0-1-2, include [dcArr][DC]
	static const double ccT_tofRDCtoTOFW[2][3] = {
		{5., -1., 4.},
		{-1., -3.5, -3.4}
	};
	return ccT_tofRDCtoTOFW[type[0]-3][type[1]];
}
double TACtrlPara::T_wireMean(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC array");
	if(type[1] >= 3)
		TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC");
	if(type[2] >= 3)
		TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not a valid MWDC type[XUV]");

	// [dcArr][DC012][XUV]
	static const double ccT_wireMean[2][3][3] = 
		{ {{1.5, 1.5, 1.5}, {1.5, 1.5, 1.5}, {1.5, 1.5, 1.5}},
		  {{1.5, 1.5, 1.5}, {1.5, 1.5, 1.5}, {1.5, 1.5, 1.5}}
		};
	return ccT_wireMean[type[0]-3][type[1]][type[2]];
}


const char *TACtrlPara::DataFileName() const{
	if("" == kDataFile) TAPopMsg::Warn("TACtrlPara", "DataFileName: not assigned");
	return kDataFile.c_str();
}
const char *TACtrlPara::RootFileName() const{
	if("" == kRootFile) TAPopMsg::Warn("TACtrlPara", "RootFileName: not assigned");
	return kRootFile.c_str();
}
const char *TACtrlPara::ConfigExpDir() const{
	if("" == kConfigExpDir) TAPopMsg::Warn("TACtrlPara", "ConfigExpDir: not assigned");
	return kConfigExpDir.c_str();
}
const char *TACtrlPara::STRROOTFile() const{
	if("" == kSTRROOTFile) TAPopMsg::Warn("TACtrlPara", "STRROOTFile: not assigned");
	return kSTRROOTFile.c_str();
}

void TACtrlPara::SetConfigExpDir(const string &dir){
	kConfigExpDir = dir;
}
void TACtrlPara::SetSTRROOTFile(const string &file){
	kSTRROOTFile = file;
}
void TACtrlPara::AssignSTR(TAAnodePara *para) const{
	static const int HVopt[2][3][3] = { // [DCarrL-R][DC0-1-2][XUV]
		// -1: no HV is applied; 0-5 -> HV[0-5] array element
		{ {3, -1, 3}, {2, 2, 2}, {4, 4, 4} }, // DCArrL, DC0-1-2 - X U V // 20171028 ->
		{ {3, -1, 3}, {3, 3, 3}, {3, 3, 3} }, // DCArrR, DC0-1-2 - X U V // 20171028 ->
	};
	// ----------------- set STRs ---------------------- //
	// use base STRs from garfield simualtion results
	static TFile *f = new TFile(kSTRROOTFile.c_str());
	static const int nAngBin = TAAnodePara::kSTRCorAngleNBins, nHV = 5;
	// HV: 900V, 1000V, 1300V, 1350V, 1500V
	static TF1 *rt[nHV][nAngBin]{0}; // [HVs] [six track-cell angle intervals]
	static TF1 *rtDumb = (TF1*)f->Get("RTDumb"); // constant zero
	static int HV[nHV] = {900, 1000, 1300, 1350, 1500}; // V
	static char name[64];
	if(!rt[0][0]){
		for(int i = 0; i < nHV; i++){
			for(int j = 0; j < nAngBin; j++){
				sprintf(name, "%d/RT%d", HV[i], j);
				rt[i][j] = (TF1*)f->Get(name);
			}
		}
		f->Close();
	} // end if not assigned
	unsigned uid = para->GetUID();
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 == type[0] || 4 == type[0]){ // MWDC array
			if(type[1] < 0 || type[1] > 2) TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCid: type[1]: %d", type[1]);
			if(type[2] < 0 || type[2] > 2) TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCType: type[2]: %d", type[2]);
			const int hv = HVopt[type[0]-3][type[1]][type[2]];
			if(hv >= nHV) TAPopMsg::Error("TACtrlPara", "AssignSTR: hv id too large: hv: %d", hv);
		for(int i = 0; i < nAngBin; i++){
			if(hv < 0) para->SetSTR(rtDumb, i);
			else para->SetSTR(rt[hv][i], i);
		}
	}
	else TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCArrId: type[0]: %d", type[0]);
} // end of member function AssignSTR

TACtrlPara::TACtrlPara() : kIsCoarseFit(false), kIs3DTracking(false){
	// for TATOFWall::GetTime().
	// count of strips from a fired strips to the fitted track. TATOFWall.C
	kNStripStrayMinR = -0.8, kNStripStrayMaxR = 0.8; // -0.95, 0.20
	kNStripStrayMinL = -0.8, kNStripStrayMaxL = 0.8; // -0.90, 0.65

	//---------  for pattern recognition -------------------//
	// for map function

	kDriftTimeQtCorrectionWeight = 6.0;
	// threshold for chi. (sqrt(chi2 / nFiredAnodeLayer)) unit: mm map.C 1.0 1.5
	kChiThre = 0.9 * ChiThrePD() * (IsDriftTimeQtCorrection() ? kDriftTimeQtCorrectionWeight : 1.);

	kDataFile = ""; // for extra use
	// 20161123_2247.dat.root merged.root 20161125_2046.dat.root merged_M.root _SIM.root -- the clip board
	// the directory name in config directory holding the selected experiment config files
	kConfigExpDir = "";
	// root file containing TF1 objects for MWDCs
	kSTRROOTFile = "";
	// used in refined fit and calibration phase //  20161126_0522.dat.root
	kRootFile = "";
}


