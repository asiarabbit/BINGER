///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArr.C													 //
//   TAT0CalibDCArr.C -- source file for class TAT0CalibDCArr						 //
//   Introduction: a tool class to calibrate DCT0(delay)s anode by anode by fitting	 //
// drift time distribution. An abstract base class.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/11/18, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

// ROOT include
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"

#include "TAPopMsg.h"
#include "TAT0CalibDCArr.h"
#include "TAMWDCArray.h"
#include "TAMWDC.h"
#include "TAAnode.h"
#include "TAMath.h"
#include "TACtrlPara.h"

using std::flush;
using std::ofstream;
using std::setw;
using std::ios_base;
using std::cin;

TAT0CalibDCArr::TAT0CalibDCArr(const string &rootfile, TAMWDCArray *dcArr)
		: fROOTFile(rootfile), fDCArr(dcArr){
}
TAT0CalibDCArr::~TAT0CalibDCArr(){}

void TAT0CalibDCArr::Refine_DTHisto(bool isCalib){
	if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArr", "Refine_DTHisto: MWDC array pointer is null");
	if(!strcmp(fROOTFile.c_str(), ""))
		TAPopMsg::Error("TAT0CalibDCArr", "Refine_DTHisto: ROOT file name is empty");
	Refine_DTHisto(fROOTFile, fDCArr, isCalib);
}
void TAT0CalibDCArr::Refine_DTHisto(const string &rootfile, TAMWDCArray *dcArr, bool isCalib){
	TAPopMsg::Info("TAT0CalibDCArr", "Refine_DTHisto: Input rootfile name: %s", rootfile.c_str());
	const double phiAvrg = dcArr->GetPhiAvrg(); // average of phi over the three MWDCs
	const bool LRTAG = bool(dcArr->GetUID()-3); // 3: L; 4: R

	// read the track tree
	const int ntrMax = 200, ntrMax3D = ntrMax / 3;
	int ntr, index, type[ntrMax], id[ntrMax], nu[ntrMax][6], firedStripId[ntrMax];
	double t[ntrMax][6], r[ntrMax][6], k[ntrMax], b[ntrMax], beta2[ntrMax], TOF[ntrMax];
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	if(!f->FindObjectAny("treeTrack")){
		TAPopMsg::Error("TASTRCalibDCArr", "Refine_DTHisto: treeTrack not found in input rootfile");
	}

	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("beta2", &beta2);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("firedStripId", firedStripId);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("k", k); // start for iterative fit, necessary
	treeTrack->SetBranchAddress("b", b); // start for iterative fit, necessary
	treeTrack->SetBranchAddress("type", type); // track type: 1[LR][XUV]
	treeTrack->SetBranchAddress("id", id); // 3-D track id
	// store the refined drift time
	TTree *treeTrackT = new TTree("treeTrackT", "updated drift times and distances");
	treeTrackT->Branch("index", &index);
	treeTrackT->Branch("ntr", &ntr);
	treeTrackT->Branch("t", t, "t[ntr][6]/D");
	treeTrackT->Branch("r", r, "r[ntr][6]/D");

	// create histograms for T0 fittings
	char name[64], title[128];
	TH1F *hdt[3][3][2][96]{0}; // [DC#][XUV][Layer][nu]
	for(int i = 0; i < 3; i++){ // loop over DCs
		for(int j = 0; j < 3; j++){ // loop over X-U-V
			for(int m = 0; m < 2; m++){
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					sprintf(name, "hdt%d_%d_%d_%d", i, j, m, k);
					sprintf(title, "%s - Sense Wire Drift Time Spectrum;t/ns", name);
					if(isCalib) hdt[i][j][m][k] = new TH1F(name, title, 500, -100., 400.);
				} // end for over k
			} // end for over m
		} // end of for over j
	} // end of for over i

	// fill the histograms //
	const int n = treeTrack->GetEntries();
	int cntTrk = 0, cntSec = 0;
	cout << "Totally " << n << " data sections\n";
	for(int i = 0; i < n; i++){ // loop over data section
		treeTrack->GetEntry(i);
		// identify 3-D tracks
		int n3DtrXUV[3]{}, n3Dtr = 0; bool isDCArrR[ntrMax3D]{};
		int trkId[ntrMax3D][3]; memset(trkId, -1, sizeof(trkId)); // track id [3D track id][XUV]
		// loop over grouped track projections
		for(int j = 0; j < ntr; j++) if(-1 != id[j]){
			for(int k = 0; k < 3; k++){ // loop over X-U-V track types
				if(type[j]%10 == k){
					trkId[id[j]][k] = j;
					n3DtrXUV[k]++;
				}
			} // end loop over X-U-V track types
		} // end for over j and if
		if(n3DtrXUV[0] != n3DtrXUV[1] || n3DtrXUV[0] != n3DtrXUV[2]){
			TAPopMsg::Error("TAT0CaliDCArr", "Refine_DTHisto: This is odd... track projections of X, U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d, ntr: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2], ntr);
		} // end if
		n3Dtr = n3DtrXUV[0];
		// // // ^^^^^^^ circulation over 3-D tracks in one data section ^^^^^^^ // // //
		for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section
			isDCArrR[jj] = bool(type[trkId[jj][0]]/10%10); // 0: L; 1: R
			if(LRTAG != isDCArrR[jj]) continue;
			int nFX = 0, nFU = 0, nFV = 0; // fired anode layers in 
			for(int j = 0; j < 6; j++){ // count effective measurements
				if(nu[trkId[jj][0]][j] != -1) nFX++;
				if(nu[trkId[jj][1]][j] != -1) nFU++;
				if(nu[trkId[jj][2]][j] != -1) nFV++;
			} // end for over j
			const int nF = nFX + nFU + nFV; // number of measured points
			double trkVec[4]{}; // track slope
			trkVec[0] = k[trkId[jj][0]]; // k1
			trkVec[2] = b[trkId[jj][0]]; // b1
			trkVec[1] = TAMath::kUV_Y(phiAvrg, k[trkId[jj][1]], k[trkId[jj][2]]); // k2
			trkVec[3] = TAMath::bUV_Y(phiAvrg, k[trkId[jj][1]], k[trkId[jj][2]], b[trkId[jj][1]], b[trkId[jj][2]]); // b2
			double anodeId[nF][2]{}; int tmp = 0; // [0]: fired anode layer id: 0-17; [1]: nu
			for(int l = 0; l < 3; l++){ // loop over X-U-V
				for(int j = 0; j < 6; j++){ // loop over 6 anode layers
					if(nu[trkId[jj][l]][j] != -1){ // one measure point
						anodeId[tmp][0] = 6 * l + j; // fired anode layer id: 0-17
						anodeId[tmp][1] = nu[trkId[jj][l]][j]; // nu: anode id in certain layer
						int DCid = j / 2; // 0-1-2: DC0-DC1-DC2
						int dcType = l; // 0-1-2: X-U-V
						int layerType = j % 2 + 1; // 1-2: anodeL1-anode L2
						int NU = nu[trkId[jj][l]][j];
						// t = T_tof + T_wire + T_drift + T0
						// substract T_wire and T_tof from the time measurement
						TAAnode *ano = dcArr->GetMWDC(DCid)->GetAnode(dcType, layerType, NU);
						unsigned uid = ano->GetUID();
						t[trkId[jj][l]][j] -= TACtrlPara::T_tofDCtoTOFW(uid) - TACtrlPara::T_wireMean(uid); // recover the rough correction of time of flight from DC to TOF wall for a refined one.
						dcArr->DriftTimeCorrection(t[trkId[jj][l]][j], r[trkId[jj][l]][j], anodeId[tmp], trkVec, firedStripId[trkId[jj][l]], beta2[trkId[jj][l]]);
						if(isCalib && hdt[DCid][dcType][layerType-1][NU]){
							hdt[DCid][dcType][layerType-1][NU]->Fill(t[trkId[jj][l]][j]);
						}
						tmp++;
					} // end if
				} // end for over j
			} // end for over l
			cout << "Processing data section " << index << " and track " << cntTrk << "\r" << flush;
			cntTrk++;
		} // end for over jj
		cntSec++;
		treeTrackT->Fill();
	} // end for over i
	f->cd("/"); treeTrackT->Write("", TObject::kOverwrite);
	
	sprintf(name, "T0Cali0-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name);
	if(!f->FindObjectAny(name)) f->mkdir(title); f->cd(title); // store drift time histograms
	for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) for(int m = 0; m < 2; m++)
	for(int k = 0; k < 96; k++) if(hdt[i][j][m][k]){
		if(isCalib) hdt[i][j][m][k]->Write("", TObject::kOverwrite);
		delete hdt[i][j][m][k]; hdt[i][j][m][k] = nullptr;
	}
	cout << "Totally \033[1m" << cntSec << " \033[0m data sections and  \033[1m" << cntTrk << " \033[0m 3D tracks have been processed.\n";
	f->Close(); delete f;
} // end of member function Refine_DTHisto

