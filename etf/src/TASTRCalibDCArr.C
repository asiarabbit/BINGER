///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArr.C													 //
//   TASTRCalibDCArr.C -- source file for class TASTRCalibDCArr						 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. An		 //
// abstract base class.																 //
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
#include "TH2F.h"
#include "TGraph.h"

#include "TAPopMsg.h"
#include "TASTRCalibDCArr.h"
#include "TAMWDCArray.h"
#include "TAMath.h"
#include "TAMWDC.h"
#include "TAAnode.h"
#include "TAAnodePara.h"

using std::flush;
using std::ofstream;
using std::setw;
using std::ios_base;

const int TASTRCalibDCArr::nr = TAAnodePara::kSTRCorRNBins;
const double TASTRCalibDCArr::rmx = TAAnodePara::kSTRCorRMax;
const int TASTRCalibDCArr::nAng = TAAnodePara::kSTRCorAngleNBins;
const double TASTRCalibDCArr::rstep = TAAnodePara::kSTRCorRStep;

TASTRCalibDCArr::TASTRCalibDCArr(const string &rootfile, TAMWDCArray *dcArr)
		: fROOTFile(rootfile){
	fDCArr = dcArr; fIsBigStatistics = true;
}
TASTRCalibDCArr::~TASTRCalibDCArr(){}

// histogram chi and write down the histograms
void TASTRCalibDCArr::ChiHistogramming(){
	if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArr", "ChiHistogramming: MWDC array pointer is null");
	ChiHistogramming(fROOTFile, fDCArr, fIsBigStatistics);
}
void TASTRCalibDCArr::ChiHistogramming(const string &rootfile, TAMWDCArray *dcArr, bool isBigSta){
	TAPopMsg::Info("TASTRCalibDCArr", "ChiHistogramming: number of drift distance bins for STR auto-calibration %f; Maximum drift distance: %f", nr, rmx);
	TAPopMsg::Info("TASTRCalibDCArr", "ChiHistogramming: Input rootfile name: %s", rootfile);
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	if(!f->FindObjectAny("treeTrack")){
		TAPopMsg::Error("TASTRCalibDCArr", "ChiHistogramming: treeTrack not found in input rootfile");
	}

	const int ntrMax = 200, ntrMax3D = ntrMax / 3;
	int ntr, index, type[ntrMax], id[ntrMax], nu[ntrMax][6];
	double kl[ntrMax], xMiss3D[ntrMax][3];
	double t[ntrMax][6], r[ntrMax][6], chi[ntrMax][6], Chi[ntrMax];
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("t", t); // T1
	treeTrack->SetBranchAddress("r", r); // R
	treeTrack->SetBranchAddress("chi", chi); // R
	treeTrack->SetBranchAddress("Chi", Chi); // R
	treeTrack->SetBranchAddress("k", kl); // R, to get the STRid
	treeTrack->SetBranchAddress("type", type); // track type: 1[LR][XUV]
	treeTrack->SetBranchAddress("id", id); // 3-D track id
	treeTrack->SetBranchAddress("xMiss3D", xMiss3D);

	// create TH2F objects for the STR correction fittings //
	TH2F *hRDCSTRCor[3][3][2][96][nAng]{0}; // [DC#][XUV][Layer][nu][STR_id]
	TH2F *hRDCSTR_RT[3][3][2][96][nAng]{0}; // [DC#][XUV][Layer][nu][STR_id] r-t 2D graph.
	char name[64], title[128];
	for(int i = 0; i < 3; i++){ // loop over DCs
		for(int j = 0; j < 3; j++){ // loop over X-U-V
			for(int m = 0; m < 2; m++){
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					for(int l = 0; l < nAng; l++){
						sprintf(name, "hRDCSTRCor_%d_%d_%d_%d_%d", i, j, m, k, l);
						sprintf(title, "%s - r-dr Relation;r/mm;dr/mm", name);
						hRDCSTRCor[i][j][m][k][l] = new TH2F(name, title, nr, 0., rmx, 300, -4.0, 4.0);
						sprintf(name, "hRDCSTR_RT_%d_%d_%d_%d_%d", i, j, m, k, l);
						sprintf(title, "%s - t-r Relation;t/ns;r/mm", name);
						hRDCSTR_RT[i][j][m][k][l] = new TH2F(name, title, 35, 0., 280., 800, -0.5, 7.5);
					} // end for over i
				} // end for over k
			} // end for over m
		} // end of for over j
	} // end of for over i

	const int n = treeTrack->GetEntries();
	int nTrk = 0;
	cout << "Totally " << n << " data sections\n";
	for(int i = 0; i < n; i++){ // loop over data sections.
		treeTrack->GetEntry(i);
		for(int j = 0; j < ntr; j++){ // loop over tracks in a data section.
			int dcType = type[j]%10; // X-U-V
			if(id[j] == -1) continue; // not a successful match
			if(Chi[j] > 0.9) continue; // nasty tracks
			nTrk++;
			for(int l = 0; l < 6; l++){
				if(nu[j][l] != -1){
					// rc: distance of anode to track
					double tt = t[j][l], dr = chi[j][l], rr = r[j][l];
					double rc = rr+dr;
					int DCid = l/2, NU = nu[j][l];
					TAAnode *ano = dcArr->GetMWDC(DCid)->GetAnode(dcType, l%2, NU);
					int STRid = ((TAAnodePara*)ano->GetPara())->GetSTRid(kl[j], dcType);
					if(isBigSta){
						hRDCSTRCor[DCid][dcType][l%2][NU][STRid]->Fill(rr, dr);
						hRDCSTR_RT[DCid][dcType][l%2][NU][STRid]->Fill(tt, rc);
					} // end if
					else{ // STRCor implemented layer by layer
						for(int k = 0; k < 80; k++){
							hRDCSTRCor[DCid][dcType][l%2][k][STRid]->Fill(rr, dr);
							hRDCSTR_RT[DCid][dcType][l%2][k][STRid]->Fill(tt, rc);
						}
					} // end else
				} // end if
			} // end for over j
		} // end for over j
		cout << "Processing data section " << i << "\r" << flush;
	} // end for over i
	cout << nTrk / 3 << " 3D tracks has been adopted for STR correction.\n";

	sprintf(name, "STRCali-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name);
	if(!f->FindObjectAny(name)) f->mkdir(title); // store drift time histograms
	f->cd(title);
	for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) for (int m = 0; m < 2; m++)
	for(int k = 0; k < 96; k++) for(int s = 0; s < nAng; s++) if(hRDCSTRCor[i][j][m][k][s])
	if(hRDCSTRCor[i][j][m][k][s]->GetEntries() > 5000.){
		hRDCSTRCor[i][j][m][k][s]->Write("", TObject::kOverwrite);
		hRDCSTR_RT[i][j][m][k][s]->Write("", TObject::kOverwrite);
	}
	cout << "\n\n\033[33;1mDONE\033[0m\n\n"; // DEBUG
	f->Close(); delete f;
} // end of member function ChiHistogramming

