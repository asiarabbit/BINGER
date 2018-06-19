// shoot.C -- shoot calibration of incident and exit beam through the target

#include <iostream>
#include <sstream>
#include <string>
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
using std::ostringstream;
using std::string;

void shoot(const char *rootfile);

const double tRef_pos_LB = -0.43;
const double tRef_pos_HB = 0.43;
const double zTa = -1863.235;
const double zT0_1 = -2699.08;
const double zVeto = zT0_1  - 100.;

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
	double t[ntrMax][6]{}, r[ntrMax][6]{}, k_[ntrMax]{}, b[ntrMax]{};
	double chi[ntrMax][6]{}, chi2[ntrMax]{}, Chi[ntrMax]{}, TOF[ntrMax]{};
	double tRef_pos; // hit position of T0_1
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("tRef_pos", &tRef_pos);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("ntrLs", ntrLs);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("k", k_);
	treeTrack->SetBranchAddress("b", b);
	treeTrack->SetBranchAddress("chi", chi);
	treeTrack->SetBranchAddress("chi2", chi2);
	treeTrack->SetBranchAddress("Chi", Chi);
	treeTrack->SetBranchAddress("gGOOD", gGOOD);
	treeTrack->SetBranchAddress("type", type);
	treeTrack->SetBranchAddress("id", id);

	// vector for ROOT objects management
	vector<TObject *> objls;

	// hit point on the target and the track information
	double taHitPos[2][2], kTa[2][2], bTa[2][2]; // [0-1][0-1]: [U-D][X-Y]
	double vetoPos[2], t0_1Pos[2];
	bool t0_1_ok = false; // if TOF stop signal is good
	TTree *treeshoot = new TTree("treeshoot", "shoot! haha~");
	treeshoot->Branch("index", &index, "index/I");
	treeshoot->Branch("taHitPos", taHitPos, "taHitPos[2][2]/D");
	treeshoot->Branch("vetoPos", vetoPos, "vetoPos[2]/D");
	treeshoot->Branch("t0_1Pos", t0_1Pos, "t0_1Pos[2]/D");
	treeshoot->Branch("kTa", kTa, "kTa[2][2]/D");
	treeshoot->Branch("bTa", bTa, "bTa[2][2]/D");
	treeshoot->Branch("t0_1_ok", &t0_1_ok, "t0_1_ok/O");

	const char ud[] = "UD", xy[] = "XY";
	ostringstream name, title;
	TH1F *hTaPos1D[2][2]{}; // [0-1][0-1]: [U-D][X-Y]
	TH2F *hTaPos2D[2]{}, *hTaPos1DMatch[2]{}; // [0-1]: [U-D]; 1DMatch: [0-1]: [X-Y]
	TH2F *hT0_1Pos = new TH2F("hT0_1Pos", "T0_1 Hit Position;X [mm];Y [mm]", 500, -100., 100., 500, -100., 100.);
	TH2F *hVetoPos = new TH2F("hVetoPos", "VETO Hit Position;X [mm];Y [mm]", 500, -100., 100., 500, -100., 100.);
	objls.push_back(hT0_1Pos); objls.push_back(hVetoPos);
	for(int i = 0; i < 2; i++){ // loop over PDCArrU-D
		for(int j = 0; j < 2; j++){ // loop over X and Y
			name.str(""); title.str("");
			name << "hTaPos1D-" << ud[i] << xy[j];
			title << "Target Hit Position " << xy[j] << " - PDCArray" << ud[i];
			title << ";" << xy[j] << " [mm]";
			hTaPos1D[i][j] = new TH1F(name.str().c_str(), title.str().c_str(), 1000, -100., 100.);
			objls.push_back(hTaPos1D[i][j]);
		} // end for over j
		name.str(""); title.str(""); // clear name and title
		name << "hTaPos2D-" << ud[i];
		title << "Target Hit Position X v.s. Y" << " - PDCArray" << ud[i] << ";X [mm];Y [mm]";
		hTaPos2D[i] = new TH2F(name.str().c_str(), title.str().c_str(), 1000, -100., 100., 1000, -100., 100.);
		objls.push_back(hTaPos2D[i]);
		name.str(""); title.str("");
		name << "hTaHitPos1DMatch-" << xy[i];
		title << "Target Hit Postion - " << xy[i] << " - PDCArray U-D Match;DCArrU [mm];DCArrD [mm]";
		hTaPos1DMatch[i] = new TH2F(name.str().c_str(), title.str().c_str(), 1000, -100., 100., 1000, -100., 100.);
		objls.push_back(hTaPos1DMatch[i]);
	} // end for over i


	const int n = treeTrack->GetEntries(); // number of data sections
	cout << "Totally " << n << " data sections would be processed.\n";
	for(int i = 0; i < n; i++){
		treeTrack->GetEntry(i);
		for(int k = 0; k < 2; k++){ // loop over U-D
			for(int l = 0; l < 2; l++){ // loop over X-Y
				kTa[k][l] = -9999.; bTa[k][l] = -9999.; taHitPos[k][l] = -9999.;
				const int TYPE = 100 + (k + 2) * 10 + l; // 120-121-130-131: UX-UY-DX-DY
				for(int j = 0; j < ntrT; j++){ // loop over tracks
					if(TYPE == type[j]){
						kTa[k][l] = k_[j]; bTa[k][l] = b[j];				
						taHitPos[k][l] = kTa[k][l] * zTa + bTa[k][l];
						if(1 == ntrLs[2+k][l]) hTaPos1D[k][l]->Fill(taHitPos[k][l]);
						if(0 == k){	// PDCArrU
							t0_1Pos[l] = kTa[k][l] * zT0_1 + bTa[k][l];
							vetoPos[l] = kTa[k][l] * zVeto + bTa[k][l];
						}
					} // end if
				} // end loop over tracks
			} // end loop over X-Y
		} // end for over k
		for(int j = 0; j < 2; j++){
			if(1 == ntrLs[2+j][0] && 1 == ntrLs[2+j][1]){ // j: U-D
				hTaPos2D[j]->Fill(taHitPos[j][0], taHitPos[j][1]);
				if(0 == j){ // PDCArrU
					hT0_1Pos->Fill(t0_1Pos[0], t0_1Pos[1]);
					hVetoPos->Fill(vetoPos[0], vetoPos[1]);
				} // end if
			}
			if(1 == ntrLs[2][j] && 1 == ntrLs[3][j]){ // j: X-Y
				hTaPos1DMatch[j]->Fill(taHitPos[0][j], taHitPos[1][j]);
			}
		} // end for over j
		t0_1_ok = tRef_pos > tRef_pos_LB && tRef_pos < tRef_pos_HB;
		treeshoot->Fill();
		cout << "Processing data section " << i << "\r" << flush;
	} // end for over entries

	// write and clean //
	const char *dir = "SHOOT";
	treeshoot->Write("", TObject::kOverwrite); // write in the outside
	if(!f->FindObjectAny(dir)) f->mkdir(dir);
	f->cd(dir);
	for(TObject *&b : objls){
		b->Write("", TObject::kOverwrite);
		delete b; b = nullptr;
	}
	f->Close(); delete f; f = nullptr;

	cout << "\n\n\033[33;1mDONE\033[0m\n\n";
} // end of function shoot





