///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArr.C													 //
//   TAT0CalibDCArr.C -- source file for class TAT0CalibDCArr						 //
//   Introduction: a tool class to calibrate DCT0(delay)s anode by anode by fitting	 //
// drift time distribution. An abstract base class.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/3.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
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
#include <cstring>
#include <unistd.h>

// ROOT include
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"

#include "TAPopMsg.h"
#include "TACtrlPara.h"
#include "TAT0CalibDCArrTa.h"
#include "TAMWDCArray2.h"
#include "TAMWDC.h"
#include "TAAnode.h"
#include "TAMath.h"
#include "TACtrlPara.h"
#include "TAParaManager.h"

using std::flush;
using std::ofstream;
using std::setw;
using std::ios_base;
using std::cin;

#define DEBUG

TAT0CalibDCArrTa::TAT0CalibDCArrTa(const string &rootfile, TAMWDCArray2 *dcArr)
		: fROOTFile(rootfile), fDCArr(dcArr){
}
TAT0CalibDCArrTa::~TAT0CalibDCArrTa(){}

void TAT0CalibDCArrTa::Refine_DTHisto(){
	if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArrTa", "Refine_DTHisto: MWDC array pointer is null");
	if(!strcmp(fROOTFile.c_str(), ""))
		TAPopMsg::Error("TAT0CalibDCArrTa", "Refine_DTHisto: ROOT file name is empty");
	Refine_DTHisto(fROOTFile, fDCArr);
}
void TAT0CalibDCArrTa::Refine_DTHisto(const string &rootfile, TAMWDCArray2 *dcArr){
	TAPopMsg::Info("TAT0CalibDCArrTa", "Refine_DTHisto: Input rootfile name: %s", rootfile.c_str());
	const bool isDCArrD = bool(dcArr->GetUID()-6); // 6: U; 7: D
	const short UDTAG = 12 + isDCArrD;

	// read the track tree
	const int ntrMax = 200;
	int ntr, ntrT, index, type[ntrMax], nu[ntrMax][6];
	double t[ntrMax][6], r[ntrMax][6], k[ntrMax], b[ntrMax];
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TAT0CalibDCArrTa", "Refine_DTHisto: Input rootfile %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	if(!f->FindObjectAny("treeTrack")){
		TAPopMsg::Error("TASTRCalibDCArrTa", "Refine_DTHisto: treeTrack not found in input rootfile");
	}

	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("k", k);
	treeTrack->SetBranchAddress("b", b);
	treeTrack->SetBranchAddress("type", type); // track type: 1[UD][XY]

	// create histograms for T0 fittings
	char name[64], title[128];
	TH1F *hdt[2][2][2][16]{0}; // [DC#][XY][Layer][nu]
	for(int i = 0; i < 2; i++){ // loop over DCs
		for(int j = 0; j < 2; j++){ // loop over X-Y
			for(int m = 0; m < 2; m++){
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					sprintf(name, "hdt%d_%d_%d_%d", i, j, m, k);
					sprintf(title, "%s - Sense Wire Drift Time Spectrum;t/ns", name);
					hdt[i][j][m][k] = new TH1F(name, title, 500, -100., 400.);
				} // end for over k
			} // end for over m
		} // end of for over j
	} // end of for over i

	// fill the histograms //
	const int n = treeTrack->GetEntries();
	int cntTrk = 0, cntSec = 0;
	cout << "Totally " << n << " data sections\n";
	// fill the histograms //
	for(int i = 0; i < n; i++){ // loop over data section
		treeTrack->GetEntry(i);
		int ntrLs[4][3]{}; // N of TrkProjs; DCArr-L-R-U-D -- [XUV - XY]
		for(int j = 0; j < ntrT; j++){
			const int dcArrId = type[j] / 10 % 10, dcType = type[j] % 10;
			ntrLs[dcArrId][dcType]++;
		}
		if(1 != ntrLs[2+isDCArrD][0] || 1 != ntrLs[2+isDCArrD][1]){
			continue; // no 3D trk is present in the specified DCArr
		}
		int trkId[2]{}; // X-Y
		for(int j = 0; j < ntrT; j++){
			if(UDTAG == type[j] / 10) trkId[type[j]%2] = j; // the wanted trk-projs
		}
		for(int l = 0; l < 2; l++){ // loop over X-Y
			for(int j = 0; j < 4; j++){ // loop over 4 anode layers
				if(nu[trkId[l]][j] != -1){ // one measure point
					int DCid = j / 2; // 0-1-2: DC0-DC1
					int dcType = l; // 0-1-2: X-Y
					int layerType = j % 2 + 1; // 1-2: anodeL1-anodeL2
					int NU = nu[trkId[l]][j];
					if(hdt[DCid][dcType][layerType-1][NU]){
						hdt[DCid][dcType][layerType-1][NU]->Fill(t[trkId[l]][j]);
					} // end inner if
				} // end if
			} // end for over j
		} // end for over l
		cout << "Processing data section " << index << " and track " << cntTrk << "\r" << flush;
		cntTrk++;
		cntSec++;
	} // end for over i
	cout << "Totally \033[1m" << cntSec << " \033[0m data sections and  \033[1m" << cntTrk << " \033[0m 3D tracks have been processed.\n";

	// write //
	TFile *fw = new TFile(("assess"+rootfile).c_str(), "UPDATE");
	sprintf(name, "T0Cali0Ta-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name);
	if(!fw->FindObjectAny(name)) fw->mkdir(title);
	fw->cd(title); // store drift time histograms
	cout << endl;
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++) for(int m = 0; m < 2; m++)
	for(int k = 0; k < 16; k++) if(hdt[i][j][m][k]){
		if(hdt[i][j][m][k]->GetEntries() > 1000.)
		{
			hdt[i][j][m][k]->Write("", TObject::kOverwrite);
			cout << "Writing Histo \033[34;1m" << i << " " << j << " " << m << " " << k << "\033[0m";
			cout << "\tPlease wait..." << "\r" << flush;
		}
		delete hdt[i][j][m][k]; hdt[i][j][m][k] = nullptr;
	}
	fw->Close(); delete fw; fw = nullptr;
	f->Close(); delete f; f = nullptr;
	cout << "\033[33;1m\n\nDONE\n\n\033[0m";
} // end of member function Refine_DTHisto

