///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAssessTa.C													     //
//   TAAssessTa.C -- source file for class TAAssessTa							     //
//   Introduction: Assess the tracking results for code performances and quality of  //
// the data. This is actually a user-interface class, and output interface to papers //
// and presentations. What is worth mentioning is that compared with TAAssess, this	 //
// class is specifically for MWDCs around the target.								 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/30.															     //
// Last modified: 2018/5/24, SUN Yazhou.									  	     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <libgen.h> // basename function

// ROOT include
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2F.h"

#include "TAAssessTa.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"
#include "TAParaManager.h"
#include "TAMath.h"
#include "TAMWDCArray2.h"
#include "TAMWDC.h"
#include "TADetectorPara.h"
#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TAGPar.h"

using std::cout;
using std::endl;
using std::flush;
using std::setw;
using std::vector;

static const double DEGREE = TAMath::DEGREE();
static const double Pi = TAMath::Pi();
static const int nAng = TAAnodePara::kSTRCorAngleNBins;
static const double angStep = TAAnodePara::kSTRCorAngleStep;

TAAssessTa *TAAssessTa::fInstance = nullptr;
bool TAAssessTa::fNullDCArr = false;

TAAssessTa *TAAssessTa::Instance(){
	if(!fInstance) fInstance = new TAAssessTa();
	return fInstance;
}
TAAssessTa::TAAssessTa() : fDetList(0), fROOTFile(""){
	if(!fDetList) fDetList = &TAParaManager::Instance()->GetDetList();
}
TAAssessTa::~TAAssessTa(){}

void TAAssessTa::EvalDCArr(int round, bool isDCArrD){
	EvalDCArr(fROOTFile, fDetList, round, isDCArrD);
}
void TAAssessTa::EvalDCArr(const string &rootfile, DetArr_t *detList, int runid, bool isDCArrD){
	if(!strcmp("", rootfile.c_str()))
		TAPopMsg::Error("TAAssessTa", "EvalDCArr: rootfile name is empty");
	if(!detList)
		TAPopMsg::Error("TAAssessTa", "EvalDCArr: Detector List pointer detList is null");
	TAMWDCArray2 *dcArrAr[2] = {(TAMWDCArray2*)(*detList)[6], (TAMWDCArray2*)(*detList)[7]};
	short UDTAG = 12 + isDCArrD; // type/10: 1[2-3-4-5] -> dcArrU-D-PDCArrU-D

	static bool usingPDC = TAGPar::Instance()->Val(83);
	// track type has experienced exchanges: 1[23]# <-> 1[45]# if PDC tracks are used for PID
	if(usingPDC){
		dcArrAr[0] = (TAMWDCArray2*)(*detList)[8];
		dcArrAr[1] = (TAMWDCArray2*)(*detList)[9];
	}
	if(!dcArrAr[0] && !dcArrAr[1]){
		TAPopMsg::Info("TAAssessTa", "EvalDCArr: Both DCArr is null. TAEventProcesssor::Configure not run yet?");
		fNullDCArr = true; // no valid DCArr is available for the class
		return;
	}
	TAMWDCArray2 *dcArr = dcArrAr[isDCArrD];
	if(!dcArr) TAPopMsg::Error("TAAssessTa", "EvalDCArr: Requested DCArr is null");

	cout << "Input rootfile: " << rootfile << endl;
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TAAssessTa", "EvalDCArr: Input rootfile %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str());
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	if(!treeTrack) TAPopMsg::Error("TAAssessTa", "EvalDCArr: treeTrack is nullptr");
	// default is for DCArrR
	const int ndir = 8;
	char dir[ndir][64] = {"miscTa", "rtTa", "drtTa", "dtTa", "rrTa", "3DrtTa", "3DdrtTa", "3DrrTa"};
	char topdir[64], ud[] = "UD";
	sprintf(topdir, "assessTa%d%c", runid, ud[isDCArrD]);
	char tmp[32];
	for(int i = 0; i < ndir; i++){
		strcpy(tmp, dir[i]);
		sprintf(dir[i], "%s%d%c", tmp, runid, ud[isDCArrD]);
	} // end for over i

	// DC parameters
	const TAMWDC *dc[2] = {dcArr->GetMWDC(0), dcArr->GetMWDC(1)};
	const TADetectorPara *dcPar[2] = {dc[0]->GetDetPara(), dc[1]->GetDetPara()};
	const double phiDC[2] = {dcPar[0]->GetPhi(), dcPar[1]->GetPhi()};
	// The x-axis of x_X, x_Y, to calculate angle-alpha
	const double al[2][3] = {{1., 0., 0.}, {0., 1., 0.}}; // X-Y al: anode axis vec in local coord.
	double ag[2][2][3]{}, agAxis[2][2][3]{}; // [DC][XY][xyz]; ag: anode orientation in global coord.
	for(int i = 2; i--;) for(int j = 2; j--;){ // i: DC0-1; j: X-Y
		dcArr->GetMWDC(i)->GetDetPara()->GetGlobalRotation(al[j], agAxis[i][j]);
		dcArr->GetMWDC(i)->GetAnode(j, 1, 0)->GetAnodePara()->GetGlobalDirection(ag[i][j]);
	}

	// read the track tree
	const int ntrMax = 100, ntrMax3D = ntrMax / 3;
	int ntr, ntrT, index, nu[ntrMax][6], gGOOD[ntrMax];
	int type[ntrMax], id[ntrMax];
	double t[ntrMax][6], r[ntrMax][6], k[ntrMax], b[ntrMax];
	double chi[ntrMax][6], chi2[ntrMax], Chi[ntrMax], TOF[ntrMax];
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("k", k);
	treeTrack->SetBranchAddress("b", b);
	treeTrack->SetBranchAddress("chi", chi);
	treeTrack->SetBranchAddress("chi2", chi2);
	treeTrack->SetBranchAddress("Chi", Chi);
	treeTrack->SetBranchAddress("gGOOD", gGOOD);
	treeTrack->SetBranchAddress("type", type);
	treeTrack->SetBranchAddress("id", id);

	double Chi3D[ntrMax3D], chi2_3D[ntrMax3D], chi3D[ntrMax3D][18];
	// x=k1*z+b1; y=k2*z+b2;
	double k1_3D[ntrMax3D], b1_3D[ntrMax3D], k2_3D[ntrMax3D], b2_3D[ntrMax3D];
	int firedStripId[ntrMax3D];
	TTree *treePID3D = (TTree*)f->Get("treePID3D"); // storing 3D tracking and 3D-PID information
	bool is3D = false; // if using 3D tracks
	bool isDCArrR_3D[ntrMax3D]{};
	int n3Dtr, n3DtrT;
	if(treePID3D){
		is3D = (bool)treePID3D->GetEntries();
		treeTrack->AddFriend(treePID3D);
		treePID3D->SetBranchAddress("n3Dtr", &n3Dtr);
		treePID3D->SetBranchAddress("n3DtrT", &n3DtrT);
		treePID3D->SetBranchAddress("Chi", Chi3D);
		treePID3D->SetBranchAddress("chi2", chi2_3D);
		treePID3D->SetBranchAddress("chi", chi3D); // residuum [x6->u6->v6]
		treePID3D->SetBranchAddress("k1", k1_3D);
		treePID3D->SetBranchAddress("b1", b1_3D);
		treePID3D->SetBranchAddress("k2", k2_3D);
		treePID3D->SetBranchAddress("b2", b2_3D);
		treePID3D->SetBranchAddress("firedStripId", firedStripId); // firedStripId == -2 marks UD3Dtrk
		treePID3D->SetBranchAddress("isDCArrR", isDCArrR_3D);
	}

