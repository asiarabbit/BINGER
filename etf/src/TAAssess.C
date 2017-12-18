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
// Last modified: 2017/12/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <vector>

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
TAAssess::TAAssess() : fDetList(0), fRunId(0){
	if(!fDetList) fDetList = &TAParaManager::Instance()->GetDetList();
}
TAAssess::~TAAssess(){}

void TAAssess::EvalDCArr(bool isDCArrR){
	EvalDCArr(fROOTFile, fDetList, isDCArrR, fRunId);
}
void TAAssess::EvalDCArr(const string &rootfile, DetArr_t *detList, bool isDCArrR, unsigned short runid){
	if(!strcmp("", rootfile.c_str()))
		TAPopMsg::Error("TAAssess", "EvalDCArr: rootfile name is empty");
	if(!detList)
		TAPopMsg::Error("TAAssess", "EvalDCArr: Detector List pointer detList is null");
	TAMWDCArray *dcArrL = (TAMWDCArray*)(*detList)[3];
	TAMWDCArray *dcArrR = (TAMWDCArray*)(*detList)[4];
	if(!dcArrL && !dcArrR)
		TAPopMsg::Error("TAAssess", "EvalDCArr: Both MWDC arrays are null. TAEventProcessor::Configure() not run yet?");

	cout << "Input rootfile: " << rootfile << endl;
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	if(!treeTrack) TAPopMsg::Error("TAAssess", "EvalDCArr: treeTrack is nullptr");
	// default is for DCArrR
	char topdir[64]; short lrtag = 11;
	sprintf(topdir, "assess%dR", runid);
	TAMWDCArray *dcArr = dcArrR;
	if(!isDCArrR){
		sprintf(topdir, "assess%dL", runid); lrtag = 10;
		dcArr = dcArrL;
	}
	const short LRTAG = lrtag; // type/10: 10 -> dcArrL; 11 -> dcArrR
	cout << "The results would be stored in ROOT file directory \"" << topdir << "\"\n";
	if(f->FindObjectAny(topdir))
		TAPopMsg::Warn("TAAssess", "EvalDCArr: directory %s alrady exists. Assess0 may have been implemented", topdir);
	else f->mkdir(topdir);
	f->cd(topdir);

	// DC parameters
	const double phiAvrg = dcArr->GetPhiAvrg();
	const TAMWDC *dc[3] = {dcArr->GetMWDC(0), dcArr->GetMWDC(1), dcArr->GetMWDC(2)};
	const TADetectorPara *dcPar[3] = {dc[0]->GetDetPara(), dc[1]->GetDetPara(), dc[2]->GetDetPara()};
	const double phiDC[3] = {dcPar[0]->GetPhi(), dcPar[1]->GetPhi(), dcPar[2]->GetPhi()};
	// for calculation of 3D coincidence deviation in x direction
	const double zDC[3] = {dcPar[0]->GetZ(), dcPar[1]->GetZ(), dcPar[2]->GetZ()};

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

	// to store all the ROOT statistical objects
	vector<TObject *> objLs[5];
	const char dir[5][64] = {"misc", "rt", "drt", "dt", "rr"};
	char name[64], title[128], xuv[4] = "XUV";
	TH1F *hchi = new TH1F("hchi", "Overall Residual Distribution;dr [mm]", 1600, -8.0, 8.0);
	TH1F *hChi = new TH1F("hChi", "\\sqrt{\\chi2/ndf}\\hbox{~Per Track};\\chi~[mm]", 800, 0, 8.);
	TH1F *hchi2 = new TH1F("hchi2", "Track~\\chi^2~Distribution~\\sum(\\chi^2);\\chi^2~[mm^2]", 1000., -0.5, 100);
	objLs[0].push_back(hchi); objLs[0].push_back(hChi); objLs[0].push_back(hchi2);
	// hrt: DCA v.s. t //
	TH2F *hrt01 = new TH2F("hrt01", "Overall Distribution of Calculated Drift Distance and Drift Time;t [ns];r [mm]", 500, -100., 400., 800, -0.5, 7.5);
	objLs[1].push_back(hrt01);
	TH2F *hrt02[3], *hrt03[3][3]; // [0-3]: [XUV]; [0-3][0-3]: [XUV][DC0-1-2]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hrt02_%d", i);
		sprintf(title, "Distribution of DCA v.s. Drift Time for %c-Tracks;t [ns];DCA [mm]", xuv[i]);
		hrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[1].push_back(hrt02[i]);
		for(int j = 0; j < 3; j++){ // loop over three DCs
			sprintf(name, "hrt03_%d_%d", i, j);
			sprintf(title, "Distribution of DCA v.s. Drift Time for %c-Track-DC%d;t [ns];DCA [mm]", xuv[i], j);
			hrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
			objLs[1].push_back(hrt03[i][j]);
		} // end loop over DCs
	} // end loop over XUV
	TH2F *hrt04 = new TH2F("hrt04", "Distribution of DCA v.s. Drift Time for DC0X1Anode40;t [mm];DCA [mm]", 500, -100., 400., 800, -0.5, 7.5);
	objLs[1].push_back(hrt04);
	TH2F *hrt04_STR[nAng];
	for(int i = 0; i < nAng; i++){
		sprintf(name, "hrt04_STR_%d", i);
		sprintf(title, "Distribution of DCA and Drift Time for DC0X1Anode40STR_%d(alpha: %f~%f);t [ns]; r [mm]", i, (i-nAng/2.)*angStep, (i-nAng/2.+1.)*angStep);
		hrt04_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		objLs[1].push_back(hrt04_STR[i]);
	}
	// hdrt: chi v.s. t //
	TH2F *hdrt01 = new TH2F("hdrt01", "Overall Distribution of Redisual and Drift Time;t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[2].push_back(hdrt01);
	TH2F *hdrt02[3], *hdrt03[3][3]; // [0-3]: [XUV]; [0-3]: [XUV][DC0-1-2]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hdrt02_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for %c-Tracks;t [ns];dr [mm]", xuv[i]);
		hdrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[2].push_back(hdrt02[i]);
		for(int j = 0; j < 3; j++){ // loop over DCs
			sprintf(name, "hdrt03_%d_%d", i, j);
			sprintf(title, "Distribution of Residual and Drift Time for %c-Tracks-DC%d", xuv[i], j);
			hdrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
			objLs[2].push_back(hdrt03[i][j]);
		} // end loop over DCs
	} // end loop over XUV
	TH2F *hdrt04 = new TH2F("hdrt04", "Distribution of Residual and Drift Time for DC0X1Anode40;t [ns];dr [mm]", 500, -100., 400., 800, -4.0, 4.0);
	objLs[2].push_back(hdrt04);
	TH2F *hdrt04_STR[nAng];
	for(int i = 0; i < nAng; i++){
		sprintf(name, "hdrt04_STR_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for DC0X1Anode40STR_%d(alpha: %f~%f);t [ns];dr [mm]", i, (i-nAng/2.)*angStep, (i-nAng/2.+1.)*angStep);
		hdrt04_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		objLs[2].push_back(hdrt04_STR[i]);
	}
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
	TH1F *hHAng = new TH1F("hHAng", "3D Track Horizontal Angle Distribution;angle [degree]", 2000, 5., 30.);
	TH1F *hVAng = new TH1F("hVAng", "3D Track Vertical Angle Distribution;angle [degree]", 600, -30., 30.);
	objLs[0].push_back(hHAng); objLs[0].push_back(hVAng);
	// statistics of number of fired anode layers per section
	TH1F *hnF[3], *hgGOOD[3]; // [XUV]
	TH2F *htt[3][3], *hrr[3][3]; // [XUV][DC0-1-2]
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
			sprintf(title, "t_{X1} v.s. t_{X2} for Vertical %c Tracks for MWDC%d;r_{X1} [mm];r_{X2} [mm]", xuv[i], j);
			hrr[i][j] = new TH2F(name, title, 500, -0.2, 6., 500., -0.2, 6.);
			objLs[4].push_back(htt[i][j]); objLs[4].push_back(hrr[i][j]);
			sprintf(name, "Hdt_DC%d%c", i, xuv[j]);
			sprintf(title, "Drift Time Distribution of MWDC%d-%c;t [ns]", i, xuv[j]);
			hdt[i][j] = new TH1F(name, title, 500, -100., 400.);
			objLs[3].push_back(hdt[i][j]);
		} // end loop over DCs
	} // end loop over XUV
	TH1F *hnF3D = new TH1F("hnF3D", "Number of All Fired Anode Layers Per 3D Track;nF", 22, -1.5, 20.5);
	TH1F *h3DMissxTotPre = new TH1F("h3DMissxTotPre", "x Deviation in 3D Coincidence at z of all MWDCs (Pre-Correction)", 50000, -100., 100.);
	TH1F *h3DMissxTotPost = new TH1F("h3DMissxTotPost", "x Deviation in 3D Coincidence at z of all MWDCs (Post-Correction)", 50000, -100., 100.);
	TH1F *hXMag = new TH1F("hXMag", "x at the Entrance of the Magnetic Field", 1000, -500., 500.);
	TH1F *hYMag = new TH1F("hYMag", "y at the Entrance of the Magnetic Field", 500, -250., 250.);
	objLs[0].push_back(hnF3D); objLs[0].push_back(h3DMissxTotPre); objLs[0].push_back(h3DMissxTotPost);
	objLs[0].push_back(hXMag); objLs[0].push_back(hYMag);
	TH1F *heff = new TH1F("heff", "MWDC efficiency - Number of X-U-V Tracks;X:Tot-DC0(X1-X2)-DC1-DC2--U--V", 30, -2.5, 27.5);
	objLs[0].push_back(heff);

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
		int n3DtrXUV[3]{}, n3Dtr, trkId[ntrMax3D][3]; // track id [3D track id][XUV]
		for(int j = 0; j < ntr; j++) if(-1 != id[j]){ // loop over tracks in one event
			if(LRTAG != type[j]/10) continue; // DCArrL or DCArrR
			for(int k = 0; k < 3; k++){ // loop over X-U-V track types
				if(type[j]%10 == k){
					trkId[id[j]][k] = j;
					n3DtrXUV[k]++;
				}
			} // end loop over X-U-V tracks types
		} // end loop over tracks in one event
		if(n3DtrXUV[0] != n3DtrXUV[1] || n3DtrXUV[0] != n3DtrXUV[2])
			TAPopMsg::Error("TAAssess", "EvalDCArr3D: This is odd... track projections of X, U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2]);
		n3Dtr = n3DtrXUV[0]; n3DtrTot += n3Dtr; n3DtrPerSec[n3Dtr]++;
//		cout << "n3Dtr: " << n3Dtr << "\ttype[0]: " << type[0] << endl; getchar(); // DEBUG
		// cache the last value of array ntrTot
		int ntrTot_pre[3] = {ntrTot[0], ntrTot[1], ntrTot[2]};
		for(int j = 0; j < ntr; j++) if(type[j]/10 == LRTAG) ntrTot[type[j]%10]++;
		for(int j = 0; j < 3; j++) ntrPerSec[j][ntrTot[j]-ntrTot_pre[j]]++; // [XUV][ntrPerSec]
		// loop over 3D tracks //
		for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section
			int nFXUV[3]{}; // fired anode layers in a data section
			bool BINGO = false;
			double eM = 10.; // xMiss3D limit
			if(fabs(xMiss3D[trkId[jj][0]][0]) < eM && fabs(xMiss3D[trkId[jj][0]][1]) < eM && fabs(xMiss3D[trkId[jj][0]][2]) < eM){ BINGO = true; effTot++; } // a valid 3D track
			if(!BINGO) continue;
			for(int j = 0; j < 6; j++){ // count effective measurements
				for(int k = 0; k < 3; k++)
					if(-1 != nu[trkId[jj][k]][j]){ nFXUV[k]++; if(BINGO) eff[j/2][k][j%2]++; }
			} // end loop over six sense wire layers for one type
			const int nF = nFXUV[0] + nFXUV[1] + nFXUV[2]; // number of measured points
			for(int j = 0; j < 3; j++) hnF[j]->Fill(nFXUV[j]); hnF3D->Fill(nF);
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
				const int DCid = l / 2;
				const int STRid = dcArr->GetMWDC(DCid)->GetSTRid(k[j], dcType);
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
					hrt03[dcType][DCid]->Fill(tt, rc);
					hdrt03[dcType][DCid]->Fill(tt, dr);
					if(0 == dcType || 0 == l || 40 == nu[j][l]){
						hrt04->Fill(tt, rc);
						hdrt04->Fill(tt, dr);
						hrt04_STR[STRid]->Fill(tt, rc);
						hdrt04_STR[STRid]->Fill(tt, dr);
					} // end inner if
				} // end if(nu[j][l] != -1)
				if(-1 != nu[j][l]) hdt[DCid][dcType]->Fill(t[j][l]);
				hSTRid->Fill(STRid);
			} // end for over six sense wire layers
			hChi->Fill(Chi[j]);
			hchi2->Fill(chi2[j]);
			hgGOOD[dcType]->Fill(gGOOD[j]);
			// fill the time loop from approximate perpendicular tracks
			double ang = atan(k[j]);
			if(0 == dcType) ang -= phiDC[1] + 0. * DEGREE;
			if(ang > -1. * DEGREE && ang < 2. * DEGREE){
				for(int l = 0; l < 3; l++){ // loop over three DCs
					if(nu[j][2*l] >= 30 && nu[j][2*l] <= 50){
						htt[dcType][l]->Fill(t[j][2*l], t[j][2*l+1]);
						hrr[dcType][l]->Fill(r[j][2*l], r[j][2*l+1]);
					} // end if
				} // end for over three DCs
			} // end if
		} // end for over track projections
		if(hasXUV[0] && hasXUV[1] && hasXUV[2]) hasAllCnt++;
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

	// write //
	for(int i = 0; i < 5; i++){
		char s[128]; sprintf(s, "%s/%s", topdir, dir[i]);
		if(!f->FindObjectAny(dir[i])) f->mkdir(s); f->cd(s);
		for(TObject *&b : objLs[i]) if(b) b->Write("", TObject::kOverwrite);
	}
	// print some information to the screen
	cout << "\n\nhasXCnt: \033[1m" << hasXUVCnt[0];
	cout << "\t\033[0mhasUCnt: \033[1m" << hasXUVCnt[1];
	cout << "\t\033[0mhasVCnt: \033[1m" << hasXUVCnt[2];
	cout << "\n\nhasAllCnt: \033[1m" << hasAllCnt;
	cout << "\033[0m\tTotal 3D track count: \033[1m" << effTot;
	cout << "\033[0m\nCoincidence Success rate: \033[1m";
	cout << double(effTot) / hasXUVCnt[0] << "\033[0m\n";
	cout << "\n__________ software efficiency __________________\n";
	cout.setf(std::ios_base::fixed);
	cout << setw(14) << "DC0" << setw(12) << "DC1" << setw(12) << "DC2" << endl;
	for(int i = 0; i < 3; i++) for(int j = 0; j < 2; j++){
		cout << xuv[i] << j + 1;
		cout << setw(13) << "\033[32;1m" << double(eff[0][i][j]) / effTot;
		cout << setw(12) << double(eff[1][i][j]) / effTot;
		cout << setw(12) << double(eff[2][i][j]) / effTot << "\033[0m\n";
	}
	cout << "\n\n\033[33;1mDONE\033[0m\n\n";
	f->Close();
} // end of member function EvalDCArr3D