void TAT0CalibDCArrTa::GenerateCalibFile(bool isShowFit){
	if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArrTa", "GenerateCalibFile: MWDC array pointer is null");
	GenerateCalibFile(fROOTFile, fDCArr, isShowFit);
}
void TAT0CalibDCArrTa::GenerateCalibFile(const string &rootfile, TAMWDCArray2 *dcArr, bool isShowFit){
	TAPopMsg::Info("TAT0CalibDCArrTa", "GenerateCalibFile: Input rootfile name: %s, MWDC Array Name: %s", rootfile.c_str(), dcArr->GetName().c_str());
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TAT0CalibDCArrTa", "GenerateCalibFile: Input rootfile %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(("assess"+rootfile).c_str(), "UPDATE");
	char name[128], title[128], topdir[64];
	sprintf(topdir, "T0Cali0Ta-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", topdir); // directory storing drift time histograms
	if(!f->FindObjectAny(topdir)){
		TAPopMsg::Error("TAT0CalibDCArrTa", "GenerateCalibFile: Directory %s doesn't exist. Maybe TAT0CalibDCArrTa::Refine_DTHisto(...) hasn't been run yet.", topdir);
	}
	f->cd(title);

	// T0 is the offset due to the electronics chain from the preamplifier to the TDC input
	double T0 = 0., sigma_t0 = 0., chi2 = 0.; // results from TH1F::Fit(...) method.
	int sigma_t0_cnt = 0; double sigma_t0_sum = 0; // for calculate the mean value of sigma_t0
	int anodeId[4]{}, uid; // [0-3]: [i,j,m,k] [DC#][XUV][1,2][80]
	TTree *treeT0 = new TTree("treeT0", "T0 of DC Anodes");
	treeT0->Branch("anodeId", anodeId, "anodeId[4]/I");
	treeT0->Branch("T0", &T0, "T0/D");
	treeT0->Branch("sigma_t0", &sigma_t0, "sigma_t0/D");
	treeT0->Branch("uid", &uid);
	TH1F *hT0 = new TH1F("hT0", "T0 Distribution Over Aondes Before T0 Correction", 501, -25.05, 25.05);

	char xy[] = "XY", strdir[] = "T0Calibration", mkstrdir[64] = "mkdir ";
	strcat(mkstrdir, strdir);
	// create a new directory to store STR correction files
	if(0 != access(strdir, F_OK)) system(mkstrdir);
	// Generate the calibration file
	cout << "\33[34;1mUPDATING T0 FILE...\033[0m" << endl;
	sprintf(name, "%s/%s_.002", strdir, dcArr->GetName().c_str());
	// check that if the T0 config file already exists
	// The T0 calibration should be implemented with the absence of the target T0 config files
	if(TAParaManager::Instance()->Exist(2)){
		TAPopMsg::Info("TAT0CalibDCArrTa", "GenerateCalibFile: T0 Calibration files already exist in config/[experiment]/T0:");
		char cmd[128]; sprintf(cmd, "echo;echo ls %s/T0/*:; ls %s/T0/*;echo", TACtrlPara::Instance()->ConfigExpDir(), TACtrlPara::Instance()->ConfigExpDir());
		system(cmd);
	}
	ofstream outFile(name);
	outFile.setf(ios_base::fixed, ios_base::floatfield);
	outFile.precision(3);
	// descriptive file header
	outFile << "###############################################################################\n";
	outFile << "# Time offset calibration file for MWDC sense wires\n";
	outFile << "# Generation Time: " << TAPopMsg::time0() << endl;
	outFile << "# This calibraiton file is automatically generated for " << dcArr->GetName() << endl;
	outFile << "# by class TAT0CalibDCArr using drift time histogram fitting\n";
	outFile << "# (Ref. NIM A 488. 1-2, p51-73 (2002)).\n";
	outFile << "#\n";
	outFile << "# File format: T0 UID. unit: ns\n";
	outFile << "#\n";
	outFile << "###############################################################################\n\n";
	// prepare the fd function
	TF1 *fd = new TF1("fd", Fermi_Dirac_Function, -20., 50., 5);
	fd->SetParName(2, "T0"); fd->SetParName(3, "sigma_t0");
	fd->SetParameter(2, 0.); fd->SetParLimits(2, -30., 30.); // T0, unit: ns
	fd->SetParameter(3, 0.01); fd->SetParLimits(3, 0.005, 20.); // sigm_t_0, unit: ns
	for(int i = 0; i < 2; i++){ // loop over DCs
		outFile << "#################### This is MWDC " << i << " #######################\n";
		for(int j = 0; j < 2; j++){ // loop over X-U-V
			outFile << "#----- " << xy[j] << " -----#\n";
			for(int m = 0; m < 2; m++){ // loop over layer 1 and layer 2
				outFile << "\t# layer " << m + 1 << "\n";
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					sprintf(name, "T0Cali0Ta-%s/histo/hdt%d_%d_%d_%d", dcArr->GetName().c_str(), i, j, m, k);
					TH1F *hdt = (TH1F*)f->Get(name); if(!hdt) continue;
					T0 = 0.; sigma_t0 = 0.; chi2 = 0.;
					anodeId[0] = i; anodeId[1] = j; anodeId[2] = m; anodeId[3] = k;
					const int nn = hdt->GetEntries();
//					printf("i: %d, j: %d, m: %d, k: %d, hdt: %s, n: %f\n", i,j,m,k,hdt->GetName(), hdt->GetEntries()); cout << "name: " << name << endl; getchar(); // DEBUG
					if(nn >= 800){ // the statistics is enough
						fd->SetParameter(2, 0.); fd->SetParameter(3, 0.1);
						const double hdtmx = hdt->GetMaximum();
						fd->SetParameter(0, 0.9*hdtmx);
						fd->SetParLimits(0, 0.1*hdtmx, 2.*hdtmx);
						if(isShowFit) hdt->Fit(fd, "QR");
						else hdt->Fit(fd, "NQR");
						chi2 = fd->GetChisquare();
						T0 = fd->GetParameter("T0");
						sigma_t0 = fd->GetParameter("sigma_t0");
//						cout << "T0: " << T0 << "\tsigma_t0: " << sigma_t0 << endl; getchar(); // DEBUG
						// to get rid of the bad T0s by estimating sigma_t0
						sigma_t0_cnt++; sigma_t0_sum += sigma_t0;
						double mean_t = sigma_t0_sum/sigma_t0_cnt;
						if(sigma_t0 > 1.5*mean_t) T0 = 0.;
						if(0) // abnormal
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
					uid = ano->GetUID();
					outFile << setw(6) << T0 << setw(10) << uid << endl;
					treeT0->Fill();
					if(0. != T0) hT0->Fill(T0);
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

	cout << "\n\n\033[33;1mhT0->GetMean(): " << hT0->GetMean();
	cout << "\thT0->GetRMS(): " << hT0->GetRMS() << endl;
	cout << "\nDONE\033[0m\n\n";

	// write //
	f->cd(topdir);
	treeT0->Write("", TObject::kOverwrite);
	hT0->Write("", TObject::kOverwrite);
	delete hT0; hT0 = nullptr;
	delete treeT0; treeT0 = nullptr;
	f->Close(); delete f; f = nullptr;
} // end of member function GenerateCalibFile


// p[4] has the same function of p[0], and has been remvoed. 2017_1223_1054
//double TAT0CalibDCArr::Fermi_Dirac_Function(double *x, double *p){
//	return p[0] * exp(-p[1]*x[0]) / (1 + exp(-(x[0]-p[2]) / p[3])); // p[2] represents T0.
//}

double TAT0CalibDCArrTa::Fermi_Dirac_Function(double *x, double *p){
	double fd_core = exp(-p[1]*x[0]) / (1 + exp(-(x[0]-p[2]) / p[3])); // p[2] represents T0.
	return p[0] * fd_core;
} // end of definition of Fermi_Dirac_Function