void TAT0CalibDCArr::GenerateCalibFile(bool isShowFit){
	if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArr", "GenerateCalibFile: MWDC array pointer is null");
	GenerateCalibFile(fROOTFile, fDCArr, isShowFit);
}
void TAT0CalibDCArr::GenerateCalibFile(const string &rootfile, TAMWDCArray *dcArr, bool isShowFit){
	TAPopMsg::Info("TAT0CalibDCArr", "GenerateCalibFile: Input rootfile name: %s", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	char name[128], title[128];
	sprintf(name, "T0Cali0-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name); // directory storing drift time histograms
	if(!f->FindObjectAny(name)){
		TAPopMsg::Error("TAT0CalibDCArr", "GenerateCalibFile: Directory %s doesn't exist. Maybe TAT0CalibDCArr::Refine_DTHisto(...) hasn't been run yet.", name);
	}
	f->cd(title);

	// T0 is the offset due to the electronics chain from the preamplifier to the TDC input
	double T0 = 0., sigma_t0 = 0., chi2 = 0.; // results from TH1F::Fit(...) method.
	int sigma_t0_cnt = 0; double sigma_t0_sum = 0; // for calculate the mean value of sigma_t0
	int anodeId[4]{}; // [0-3]: [i,j,m,k] [DC#][XUV][1,2][80]
	TTree *treeT0 = new TTree("treeT0", "T0 of DC Anodes");
	treeT0->Branch("anodeId", anodeId, "anodeId[4]/I");
	treeT0->Branch("T0", &T0, "T0/D");
	treeT0->Branch("sigma_t0", &sigma_t0, "sigma_t0/D");
	TH1F *hT0 = new TH1F("hT0", "T0 Distribution Over Aondes Before T0 Correction", 501, -25.05, 25.05);

	char xuv[] = "XUV";
	// Generate the calibration file
	cout << "\33[34;1mUPDATING T0 FILE...\033[0m" << endl;
	sprintf(name, "%s_.002", dcArr->GetName());
	ofstream outFile(name);
	outFile.setf(ios_base::fixed, ios_base::floatfield);
	outFile.precision(3);
	// descriptive file header
	outFile << "###############################################################################\n";
	outFile << "# Time offset calibration file for MWDC sense wires - Generation Time: ";
	outFile << TAPopMsg::time0() << endl;
	outFile << "# This calibraiton file is automatically generated for " << dcArr->GetName() << endl;
	outFile << "# by class TAT0CalibDCArr using drift time histogram fitting ";
	outFile << "# (Ref. NIM A 488. 1-2, p51-73 (2002)).\n";
	outFile << "#\n";
	outFile << "# File format: T0 UID. unit: ns\n";
	outFile << "#\n";
	outFile << "###############################################################################\n\n";
	// prepare the fd function
	TF1 *fd = new TF1("fd", Fermi_Dirac_Function, -50., 70., 5);
	fd->SetParName(2, "T0"); fd->SetParName(3, "sigma_t0");
	fd->SetParameter(2, 0.); fd->SetParLimits(2, -20., 20.); // T0, unit: ns
	fd->SetParameter(3, 0.1); fd->SetParLimits(3, 0.005, 20.); // sigm_t_0, unit: ns
	for(int i = 0; i < 3; i++){ // loop over DCs
		outFile << "#################### This is MWDC " << i << " #######################\n";
		for(int j = 0; j < 3; j++){ // loop over X-U-V
			outFile << "#----- " << xuv[j] << " -----#\n";
			for(int m = 0; m < 2; m++){ // loop over layer 1 and layer 2
				outFile << "\t# layer " << m + 1 << "\n";
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					sprintf(name, "T0Cali0-%s/histo/hdt%d_%d_%d_%d", dcArr->GetName().c_str(), i, j, m, k);
					TH1F *hdt = (TH1F*)f->Get(name); if(!hdt) continue;
					T0 = 0.; sigma_t0 = 0.; chi2 = 0.;
					anodeId[0] = i; anodeId[1] = j; anodeId[2] = m; anodeId[3] = k;
					const int nn = hdt->GetEntries();
					if(nn >= 800){ // the statistics is enough.
						fd->SetParameter(2, 0.); fd->SetParameter(3, 0.1);
						if(isShowFit) hdt->Fit(fd, "QR");
						else hdt->Fit(fd, "NQR");
						chi2 = fd->GetChisquare();
						T0 = fd->GetParameter("T0");
						sigma_t0 = fd->GetParameter("sigma_t0");
						// to get rid of the bad T0s by estimating sigma_t0
						sigma_t0_cnt++; sigma_t0_sum += sigma_t0;
						double mean_t = sigma_t0_sum/sigma_t0_cnt;
						if(sigma_t0 > 3.*mean_t)
						T0 = 0.; if(0) // abnormal
						{
							cout << "\n\ni: " << i << "\tj: " << j;
							cout << "\tm: " << m << "\tk: " << k;
							cout << "\tnn: " << nn << "\t\033[32;1mT0: " << T0;
							cout << "\tsigma_t0_mean: " << mean_t << "\tsigma_t0_cnt: " << sigma_t0_cnt;
							cout << "\tsigma_t0: " << sigma_t0 << "\tchi2: " << chi2 << "\033[0m";
							cout << "\nEnter 0 to reject, or 1 to accept the current T0.\n";
							cout << "Please enter your choice here: ";
							bool pass = true;
							while(!(cin >> pass)){
								cin.clear(); // reset input
								while(cin.get() != '\n')
									continue; // get rid of bad input
								cout << "Please enter 0 or 1 only: ";
							}
							if(!pass) T0 = 0.;
							cout << "T0 is then " << T0 << endl;
							getchar(); // DEBUG
						} // end if
					} // end if(nn >= 1000)
					cout << "Fitting historgrams of anode \033[1m" << i << " ";
					cout << j << " " << m << setw(3) << k << "\033[0m\r" << flush;
					TAAnode *ano = dcArr->GetMWDC(i)->GetAnode(j, m+1, k);
					outFile << T0 << setw(10) << ano->GetUID() << endl;
					treeT0->Fill();
					if(T0 != 0) hT0->Fill(T0);
//					cout << "Mark 1\n"; getchar(); // DEBUG
					if(isShowFit) hdt->Write("", TObject::kOverwrite);
					else hdt->~TH1F();
				} // end for over k
				outFile << endl;
			} // end for over m
			outFile << endl;
		} // end of for over j
		outFile << endl;
	} // end of for over i
	outFile << endl;
	outFile.close();

	f->cd(("T0Cali0-"+dcArr->GetName()).c_str());
	treeT0->Write("", TObject::kOverwrite);
	hT0->Write("", TObject::kOverwrite);
	cout << "\n\n\033[33;1mhT0->GetMean(): " << hT0->GetMean();
	cout << "\thT0->GetRMS(): " << hT0->GetRMS() << endl;
	cout << "\nDONE\033[0m\n\n";
	sleep(1);
	f->Close(); delete f;
} // end of member function GenerateCalibFile

double TAT0CalibDCArr::Fermi_Dirac_Function(double *x, double *p){
	double fd_core = exp(-p[1]*(x[0]-p[4])) / (1 + exp(-(x[0]-p[2]) / p[3])); // p[2] represents T0.
	return p[0] * fd_core;
} // end of definition of Fermi_Dirac_Function