// read the chi histograms and fit them to get the mean, which would be used as
// the correction of the initial STRs
void TASTRCalibDCArr::GenerateSTRCorFile(){
	if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArr", "GenerateCalibFile: MWDC array pointer is null");
	GenerateCalibFile(fROOTFile, fDCArr);
}
void TASTRCalibDCArr::GenerateCalibFile(const string &rootfile, TAMWDCArray *dcArr, int round){
	TAPopMsg::Info("TASTRCalibDCArr", "GenerateCalibFile: Input rootfile name: %s, STR auto-calibration round id: %d", rootfile.c_str(), round);
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	char name[128], title[128], xuv[] = "XUV";
	sprintf(name, "STRCali-%s", dcArr->GetName().c_str());
	sprintf(title, "%s/histo", name); // directory storing drift time histograms
	if(!f->FindObjectAny(name)){
		TAPopMsg::Error("TASTRCalibDCArr", "GenerateCalibFile: Directory %s doesn't exist. Maybe TASTRCalibDCArr::ChiHistogramming(...) hasn't been run yet.", name);
	}
	f->cd(title);
	// make directory to store calibration results
	char dir[64]; // directory to store results of the auto-calibration round
	sprintf(dir, "%s/round_%d", name, round);
	cout << "dir: " << dir << endl;
	if(!f->cd(dir)) f->mkdir(dir);

	TH1F *hmeanTot = new TH1F("hmeanTot", "hmeanTot", 500, -0.8, 0.8);
	TH1F *hsigmaTot = new TH1F("hsigmaTot", "hsigmaTot", 500, 0., 1.5);
	TH2F *h2void = new TH2F("h2void", "h2void", nr, 0., rmx, 800, -4.0, 4.0); // for slick use
	TGraph *gsigmaTot = new TGraph();
	gsigmaTot->SetNameTitle("gsigmaTot", "Drift Distance v.s. Spatial Resolution;r/mm;\\sigma/mm");
	TH1F *hmean[3][3], *hsigma[3][3]; // [DC#][XUV]
	TGraph *gsigma[3][3]; // [DC#][XUV]
	for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++){
		sprintf(name, "hmean_DC%d_%c", i, xuv[j]);
		hmean[i][j] = new TH1F(name, name, 500, -0.8, 0.8);
		sprintf(name, "hsigma_DC%d_%c", i, xuv[j]);
		hsigma[i][j] = new TH1F(name, name, 500, 0., 1.5);
		sprintf(name, "gsigma_DC%d_%c", i, xuv[j]);
		sprintf(title, "Drift Distance v.s. Spatial Resolution- DC%d %c;r/mm;\\sigma/mm", i, xuv[j]);
		gsigma[i][j] = new TGraph();
		gsigma[i][j]->SetNameTitle(name, title);
	}

	// filling the recording tree
	int anodeId[4];
	double sigma_tree[nAng][nr], mean_tree[nAng][nr];
	if(nAng > 6 || nr > 60)
		TAPopMsg::Error("TASTRCalibDCArr", "GenerateCalibFile: nAng or nr is too small: nAng: %d, nr: %d", nAng, nr);
	TTree *treeSigma = new TTree("treeSigma", "Spatial Resolution and STR Correction");
	treeSigma->Branch("anodeId", anodeId, "anodeId[4]/I");
	treeSigma->Branch("sigma", sigma_tree, "sigma[6][60]/D");
	treeSigma->Branch("mean", mean_tree, "mean[6][60]/D");
	// for filling r-sigma graph
	double sigma_g[3][3][nr]{};
	int sigma_g_cnt[3][3][nr]{};

	// Generate the calibration file
	cout << "\33[34;1mUPDATING STRCorrection FILE...\033[0m" << endl;
	sprintf(name, "%s_.003", dcArr->GetName());
	ofstream outFile(name);
	outFile.setf(ios_base::fixed, ios_base::floatfield);
	outFile.precision(3);
	// descriptive file header
	outFile << "###############################################################################\n";
	outFile << "# Space Time Relation (STR) calibration file for MWDC sense wires";
	outFile << " - Generation Time: " << TAPopMsg::time0() << endl;
	outFile << "# This calibraiton file is automatically generated for " << dcArr->GetName() << endl;
	outFile << "# by class TASTRCalibDCArr by fitting track fitting residues ";
	outFile << "# (Ref. NIM A 488. 1-2, p51-73 (2002)).\n";
	outFile << "#\n";
	outFile << "# File format is as follows (three lines per each anode):\n";
	outFile << "# Info: anode_UID angle_No valid_bin_cnt";
	outFile << "# STR_correction_drift_time_bin_array\n";
	outFile << "# STR_correction_array\n";
	outFile << "#\n";
	outFile << "###############################################################################\n\n";
	TF1 *fgaus = new TF1("fgaus", "gaus", -1.5, 1.5);
	for(int i = 0; i < 3; i++){ // loop over DCs
		outFile << "#################### This is MWDC " << i << " #######################\n";
		for(int j = 0; j < 3; j++){ // loop over X-U-V
			outFile << "#----- " << xuv[j] << " -----#\n";
			for(int m = 0; m < 2; m++){ // loop over layer 1 and layer 2
				outFile << "\t# layer " << m + 1 << "\n";
				const int nAnodePerLayer = dcArr->GetMWDC(i)->GetNAnodePerLayer();
				for(int k = 0; k < nAnodePerLayer; k++){ // loop over anodes per layer
					anodeId[0] = i; anodeId[1] = j; anodeId[2] = m; anodeId[3] = k;
					// temporarily stores the result
					double strCor[nAng][nr]{}, strCorSigma[nAng][nr]{};
					TAAnode *ano = dcArr->GetMWDC(i)->GetAnode(j, m+1, k);
					for(int str_id = 0; str_id < nAng; str_id++){
						sprintf(name, "STRCali/histo/hRDCSTRCor_%d_%d_%d_%d_%d", i, j, m, k, str_id);
						TH2F *h2 = h2void;
						if(f->Get(name)) h2 = (TH2F*)f->Get(name);
						int valid_bin_cnt = 0; // valid drift distance bin number
						int valid_bin_array[nr]; // valid bin number array
						for(int l = 0; l < nr; l++){ // loop over drift distance bins
							// get the projection of the l-th channal along X axis,
							// from which mean and sigma would be estimated.
							// project the l-th bin in X-axis to Y axis
							TH1D *hpro = h2->ProjectionY("hpro", l+1, l+1);
							int npro = hpro->GetEntries();
							if(0) if(0 == l) npro = 0; // the first bin is biased.
							if(npro < 500){ // stastics is too small
								strCor[str_id][l] = 0.;
								strCorSigma[str_id][l] = 0.;
							}
							else{
								// mean, unit: mm
								fgaus->SetParameter(1, 0.);
								fgaus->SetParLimits(1, -1., 1.);
								// sigma, unit: mm
								fgaus->SetParameter(2, 0.2);
								fgaus->SetParLimits(2, 0., 1.);
								hpro->Fit(fgaus, "NQR");
								double mean = fgaus->GetParameter("Mean");
								double sigma = fgaus->GetParameter("Sigma");
								hmean[i][j]->Fill(mean); hmeanTot->Fill(mean);
								hsigma[i][j]->Fill(sigma); hsigmaTot->Fill(sigma);
								if((mean > -0.4 && mean < 0.4) && (sigma < 1.2 && sigma > 0.)){
									strCor[str_id][l] = mean;
									strCorSigma[str_id][l] = sigma;
									// more statistics brings about more weight in the sigma average.
									sigma_g[i][j][l] += sigma * npro; // [DC][XUV]
									sigma_g_cnt[i][j][l] += npro; // [DC][XUV]
									valid_bin_array[valid_bin_cnt++] = l;
								}
//								if(j == 1) strCor[str_id][l] = 0.; // U tracks are too nasty to be trusted.
							} // end else
							mean_tree[str_id][l] = strCor[str_id][l];
							sigma_tree[str_id][l] = strCorSigma[str_id][l];
							// iterative correction, so it's addition, not replacement.
							strCor[str_id][l] +=
								((TAAnodePara*)ano->GetPara())->GetSTRCorrection(str_id)[l];
							delete hpro; hpro = nullptr; // destruct the object and free the memory
							cout << "Processing bin of " << i << " " << j << " " << m << " " << k << " ";
							cout << str_id << " " << l << "\r" << flush;
						} // end for over l
						outFile << "Info: " << setw(10) << ano->GetUID() << " ";
						outFile << setw(5) << str_id << " " << setw(5) << valid_bin_cnt << endl;
						for(int l = 0; l < valid_bin_cnt; l++) // output valid_bin_array
							outFile << setw(5) << valid_bin_array[l] << " ";
						outFile << endl;
						for(int l = 0; l < valid_bin_cnt; l++) // output STR correction array
							outFile << setw(5) << strCor[str_id][l] << " ";
						outFile << endl;
					} // loop over STR id
					treeSigma->Fill();
					outFile << endl;
				} // end for over k
				outFile << endl;
			} // end for over m
			outFile << endl;
		} // end of for over j
		outFile << endl;
	} // end of for over i
	outFile << endl;
	outFile.close();

	double sigmaTot[nr]{}; int sigmaTot_cnt[nr]{};
	for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++)
	for(int l = 0; l < nr; l++) if(sigma_g_cnt[i][j][l] > 0){
		sigmaTot[l] += sigma_g[i][j][l];
		sigmaTot_cnt[l]  += sigma_g_cnt[i][j][l];
		sigma_g[i][j][l] /= sigma_g_cnt[i][j][l];
		gsigma[i][j]->SetPoint(l, rstep*(l+0.5), sigma_g[i][j][l]); // l+0.5: bin center
	}
	for(int l = 0; l < nr; l++) if(sigmaTot_cnt[l] > 0)
		gsigmaTot->SetPoint(l, rstep*(l+0.5), sigmaTot[l]/sigmaTot_cnt[l]);

	// write //
	f->cd(dir); // stores sigma, mean and treeSgima
	for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++){
		hmean[i][j]->Write("", TObject::kOverwrite);
		hsigma[i][j]->Write("", TObject::kOverwrite);
		gsigma[i][j]->Write("", TObject::kOverwrite);
	}
	hmeanTot->Write("", TObject::kOverwrite);
	hsigmaTot->Write("", TObject::kOverwrite);
	gsigmaTot->Write("", TObject::kOverwrite);
	treeSigma->Write("", TObject::kOverwrite);

	cout << "\n\n\033[33;1mDONE\033[0m\n\n";
	f->Close(); delete f;
} // end of member function GenerateCalibFile






