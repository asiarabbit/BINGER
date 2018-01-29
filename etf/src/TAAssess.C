///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAssess.C													     //
//   TAAssess.C -- source file for class TAAssess								     //
//   Introduction: Assess the tracking results for code performances and quality of  //
// the data. This is actually a user-interface class, and output interface to papers //
// and presentations.																 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/12/14.															     //
// Last modified: 2018/1/5, SUN Yazhou.										  	     //
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

// ROOT include
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2F.h"

#include "TAAssess.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"
#include "TAParaManager.h"
#include "TAMath.h"
#include "TAMWDCArray.h"
#include "TAMWDC.h"
#include "TADetectorPara.h"
#include "TAAnode.h"
#include "TAAnodePara.h"

using std::cout;
using std::endl;
using std::flush;
using std::setw;
using std::vector;

static const double DEGREE = TAMath::DEGREE();
static const double Pi = TAMath::Pi();
static const int nAng = TAAnodePara::kSTRCorAngleNBins;
static const double angStep = TAAnodePara::kSTRCorAngleStep;

TAAssess *TAAssess::fInstance = nullptr;

TAAssess *TAAssess::Instance(){
	if(!fInstance) fInstance = new TAAssess();
	return fInstance;
}
TAAssess::TAAssess() : fDetList(0){
	if(!fDetList) fDetList = &TAParaManager::Instance()->GetDetList();
}
TAAssess::~TAAssess(){}

void TAAssess::EvalDCArr(int round, bool isDCArrR){
	EvalDCArr(fROOTFile, fDetList, round, isDCArrR);
}
void TAAssess::EvalDCArr(const string &rootfile, DetArr_t *detList, int runid, bool isDCArrR){
	if(!strcmp("", rootfile.c_str()))
		TAPopMsg::Error("TAAssess", "EvalDCArr: rootfile name is empty");
	if(!detList)
		TAPopMsg::Error("TAAssess", "EvalDCArr: Detector List pointer detList is null");
	TAMWDCArray *dcArrAr[2] = {(TAMWDCArray*)(*detList)[3], (TAMWDCArray*)(*detList)[4]};
	if(!dcArrAr[0] && !dcArrAr[1])
		TAPopMsg::Error("TAAssess", "EvalDCArr: Both DCArr is null. TAEventProcesssor::Configure not run yet?");
	TAMWDCArray *dcArr = dcArrAr[isDCArrR];
	if(!dcArr) TAPopMsg::Error("TAAssess", "EvalDCArr: Required DCArr is null");

	cout << "Input rootfile: " << rootfile << endl;
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TAAssess", "EvalDCArr: Input rootfile %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str());
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	if(!treeTrack) TAPopMsg::Error("TAAssess", "EvalDCArr: treeTrack is nullptr");
	// default is for DCArrR
	const short LRTAG = 10 + isDCArrR; // type/10: 10 -> dcArrL; 11 -> dcArrR
	const int ndir = 8;
	char dir[ndir][64] = {"misc", "rt", "drt", "dt", "rr", "3Drt", "3Ddrt", "3Drr"};
	char topdir[64], lr[] = "LR";
	sprintf(topdir, "assess%d%c", runid, lr[isDCArrR]);
	// XXX: slick use of sprintf; ATTENTION: sprintf is an iterator for self-assignment;
	// dir[i] must be the 1st par in the code below, or dir would be altered to 1st-par firstly
	for(int i = 0; i < ndir; i++) sprintf(dir[i], "%s%d%c", dir[i], runid, lr[isDCArrR]);

	// DC parameters
	const double phiAvrg = dcArr->GetPhiAvrg();
	const TAMWDC *dc[3] = {dcArr->GetMWDC(0), dcArr->GetMWDC(1), dcArr->GetMWDC(2)};
	const TADetectorPara *dcPar[3] = {dc[0]->GetDetPara(), dc[1]->GetDetPara(), dc[2]->GetDetPara()};
	const double phiDC[3] = {dcPar[0]->GetPhi(), dcPar[1]->GetPhi(), dcPar[2]->GetPhi()};
	// for calculation of 3D coincidence deviation in x direction
	const double zDC[3] = {dcPar[0]->GetZ(), dcPar[1]->GetZ(), dcPar[2]->GetZ()};
	// The x-axis of xX, xU, xV, to calculate angle-alpha
	const double al[3][3] = {{1., 0., 0.}, {-sqrt(3.), 1., 0.}, {sqrt(3.), 1., 0.}}; // X-U-V
	double ag[3][3][3]{}, agAxis[3][3][3]{}; // [DC][XUV][xyz];
	for(int i = 3; i--;) for(int j = 3; j--;){ // i: DC0-1-2; j: X-U-V
		dcArr->GetMWDC(i)->GetDetPara()->GetGlobalRotation(al[j], agAxis[i][j]);
		dcArr->GetMWDC(i)->GetAnode(j, 1, 0)->GetAnodePara()->GetGlobalDirection(ag[i][j]);
	}

	// read the track tree
	const int ntrMax = 200, ntrMax3D = ntrMax / 3;
	int ntr, index, nu[ntrMax][6], gGOOD[ntrMax];
	int type[ntrMax], id[ntrMax], firedStripId[ntrMax];
	double t[ntrMax][6], r[ntrMax][6], k[ntrMax], b[ntrMax];
	double chi[ntrMax][6], chi2[ntrMax], Chi[ntrMax];
	double TOF[ntrMax], nStripStray[ntrMax], xMiss3D[ntrMax][3];
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrack->SetBranchAddress("firedStripId", firedStripId);
	treeTrack->SetBranchAddress("nStripStray", nStripStray);
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
	treeTrack->SetBranchAddress("xMiss3D", xMiss3D);

	double Chi3D[ntrMax3D], chi2_3D[ntrMax3D], chi3D[ntrMax3D][18];
	// x=k1*z+b1; y=k2*z+b2;
	double k1_3D[ntrMax3D], b1_3D[ntrMax3D], k2_3D[ntrMax3D], b2_3D[ntrMax3D];
	TTree *treePID3D = (TTree*)f->Get("treePID3D"); // storing 3D tracking and 3D-PID information
	bool is3D = false; // if using 3D tracks
	if(treePID3D){
		is3D = (bool)treePID3D->GetEntries();
		treeTrack->AddFriend(treePID3D);
		treePID3D->SetBranchAddress("Chi", Chi3D);
		treePID3D->SetBranchAddress("chi2", chi2_3D);
		treePID3D->SetBranchAddress("chi", chi3D); // residuum [x6->u6->v6]
		treePID3D->SetBranchAddress("k1", k1_3D);
		treePID3D->SetBranchAddress("b1", b1_3D);
		treePID3D->SetBranchAddress("k2", k2_3D);
		treePID3D->SetBranchAddress("b2", b2_3D);
	}