/////////////////////////////////////// BEGIN OF THE HISTOGRAM DEFINITION ///////////////////////////
	// to store all the ROOT statistical objects
	vector<TObject *> objLs[ndir];
	char name[64], title[128], xy[] = "XY";
	TH1F *hchi = new TH1F("hchi", "Overall Residual Distribution;dr [mm]", 1600, -8.0, 8.0);
	TH1F *hChi = new TH1F("hChi", "\\sqrt{\\chi2/ndf}\\hbox{~Per Track};\\chi~[mm]", 800, 0, 8.);
	TH1F *hchi2 = new TH1F("hchi2", "Track~\\chi^2~Distribution~\\sum(\\chi^2);\\chi^2~[mm^2]", 1000., -0.5, 100);
	objLs[0].push_back(hchi); objLs[0].push_back(hChi); objLs[0].push_back(hchi2);
	TH1F *hchi_3D = new TH1F("hchi_3D", "Overall Residual Distribution (for 3D Tracks);dr [mm]", 1600, -8.0, 8.0);
	TH1F *hChi_3D = new TH1F("hChi_3D", "\\sqrt{\\chi2/ndf}\\hbox{~Per Track} (for 3D Tracks);\\chi~[mm]", 800, 0, 8.);
	TH1F *hchi2_3D = new TH1F("hchi2_3D", "Track~\\chi^2~Distribution~\\sum(\\chi^2)~(for~3D~Tracks);\\chi^2~[mm^2]", 1000., -0.5, 100);
	objLs[6].push_back(hchi_3D); objLs[6].push_back(hChi_3D); objLs[6].push_back(hchi2_3D);
	// hrt: DCA v.s. t //
	TH2F *hrt01 = new TH2F("hrt01", "Overall Distribution of Calculated Drift Distance and Drift Time;t [ns];r [mm]", 500, -100., 400., 800, -0.5, 7.5);
	objLs[1].push_back(hrt01);
	TH2F *hrt01_3D = new TH2F("hrt01_3D", "Overall Distribution of Calculated Drift Distance and Drift Time for 3D Tracks;t [ns];r [mm]", 500, -100., 400., 800, -0.5, 7.5);
	objLs[5].push_back(hrt01_3D);
	TH2F *hrt02[2], *hrt03[2][2]; // [0-1]: [XY]; [0-1][0-1]: [XY][DC0-1]
	TH2F *hrt02_3D[2], *hrt03_3D[2][2]; // [0-1]: [XY]; [0-1][0-1]: [XY][DC0-1]
	for(int i = 0; i < 2; i++){ // loop over XY
		sprintf(name, "hrt02_%d", i);
		sprintf(title, "Distribution of DCA v.s. Drift Time for %c-Tracks;t [ns];DCA [mm]", xy[i]);
		hrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[1].push_back(hrt02[i]);
		sprintf(name, "hrt02_3D_%d", i);
		sprintf(title, "Distribution of DCA v.s. Drift Time for 3D Tracks in %c Sense Wires;t [ns];DCA [mm]", xy[i]);
		hrt02_3D[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[5].push_back(hrt02_3D[i]);
		for(int j = 0; j < 2; j++){ // loop over two DCs
			sprintf(name, "hrt03_%d_%d", i, j);
			sprintf(title, "Distribution of DCA v.s. Drift Time for %c-Track-DC%d;t [ns];DCA [mm]", xy[i], j);
			hrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
			objLs[1].push_back(hrt03[i][j]);
			sprintf(name, "hrt03_3D_%d_%d", i, j);
			sprintf(title, "Distribution of DCA v.s. Drift Time for 3D Tracks in DC%d %c Sense Wires;t [ns];DCA [mm]", j, xy[i]);
			hrt03_3D[i][j] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
			objLs[5].push_back(hrt03_3D[i][j]);
		} // end loop over DCs
	} // end loop over XY
	TH2F *hrt04 = new TH2F("hrt04", "Distribution of DCA v.s. Drift Time for DC0X1Anode40;t [mm];DCA [mm]", 500, -100., 400., 800, -0.5, 7.5);
	objLs[1].push_back(hrt04);
	TH2F *hrt04_3D = new TH2F("hrt04_3D", "Distribution of DCA v.s. Drift Time for DC0X1Anode40(3D Tracks);t [mm];DCA [mm]", 500, -100., 400., 800, -0.5, 7.5);
	objLs[5].push_back(hrt04_3D);

	TH2F *hrt04_STR[nAng];
	TH2F *hrt04_3D_STR[nAng];
	for(int i = 0; i < nAng; i++){
		sprintf(name, "hrt04_STR_%d", i);
		sprintf(title, "Distribution of DCA and Drift Time for DC0X1Anode40STR_%d(alpha: %f~%f);t [ns]; r [mm]", i, (i-nAng/2.)*angStep/DEGREE, (i-nAng/2.+1.)*angStep/DEGREE);
		hrt04_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[1].push_back(hrt04_STR[i]);
		sprintf(name, "hrt04_3D_STR_%d", i);
		sprintf(title, "Distribution of DCA and Drift Time for DC0X1Anode40STR_%d(alpha: %f~%f)(3D Tracks);t [ns]; r [mm]", i, (i-nAng/2.)*angStep/DEGREE, (i-nAng/2.+1.)*angStep/DEGREE);
		hrt04_3D_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[5].push_back(hrt04_3D_STR[i]);
	}
	// hdrt: chi v.s. t //
	TH2F *hdrt01 = new TH2F("hdrt01", "Overall Distribution of Redisual and Drift Time;t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[2].push_back(hdrt01);
	TH2F *hdrt01_3D = new TH2F("hdrt01_3D", "Overall Distribution of Redisual and Drift Time for 3D Tracks;t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[6].push_back(hdrt01_3D);
	TH2F *hdrt02[2]{}, *hdrt03[2][2]{}; // [0-1]: [XY]; [0-1][0-1]: [XY][DC0-1]
	TH2F *hdrt02_3D[2]{}, *hdrt03_3D[2][2]{}; // [0-1]: [XY]; [0-1][0-1]: [XY][DC0-1]
	for(int i = 0; i < 2; i++){ // loop over XY
		sprintf(name, "hdrt02_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for %c-Tracks;t [ns];dr [mm]", xy[i]);
		hdrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[2].push_back(hdrt02[i]);
		sprintf(name, "hdrt02_3D_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for 3D Tracks in %c Sense Wires;t [ns];dr [mm]", xy[i]);
		hdrt02_3D[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[6].push_back(hdrt02_3D[i]);
		for(int j = 0; j < 2; j++){ // loop over DCs
			sprintf(name, "hdrt03_%d_%d", i, j);
			sprintf(title, "Distribution of Residual and Drift Time for %c-Tracks-DC%d", xy[i], j);
			hdrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
			objLs[2].push_back(hdrt03[i][j]);
			sprintf(name, "hdrt03_3D_%d_%d", i, j);
			sprintf(title, "Distribution of Residual and Drift Time for 3D Tracks in DC%d %c Sense Wires", j, xy[i]);
			hdrt03_3D[i][j] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
			objLs[6].push_back(hdrt03_3D[i][j]);
		} // end loop over DCs
	} // end loop over XY
	TH2F *hdrt04 = new TH2F("hdrt04", "Distribution of Residual and Drift Time for DC0X1Anode40;t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[2].push_back(hdrt04);
	TH2F *hdrt04_3D = new TH2F("hdrt04_3D", "Distribution of Residual and Drift Time for DC0X1Anode40(3D tracks);t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[6].push_back(hdrt04_3D);

	TH2F *hdrt04_STR[nAng]{};
	TH2F *hdrt04_3D_STR[nAng]{};
	for(int i = 0; i < nAng; i++){
		sprintf(name, "hdrt04_STR_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for DC0X1Anode40STR_%d(alpha: %f~%f);t [ns];dr [mm]", i, (i-nAng/2.)*angStep/DEGREE, (i-nAng/2.+1.)*angStep/DEGREE);
		hdrt04_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[2].push_back(hdrt04_STR[i]);
		sprintf(name, "hdrt04_3D_STR_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for DC0X1Anode40STR_%d(alpha: %f~%f)(3D tracks);t [ns];dr [mm]", i, (i-nAng/2.)*angStep/DEGREE, (i-nAng/2.+1.)*angStep/DEGREE);
		hdrt04_3D_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[6].push_back(hdrt04_3D_STR[i]);
	}
	TH2F *hdrt04_sample = new TH2F("hdrt04_sample", "dr-DCA Spectra - for STRCor Evaluation;DCA [mm];dr [mm]", 60, 0., 5.5, 800, -4.0, 4.0);
	TH2F *hdrt04_3D_sample = new TH2F("hdrt04_3D_sample", "dr-DCA Spectra (3D) - for STRCor Evaluation;DCA [mm];dr [mm]", 60, 0., 5.5, 800, -4.0, 4.0);
	objLs[2].push_back(hdrt04_sample); objLs[6].push_back(hdrt04_3D_sample);
	TH1F *hSTRid = new TH1F("hSTRid", "STR id Distribution;STRid", nAng+3, -1.5, nAng+1.5);
	TH1F *hntrTot = new TH1F("hntrTot", "Total Track Count;nSec-X-Y-3D", 7, -2.5, 4.5); // -1012
	objLs[0].push_back(hSTRid); objLs[0].push_back(hntrTot);
	TH1F *hntrPerSec[2];
	for(int i = 0; i < 2; i++){ // loop over XY
		sprintf(name, "hntrPerSec%c", xy[i]);
		sprintf(title, "Number of %c Tracks Per Data Section;ntracks/section", xy[i]);
		hntrPerSec[i] = new TH1F(name, title, 11, -1.5, 9.5);
		objLs[0].push_back(hntrPerSec[i]);
	} // end loop over XUV
	TH1F *hntrPerSec3D = new TH1F("hntrPerSec3D", "3D Track Horizontal Angle Distribution; ntracks/section", 11, -1.5, 9.5);
	TH1F *hHAng = new TH1F("hHAng", "3D Track Horizontal Angle Distribution;angle [degree]", 2000, -50., 30.);
	TH1F *hVAng = new TH1F("hVAng", "3D Track Vertical Angle Distribution;angle [degree]", 600, -30., 30.);
	objLs[0].push_back(hHAng); objLs[0].push_back(hVAng);
	// statistics of number of fired anode layers per section
	TH1F *hnF[2], *hgGOOD[2]; // [XY]
	TH2F *htt[2][2], *hrr[2][2]; // [XY][DC0-1]
	TH2F *htt_3D[2][2], *hrr_3D[2][2]; // [XY][DC0-1]
	TH1F *hdt[2][2]; // [DC0-1][XY]
	for(int i = 0; i < 2; i++){ // loop over XY
		sprintf(name, "hnF_%c", xy[i]);
		sprintf(title, "Number of Fired %c Anode Layers Per 3D Track;nF%c", xy[i], xy[i]);
		hnF[i] = new TH1F(name, title, 10, -1.5, 8.5);
		sprintf(name, "hgGOOD%c", xy[i]);
		sprintf(title, "gGOOD-Fired %c Anode Layers per Track", xy[i]);
		hgGOOD[i] = new TH1F(name, title, 11, -1.5, 9.5);
		objLs[0].push_back(hnF[i]); objLs[0].push_back(hgGOOD[i]);
		for(int j = 0; j < 2; j++){ // loop over DCs
			sprintf(name, "htt%c%d", xy[i], j);
			sprintf(title, "t_{X1} v.s. t_{X2} for Vertical %c Tracks for MWDC%d;t_{X1} [ns];t_{X2} [ns]", xy[i], j);
			htt[i][j] = new TH2F(name, title, 500, -100., 400., 500, -100., 400.);
			sprintf(name, "hrr%cdc%d", xy[i], j);
			sprintf(title, "r_{X1} v.s. r_{X2} for Vertical %c Tracks for MWDC%d;r_{X1} [mm];r_{X2} [mm]", xy[i], j);
			hrr[i][j] = new TH2F(name, title, 500, -0.2, 6., 500., -0.2, 6.);
			objLs[4].push_back(htt[i][j]); objLs[4].push_back(hrr[i][j]);
			sprintf(name, "htt_3D_%cdc%d", xy[i], j);
			sprintf(title, "t_{X1} v.s. t_{X2} for Vertical 3D %c Tracks for MWDC%d;t_{X1} [ns];t_{X2} [ns]", xy[i], j);
			htt_3D[i][j] = new TH2F(name, title, 500, -100., 400., 500, -100., 400.);
			sprintf(name, "hrr_3D_%cdc%d", xy[i], j);
			sprintf(title, "r_{X1} v.s. r_{X2} for Vertical 3D %c Tracks for MWDC%d;r_{X1} [mm];r_{X2} [mm]", xy[i], j);
			hrr_3D[i][j] = new TH2F(name, title, 500, -0.2, 6., 500, -0.2, 6.);
			objLs[7].push_back(htt_3D[i][j]); objLs[7].push_back(hrr_3D[i][j]);
			sprintf(name, "Hdt_DC%d%c", i, xy[j]);
			sprintf(title, "Drift Time Distribution of MWDC%d-%c;t [ns]", i, xy[j]);
			hdt[i][j] = new TH1F(name, title, 500, -100., 400.);
			objLs[3].push_back(hdt[i][j]);
		} // end loop over DCs
	} // end loop over XUV
	TH1F *hnF3D = new TH1F("hnF3D", "Number of All Fired Anode Layers Per 3D Track;nF", 22, -1.5, 20.5);
	TH1F *hXMag = new TH1F("hXMag", "x at the Entrance (z=-500) of the Magnetic Field", 1000, -500., 500.);
	TH1F *hYMag = new TH1F("hYMag", "y at the Entrance (z=-500) of the Magnetic Field", 500, -250., 250.);
	objLs[0].push_back(hnF3D);
	objLs[0].push_back(hXMag); objLs[0].push_back(hYMag);
	TH1F *heff = new TH1F("heff", "MWDC efficiency - Number of X-Y Tracks;X:Tot-DC0(X1-X2)-DC1--Y", 30, -2.5, 27.5);
	objLs[0].push_back(heff);
	TH1F *hr_ = new TH1F("hr_", "hr_", 500, -6.5, 6.5); // a temporary histogram for testing
	objLs[0].push_back(hr_);
///////////////////////////////////////// END OF THE HISTOGRAM DEFINITION //////////////////////////


	const int n = treeTrack->GetEntries(); // number of data sections
	int ntrTot[2]{}, n3DtrTot = 0; // total number of tracks of all kinds of DC types (X-Y)
	int ntrPerSec[2][50]{}, n3DtrPerSec[ntrMax3D]{}; // [XY][n3Dtr]
	cout << "Totally " << n << " data sections would be processed.\n";
	int hasAllCnt = 0; // count of sections that have X and Y tracks
	int hasXYCnt[2]{}; // count of sections that have X or Y tracks
	int effTot = 0, eff[2][2][2]{}; // efficiency[DC][type][layer]
	for(int i = 0; i < n; i++){ // loop over data sections
		treeTrack->GetEntry(i);
		// check the trk type in UD
		int ntrLs[6][3]{}; // N of TrkProjs; DCArr-L-R-U-D -- [XUV - XY]
		for(int j = 0; j < ntrT; j++){
			const int dcArrId = type[j] / 10 % 10, dcType = type[j] % 10;
			ntrLs[dcArrId][dcType]++;
		}
		// cache the last value of array ntrTot
		int ntrTot_pre[2] = {ntrTot[0], ntrTot[1]};
		for(int j = 0; j < ntrT; j++) if(UDTAG == type[j]/10) ntrTot[type[j]%10]++;
		for(int j = 0; j < 2; j++) ntrPerSec[j][ntrTot[j]-ntrTot_pre[j]]++; // [XY][ntrPerSec]
		// DCArrU-D 3D track statistics
		// only one U(D)Xtrk and one U(D)Ytrk is present, only then would there be a 3D track
		if(1 == ntrLs[2+isDCArrD][0] && 1 == ntrLs[2+isDCArrD][1]){
			n3DtrPerSec[1]++; n3DtrTot++; effTot++;
			// extract id of the wanted tracks
			int trkId[2]{}; // X-Y
			for(int j = 0; j < ntrT; j++){
				if(UDTAG == type[j] / 10) trkId[type[j]%2] = j; // the wanted trk-projs
			}
			// get the number of fired anode layers for each DC type
			int nFXY[2]{}; // number of fired anode layers in a data section
			for(int j = 0; j < 2; j++){ // loop over XY
				for(int k = 0; k < 4; k++){ // loop over DC0-X1X2-DC1-X1X2
					if(-1 != nu[trkId[j]][k]){
						nFXY[j]++;
						eff[k/2][j][k%2]++;
					} // end outer if
				} // end loop over DC0-X1X2-DC1-X1X2
			} // end loop over XUV
			const int nF = nFXY[0] + nFXY[1]; // number of measured points
			for(int j = 0; j < 2; j++) hnF[j]->Fill(nFXY[j]); // loop ove XY
			hnF3D->Fill(nF);
			double p[4]; // [0-3]: k1, k2, b1, b2
			p[0] = k[trkId[0]]; // k1
			p[1] = k[trkId[1]]; // k2
			p[2] = b[trkId[0]]; // b1
			p[3] = b[trkId[1]]; // b2
			hHAng->Fill(atan(p[0])/DEGREE); hXMag->Fill(p[0]*(-500.)+p[2]);
			hVAng->Fill(atan(p[1])/DEGREE); hYMag->Fill(p[1]*(-500.)+p[3]);
			// 3D statistics
			if(is3D){ // if 3D tracking has been implemented
				int trk3DId;
				for(int j = 0; j < n3DtrT; j++){
					if(-2 == firedStripId[j] && isDCArrR_3D[j] == isDCArrD)
						trk3DId = j; // trk3DId <-> trkId[2], related to the same particle
				}
				hChi_3D->Fill(Chi3D[trk3DId]);
				hchi2_3D->Fill(chi2_3D[trk3DId]);
				const double bb[3] = {k1_3D[trk3DId], k2_3D[trk3DId], 1.}; // track direction vector
				double alpha[2][2]{}; // angle between track projection and drift cell; [DC][XY]
				for(int j = 0; j < 2; j++){ // loop over XY
					const int it = trkId[j]; // the id of the current track projection
					// alpha-angle: track projection to normal plane of sense wires
					for(int k = 2; k--;){ // v.s. the detector-local z-axis
						alpha[k][j] = TAMath::AlphaAngle3D(bb, ag[k][j], agAxis[k][j]);
					} // end loop over the two MWDCs

					for(int k = 0; k < 4; k++){ // loop over DC0-X1X2-DC1-X1X2
						const int dcId = k / 2;
						const int STRid = TAAnodePara::GetSTRid(alpha[dcId][j]);
						if(-1 != nu[it][k]){
							const double tt = t[it][k], dr = chi3D[trk3DId][j*6+k];
							const double rc = r[it][k] + dr; // rc: calculated r
							hchi_3D->Fill(dr);
							hrt01_3D->Fill(tt, rc);
							hdrt01_3D->Fill(tt, dr);
							hrt02_3D[j]->Fill(tt, rc);
							hdrt02_3D[j]->Fill(tt, dr);
							hrt03_3D[j][dcId]->Fill(tt, rc);
							hdrt03_3D[j][dcId]->Fill(tt, dr);

							if(0 == j && 0 == k && 8 == nu[j][k]){
								hrt04_3D->Fill(tt, rc);
								hdrt04_3D->Fill(tt, dr);
								hrt04_3D_STR[STRid]->Fill(tt, rc);
								hdrt04_3D_STR[STRid]->Fill(tt, dr);
								hdrt04_3D_sample->Fill(rc, dr);
							} // end inner if
						} // end if
					} // end loop over six sense wire layers for one type
//					hr_->Fill(alpha[2][0]/DEGREE);
					for(int k = 0; k < 2; k++){ // loop over MWDCs
						if(alpha[k][j] > -1. * DEGREE && alpha[k][j] < 1. * DEGREE){
							// nAnodePerLayaer
							int napl = dcArr->GetMWDC(k)->GetNAnodePerLayer();
							if(nu[it][2*k] >= 0.3*napl && nu[it][2*k] <= 0.7*napl){
								htt_3D[j][k]->Fill(t[it][2*k], t[it][2*k+1]);
								hrr_3D[j][k]->Fill(r[it][2*k], r[it][2*k+1]);
							} // end if(nu...)
						} // end if(ang > ..)
					} // end for over MWDCs
				} // end loop over XY
			} // end if(is3D)
		} // end if(only one trk is found in the current DCArr)
		else{
			n3DtrPerSec[0]++;
		}

		bool hasXY[2]{}; // whether the data section has X or Y track projections
		for(int j = 0; j < ntrT; j++){ // end for over track projections
			if(type[j]/10 != UDTAG) continue; // DCArrU or DCArrD
			for(int k = 0; k < 2; k++){ // loop over XY
				if(type[j]%10 == k) hasXY[k] = true;
			} // end for over XY
			const int dcType = type[j]%10;
			heff->Fill(dcType*8 + 2);
			for(int l = 0; l < 4; l++){ // loop over 4 anode layers
				const int dcId = l / 2;
				const int STRid = dcArr->GetMWDC(dcId)->GetSTRid(k[j], dcType);
				if(-1 != nu[j][l]){
					heff->Fill(dcType*8 + 2 + l + 1);
					// rc: DCA
					const double tt = t[j][l], dr = chi[j][l];
					const double rc = r[j][l] + dr;
					hchi->Fill(dr);
					hrt01->Fill(tt, rc);
					hdrt01->Fill(tt, dr);
					hrt02[dcType]->Fill(tt, rc);
					hdrt02[dcType]->Fill(tt, dr);
					hrt03[dcType][dcId]->Fill(tt, rc);
					hdrt03[dcType][dcId]->Fill(tt, dr);

					if(0 == dcType && 0 == l && 8 == nu[j][l]){
						hrt04->Fill(tt, rc);
						hdrt04->Fill(tt, dr);
						hrt04_STR[STRid]->Fill(tt, rc);
						hdrt04_STR[STRid]->Fill(tt, dr);
						hdrt04_sample->Fill(rc, dr);
					} // end inner if
				} // end if(nu[j][l] != -1)
				if(-1 != nu[j][l]) hdt[dcId][dcType]->Fill(t[j][l]);
				if(l%2 == 0) hSTRid->Fill(STRid);
			} // end for over four sense wire layers
			hChi->Fill(Chi[j]);
			hchi2->Fill(chi2[j]);
			hgGOOD[dcType]->Fill(gGOOD[j]);
			// fill the time loop from approximate perpendicular tracks
			double ang0 = atan(k[j]), ang = ang0;
			for(int l = 0; l < 2; l++){ // loop over two DCs
				if(0 == dcType) ang = ang0 - phiDC[l] + 0. * DEGREE;
				if(0 == dcType && 1 == l) hr_->Fill(ang/DEGREE);
				if(ang > -1.5 * DEGREE && ang < 1.5 * DEGREE){
					int napl = dcArr->GetMWDC(l)->GetNAnodePerLayer();
					if(nu[j][2*l] >= 0.3*napl && nu[j][2*l] <= 0.7*napl){
						htt[dcType][l]->Fill(t[j][2*l], t[j][2*l+1]);
						hrr[dcType][l]->Fill(r[j][2*l], r[j][2*l+1]);
					} // end if
				} // end if
			} // end for over three DCs
		} // end for over track projections
		if(hasXY[0] && hasXY[1]) hasAllCnt++;
		for(int j = 2; j--;) if(hasXY[j]) hasXYCnt[j]++;
		cout << "Data section " << i << " processed.\r" << flush;
	} // end loop over data sections
	// post-loop histogramming
	hntrTot->SetBinContent(2, n); // nsec
	hntrTot->SetBinContent(6, n3DtrTot); // n 3D tracks
	for(int i = 0; i < 2; i++){ // loop over X-Y
		hntrTot->SetBinContent(i+3, ntrTot[i]); // X-Y
		for(int j = 0; j < 10; j++) // for data sections with 0-10 tracks
			hntrPerSec[i]->SetBinContent(j+2, ntrPerSec[i][j]); // X-Y
	} // end for over X-Y
	for(int i = 0; i < 10; i++) hntrPerSec3D->SetBinContent(i+2, n3DtrPerSec[i]);

	// print some information to the screen
	cout << "\n\nhasXCnt: \033[1m" << hasXYCnt[0];
	cout << "\t\033[0mhasYCnt: \033[1m" << hasXYCnt[1];
	cout << "\n\033[0mXTrkCnt: \033[1m" << ntrTot[0];
	cout << "\t\033[0mYTrkCnt: \033[1m" << ntrTot[1];
	cout << "\n\033[0mhasAllCnt: \033[1m" << hasAllCnt;
	cout << "\033[0m\tTotal 3D track count: \033[1m" << n3DtrTot;
	cout << "\n\033[31;1m_______________ \033[32;1msoftware efficiency ";
	cout << "\033[31;1m__________________\033[0m\n\n";
	cout.setf(std::ios_base::fixed);
	cout << setw(14) << "DC0" << setw(12) << "DC1" << endl;
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++){ // loop over XY - LAYER1-2
		cout << xy[i] << j + 1;
		double effDC0 = eff[0][i][j], effDC1 = eff[1][i][j];
		if(0 == effTot){ effDC0 = 0; effDC1 = 0; } // no 3D tracking
		else{ effDC0 /= effTot; effDC1 /= effTot; }
		if(!n3DtrTot && 0 == i){ // only calculate X efficiencies
			const double n = double(ntrTot[0]);
			effDC0 = heff->GetBinContent(6+0*2+j) / n;
			effDC1 = heff->GetBinContent(6+1*2+j) / n;
		}
		cout << setw(13) << "\033[32;1m" << effDC0;
		cout << setw(12) << effDC1 << "\033[0m\n";
		if(j%2 == 1) cout << endl;
	}
	cout << "\n\n\033[33;1mDONE\033[0m\n\n";

	// write //
	cout << "The results would be stored in ROOT file directory \"\033[36;1m" << topdir << "\"\n\033[0m";
//	if(!f->FindObject(topdir)) f->mkdir(topdir); f->cd(topdir);
	char tail[96]; strcpy(tail, rootfile.c_str());
	char s[128]; strcpy(s, ("assess" + string(basename(tail))).c_str());
//	if(0 == runid && 0 == access(s, F_OK)) system(("rm "+string(s)).c_str());
	TFile *fw = new TFile(s, "UPDATE");
	if(!fw->FindObject(topdir)) fw->mkdir(topdir);
	fw->cd(topdir);
	for(int i = 0; i < ndir; i++){
		sprintf(s, "%s/%s", topdir, dir[i]);
		if(!fw->FindObject(dir[i])) fw->mkdir(s);
		fw->cd(s);
		for(TObject *&b : objLs[i]) if(b){
			b->Write("", TObject::kOverwrite);
		}
	}
	for(auto &ls : objLs) for(auto &b : ls) if(b){
		delete b; b = nullptr;
	}
	for(auto &ls : objLs) ls.clear();
	fw->Close(); delete fw; fw = nullptr;
	f->Close(); delete f; f = nullptr;
} // end of member function EvalDCArr

// evaluation done after Eval event by event
void TAAssessTa::PostEval(int round, bool isDCArrD){
	if(fNullDCArr) return; // no valid dcArr available for the class
	if(!strcmp("", fROOTFile.c_str()))
		TAPopMsg::Error("TAAssessTa", "PostEval: rootfile name is empty");
	TAPopMsg::Info("TAAssessTa", "PostEval: Input rootfile: %s", fROOTFile.c_str());
	if(0 != access(fROOTFile.c_str(), F_OK))
		TAPopMsg::Error("TAAssessTa", "PostEval: Input rootfile %s doesn't exist", fROOTFile.c_str());

	char tail[96]; strcpy(tail, fROOTFile.c_str());
	bool success0 = PostEval(("assess" + string(basename(tail))).c_str(), round, isDCArrD, true);
	bool success1 = PostEval(("assess" + string(basename(tail))).c_str(), round, isDCArrD, false);
	if(!success0 && !success1)
		TAPopMsg::Info("TAAssessTa", "PostEval: No eligible hrt_04_sample is found whatsoever and wheresoever\n");
	else
		cout << "\033[33;1m\n\nDONE\n\n\033[0m";
}
bool TAAssessTa::PostEval(const string &rootfile, int round, bool isDCArrD, bool is3D){
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	char name[128], ud[] = "UD", dir[64]; // name: TH2F name - dr-DCA
	if(is3D){
		sprintf(dir, "assessTa%d%c/3DdrtTa%d%c", round, ud[isDCArrD], round, ud[isDCArrD]);
		sprintf(name, "%s/hdrt04_3D_sample", dir);
	}
	else{
		sprintf(dir, "assessTa%d%c/drtTa%d%c", round, ud[isDCArrD], round, ud[isDCArrD]);
		sprintf(name, "%s/hdrt04_sample", dir);
	}
	TH2F *h2 = (TH2F*)f->Get(name);
	if(!h2){
//		TAPopMsg::Error("TAAssessTa", "PostEval: %s doesn't exist", name);
		f->Close(); delete f; f = nullptr;
		return false;
	}
	TAPopMsg::Info("TAAssessTa", "PostEval: isDCArrD: %d, is3D: %d", isDCArrD, is3D);
	TH1D *hprojx = h2->ProjectionX();
	TF1 *fgaus = new TF1("fgaus", "gaus", -4., 4.);
	TGraph *gSigma = new TGraph(); // DCA-sigma for MWDC resolution estimation
	TGraph *gMean = new TGraph(); // DCA-dr for STR correction
	char title[128] = "Saptial Resolution v.s. DCA;DCA [mm];\\sigma~[mm]";
	if(is3D) strcpy(title, "Saptial Resolution v.s. DCA (3D);DCA [mm];\\sigma~[mm]");
	gSigma->SetNameTitle("gSigma_4", title); gSigma->SetMarkerStyle(22);
	strcpy(title, "STR Correction v.s. Drift Distance;DCA [mm];<dr> [mm]");
	if(is3D) strcpy(title, "STR Correction v.s. Drift Distance (3D);DCA [mm];<dr> [mm]");
	gMean->SetNameTitle("gMean_4", title); gMean->SetMarkerStyle(22);
	int gSigma_cnt = 0, gMean_cnt = 0;
	const int n = h2->GetNbinsX(), nn = n;
	for(int i = 0; i < nn; i++){
		TH1D *hproj = h2->ProjectionY("hproj", i+1, i+1);
//		cout << "hproj->GetEntries(): " << hproj->GetEntries() << endl; // DEBUG
//		getchar(); // DEBUG
		if(hproj->GetEntries() < 100) continue;
		// mean, unit: mm
		fgaus->SetParameter(1, 0.);
		fgaus->SetParLimits(1, -1., 1.);
		// sigma, unit: mm
		fgaus->SetParameter(2, 0.2);
		fgaus->SetParLimits(2, 0., 1.);
		// fit range
		double span = 1.5*hproj->GetRMS();
		span = span < 1.5 ? span : 1.5;
		const double mm = hproj->GetMean();
		fgaus->SetRange(mm-span, mm+span);
		hproj->Fit(fgaus, "NQR"); // Fit the son of a bitch
		const double mean = fgaus->GetParameter("Mean");
		const double sigma = fgaus->GetParameter("Sigma");
		if(!((mean > -0.4 && mean < 0.4) && (sigma < 0.8 && sigma > 0.)))
			continue;
		double rm = hprojx->GetBinCenter(i+1);
		gSigma->SetPoint(gSigma_cnt++, rm, sigma);
		gMean->SetPoint(gMean_cnt++, rm, mean);
	} // end for over i

	f->cd(dir);
	if(gSigma->GetN()) gSigma->Write("", TObject::kOverwrite);
	else TAPopMsg::Info("TAAssessTa", "PostEval: gSigma has no data");
	if(gMean->GetN()) gMean->Write("", TObject::kOverwrite);
	else TAPopMsg::Info("TAAssessTa", "PostEval: gMean has no data");

	delete fgaus; fgaus = nullptr; delete gSigma; gSigma = nullptr;
	delete gMean; gMean = nullptr; f->Close(); delete f; f = nullptr;

	return true;
} // end of member function PostEval












