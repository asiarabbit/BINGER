///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrTa.C												 //
//   TASTRCalibDCArrTa.C -- source file for class TASTRCalibDCArrTa					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. An		 //
// abstract base class.	Note that this is for DCs around the target, made by P.Ma.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/8.															     //
// Last modified: 2018/6/9, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <unistd.h>

// ROOT include
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"

#include "TAPopMsg.h"
#include "TASTRCalibDCArrTa.h"
#include "TAParaManager.h"
#include "TAMWDCArray2.h"
#include "TAMath.h"
#include "TADetectorPara.h"
#include "TAMWDC.h"
#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TAGPar.h"

using std::flush;
using std::ofstream;
using std::setw;
using std::ios_base;

// dcArrId: 0 1 2 3 4 5
// detId:   3 4 6 7 8 9
static int detIdMap[6] = {3, 4, 6, 7, 8, 9}; // [0-5]: [DCArr_L-R-DCTaArr_U-D-PDCArr_U-D]
const int TASTRCalibDCArrTa::nr_ = TAAnodePara::kSTRCorRNBins;
const double TASTRCalibDCArrTa::rmx_ = TAAnodePara::kSTRCorRMax;
const int TASTRCalibDCArrTa::nAng = TAAnodePara::kSTRCorAngleNBins;
const double TASTRCalibDCArrTa::rstep = TAAnodePara::kSTRCorRStep;
short TASTRCalibDCArrTa::fNAnodePerLayer = -1;

TASTRCalibDCArrTa::TASTRCalibDCArrTa(const string &rootfile, TAMWDCArray2 *dcArr)
		: fROOTFile(rootfile){
	fDCArr = dcArr; fIsBigStatistics = true;
}
TASTRCalibDCArrTa::~TASTRCalibDCArrTa(){}