/////////////////////////////////////// BEGIN OF THE HISTOGRAM DEFINITION ///////////////////////////
	// to store all the ROOT statistical objects
	vector<TObject *> objLs[ndir];
	char name[64], title[128], xuv[4] = "XUV";
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
	TH2F *hrt02[3], *hrt03[3][3]; // [0-3]: [XUV]; [0-3][0-3]: [XUV][DC0-1-2]
	TH2F *hrt02_3D[3], *hrt03_3D[3][3]; // [0-3]: [XUV]; [0-3][0-3]: [XUV][DC0-1-2]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hrt02_%d", i);
		sprintf(title, "Distribution of DCA v.s. Drift Time for %c-Tracks;t [ns];DCA [mm]", xuv[i]);
		hrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[1].push_back(hrt02[i]);
		sprintf(name, "hrt02_3D_%d", i);
		sprintf(title, "Distribution of DCA v.s. Drift Time for 3D Tracks in %c Sense Wires;t [ns];DCA [mm]", xuv[i]);
		hrt02_3D[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[5].push_back(hrt02_3D[i]);
		for(int j = 0; j < 3; j++){ // loop over three DCs
			sprintf(name, "hrt03_%d_%d", i, j);
			sprintf(title, "Distribution of DCA v.s. Drift Time for %c-Track-DC%d;t [ns];DCA [mm]", xuv[i], j);
			hrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
			objLs[1].push_back(hrt03[i][j]);
			sprintf(name, "hrt03_3D_%d_%d", i, j);
			sprintf(title, "Distribution of DCA v.s. Drift Time for 3D Tracks in DC%d %c Sense Wires;t [ns];DCA [mm]", j, xuv[i]);
			hrt03_3D[i][j] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
			objLs[5].push_back(hrt03_3D[i][j]);
		} // end loop over DCs
	} // end loop over XUV
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
	TH2F *hdrt02[3]{}, *hdrt03[3][3]{}; // [0-3]: [XUV]; [0-3]: [XUV][DC0-1-2]
	TH2F *hdrt02_3D[3]{}, *hdrt03_3D[3][3]{}; // [0-3]: [XUV]; [0-3]: [XUV][DC0-1-2]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hdrt02_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for %c-Tracks;t [ns];dr [mm]", xuv[i]);
		hdrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[2].push_back(hdrt02[i]);
		sprintf(name, "hdrt02_3D_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for 3D Tracks in %c Sense Wires;t [ns];dr [mm]", xuv[i]);
		hdrt02_3D[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[6].push_back(hdrt02_3D[i]);
		for(int j = 0; j < 3; j++){ // loop over DCs
			sprintf(name, "hdrt03_%d_%d", i, j);
			sprintf(title, "Distribution of Residual and Drift Time for %c-Tracks-DC%d", xuv[i], j);
			hdrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
			objLs[2].push_back(hdrt03[i][j]);
			sprintf(name, "hdrt03_3D_%d_%d", i, j);
			sprintf(title, "Distribution of Residual and Drift Time for 3D Tracks in DC%d %c Sense Wires", j, xuv[i]);
			hdrt03_3D[i][j] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
			objLs[6].push_back(hdrt03_3D[i][j]);
		} // end loop over DCs
	} // end loop over XUV
	TH2F *hdrt04 = new TH2F("hdrt04", "Distribution of Residual and Drift Time for DC0X1Anode40;t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[2].push_back(hdrt04);
	TH2F *hdrt04_3D = new TH2F("hdrt04_3D", "Distribution of Residual and Drift Time for DC0X1Anode40(3D tracks);t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[6].push_back(hdrt04_3D);	// The x-axis of xX, xU, xV, to calculate angle-alpha

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
	TH1F *hntrTot = new TH1F("hntrTot", "Total Track Count;nSec-X-U-V-3D", 8, -2.5, 5.5); // -10123
	objLs[0].push_back(hSTRid); objLs[0].push_back(hntrTot);
	TH1F *hntrPerSec[3];
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hntrPerSec%c", xuv[i]);
		sprintf(title, "Number of %c Tracks Per Data Section;ntracks/section", xuv[i]);
		hntrPerSec[i] = new TH1F(name, title, 11, -1.5, 9.5);
		objLs[0].push_back(hntrPerSec[i]);
	} // end loop over XUV
	TH1F *hntrPerSec3D = new TH1F("hntrPerSec3D", "3D Track Horizontal Angle Distribution; ntracks/section", 11, -1.5, 9.5);
	TH1F *hHAng = new TH1F("hHAng", "3D Track Horizontal Angle Distribution;angle [degree]", 2000, -50., 30.);
	TH1F *hVAng = new TH1F("hVAng", "3D Track Vertical Angle Distribution;angle [degree]", 600, -30., 30.);
	objLs[0].push_back(hHAng); objLs[0].push_back(hVAng);
	// statistics of number of fired anode layers per section
	TH1F *hnF[3], *hgGOOD[3]; // [XUV]
	TH2F *htt[3][3], *hrr[3][3]; // [XUV][DC0-1-2]
	TH2F *htt_3D[3][3], *hrr_3D[3][3]; // [XUV][DC0-1-2]
	TH1F *h3DMissxPre[3], *h3DMissxPost[3], *hdt[3][3]; // [DC0-1-2][XUV]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hnF_%d", i);
		sprintf(title, "Number of Fired %c Anode Layers Per 3D Track;nF%c", xuv[i], xuv[i]);
		hnF[i] = new TH1F(name, title, 10, -1.5, 8.5);
		sprintf(name, "hgGOOD%c", xuv[i]);
		sprintf(title, "gGOOD-Fired %c Anode Layers per Track", xuv[i]);
		hgGOOD[i] = new TH1F(name, title, 11, -1.5, 9.5);
		sprintf(name, "h3DMissx%dPre", i);
		sprintf(title, "x Deviation in 3D Coincidence at z of MWDC%d (Pre-Correction)", i);
		h3DMissxPre[i] = new TH1F(name, title, 5000, -100., 100.);
		sprintf(name, "h3DMissx%dPost", i);
		sprintf(title, "x Deviation in 3D Coincidence at z of MWDC%d (Post-Correction)", i);
		h3DMissxPost[i] = new TH1F(name, title, 5000, -100., 100.);
		objLs[0].push_back(hnF[i]); objLs[0].push_back(hgGOOD[i]);
		objLs[0].push_back(h3DMissxPre[i]); objLs[0].push_back(h3DMissxPost[i]);
		for(int j = 0; j < 3; j++){ // loop over DCs
			sprintf(name, "htt%c%d", xuv[i], j);
			sprintf(title, "t_{X1} v.s. t_{X2} for Vertical %c Tracks for MWDC%d;t_{X1} [ns];t_{X2} [ns]", xuv[i], j);
			htt[i][j] = new TH2F(name, title, 500, -100., 400., 500, -100., 400.);
			sprintf(name, "hrr%c%d", xuv[i], j);
			sprintf(title, "r_{X1} v.s. r_{X2} for Vertical %c Tracks for MWDC%d;r_{X1} [mm];r_{X2} [mm]", xuv[i], j);
			hrr[i][j] = new TH2F(name, title, 500, -0.2, 6., 500., -0.2, 6.);
			objLs[4].push_back(htt[i][j]); objLs[4].push_back(hrr[i][j]);
			sprintf(name, "htt_3D_%c%d", xuv[i], j);
			sprintf(title, "t_{X1} v.s. t_{X2} for Vertical 3D %c Tracks for MWDC%d;t_{X1} [ns];t_{X2} [ns]", xuv[i], j);
			htt_3D[i][j] = new TH2F(name, title, 500, -100., 400., 500, -100., 400.);
			sprintf(name, "hrr_3D_%c%d", xuv[i], j);
			sprintf(title, "r_{X1} v.s. r_{X2} for Vertical 3D %c Tracks for MWDC%d;r_{X1} [mm];r_{X2} [mm]", xuv[i], j);
			hrr_3D[i][j] = new TH2F(name, title, 500, -0.2, 6., 500., -0.2, 6.);
			objLs[7].push_back(htt_3D[i][j]); objLs[7].push_back(hrr_3D[i][j]);
			sprintf(name, "Hdt_DC%d%c", i, xuv[j]);
			sprintf(title, "Drift Time Distribution of MWDC%d-%c;t [ns]", i, xuv[j]);
			hdt[i][j] = new TH1F(name, title, 500, -100., 400.);
			objLs[3].push_back(hdt[i][j]);
		} // end loop over DCs
	} // end loop over XUV
	TH1F *hnF3D = new TH1F("hnF3D", "Number of All Fired Anode Layers Per 3D Track;nF", 22, -1.5, 20.5);
	TH1F *h3DMissxTotPre = new TH1F("h3DMissxTotPre", "x Deviation in 3D Coincidence at z of all MWDCs (Pre-Correction)", 500, -100., 100.);
	TH1F *h3DMissxTotPost = new TH1F("h3DMissxTotPost", "x Deviation in 3D Coincidence at z of all MWDCs (Post-Correction)", 500, -100., 100.);
	TH1F *hXMag = new TH1F("hXMag", "x at the Entrance of the Magnetic Field", 1000, -500., 500.);
	TH1F *hYMag = new TH1F("hYMag", "y at the Entrance of the Magnetic Field", 500, -250., 250.);
	objLs[0].push_back(hnF3D); objLs[0].push_back(h3DMissxTotPre); objLs[0].push_back(h3DMissxTotPost);
	objLs[0].push_back(hXMag); objLs[0].push_back(hYMag);
	TH1F *heff = new TH1F("heff", "MWDC efficiency - Number of X-U-V Tracks;X:Tot-DC0(X1-X2)-DC1-DC2--U--V", 30, -2.5, 27.5);
	objLs[0].push_back(heff);
	TH1F *hr_ = new TH1F("hr_", "hr_", 500, -6.5, 6.5); // a temporary histogram for testing
	objLs[0].push_back(hr_);
