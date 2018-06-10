// shoot.C -- shoot calibration of incident and exit beam through the target

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

using std::endl;
using std::cout;
using std::flush;
using std::vector;

void shoot(const char *rootfile);

int main(int argc, char **argv){
	if(argc < 2){
		cout << "./sho <rootfile>. rootfile name has to be provided as the argument\n";
		exit(1);
	} // end if

	shoot(argv[1]);
	return 0;
} // end of the main function

void shoot(const char *rootfile){
    TFile *f = new TFile(rootfile, "UPDATE");

	// read the track tree
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	const int ntrMax = 100;
	int ntr, ntrT, index, nu[ntrMax][6]{}, gGOOD[ntrMax]{};
	int type[ntrMax]{}, id[ntrMax]{};
	int ntrLs[6][3]{}; // N of TrkProjs; DCArr-L-R-U-D -- [XUV - XY]
	double t[ntrMax][6]{}, r[ntrMax][6]{}, k[ntrMax]{}, b[ntrMax]{};
	double chi[ntrMax][6]{}, chi2[ntrMax]{}, Chi[ntrMax]{}, TOF[ntrMax]{};
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("ntrLs", ntrLs);
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

	vector<TObject *> objls;
	TH1F *hXTaU = new TH1F("hXTaU", "hXTaU", 1000, -100., 100.); objls.push_back(hXTaU);
	TH1F *hXTaD = new TH1F("hXTaD", "hXTaD", 1000, -100., 100.); objls.push_back(hXTaD);
	TH2F *hXTaUvsD = new TH2F("hXTaUvsD", "Target Hit X - U trk v.s. D trk;X-Utrk [mm];X-Dtrk [mm]", 1000, -100., 100., 1000, -100., 100.); objls.push_back(hXTaUvsD);
	TH1F *hdXTa = new TH1F("hdXTa", "XTaU - XTaD", 1000, -300., 100.); objls.push_back(hdXTa);
	TH1F *hkU = new TH1F("hkU", "k from tracks in DCArrU", 1000, -1., 1.); objls.push_back(hkU);
	TH1F *hkD = new TH1F("hkD", "k from tracks in DCArrD", 1000, -1., 1.); objls.push_back(hkD);
	TH1F *hdkTa = new TH1F("hdkTa", "kTaU - kTaD", 1000, -1., 1.); objls.push_back(hdkTa);
	TH2F *htof2vsPDCD1X = new TH2F("htof2vsPDCD1X", "htof2vsPDCD1X", 1000, -300., 300., 1000, 50., 65.); objls.push_back(htof2vsPDCD1X);
	const double zTa = -1863.235;
//	const double zPDCD1 = -939.039;


	const int n = treeTrack->GetEntries(); // number of data sections
	cout << "Totally " << n << " data sections would be processed.\n";
	for(int i = 0; i < n; i++){
		treeTrack->GetEntry(i);
		double kU = 0., bU = 0., kD = 0., bD = 0.;
		double xTaU = 0., xTaD = 0.;
		if(1 == ntrLs[2][0] && 1 == ntrLs[3][0]){
			for(int j = 0; j < ntrT; j++){ // loop over tracks
				if(120 == type[j]){ // DCArrU
					kU = k[j]; bU = b[j];
					xTaU = kU*zTa + bU;
				}
				if(130 == type[j]){ // DCArrD
					kD = k[j]; bD = b[j];
					xTaD = kD*zTa + bD;
				}
			} // end loop over tracks
			// track slope statistics
			hkU->Fill(kU); hkD->Fill(kD); hdkTa->Fill(kU - kD);
			// target hit position - X
			hXTaU->Fill(xTaU); hXTaD->Fill(xTaD);
			hdXTa->Fill(xTaU - xTaD); hXTaUvsD->Fill(xTaU, xTaD);
		} // end if
		cout << "Processing data section " << i << "\r" << flush;
	} // end for over entries

	// write and clean //
	const char *dir = "shoot";
	if(!f->FindObjectAny(dir)) f->mkdir(dir);
	f->cd(dir);
	for(TObject *&b : objls){
		b->Write("", TObject::kOverwrite);
		delete b; b = nullptr;
	}
	f->Close(); delete f; f = nullptr;

	cout << "\n\n\033[33;1mDONE\033[0m\n\n";
} // end of function shoot