// histogram chi and write down the histograms
// is3D: whether to use 3D chi to fill the histograms
void TASTRCalibDCArrTa::ChiHistogramming(bool is3D){
	if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArrTa", "ChiHistogramming: MWDC array pointer is null");
	ChiHistogramming(fROOTFile, fDCArr, is3D, fIsBigStatistics);
}
// is3D: whether to use 3D chi to fill the histograms
void TASTRCalibDCArrTa::ChiHistogramming(const string &rootfile, TAMWDCArray2 *dcArr, bool is3D, bool isBigSta){
	
	if(!dcArr) TAPopMsg::Error("TASTRCalibDCArrTa", "ChiHistogramming: null dcArr pointer");
	const short DETID = dcArr->GetDetId(); // 6: DCArrTaU; 7: DCArrTaD, 8: PDCArrU, 9: PDCArrD
	// for PDCs, the rmx and nr should be doubled
	double coeff = 1.; if(8 == DETID || 9 == DETID) coeff = 2.;
	const short nr = coeff * nr_; const double rmx = coeff * rmx_;
	bool usingPDC = TAGPar::Instance()->Val(83);
	if(usingPDC){ // DCTas and PDCs are exchanged
		// dcArrId: 0 1 2 3 4 5
		// detId:   3 4 8 9 6 7
		detIdMap[2] = 8; detIdMap[3] = 9;
		detIdMap[4] = 6; detIdMap[5] = 7;
	}

	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TASTRCalibDCArrTa", "ChiHistogramming: Input rootfile %s doesn't exist", rootfile.c_str());
	TAPopMsg::Info("TASTRCalibDCArrTa", "ChiHistogramming: Input rootfile name: %s", rootfile.c_str());
	TAPopMsg::Info("TASTRCalibDCArrTa", "ChiHistogramming: number of drift distance bins for STR auto-calibration %d; Maximum drift distance: %f", nr, rmx);
	TFile *f = new TFile(rootfile.c_str());
	if(!f->FindObjectAny("treeTrack"))
		TAPopMsg::Error("TASTRCalibDCArrTa", "ChiHistogramming: treeTrack not found in input rootfile");
	if(is3D && !f->FindObjectAny("treePID3D"))
		TAPopMsg::Error("TASTRCalibDCArrTa", "ChiHistogramming: 3D chi histogramming is required, yet treePID3D is not found");
//	TAParaManager::Instance()->UpdateSTRCorrection(); // keep up-to-date with the newest calibration
	fNAnodePerLayer = dcArr->GetMWDC(0)->GetNAnodePerLayer();
	// The x-axis of xX, xY, to calculate angle-alpha
	const double al[2][3] = {{1., 0., 0.}, {0., 1., 0.}}; // X-Y
	double ag[2][2][3]{}, agAxis[2][2][3]{}; // [DC#][XY][xyz];
	for(int i = 2; i--;) for(int j = 2; j--;){ // i: DC0-1-2; j: X-Y
		dcArr->GetMWDC(i)->GetDetPara()->GetGlobalRotation(al[j], agAxis[i][j]);
		dcArr->GetMWDC(i)->GetAnode(j, 1, 0)->GetAnodePara()->GetGlobalDirection(ag[i][j]);
	}

	const int ntrMax = 100, ntrMax3D = ntrMax / 3;
	int ntrT, index, type[ntrMax], id[ntrMax], nu[ntrMax][6];
	double kl[ntrMax];
	double t[ntrMax][6], r[ntrMax][6], chi[ntrMax][6], Chi[ntrMax];
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("t", t); // T1
	treeTrack->SetBranchAddress("r", r); // R
	treeTrack->SetBranchAddress("chi", chi); // R
	treeTrack->SetBranchAddress("Chi", Chi); // R
	treeTrack->SetBranchAddress("k", kl); // R, to get the STRid
	treeTrack->SetBranchAddress("type", type); // track type: 1[LR][XUV]
	treeTrack->SetBranchAddress("id", id); // 3-D track id

	double Chi3D[ntrMax3D], chi2_3D[ntrMax3D], chi3D[ntrMax3D][18];
	// x=k1*z+b1; y=k2*z+b2;
	double k1_3D[ntrMax3D], b1_3D[ntrMax3D], k2_3D[ntrMax3D], b2_3D[ntrMax3D];
	TTree *treePID3D = (TTree*)f->Get("treePID3D"); // storing 3D tracking and 3D-PID information
	if(is3D && treePID3D){
		treeTrack->AddFriend(treePID3D);
		treePID3D->SetBranchAddress("Chi", Chi3D);
		treePID3D->SetBranchAddress("chi2", chi2_3D);
		treePID3D->SetBranchAddress("chi", chi3D); // residuum [x6->u6->v6]
		treePID3D->SetBranchAddress("k1", k1_3D);
		treePID3D->SetBranchAddress("b1", b1_3D);
		treePID3D->SetBranchAddress("k2", k2_3D);
		treePID3D->SetBranchAddress("b2", b2_3D);
	}

/////////////////////////// Chi histogramming begins ///////////////////////////////////////
	// create TH2F objects for the STR correction fittings //
	if(fNAnodePerLayer <= 0) TAPopMsg::Error("TASTRCalibDCArrTa", "ChiHistogramming: minus fNAnodePerLayer");
	const int nAnodePerLayer = fNAnodePerLayer;
	TH2F *hDCSTRCor[2][2][2][nAnodePerLayer][nAng]{0}; // [DC#][XY][Layer][nu][STR_id]
	TH2F *hDCSTR_RT[2][2][2][nAnodePerLayer][nAng]{0}; // [DC#][XY][Layer][nu][STR_id] r-t 2D graph
	char name[64], title[256];
	for(int i = 0; i < 2; i++){ // loop over DCs
		for(int j = 0; j < 2; j++){ // loop over X-Y
			for(int m = 0; m < 2; m++){
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					for(int l = 0; l < nAng; l++){
						sprintf(name, "hDCSTRCor_%d_%d_%d_%d_%d", i, j, m, k, l);
						sprintf(title, "%s - dr-DriftDistance Relation;r [mm];dr [mm]", name);
						hDCSTRCor[i][j][m][k][l] = new TH2F(name, title, nr, 0., rmx, 300, -3.0, 3.0);
						sprintf(name, "hDCSTR_RT_%d_%d_%d_%d_%d", i, j, m, k, l);
						sprintf(title, "%s - DCA-t Relation;t [ns];DCA [mm]", name);
						hDCSTR_RT[i][j][m][k][l] = new TH2F(name, title, 35, 0., 280., 100, -0.5, 5.5);
					} // end for over i
				} // end for over k
			} // end for over m
		} // end of for over j
	} // end of for over i

	const int n = treeTrack->GetEntries();
	int nTrk = 0;
	cout << "Totally " << n << " data sections\n";
	for(int i = 0; i < n; i++){ // loop over data sections
		treeTrack->GetEntry(i);
		if(!is3D){
			for(int j = 0; j < ntrT; j++){ // loop over tracks in a data section
				const short dcArrId = type[j] / 10 % 10;
				if(DETID != detIdMap[dcArrId]) continue;
				int dcType = type[j]%10; // X-Y
//				if(-1 == id[j]) continue; // not a successful match
				if(Chi[j] > 0.9) continue; // nasty tracks
				nTrk++;
				for(int l = 0; l < 4; l++){
					if(nu[j][l] != -1){
						// rc: distance of anode to track
						const double tt = t[j][l], dr = chi[j][l], rr = r[j][l];
						const double rc = rr+dr;
						const int DCid = l/2, NU = nu[j][l];
						TAMWDC *dc = dcArr->GetMWDC(DCid);
						TAAnodePara *anoPar = dc->GetAnode(dcType, l%2+1, NU)->GetAnodePara();
						const int STRid = anoPar->GetSTRid(kl[j], dcType);
						const short nAnodePerLayer = dc->GetNAnodePerLayer();
						if(isBigSta){
							hDCSTRCor[DCid][dcType][l%2][NU][STRid]->Fill(rr, dr);
							hDCSTR_RT[DCid][dcType][l%2][NU][STRid]->Fill(tt, rc);
						} // end if
						else{ // every hit is shared by all anodes in the layer
							for(int k = 0; k < nAnodePerLayer; k++){
								hDCSTRCor[DCid][dcType][l%2][k][STRid]->Fill(rr, dr);
								hDCSTR_RT[DCid][dcType][l%2][k][STRid]->Fill(tt, rc);
							} // end for over anodes in a sense wire layer
						} // end else
					} // end if
				} // end for over j
			} // end for over j
		} // end of if(!is3D)
		else if(treePID3D){ // using chi from 3D tracks
			// identify 3-D tracks
			int n3DtrXY[2]{}, n3Dtr = 0;
			int trkId[ntrMax3D][3]; memset(trkId, -1, sizeof(trkId)); // track id [3D track id][XUV]
			// loop over grouped track projections
			for(int j = 0; j < ntrT; j++) if(-1 != id[j]){
				for(int k = 0; k < 2; k++){ // loop over X-Y track types
					if(type[j]%10 == k){
						trkId[id[j]][k] = j;
						n3DtrXY[k]++;
					}
				} // end loop over X-U-V track types
			} // end for over j and if
			if(n3DtrXY[0] != n3DtrXY[1]){
				TAPopMsg::Error("TASTRCaliDCArr", "ChiHistogramming: This is odd... track projections of X and Y are not consistent: n3DtrX: %d, n3DtrY: %d, ntrT: %d", n3DtrXY[0], n3DtrXY[1], ntrT);
			} // end if
			n3Dtr = n3DtrXY[0];
			// // // ^^^^^^^ circulation over 3-D tracks in one data section ^^^^^^^ // // //
			for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section
				const short dcArrId = type[trkId[jj][0]]/10%10;
				if(DETID != detIdMap[dcArrId]) continue;
				if(Chi3D[jj] > 0.9) continue; // nasty tracks
				const double b[3] = {k1_3D[jj], k2_3D[jj], 1.}; // track direction vector
				double alpha[2][2]{}; // angle between track projection and drift cell; [DC][XY]
				for(int j = 0; j < 2; j++){ // loop over X-Y
					// alpha-angle: track projection to normal plane of sense wires
					for(int k = 2; k--;) // v.s. the detector-local z-axis
						alpha[k][j] = TAMath::AlphaAngle3D(b, ag[k][j], agAxis[k][j]);
//					cout << "alpha[0][0]: " << alpha[0][0] << endl; getchar(); // DEBUG
					const int it = trkId[jj][j]; // id of track projections
					for(int k = 0; k < 4; k++){ // loop over DC0X1X2-DC1X1X2
						const int DCid = k/2, NU = nu[it][k];
						if(-1 == NU) continue;
						const short nAnodePerLayer = dcArr->GetMWDC(DCid)->GetNAnodePerLayer();
						const double tt = t[it][k], rr = r[it][k], dr = chi3D[jj][j*6+k];
						const double rc = rr+dr;
//						cout << "tt: " << tt << endl; // DEBUG
//						cout << "rc: " << rc << "\trr: " << rr << "\tdr: " << dr << endl; getchar(); // DEBUG
						const int STRid = TAAnodePara::GetSTRid(alpha[DCid][j]);
						if(isBigSta){
							hDCSTRCor[DCid][j][k%2][NU][STRid]->Fill(rr, dr);
							hDCSTR_RT[DCid][j][k%2][NU][STRid]->Fill(tt, rc);
						} // end if
						else{ // every hit is shared by all anodes in the layer
							for(int l = 0; l < nAnodePerLayer; l++){
								hDCSTRCor[DCid][j][k%2][l][STRid]->Fill(rr, dr);
								hDCSTR_RT[DCid][j][k%2][l][STRid]->Fill(tt, rc);
							} // end for over anodes in a sense wire layer
						} // end else
					} // end loop over 6 sense wire layers for a sense wire type
				} // end loop over X-U-V
			} // end loop over 3D tracks
		} // end else
		cout << "Processing data section " << i << "\r" << flush;
	} // end for over i
