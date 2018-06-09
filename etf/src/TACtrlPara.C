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
// Last modified: 2018/6/7, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TFile.h"

#include <cstring>
#include "TACtrlPara.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TAGPar.h"
#include "TAAnodePara.h"
#include "TAGPar.h"

TACtrlPara *TACtrlPara::kInstance = nullptr;
static const TAGPar *gp = TAGPar::Instance();
double TACtrlPara::fSigmaR = 0.2;

TACtrlPara *TACtrlPara::Instance(){
	if(!kInstance) kInstance = new TACtrlPara();
	return kInstance;
}
TACtrlPara::~TACtrlPara(){
	if(kInstance){
		delete kInstance; kInstance = nullptr;
	}
}

bool TACtrlPara::IsCheckBunchIdAlignment(){ return false; }
bool TACtrlPara::IsDriftTimeQtCorrection(){ return false; }
bool TACtrlPara::IsCoarseFit(){ return kIsCoarseFit; }
bool TACtrlPara::Is3DTracking(){ return kIs3DTracking; }
// tolerance window for 3D coincidence test of X U and V track projections
// 5: half a DC cell, given all kinds of errors
double TACtrlPara::Get3DCoincideWindow(){ return gp->Val(40); }
// used in map() as the averaged d2 over fired anodes. D2 <= nFiredAnodeLayer * D2Thre()   (PerDot)
// for eliminating falsely fired andoes. BINGO unit: mm^2
double TACtrlPara::D2Thre(unsigned uid){
	if(999999999 != uid){
		int type[6]; TAUIDParser::DNS(type, uid);
		if(6 == type[0]){ // DCArrU -> Medium sized DC with drift cell size to be 5mm
			return gp->Val(41) / 2.;
		}
		if(8 == type[0] || 9 == type[0]){ // PDC -> drift distance: 10mm
			return gp->Val(102); // 102: exclusive for PDCs
		}
	}
	return gp->Val(41);
}
bool TACtrlPara::TimeThre(double t, unsigned uid){
	if(999999999 != uid){
		int type[6]; TAUIDParser::DNS(type, uid);
		if(6 == type[0]){ // DCArrU -> Medium sized DC with drift cell size to be 5mm
			return t > gp->Val(42) && t < gp->Val(43) / 2.;
		}
		if(8 == type[0] || 9 == type[0]){ // PDC -> drift distance: 10mm
//			cout << gp->Val(103) << " " << gp->Val(104) << endl; getchar(); // DEBUG
			return t > gp->Val(103) && t < gp->Val(104); // 103, 104: exclusive for PDCs
		}
	}
	return t > gp->Val(42) && t < gp->Val(43);
}
// threshold for chi per dot, to eliminate false combinations. 4.0
double TACtrlPara::ChiThrePD(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(IsCoarseFit()){
		// 2.5 - 5. - 10.: drift cell size
		if(3 == type[0] || 4 == type[0]) return 5.; // DCArrL-R
		if(6 == type[0]) return 2.5; // DCArrU
		if(7 == type[0]) return 5.; // DCArrD
		if(8 == type[0] || 9 == type[0]) return 10.; // PDCArr-U-D
	}
	if(8 == type[0] || 9 == type[0]) return gp->Val(105); // 105: exclusive for PDCs
	return gp->Val(44);
}
// threshold for chi. (sqrt(chi2 / nFiredAnodeLayer)) unit: mm map.C 1.0 1.5
double TACtrlPara::ChiThre(unsigned uid){
		double chiThre = 0.8 * ChiThrePD(uid) * (IsDriftTimeQtCorrection() ? kDriftTimeQtCorrectionWeight : 1.);
	return chiThre;
}
int TACtrlPara::Vicinity(){
	return gp->Val(45);
} // used in discerning multiple tracks, unit: cell
int TACtrlPara::StripTolerance(){ return gp->Val(46); } // used in discerning multi-trks, unit: strip
// TATrack::kBFGSFit; // kNormalFit: 0; kBFGSFit: 1 kIterFit: 2
int TACtrlPara::FitMethod(){ return gp->Val(47); }
// only effective if input fit method is kNormalFit
// allowed value: -2, -1, 0, 1, 2, 3, with calculation depth increasing. unit: mm^2
int TACtrlPara::Precision(){ return gp->Val(48); }
// MWDCArrayR_DC1_U: installation error
double TACtrlPara::DCArrR_DC1UHorizontalDeviation(){ return 4.; }