///////////////////////////////////////// END OF THE HISTOGRAM DEFINITION //////////////////////////


	const int n = treeTrack->GetEntries(); // number of data sections
	int ntrTot[3]{}, n3DtrTot = 0; // total number of tracks of all kinds of type
	int ntrPerSec[3][100]{}, n3DtrPerSec[ntrMax3D]{}; // [XUV][n3Dtr]
	cout << "Totally " << n << " data sections would be processed.\n";
	int hasAllCnt = 0; // count of sections that have X, U and V tracks
	int hasXUVCnt[3]{}; // count of sections that have X, U or V tracks
	int effTot = 0, eff[3][3][2]{}; // efficiency[DC][type][layer]
	for(int i = 0; i < n; i++){ // loop over data sections
		treeTrack->GetEntry(i);
		// identify 3-D tracks //
		int n3DtrXUV[3]{}, n3DtrXUVT[3]{}; // DC L or R; DC L and R
		int n3Dtr, n3DtrT, trkId[ntrMax3D][3]{}; // track id [3D track id][XUV]
		memset(trkId, -1, sizeof(trkId));
		for(int j = 0; j < ntr; j++) if(-1 != id[j]){ // loop over tracks in one event
			for(int k = 0; k < 3; k++){ // loop over X-U-V track types
				if(type[j]%10 == k){
					trkId[id[j]][k] = j;
					n3DtrXUVT[k]++;
					if(LRTAG == type[j]/10) n3DtrXUV[k]++;
				}
			} // end loop over X-U-V tracks types
		} // end loop over tracks in one event
		if(n3DtrXUV[0] != n3DtrXUV[1] || n3DtrXUV[0] != n3DtrXUV[2])
			TAPopMsg::Error("TAAssess", "EvalDCArr3D: This is odd... track projections of X, U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2]);
		if(n3DtrXUVT[0] != n3DtrXUVT[1] || n3DtrXUVT[0] != n3DtrXUVT[2])
			TAPopMsg::Error("TAAssess", "EvalDCArr3D: This is odd... track projections of X, U and V are not consistent: n3DtrXT: %d, n3DtrUT: %d, n3DtrVT: %d", n3DtrXUVT[0], n3DtrXUVT[1], n3DtrXUVT[2]);
		n3Dtr = n3DtrXUV[0]; n3DtrTot += n3Dtr; n3DtrPerSec[n3Dtr]++; n3DtrT = n3DtrXUVT[0];
//		cout << "n3DtrT: " << n3DtrT << endl; // DEBUG
//		cout << "n3Dtr: " << n3Dtr << "\ttype[0]: " << type[0] << endl; getchar(); // DEBUG
		// cache the last value of array ntrTot
		int ntrTot_pre[3] = {ntrTot[0], ntrTot[1], ntrTot[2]};
		for(int j = 0; j < ntr; j++) if(LRTAG == type[j]/10) ntrTot[type[j]%10]++;
		for(int j = 0; j < 3; j++) ntrPerSec[j][ntrTot[j]-ntrTot_pre[j]]++; // [XUV][ntrPerSec]
		// loop over 3D tracks //
		for(int jj = 0; jj < n3DtrT; jj++){ // loop over 3D tracks in a data section
//			cout << "LRTAG: " << LRTAG << "\ttype[trkId[jj][0]]/10: " << type[trkId[jj][0]]/10 << endl; getchar(); // DEBUG
			if(LRTAG != type[trkId[jj][0]]/10) continue; // not the required MWDC array
			// check 3D match quality
			bool BINGO = false; const double eM = 10.; // xMiss3D limit
			if(fabs(xMiss3D[trkId[jj][0]][0]) < eM && fabs(xMiss3D[trkId[jj][0]][1]) < eM && fabs(xMiss3D[trkId[jj][0]][2]) < eM){ BINGO = true; effTot++; } // a valid 3D track
			if(!BINGO) continue;
			// count effective measurements
			int nFXUV[3]{}; // fired anode layers in a data section
			for(int j = 0; j < 3; j++){ // loop over XUV
				for(int k = 0; k < 6; k++) // loop over DC0-X1X2-DC1-X1X2-DC2-X1X2
					if(-1 != nu[trkId[jj][j]][k]){ nFXUV[j]++; if(BINGO) eff[k/2][j][k%2]++; }
			} // end loop over XUV
			const int nF = nFXUV[0] + nFXUV[1] + nFXUV[2]; // number of measured points
			for(int j = 0; j < 3; j++) hnF[j]->Fill(nFXUV[j]);
			hnF3D->Fill(nF);
			double p[4]; // [0-3]: k1, k2, b1, b2
			p[0] = k[trkId[jj][0]]; // k1
			p[2] = b[trkId[jj][0]]; // b1
			p[1] = TAMath::kUV_Y(phiAvrg, k[trkId[jj][1]], k[trkId[jj][2]]); // k2
			p[3] = TAMath::bUV_Y(phiAvrg, k[trkId[jj][1]], k[trkId[jj][2]], b[trkId[jj][1]], b[trkId[jj][2]]); // b2
			hHAng->Fill(atan(p[0])/DEGREE); hXMag->Fill(p[0]*1050.+p[2]);
			hVAng->Fill(atan(p[1])/DEGREE); hYMag->Fill(p[1]*1050.+p[3]);
			/// calculate and fill the 3D track coincidence index: x deviations
			double k1t = TAMath::kUV_X(phiAvrg, k[trkId[jj][1]], k[trkId[jj][2]]); // k1
			double b1t = TAMath::bUV_X(phiAvrg, k[trkId[jj][1]], k[trkId[jj][2]], b[trkId[jj][1]], b[trkId[jj][2]]); // b1
			for(int k = 0; k < 3; k++){ // loop over DCs
				double xt = k1t*zDC[k]+b1t, x = p[0]*zDC[k]+p[2];
				h3DMissxPost[k]->Fill(x-xt);
				h3DMissxTotPost->Fill(x-xt);
				h3DMissxPre[k]->Fill(xMiss3D[trkId[jj][0]][k]);
				h3DMissxTotPre->Fill(xMiss3D[trkId[jj][0]][k]);
//				cout << "\nk: " << k << "\tx: " << x << "\txt: " << xt << endl; // DEBUG
//				cout << "\tx-xt: " << x-xt; // DEBUG
//				cout << "\txMiss3D: " << xMiss3D[trkId[jj][0]][k] << endl; getchar(); // DEBUG
			} // end loop over DCs
			// 3D statistics
			if(is3D){ // if 3D tracking has been implemented
				hChi_3D->Fill(Chi3D[jj]);
				hchi2_3D->Fill(chi2_3D[jj]);
				const double b[3] = {k1_3D[jj], k2_3D[jj], 1.}; // track direction vector
				double alpha[3][3]{}; // angle between track projection and drift cell; [DC][XUV]
				for(int j = 0; j < 3; j++){ // loop over XUV
					// alpha-angle: track projection to normal plane of sense wires
					for(int k = 3; k--;){ // v.s. the detector-local z-axis
						alpha[k][j] = TAMath::AlphaAngle3D(b, ag[k][j], agAxis[k][j]);
//						double a1 = TAMath::VecAng3D(b, agAxis[k][j]) - Pi / 2.; // DEBUG
//						double a2 = alpha[k][j]; // DEBUG
//						cout << "a1: " << a1  << "\ta2: " << a2 << endl; getchar(); // DEBUG
//						if(fabs(a1)<1.*DEGREE){ // DEBUG
//							cout << "a1: " << a1  << "\ta2: " << a2 << endl; getchar(); // DEBUG
//						} // DEBUG
//						double c1[3] = {0,0,1}, c2[3] = {0,1,0}, c3[3] = {1,0,0}; // DEBUG
//						cout << "alpha: " << TAMath::AlphaAngle3D(c1,c2,c3) << endl; // DEBUG
//						getchar(); // DEBUG
					} // end for
//					cout << "alpha[0][0]: " << alpha[0][0] << endl; getchar(); // DEBUG
					const int it = trkId[jj][j]; // id of track projections
					for(int k = 0; k < 6; k++){ // loop over DC0-X1X2-DC1-X1X2-DC2-X1X2
						const int dcId = k / 2;
						const int STRid = TAAnodePara::GetSTRid(alpha[dcId][j]);
						if(-1 != nu[it][k]){
							const double tt = t[it][k], dr = chi3D[jj][j*6+k];
							const double rc = r[it][k] + dr;
							hchi_3D->Fill(dr);
							hrt01_3D->Fill(tt, rc);
							hdrt01_3D->Fill(tt, dr);
							hrt02_3D[j]->Fill(tt, rc);
							hdrt02_3D[j]->Fill(tt, dr);
							hrt03_3D[j][dcId]->Fill(tt, rc);
							hdrt03_3D[j][dcId]->Fill(tt, dr);

							if(0 == j && 0 == k && 40 == nu[j][k]){
								hrt04_3D->Fill(tt, rc);
								hdrt04_3D->Fill(tt, dr);
								hrt04_3D_STR[STRid]->Fill(tt, rc);
								hdrt04_3D_STR[STRid]->Fill(tt, dr);
								hdrt04_3D_sample->Fill(rc, dr);
							} // end inner if
						}
					} // end loop over six sense wire layers for one type
//					hr_->Fill(alpha[2][0]/DEGREE);
					for(int k = 0; k < 3; k++){ // loop over MWDCs
						if(alpha[k][j] > -1. * DEGREE && alpha[k][j] < 1. * DEGREE){
							if(nu[it][2*k] >= 30 && nu[it][2*k] <= 50){
								htt_3D[j][k]->Fill(t[it][2*k], t[it][2*k+1]);
								hrr_3D[j][k]->Fill(r[it][2*k], r[it][2*k+1]);
							} // end if(nu...)
						} // end if(ang > ..)
					} // end for over MWDCs
				} // end loop over XUV
			} // end if(is3D)
		} // end loop over 3D tracks
		bool hasXUV[3]{}; // whether the data section has X, U or V track projections
		for(int j = 0; j < ntr; j++){ // end for over track projections
			if(type[j]/10 != LRTAG) continue; // DCArrL or DCArrR
			for(int k = 0; k < 3; k++){ // end for over XUV
				if(type[j]%10 == k) hasXUV[k] = true;
			} // end foor over XUV
			const int dcType = type[j]%10;
			heff->Fill(dcType*8 + 2);
			for(int l = 0; l < 6; l++){
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

					if(0 == dcType && 0 == l && 40 == nu[j][l]){
						hrt04->Fill(tt, rc);
						hdrt04->Fill(tt, dr);
						hrt04_STR[STRid]->Fill(tt, rc);
						hdrt04_STR[STRid]->Fill(tt, dr);
						hdrt04_sample->Fill(rc, dr);
					} // end inner if
				} // end if(nu[j][l] != -1)
				if(-1 != nu[j][l]) hdt[dcId][dcType]->Fill(t[j][l]);
				if(l%2 == 0) hSTRid->Fill(STRid);
			} // end for over six sense wire layers
			hChi->Fill(Chi[j]);
			hchi2->Fill(chi2[j]);
			hgGOOD[dcType]->Fill(gGOOD[j]);
			// fill the time loop from approximate perpendicular tracks
			double ang0 = atan(k[j]), ang = ang0;
			for(int l = 0; l < 3; l++){ // loop over three DCs
				if(0 == dcType) ang = ang0 - phiDC[l] + 0. * DEGREE;
				if(0 == dcType && 1 == l) hr_->Fill(ang/DEGREE);
				if(ang > -2. * DEGREE && ang < 1. * DEGREE){
					if(nu[j][2*l] >= 30 && nu[j][2*l] <= 50){
						htt[dcType][l]->Fill(t[j][2*l], t[j][2*l+1]);
						hrr[dcType][l]->Fill(r[j][2*l], r[j][2*l+1]);
					} // end if
				} // end if
			} // end for over three DCs
		} // end for over track projections
		if(hasXUV[0] && hasXUV[1] && hasXUV[2]) hasAllCnt++;
		if(hasXUV[0] && !hasXUV[1] && hasXUV[2]){ // DEBUG
			cout << "\nindex: " << index << "\ti: " << i << endl; getchar(); // DEBUG
		} // DEBUG
		for(int j = 3; j--;) if(hasXUV[j]) hasXUVCnt[j]++;
		cout << "Data section " << i << " processed.\r" << flush;
	} // end loop over data sections
	// post-loop histogramming
	hntrTot->SetBinContent(2, n); // nsec
	hntrTot->SetBinContent(6, n3DtrTot); // n 3D tracks
	for(int i = 0; i < 3; i++){ // loop over X-U-V
		hntrTot->SetBinContent(i+3, ntrTot[i]); // X-U-V
		for(int j = 0; j < 10; j++) // for data sections with 0-10 tracks
			hntrPerSec[i]->SetBinContent(j+2, ntrPerSec[i][j]); // X-U-V
	} // end for over X-U-V
	for(int i = 0; i < 10; i++) hntrPerSec3D->SetBinContent(i+2, n3DtrPerSec[i]);

	// print some information to the screen
	cout << "\n\nhasXCnt: \033[1m" << hasXUVCnt[0];
	cout << "\t\033[0mhasUCnt: \033[1m" << hasXUVCnt[1];
	cout << "\t\033[0mhasVCnt: \033[1m" << hasXUVCnt[2] << "\n\033[0m";
	cout << "\nXTrkCnt: \033[1m" << ntrTot[0];
	cout << "\t\033[0mUTrkCnt: \033[1m" << ntrTot[1];
	cout << "\t\033[0mVTrkCnt: \033[1m" << ntrTot[2] << endl;
	cout << "\nhasAllCnt: \033[1m" << hasAllCnt;
	cout << "\033[0m\tTotal 3D track count: \033[1m" << n3DtrTot;
	cout << "\033[0m\nMatch Success rate (n3DtrTot/ntrTotX): \033[1m";
	cout << double(n3DtrTot) / ntrTot[0] << "\033[0m\n";
	cout << "\n\033[31;1m_______________ \033[32;1msoftware efficiency ";
	cout << "\033[31;1m__________________\033[0m\n\n";
	cout.setf(std::ios_base::fixed);
	cout << setw(14) << "DC0" << setw(12) << "DC1" << setw(12) << "DC2" << endl;
	for(int i = 0; i < 3; i++) for(int j = 0; j < 2; j++){ // loop over XUV - LAYER1-2
		cout << xuv[i] << j + 1;
		double effDC0 = eff[0][i][j], effDC1 = eff[1][i][j], effDC2 = eff[2][i][j];
		if(0 == effTot){ effDC0 = 0; effDC1 = 0; effDC2 = 0; }
		else{ effDC0 /= effTot; effDC1 /= effTot; effDC2 /= effTot; }
		if(!n3DtrTot && 0 == i){ // only calculate X efficiencies
			const double n = double(ntrTot[0]);
			effDC0 = heff->GetBinContent(6+0*2+j) / n;
			effDC1 = heff->GetBinContent(6+1*2+j) / n;
			effDC2 = heff->GetBinContent(6+2*2+j) / n;
		}
		cout << setw(13) << "\033[32;1m" << effDC0;
		cout << setw(12) << effDC1;
		cout << setw(12) << effDC2 << "\033[0m\n";
		if(j%2 == 1) cout << endl;
	}
	cout << "\n\n\033[33;1mDONE\033[0m\n\n";

	// write //
	
	cout << "The results would be stored in ROOT file directory \"\033[36;1m" << topdir << "\"\n\033[0m";