//	cout << nTrk / 3 << " 3D tracks has been adopted for STR correction.\n";

	TFile *fw = new TFile(("assess"+rootfile).c_str(), "UPDATE");
	sprintf(name, "STRCali-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name);
	if(!fw->FindObjectAny(name)) fw->mkdir(title); // store drift time histograms
	fw->cd(title); cout << endl;
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++) for (int m = 0; m < 2; m++)
	for(int k = 0; k < fNAnodePerLayer; k++) for(int s = 0; s < nAng; s++){
		if(hDCSTRCor[i][j][m][k][s]){
			if(hDCSTRCor[i][j][m][k][s]->GetEntries() > 2000.){
				hDCSTRCor[i][j][m][k][s]->Write("", TObject::kOverwrite);
				hDCSTR_RT[i][j][m][k][s]->Write("", TObject::kOverwrite);
				cout << "Writing Histo \033[34;1m" << i << " " << j << " " << m;
				cout << " " << k << " " << s << "\033[0m";
				cout << "   \tPlease wait..." << "\r" << flush;
			} // end innter if
		} // end outer if
		delete hDCSTRCor[i][j][m][k][s]; hDCSTRCor[i][j][m][k][s] = nullptr;
		delete hDCSTR_RT[i][j][m][k][s]; hDCSTR_RT[i][j][m][k][s] = nullptr;
	} // end the great loop
	cout << "\n\n\033[33;1mDONE\033[0m\n\n"; // DEBUG
	fw->Close(); delete fw; fw = nullptr;
	f->Close(); delete f; f = nullptr;
} // end of member function ChiHistogramming