void TACtrlPara::GetNStripStrayRangeL(double &minL, double &maxL) const{
	minL = gp->Val(49); maxL = gp->Val(50); // DCArrayL
}
void TACtrlPara::GetNStripStrayRangeR(double &minR, double &maxR) const{
	minR = gp->Val(51); maxR = gp->Val(52); // DCArrayR
}
// used in Dsquare() and refinedFit, BFGSFit, iterFit
double TACtrlPara::DsquareThresholdPerDot(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0] && 6 != type[0] && 7 != type[0] && 8 != type[0] && 9 != type[0]){
		TAPopMsg::Error("TACtrlPara", "DsquareThresholdPerDot: Not an MWDC array");
		return -9999.;
	}
	static double d2Thre[6] = {40., 40., 20., 40., 60., 60.}; // MWDC Array L-R-U-D-PDCU-D
	int dcArrId = -9999;
	if(3 == type[0] || 4 == type[0]) dcArrId = type[0] - 3; // 0-1: L-R
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0])
		dcArrId = type[0] - 6 + 2; // 2-3-4-5: U-D-PDC-U-D
	return d2Thre[dcArrId];
} // used in map.C - TATrack::SetDsquareThresholdPerDot, Dsquare(..., D2Threshold)
// calculate the minmum deviation of a track off the fired strips in a TOF wall
bool TACtrlPara::TOFWallStripStrayTest(double strayMin, unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid);
	int detId = type[0];
	if(3 == detId) // MWDC array L
		return strayMin > gp->Val(49) && strayMin < gp->Val(50);
	else if(4 == detId) // MWDC array R
		return strayMin > gp->Val(51) && strayMin < gp->Val(52);
	else{
		TAPopMsg::Error("TACtrlPara", "TOFWallStripStrayTest: invalid detId: %d", detId);
		return false;
	}
}
double TACtrlPara::T_tofDCtoTOFW(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0] && 6 != type[0] && 7 != type[0] && 8 != type[0] && 9 != type[0])
		TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC array");
	// MWDC Array L-R
	if(3 == type[0] || 4 == type[0]){
		if(type[1] >= 3) TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC");
	}
	// MWDC Array U-D
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0]){
		if(type[1] >= 2) TAPopMsg::Error("TACtrlPara", "T_tofDCtoTOFW: Not an MWDC");
	}

	// DC0-1-2, include [dcArr][DC]
	const double ccT_tofDCtoTOFW[6][3] = { // [L-R-U-D-PDC-U-D][DC0-1-2 or DC0-1]
		{gp->Val(34), gp->Val(35), gp->Val(36)}, // {7.4, 4.8, 2.0}, // beta = 0.5
		{gp->Val(37), gp->Val(38), gp->Val(39)}, // {6.4, 4.0, 1.4} // beta = 0.6
		{gp->Val(55), gp->Val(56), 0.},
		{gp->Val(57), gp->Val(58), 0.},
		{gp->Val(69), gp->Val(70), 0.},
		{gp->Val(71), gp->Val(72), 0.}
	};
	int dcArrId = -9999;
	if(3 == type[0] || 4 == type[0]) dcArrId = type[0] - 3; // 0-1: L-R
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0])
		dcArrId = type[0] - 6 + 2; // 2-3-4-5: U-D-PDC-U-D
	return ccT_tofDCtoTOFW[dcArrId][type[1]];
}
double TACtrlPara::T_wireMean(unsigned uid){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0] && 6 != type[0] && 7 != type[0] && 8 != type[0] && 9 != type[0])
		TAPopMsg::Error("TACtrlPara", "T_wireMean: Not an MWDC array");
	// MWDC Array L-R
	if(3 == type[0] || 4 == type[0]){
		if(type[1] >= 3) TAPopMsg::Error("TACtrlPara", "T_wireMean: Not an MWDC");
		if(type[2] >= 3) TAPopMsg::Error("TACtrlPara", "T_wireMean: Not a valid MWDC type[XUV]");
	}
	// MWDC Array U-D
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0]){
		if(type[1] >= 2) TAPopMsg::Error("TACtrlPara", "T_wireMean: Not an MWDC");
		if(type[2] >= 2) TAPopMsg::Error("TACtrlPara", "T_wireMean: Not a valid MWDC type[XY]");
	}

	// [dcArrL-R][DC012 or DC01][XUV or XY]
	static const double ccT_wireMean[6][3][3] = {
		{ {1.5, 1.5, 1.5}, {1.5, 1.5, 1.5}, {1.5, 1.5, 1.5} }, // DCArrL
		{ {1.5, 1.5, 1.5}, {1.5, 1.5, 1.5}, {1.5, 1.5, 1.5} }, // DCArrR
		{ {0.2, 0.2, 0.0}, {0.2, 0.2, 0.0}, {0.0, 0.0, 0.0} }, // DCArrU
		{ {0.4, 0.4, 0.0}, {0.4, 0.4, 0.0}, {0.0, 0.0, 0.0} }, // DCArrD
		{ {0.4, 0.4, 0.0}, {0.4, 0.4, 0.0}, {0.0, 0.0, 0.0} }, // PDCArrU
		{ {0.4, 0.4, 0.0}, {0.4, 0.4, 0.0}, {0.0, 0.0, 0.0} }  // PDCArrD
	};

	int dcArrId = -9999;
	if(3 == type[0] || 4 == type[0]) dcArrId = type[0] - 3; // 0-1: L-R
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0])
		dcArrId = type[0] - 6 + 2; // 2-3-4-5: U-D-PDC-U-D
	return ccT_wireMean[dcArrId][type[1]][type[2]];
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
	if("" == kConfigExpDir) TAPopMsg::Error("TACtrlPara", "ConfigExpDir: not assigned");
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
	static TAGPar *gp = TAGPar::Instance();
	const int HVopt[6][3][3] = { // [DCarrL-R-U-D][DC0-1-2 or 0-1][XUV or XY]
		// -1: no HV is applied; 0-5 -> HV[0-5] array element
		// DCArrL, DC0-1-2 - X-U-V
		{ {(int)gp->Val(13), (int)gp->Val(14), (int)gp->Val(15)},
		  {(int)gp->Val(16), (int)gp->Val(17), (int)gp->Val(18)},
		  {(int)gp->Val(19), (int)gp->Val(20), (int)gp->Val(21)} },
		// DCArrR, DC0-1-2 - X-U-V
		{ {(int)gp->Val(22), (int)gp->Val(23), (int)gp->Val(24)},
		  {(int)gp->Val(25), (int)gp->Val(26), (int)gp->Val(27)},
		  {(int)gp->Val(28), (int)gp->Val(29), (int)gp->Val(30)} },
		// DCArrU, DC0-1 - X-Y
		{ {(int)gp->Val(59), (int)gp->Val(60), 				 -1},
		  {(int)gp->Val(61), (int)gp->Val(62), 				 -1},
		  {				 -1, 			   -1, 		 		 -1} },
		// DCArrD, DC0-1 - X-Y
		{ {(int)gp->Val(63), (int)gp->Val(64), 				 -1},
		  {(int)gp->Val(65), (int)gp->Val(66), 				 -1},
		  {				 -1, 			   -1, 		 		 -1} },
		// PDCArrU, DC0-1 - X-Y
		{ {(int)gp->Val(73), (int)gp->Val(74), 				 -1},
		  {(int)gp->Val(75), (int)gp->Val(76), 				 -1},
		  {				 -1, 			   -1, 		 		 -1} },
		// PDCArrD, DC0-1 - X-Y
		{ {(int)gp->Val(77), (int)gp->Val(78), 				 -1},
		  {(int)gp->Val(79), (int)gp->Val(80), 				 -1},
		  {				 -1, 			   -1, 		 		 -1} }
	};