//	if(!f->FindObjectAny(topdir)) f->mkdir(topdir); f->cd(topdir);
	char s[128]; strcpy(s, ("assess"+rootfile).c_str());
//	if(0 == runid && 0 == access(s, F_OK)) system(("rm "+string(s)).c_str());
	TFile *fw = new TFile(s, "UPDATE");
	if(!fw->FindObjectAny(topdir)) fw->mkdir(topdir);
	fw->cd(topdir);
	for(int i = 0; i < ndir; i++){
		sprintf(s, "%s/%s", topdir, dir[i]);
		if(!fw->FindObjectAny(dir[i])) fw->mkdir(s);
		fw->cd(s);
		for(TObject *&b : objLs[i]) if(b){
			b->Write("", TObject::kOverwrite);
		}
		objLs[i].clear();
	}
	for(auto &ls : objLs) for(auto &b : ls) if(b) delete b;
	fw->Close(); delete fw; fw = nullptr;
	f->Close(); delete f; f = nullptr;
} // end of member function EvalDCArr3D

// evaluation done after Eval event by event
void TAAssess::PostEval(int round, bool isDCArrR){
	if(!strcmp("", fROOTFile.c_str()))
		TAPopMsg::Error("TAAssess", "PostEval: rootfile name is empty");
	TAPopMsg::Info("TAAssess", "PostEval: Input rootfile: %s", fROOTFile.c_str());
	if(0 != access(fROOTFile.c_str(), F_OK))
		TAPopMsg::Error("TAAssess", "PostEval: Input rootfile %s doesn't exist", fROOTFile.c_str());

	bool success0 = PostEval("assess"+fROOTFile, round, isDCArrR, true);
	bool success1 = PostEval("assess"+fROOTFile, round, isDCArrR, false);
	if(!success0 && !success1)
		TAPopMsg::Info("TAAssess", "PostEval: No eligible hrt_04_sample is found whatsoever and wheresoever\n");
	else
		cout << "\033[33;1m\n\nDONE\n\n\033[0m";
}
bool TAAssess::PostEval(const string &rootfile, int round, bool isDCArrR, bool is3D){
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	char name[128], lr[] = "LR", dir[64]; // name: TH2F name - dr-DCA
	if(is3D){
		sprintf(dir, "assess%d%c/3Ddrt%d%c", round, lr[isDCArrR], round, lr[isDCArrR]);
		sprintf(name, "%s/hdrt04_3D_sample", dir);
	}
	else{
		sprintf(dir, "assess%d%c/drt%d%c", round, lr[isDCArrR], round, lr[isDCArrR]);
		sprintf(name, "%s/hdrt04_sample", dir);
	}
	TH2F *h2 = (TH2F*)f->Get(name);
	if(!h2){
//		TAPopMsg::Error("TAAssess", "PostEval: %s doesn't exist", name);
		return false;
	}
	TAPopMsg::Info("TAAssess", "PostEval: isDCArrR: %d, is3D: %d", isDCArrR, is3D);
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
	else TAPopMsg::Info("TAAssess", "PostEval: gSigma has no data");
	if(gMean->GetN()) gMean->Write("", TObject::kOverwrite);
	else TAPopMsg::Info("TAAssess", "PostEval: gMean has no data");

	delete fgaus; fgaus = nullptr; delete gSigma; gSigma = nullptr;
	delete gMean; gMean = nullptr; f->Close(); delete f; f = nullptr;

	return true;
}