// read the chi histograms and fit them to get the mean, which would be used as
// the correction of the initial STRs
// round: the iteration id
void TASTRCalibDCArrTa::GenerateSTRCorFile(int round){
	if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArrTa", "GenerateCalibFile: MWDC array pointer is null");
	GenerateCalibFile(fROOTFile, fDCArr, round);
}
void TASTRCalibDCArrTa::GenerateCalibFile(const string &rootfile, TAMWDCArray2 *dcArr, int round){
	if(!dcArr) TAPopMsg::Error("TASTRCalibDCArrTa", "GenerateCalibFile: null dcArr pointer");
	fNAnodePerLayer = dcArr->GetMWDC(0)->GetNAnodePerLayer();
	const short DETID = dcArr->GetDetId(); // 6: DCArrTaU; 7: DCArrTaD, 8: PDCArrU, 9: PDCArrD
	// for PDCs, the rmx and nr should be doubled
	double coeff = 1.; if(8 == DETID || 9 == DETID) coeff = 2.;
	const int nr = coeff * nr_; const double rmx = coeff * rmx_;


	TAPopMsg::Info("TASTRCalibDCArrTa", "GenerateCalibFile: Input rootfile name: %s, STR auto-calibration round id: %d, MWDC Array Name: %s", rootfile.c_str(), round, dcArr->GetName().c_str());
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TASTRCalibDCArrTa", "GenerateCalibFile: Input rootfile %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(("assess"+rootfile).c_str(), "UPDATE");
	char name[128], title[256], xy[] = "XY";
	sprintf(name, "STRCali-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name); // directory storing drift time histograms
	if(!f->FindObjectAny(name)){
		TAPopMsg::Error("TASTRCalibDCArrTa", "GenerateCalibFile: Directory %s doesn't exist. Maybe TASTRCalibDCArrTa::ChiHistogramming(...) hasn't been run yet.", name);
	}

	TH1F *hmeanTot = new TH1F("hmeanTot", "hmeanTot", 500, -0.8, 0.8);
	TH1F *hsigmaTot = new TH1F("hsigmaTot", "hsigmaTot", 500, 0., 1.5);
	TH2F *h2void = new TH2F("h2void", "h2void", nr, 0., rmx, 800, -4.0, 4.0); // for slick use
	TGraph *gsigmaTot = new TGraph();
	gsigmaTot->SetNameTitle("gsigmaTot", "Drift Distance v.s. Spatial Resolution;r [mm];\\sigma~[mm]");
	TH1F *hmean[2][2], *hsigma[2][2]; // [DC#][XY]
	TGraph *gsigma[2][2]; // [DC#][XY]
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++){
		sprintf(name, "hmean_DC%d_%c", i, xy[j]);
		hmean[i][j] = new TH1F(name, name, 500, -0.8, 0.8);
		sprintf(name, "hsigma_DC%d_%c", i, xy[j]);
		hsigma[i][j] = new TH1F(name, name, 500, 0., 1.5);
		sprintf(name, "gsigma_DC%d_%c", i, xy[j]);
		sprintf(title, "Drift Distance v.s. Spatial Resolution- DC%d %c;r [mm];\\sigma~[mm]", i, xy[j]);
		gsigma[i][j] = new TGraph();
		gsigma[i][j]->SetNameTitle(name, title);
	}

	// filling the recording tree
	int anodeId[4];
	double sigma_tree[nAng][nr], mean_tree[nAng][nr];
	if(nAng > 6 || nr > 120)
		TAPopMsg::Error("TASTRCalibDCArrTa", "GenerateCalibFile: nAng or nr is too large: nAng: %d, nr: %d", nAng, nr);
	int nAng_tree = nAng, nr_tree = nr;
	TTree *treeSigma = new TTree("treeSigma", "Spatial Resolution and STR Correction");
	treeSigma->Branch("anodeId", anodeId, "anodeId[4]/I");
	treeSigma->Branch("nAng", &nAng_tree);
	treeSigma->Branch("nr", &nr_tree);
	treeSigma->Branch("sigma", sigma_tree, "sigma[nAng][nr]/D");
	treeSigma->Branch("mean", mean_tree, "mean[nAng][nr]/D");
	// for filling r-sigma graph
	double ***sigma_g = new double**[2];
	int ***sigma_g_cnt = new int**[2];
	for(int i = 0; i < 2; i++){
		sigma_g[i] = new double*[2];
		sigma_g_cnt[i] = new int*[2];
		for(int j = 0; j < 2; j++){
			sigma_g[i][j] = new double[nr];
			sigma_g_cnt[i][j] = new int[nr];
			for(int k = 0; k < nr; k++){
				sigma_g[i][j][k] = 0.;
				sigma_g_cnt[i][j][k] = 0;
			} // end for over j
		} // end for over j
	} // end for over i
	double **strCor = new double*[nAng], **strCorSigma = new double*[nAng];
	for(int i = 0; i < nAng; i++){
		strCor[i] = new double[nr];
		strCorSigma[i] = new double[nr];
	} // end for over i

	// Generate the calibration file
	char strdir[] = "STRCorrection", mkstrdir[64] = "mkdir ";
	strcat(mkstrdir, strdir);
	// create a new directory to store STR correction files
	if(0 != access(strdir, F_OK)) system(mkstrdir);
	cout << "\33[34;1mUPDATING STRCorrection FILE...\033[0m" << endl;
	sprintf(name, "%s/%s_.003", strdir, dcArr->GetName().c_str());
	ofstream outFile(name);
	outFile.setf(ios_base::fixed, ios_base::floatfield);
	outFile.precision(3);
	// descriptive file header
	outFile << "###############################################################################\n";
	outFile << "# Space Time Relation (STR) calibration file for MWDC sense wires\n";
	outFile << "# Generation Time: " << TAPopMsg::time0() << endl;
	outFile << "# This calibraiton file is automatically generated for " << dcArr->GetName() << endl;
	outFile << "# by class TASTRCalibDCArrTa by fitting track fitting residues\n";
	outFile << "# (Ref. NIM A 488. 1-2, p51-73 (2002)).\n";
	outFile << "#\n";
	outFile << "# File format is as follows (three lines per each wire layer):\n";
	outFile << "# Info: anode_UID angle_No valid_bin_cnt\n";
	outFile << "# STR_correction_drift_time_bin_array\n";
	outFile << "# STR_correction_array\n";
	outFile << "#\n";
	outFile << "###############################################################################\n\n";
	TF1 *fgaus = new TF1("fgaus", "gaus", -1.5, 1.5);
	for(int i = 0; i < 2; i++){ // loop over DCs
		outFile << "#################### This is MWDC " << i << " #######################\n";
		for(int j = 0; j < 2; j++){ // loop over X-Y
			outFile << "\n#----- " << xy[j] << " -----#\n";
			for(int m = 0; m < 2; m++){ // loop over layer 1 and layer 2
				outFile << "\n\t# layer " << m + 1;
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					anodeId[0] = i; anodeId[1] = j; anodeId[2] = m; anodeId[3] = k;
					// temporarily stores the result
					for(int ii = 0; ii < nAng; ii++) for(int jj = 0; jj < nAng; jj++){
						strCor[ii][jj] = 0.; strCorSigma[ii][jj] = 0.;
					}
					TAAnode *ano = dcArr->GetMWDC(i)->GetAnode(j, m+1, k);
					for(int str_id = 0; str_id < nAng; str_id++){
						sprintf(name, "STRCali-%s/histo/hDCSTRCor_%d_%d_%d_%d_%d", dcArr->GetName().c_str(), i, j, m, k, str_id);
						TH2F *h2 = h2void;
						if(!(h2 = (TH2F*)f->Get(name))) continue;
						int valid_bin_cnt = 0; // valid drift distance bin number
						int valid_bin_array[nr]{}; // valid bin number array
						for(int l = 0; l < nr; l++){ // loop over drift distance bins
							// get the projection of the l-th channal along X axis,
							// from which mean and sigma would be estimated
							// project the l-th bin in X-axis to Y axis
							TH1D *hpro = h2->ProjectionY("hpro", l+1, l+1);
							double npro = hpro->GetEntries();
							const double rms = hpro->GetRMS();
							if(0) if(0 == l) npro = 0; // the first bin is biased
							if(npro < 200. || rms > 0.9){ // stastics is too small or STRcor nasty
								strCor[str_id][l] = 0.;
								strCorSigma[str_id][l] = 0.;
								continue;
							} // end if
							// mean, unit: mm
							fgaus->SetParameter(1, 0.);
							fgaus->SetParLimits(1, -1., 1.);
							// sigma, unit: mm
							fgaus->SetParameter(2, 0.2);
							fgaus->SetParLimits(2, 0., 1.);
							// fit range
							double span = 1.5*hpro->GetRMS();
							span = span < 1.5 ? span : 1.5;
							const double mm = hpro->GetMean();
							fgaus->SetRange(mm-span, mm+span);
							hpro->Fit(fgaus, "NQR"); // no printings, no draw, fit within range
							double mean = fgaus->GetParameter("Mean");
							double sigma = fgaus->GetParameter("Sigma");
							hmean[i][j]->Fill(mean); hmeanTot->Fill(mean);
							hsigma[i][j]->Fill(sigma); hsigmaTot->Fill(sigma);
//							cout << "mean: " << mean << "\tsigma: " << sigma << endl; getchar(); // DEBUG
							if((mean > -0.5 && mean < 0.5) && (sigma < 0.5 && sigma > 0.)){
								strCor[str_id][l] = mean;
								strCorSigma[str_id][l] = sigma;
								// more statistics brings about more weight in the sigma average
								sigma_g[i][j][l] += sigma * npro; // [DC][XY][rbin]
								sigma_g_cnt[i][j][l] += npro; // [DC][XY][rbin]
								valid_bin_array[valid_bin_cnt++] = l;
							}
//							if(j == 1) strCor[str_id][l] = 0.; // Y tracks are too nasty to be trusted
							mean_tree[str_id][l] = strCor[str_id][l];
							sigma_tree[str_id][l] = strCorSigma[str_id][l];
							// iterative correction, so it's increment, not replacement
							strCor[str_id][l] +=
								ano->GetAnodePara()->GetSTRCorrection(str_id)[l];
							cout << "Processing bin of " << i << " " << j << " " << m;
							cout << " " << k << " " << str_id << " " << l << "\r" << flush;
							if(hpro) delete hpro;
							hpro = nullptr;
						} // end for over l
						if(h2) delete h2;
						h2 = nullptr;
						if(0 == valid_bin_cnt) continue; // empty correction
						outFile << "\nInfo: " << setw(10) << ano->GetUID() << " ";
						outFile << setw(7) << str_id << " " << setw(5) << valid_bin_cnt << endl;
						for(int l = 0; l < valid_bin_cnt; l++) // output valid_bin_array
							outFile << valid_bin_array[l] << setw(7);
						outFile << "\n";
						for(int l = 0; l < valid_bin_cnt; l++) // output STR correction array
							outFile << strCor[str_id][valid_bin_array[l]] << setw(7);
						outFile << "\n";
					} // loop over STR id
					treeSigma->Fill();
				} // end for over k
			} // end for over m
		} // end of for over j
	} // end of for over i
	outFile << endl;
	outFile.close();

	double *sigmaTot = new double[nr];
	int *sigmaTot_cnt = new int[nr];
	for(int i = 0; i < nr; i++){ // initialize sigmaTot and sigmaTot_cnt
		sigmaTot[i] = 0.; sigmaTot_cnt[i] = 0;
	} // end for over i
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++) for(int l = 0; l < nr; l++) if(sigma_g_cnt[i][j][l] > 0){
			sigmaTot[l] += sigma_g[i][j][l];
			sigmaTot_cnt[l]  += sigma_g_cnt[i][j][l];
			sigma_g[i][j][l] /= sigma_g_cnt[i][j][l];
			gsigma[i][j]->SetPoint(l, rstep*(l+0.5), sigma_g[i][j][l]); // l+0.5: bin center
		} // end for and if
	} // end for over i
	for(int i = 0; i < nr; i++) if(sigmaTot_cnt[i] > 0){
		gsigmaTot->SetPoint(i, rstep*(i+0.5), sigmaTot[i]/sigmaTot_cnt[i]);
	} // end for over i

	// free the dynamically allocated memory //
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			delete [] sigma_g[i][j];
			delete [] sigma_g_cnt[i][j];
		} // end for over j
		delete [] sigma_g[i];
		delete [] sigma_g_cnt[i];
	} // end for over i
	for(int i = 0; i < nAng; i++){
		delete [] strCor[i];
		delete [] strCorSigma[i];
	} // end for over i
	delete [] sigma_g; sigma_g = nullptr;
	delete [] sigma_g_cnt; sigma_g_cnt = nullptr;
	delete [] sigmaTot; sigmaTot = nullptr;
	delete [] sigmaTot_cnt; sigmaTot_cnt = nullptr;
	delete [] strCor; strCor = nullptr;
	delete [] strCorSigma; strCorSigma = nullptr;

	// write //
	// make directory to store calibration results
	f->cd("/");
	char dir[128], subdir[64]; // directory to store results of the auto-calibration round
	sprintf(subdir, "round%s_%d", dcArr->GetName().c_str(), round); // PDCArray[L-R]
	sprintf(dir, "STRCali-%s/%s", dcArr->GetName().c_str(), subdir);
	if(!f->FindObjectAny(subdir)) f->mkdir(dir);
	f->cd(dir); // stores sigma, mean and treeSgima
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++){
		hmean[i][j]->Write("", TObject::kOverwrite);
		hsigma[i][j]->Write("", TObject::kOverwrite);
		gsigma[i][j]->Write("", TObject::kOverwrite);
	}
	hmeanTot->Write("", TObject::kOverwrite);
	hsigmaTot->Write("", TObject::kOverwrite);
	gsigmaTot->Write("", TObject::kOverwrite);
	treeSigma->Write("", TObject::kOverwrite);
	// free memory
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++){
		delete hmean[i][j]; hmean[i][j] = nullptr;
		delete hsigma[i][j]; hsigma[i][j] = nullptr;
		delete gsigma[i][j]; gsigma[i][j] = nullptr;
	}
	delete hmeanTot; hmeanTot = nullptr;
	delete hsigmaTot; hsigmaTot = nullptr;
	delete gsigmaTot; gsigmaTot = nullptr;
	delete treeSigma; treeSigma = nullptr;
	if(h2void) delete h2void;
	h2void = nullptr;
	f->Close(); delete f; f = nullptr;
//	TAParaManager::Instance()->UpdateSTRCorrection(); // keep up-to-date with the newest calibration

	cout << "\n\n\033[33;1mDONE\033[0m\n\n";
} // end of member function GenerateCalibFile