/////////////////// THIS IS FOR EXP_PION_2017 ///////////////////////////////////////////
//		{ {3, -1, 3}, {2, 2, 2}, {4, 4, 4} }, // DCArrL, DC0-1-2 - X U V // 20171028 ->
//		{ {3, -1, 3}, {3, 3, 3}, {3, 3, 3} }, // DCArrR, DC0-1-2 - X U V // 20171028 ->
/////////////////////////////////////////////////////////////////////////////////////////

	// ----------------- set STRs ---------------------- //
	// use base STRs from garfield simualtion results
	static TFile *f = new TFile(kSTRROOTFile.c_str());
	static const int nAngBin = TAAnodePara::kSTRCorAngleNBins, nHV = 5;
	// STRs for DC downstream of the dipole magnet
	// HV: 900V, 1000V, 1300V, 1350V, 1500V
	static TF1 *rt[nHV][nAngBin]{0}; // [HVs] [six track-cell angle intervals]
//	static TF1 *rtDumb = (TF1*)f->Get("RTDumb"); // constant zero
	static const int HV[nHV] = {900, 1000, 1300, 1350, 1500}; // V
	static char name[64];
	if(!rt[0][0]){ // to make sure that this assignment will only be carried out once
		for(int i = 0; i < nHV; i++){
			for(int j = 0; j < nAngBin; j++){
				sprintf(name, "%d/RT%d", HV[i], j);
				rt[i][j] = (TF1*)f->Get(name);
			}
		}
	} // end if not assigned
	// // STRs for MWDCTaMs // //
	static const int nHVM = 1, nHVL = 1;
	static const int HVM[nHVM] = {1400}, HVL[nHVL] = {1400}; // unit: V
	static TF1 *rtM[nHVM][nAngBin]{0}, *rtL[nHVM][nAngBin]{0};
	// assign rtM for DCTaM
	if(!rtM[0][0]){
		for(int i = 0; i < nHVM; i++){
			for(int j = 0; j < nAngBin; j++){
				sprintf(name, "%dM/RT%d", HVM[i], j);
				rtM[i][j] = (TF1*)f->Get(name);
			} // end for over j
		} // end for over i
	} // end if not assigned
	// assign rtL for DCTaL
	if(!rtL[0][0]){
		for(int i = 0; i < nHVL; i++){
			for(int j = 0; j < nAngBin; j++){
				sprintf(name, "%dL/RT%d", HVL[i], j);
				rtL[i][j] = (TF1*)f->Get(name);
			} // end for over j
		} // end for over i
	} // end if not assigned
	// // STRs for PDCs // //
	static const int nHVP = 4;
	static const int HVP[nHVP] = {1000, 1400, 1200, 1150}; // unit: V
	static TF1 *rtP[nHVP][nAngBin]{0};
	// assign rtM for DCTaM
	if(!rtP[0][0]){
		for(int i = 0; i < nHVP; i++){
			for(int j = 0; j < nAngBin; j++){
				sprintf(name, "%dP/RT%d", HVP[i], j);
				rtP[i][j] = (TF1*)f->Get(name);
			} // end for over j
		} // end for over i
	} // end if not assigned


	unsigned uid = para->GetUID();
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 == type[0] || 4 == type[0]){ // MWDC array L-R
			if(type[1] < 0 || type[1] > 2) TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCid: type[1]: %d", type[1]);
			if(type[2] < 0 || type[2] > 2) TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCType: type[2]: %d", type[2]);
			const int hv = HVopt[type[0]-3][type[1]][type[2]];
			if(hv >= nHV) TAPopMsg::Error("TACtrlPara", "AssignSTR: hv id too large: hv: %d", hv);
		for(int i = 0; i < nAngBin; i++){
			// XXX: RTDumb -> 1300V, facilitate simulation
			if(hv < 0) para->SetSTR(rt[2][i], i); // rtDumb, rt[0][i]
			else if(!rt[hv][i])
				TAPopMsg::Error("TACtrlPara", "AssignSTR: required rt is nullptr: hv: %d, nang: %d", hv, i);
//			else para->SetSTR(rtM[hv][i], i); // XXX: note that this is only for P. Ma triplet-DCTaM Test
			else para->SetSTR(rt[hv][i], i);
		} // end for over i
	} // end if(3 == type[0] || 4 == type[0])
	else if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0]){ // MWDC array U-D
			if(type[1] < 0 || type[1] > 1) TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCid: type[1]: %d", type[1]);
			if(type[2] < 0 || type[2] > 1) TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCType: type[2]: %d", type[2]);
			const int hv = HVopt[type[0]-6+2][type[1]][type[2]];
			if(hv >= nHV) TAPopMsg::Error("TACtrlPara", "AssignSTR: hv id too large: hv: %d", hv);
		for(int i = 0; i < nAngBin; i++){ // loop over angle alpha intervals
			// XXX: RTDumb -> 1300V, facilitate simulation
			if(hv < 0) para->SetSTR(rt[2][i], i); // rtDumb, rt[0][i]
			else{
				switch(type[0]){
					case 6: para->SetSTR(rtM[hv][i], i); break; // DCTaM
					case 7: para->SetSTR(rtL[hv][i], i); break; // DCTaL
					case 8:
					case 9: para->SetSTR(rtP[hv][i], i); break; // PDCU-D
					default: break;
				}
			} // end else
		} // end for over i
	} // end else if
	else TAPopMsg::Error("TACtrlPara", "AssignSTR: input anode para uid error: DCArrId: type[0]: %d", type[0]);

	f->Close();
} // end of member function AssignSTR

TACtrlPara::TACtrlPara() : kIsCoarseFit(false), kIs3DTracking(false){
	//---------  for pattern recognition -------------------//
	// for map function

	kDriftTimeQtCorrectionWeight = 6.0;
	fSigmaR = 0.2;

	kDataFile = ""; // for extra use
	// 20161123_2247.dat.root merged.root 20161125_2046.dat.root merged_M.root _SIM.root -- the clip board
	// the directory name in config directory holding the selected experiment config files
	kConfigExpDir = "";
	// root file containing TF1 objects for MWDCs
	kSTRROOTFile = "";
	// used in refined fit and calibration phase //  20161126_0522.dat.root
	kRootFile = "";
}

// spatial resolution used to smear drift distance during the generation of simulation data
// This method is used in TAAnodePara::GetSpatialResolution()
double TACtrlPara::GetSimSpatialResolution(){
	return fSigmaR;
}
void TACtrlPara::SetSimSpatialResolution(double sigmar){
	if(sigmar < 0. || sigmar > 3.)
		TAPopMsg::Error("TACtrlPara", "SetSimSpatialResolution: input sigma-r is abnormal: %f", sigmar);
	fSigmaR = sigmar;
}
















