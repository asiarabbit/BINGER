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

#include "TAEventProcessor.h"
#include "TAGPar.h"
#include "TAParaManager.h"
#include "TADetectorPara.h"
#include "TAMWDCArray.h"
#include "TAMWDCArray2.h"
#include "TAMWDC.h"
#include "TAMWDCTa.h"
#include "TATOFWall.h"
#include "TAMath.h"

using std::endl;
using std::cout;
using std::flush;
using std::vector;
using std::ostringstream;
using std::string;

void shoot(const char *rootfile);

const double tRef_pos_LB = -0.43;
const double tRef_pos_HB = 0.43;

int main(int argc, char **argv){
	if(argc < 2){
		cout << "./sho <rootfile>. rootfile name has to be provided as the argument\n";
		exit(1);
	} // end if

	shoot(argv[1]);
	return 0;
} // end of the main function

void shoot(const char *rootfile){
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TAParaManager::ArrDet_t &det_vec = ep->GetParaManager()->GetDetList();
	TAMWDCArray *dcArr[2]{0}; // MWDC arrays downstream of the dipole magnet
	TAMWDCArray2 *pdcArr2[2]{0}; // PDC arrays upstream of the dipole magnet
	dcArr[0] = (TAMWDCArray*)det_vec[3]; // dc array L
	dcArr[1] = (TAMWDCArray*)det_vec[4]; // dc array R
	pdcArr2[0] = (TAMWDCArray2*)det_vec[8]; // pdc array U
	pdcArr2[1] = (TAMWDCArray2*)det_vec[9]; // pdc array D
	TATOFWall *tofw[2]{0};
	if(dcArr[0]) tofw[0] = dcArr[0]->GetTOFWall();
	if(dcArr[1]) tofw[1] = dcArr[1]->GetTOFWall();
	// dc downstream and upstream of the dipole magnet, pdc: same role as dc2
	TAMWDC *dc[2][3]{0}, *pdc2[2][2]{0};
	for(int i = 2; i--;) for(int j = 3; j--;) if(dcArr[i]) dc[i][j] = dcArr[i]->GetMWDC(j);
	for(int i = 2; i--;) for(int j = 2; j--;) if(pdcArr2[i]) pdc2[i][j] = pdcArr2[i]->GetMWDC(j);
	
	TAGPar *gp = TAGPar::Instance();
	const double zTa = gp->Val(84); // -1863.235
	const double zT0_1 = gp->Val(85); // -2699.08;
	const double zVeto = zT0_1  - 100.;
	double zPDC[4]{};
	// {-2560.91, -2018.2, -1730.02, -939.039};
	for(int i = 0; i < 2; i++) for(int j = 0; j < 2; j++){
		zPDC[i*2+j] = pdc2[i][j]->GetDetPara()->GetZ();
	}
	double kDC[3], bDC[3], kTOFW, bTOFW;
	double zDC[3], xDC[3], yDC[3], zTOFW, xTOFW, yTOFW;
	for(int i = 0; i < 3; i++){
		double phi = dc[1][i]->GetDetPara()->GetPhi();
		double zc = dc[1][i]->GetDetPara()->GetZ();
		double xc = dc[1][i]->GetDetPara()->GetX();
		double yc = dc[1][i]->GetDetPara()->GetY();
		kDC[i] = tan(phi+TAMath::Pi()/2.);
		bDC[i] = xc - kDC[i] * zc;
		zDC[i] = zc; xDC[i] = xc; yDC[i] = yc;
	}
	double phi = tofw[1]->GetDetPara()->GetPhi();
	double zc = tofw[1]->GetDetPara()->GetZ();
	double xc = tofw[1]->GetDetPara()->GetX();
	double yc = tofw[1]->GetDetPara()->GetY();
	kTOFW = tan(phi+TAMath::Pi()/2.);
	bTOFW = xc - kTOFW * zc;
	zTOFW = zc; xTOFW = xc; yTOFW = yc;


    TFile *f = new TFile(rootfile, "UPDATE");

	// read the track tree
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *vme = (TTree*)f->Get("vme");
	TTree *treePID3D = (TTree*)f->Get("treePID3D");
	treeTrack->AddFriend(vme);
	treeTrack->AddFriend(treePID3D);
	const int ntrMax = 100, n3DtrMax = ntrMax / 3.;
	int ntr, ntrT, index, nu[ntrMax][6]{}, gGOOD[ntrMax]{};
	int type[ntrMax]{}, id[ntrMax]{}, firedStripId[ntrMax];
	int ntrLs[6][3]{}; // N of TrkProjs; DCArr-L-R-U-D -- [XUV - XY]
	double t[ntrMax][6]{}, r[ntrMax][6]{}, k_[ntrMax]{}, b[ntrMax]{}, aoz[ntrMax]{};
	double TOT_DC[ntrMax][6]; // TOT dcs, having already checked
	double chi[ntrMax][6]{}, chi2[ntrMax]{}, Chi[ntrMax]{}, TOF[ntrMax]{};
	double tRef_pos; // hit position of T0_1
	unsigned sca[16];
	double k2[n3DtrMax], b2[n3DtrMax];
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("tRef_pos", &tRef_pos);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("ntrLs", ntrLs);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrack->SetBranchAddress("TOT_DC", TOT_DC);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("k", k_);
	treeTrack->SetBranchAddress("b", b);
	treeTrack->SetBranchAddress("chi", chi);
	treeTrack->SetBranchAddress("chi2", chi2);
	treeTrack->SetBranchAddress("Chi", Chi);
	treeTrack->SetBranchAddress("firedStripId", firedStripId);
	treeTrack->SetBranchAddress("gGOOD", gGOOD);
	treeTrack->SetBranchAddress("type", type);
	treeTrack->SetBranchAddress("id", id);
	treeTrack->SetBranchAddress("aoz", aoz);
	vme->SetBranchAddress("sca", sca);
	treePID3D->SetBranchAddress("k2", k2);
	treePID3D->SetBranchAddress("b2", b2);

	// vector for ROOT objects management
	vector<TObject *> objls;

	// hit point on the target and the track information
	double taHitPos[2][2], kTa[2][2], bTa[2][2]; // [0-1][0-1]: [U-D][X-Y]
	double TOT_Ta[2][2][6], TOT_Avrg_Ta[2][2]; // [0-1][0-1]: [U-D][X-Y]
	int TOTcnt_Ta[2][2]; // [0-1][0-1]: [U-D][X-Y]
	double vetoPos[2], t0_1Pos[2];
	double PDCPos[4][2]; // [0-3][0-1]: [PDCU0-1--PDCD0-1][X-Y]
	double chiTa[2][2][6]; // [U-D][X-Y]
	double kDC_, bDC_;
	int nuTa[2][2][6], nuDCR[6]; // only the first track would be stored
	double DCRPos[6][2], TOFWPos[2]; // DCRPos: [DC0X1X2-DC1X1X2-DC2X1X2][X-Y]
	bool t0_1_ok = false; // if TOF stop signal is good
	// if the particle hit around the TOF Wall strip gaps,
	// which would result in a longer tof2
	double stripGap = -9999.;
	const double stripGapP0 = -0.327509, stripGapP1 = 40.3689; // strip Gap function: s(stripId) = p0+stripId*p1
	double sca1drv = 0.;
	TTree *treeshoot = new TTree("treeshoot", "shoot! haha~");
	treeshoot->Branch("index", &index, "index/I");
	treeshoot->Branch("taHitPos", taHitPos, "taHitPos[2][2]/D");
	treeshoot->Branch("vetoPos", vetoPos, "vetoPos[2]/D");
	treeshoot->Branch("t0_1Pos", t0_1Pos, "t0_1Pos[2]/D");
	treeshoot->Branch("PDCPos", PDCPos, "PDCPos[4][2]/D");
	treeshoot->Branch("kTa", kTa, "kTa[2][2]/D");
	treeshoot->Branch("bTa", bTa, "bTa[2][2]/D");
	treeshoot->Branch("kDC", &kDC_, "kDC/D");
	treeshoot->Branch("bDC", &bDC_, "bDC/D");
	treeshoot->Branch("TOT_Avrg_Ta", TOT_Avrg_Ta, "TOT_Avrg_Ta[2][2]/D");
	treeshoot->Branch("TOTcnt_Ta", TOTcnt_Ta, "TOTcnt_Ta[2][2]/I");
	treeshoot->Branch("chiTa", chiTa, "chiTa[2][2][6]/D");
	treeshoot->Branch("nuTa", nuTa, "nuTa[2][2][6]/I"); // U-D -- X-Y -- DC0X1-X2-DC1-X1-X2
	treeshoot->Branch("nuDCR", nuDCR, "nuDCR[6]/I");
	treeshoot->Branch("DCRPos", DCRPos, "DCRPos[3][2]/D");
	treeshoot->Branch("TOFWPos", TOFWPos, "TOFWPos[2]/D");
	treeshoot->Branch("t0_1_ok", &t0_1_ok, "t0_1_ok/O");
	// d(#)/dt - derivative of daq and beam over time
	treeshoot->Branch("sca1drv", &sca1drv, "sca1drv/D"); // sca1: trigger request
	treeshoot->Branch("stripGap", &stripGap, "stripGap/D");

	const char ud[] = "UD", xy[] = "XY";
	ostringstream name, title;
	TH1F *hTaPos1D[2][2]{}; // [0-1][0-1]: [U-D][X-Y]
	TH2F *hTaPos2D[2]{}, *hTaPos1DMatch[2]{}; // [0-1]: [U-D]; 1DMatch: [0-1]: [X-Y]
	TH2F *hT0_1Pos = new TH2F("hT0_1Pos", "T0_1 Hit Position;X [mm];Y [mm]", 1000, -70., 70., 1000, -70., 70.);
	TH2F *hVetoPos = new TH2F("hVetoPos", "VETO Hit Position;X [mm];Y [mm]", 1000, -70., 70., 1000, -70., 70.);
	TH2F *hPDCPos[4];
	objls.push_back(hT0_1Pos); objls.push_back(hVetoPos);
	for(int i = 0; i < 2; i++){ // loop over PDCArrU-D
		for(int j = 0; j < 2; j++){ // loop over X and Y
			name.str(""); title.str("");
			name << "hTaPos1D-" << ud[i] << xy[j];
			title << "Target Hit Position " << xy[j] << " - PDCArray" << ud[i];
			title << ";" << xy[j] << " [mm]";
			hTaPos1D[i][j] = new TH1F(name.str().c_str(), title.str().c_str(), 1000, -70., 70.);
			objls.push_back(hTaPos1D[i][j]);
		} // end for over j
		name.str(""); title.str(""); // clear name and title
		name << "hTaPos2D-" << ud[i];
		title << "Target Hit Position X v.s. Y" << " - PDCArray" << ud[i] << ";X [mm];Y [mm]";
		hTaPos2D[i] = new TH2F(name.str().c_str(), title.str().c_str(), 1000, -70., 70., 1000, -70., 70.);
		objls.push_back(hTaPos2D[i]);
		name.str(""); title.str("");
		name << "hTaHitPos1DMatch-" << xy[i];
		title << "Target Hit Postion - " << xy[i] << " - PDCArray U-D Match;DCArrU [mm];DCArrD [mm]";
		hTaPos1DMatch[i] = new TH2F(name.str().c_str(), title.str().c_str(), 1000, -70., 70., 1000, -70., 70.);

		for(int j = 0; j < 2; j++){	// DC0-1
			name.str(""); title.str("");
			name << "hPDCPos-" << ud[i] << "DC-" << j;
			title << "Beam Position PDC-" << ud[i] << "DC-" << j << ";X [mm];Y [mm]";
			hPDCPos[i*2+j] = new TH2F(name.str().c_str(), title.str().c_str(), 1000, -70., 70., 1000, -70., 70.);
			objls.push_back(hPDCPos[i*2+j]);
		} // end loop over j
		objls.push_back(hTaPos1DMatch[i]);
	} // end for over i
	TH1F *hNu[3][2], *hDCRPosX[3], *hTOFWFiredStrip, *hTOFWPosX;
	for(int i = 0; i < 3; i++){ // loop over DCs in DCArrR
		for(int j = 0; j < 2; j++){ // loop over X1-2
			name.str(""); title.str("");
			name << "hNuDC" << i << "X" << j + 1;
			title << "Hit Sense wire Distribution of DCArrR-DC" << i << "X" << j + 1 << ";Sense Wire Id";
			hNu[i][j] = new TH1F(name.str().c_str(), title.str().c_str(), 101, -10.5, 90.5);
			objls.push_back(hNu[i][j]);
		}
		name.str(""); title.str("");
		name << "hDCRPosX-DC" << i;
		title << "Hit Position Distribution of DCArrR-DC" << i << "X;X (from Hit Point to DC Center) [mm]";
		hDCRPosX[i] = new TH1F(name.str().c_str(), title.str().c_str(), 600, -600., 600.);
		objls.push_back(hDCRPosX[i]);
	} // end for over i
	hTOFWFiredStrip = new TH1F("hTOFWFiredStrip", "Hit TOF Wall Distribution;Strip Id", 32, -1.5, 30.5);
	objls.push_back(hTOFWFiredStrip);
	hTOFWPosX = new TH1F("hTOFWPosX", "Hit Position Distribution of TOFWR;X [mm]", 800, -800., 800.);
	objls.push_back(hTOFWPosX);


	const int n = treeTrack->GetEntries(); // number of data sections
//	n = 100000; // DEBUG
	cout << "Totally " << n << " data sections would be processed.\n";
	for(int i = 0; i < n; i++){
		treeTrack->GetEntry(i);
//		if(-9999. == aoz[0]) continue; // only valid physical events are of our interest

		memset(nuTa, -1, sizeof(nuTa));
		memset(nuDCR, -1, sizeof(nuDCR));
		for(int k = 0; k < 2; k++){ // loop over U-D
			for(int l = 0; l < 2; l++){ // loop over X-Y

				// initilization //
				kTa[k][l] = -9999.; bTa[k][l] = -9999.;
				taHitPos[k][l] = -9999.;
				PDCPos[2*k][l] = -9999.; PDCPos[2*k+1][l] = -9999.;
				vetoPos[l] = -9999.; t0_1Pos[l] = -9999.;
				for(int ii = 0; ii < 6; ii++){
					chiTa[k][l][ii] = -9999.; TOT_Ta[k][l][ii] = -9999.;
					nuTa[k][l][ii] = -1;
				} // end for over ii

				const int TYPE = 100 + (k + 2) * 10 + l; // 120-121-130-131: UX-UY-DX-DY
				for(int j = 0; j < ntrT; j++){ // loop over tracks
					if(TYPE == type[j]){
						kTa[k][l] = k_[j]; bTa[k][l] = b[j];
						for(int ii = 0; ii < 6; ii++){
							chiTa[k][l][ii] = chi[j][ii];
							nuTa[k][l][ii] = nu[j][ii];
							TOT_Ta[k][l][ii] = TOT_DC[j][ii];
						}
						taHitPos[k][l] = kTa[k][l] * zTa + bTa[k][l];
						for(int m = 0; m < 2; m++){ // loop over PDC[UD]DC0-1
							PDCPos[2*k+m][l] = kTa[k][l] * zPDC[2*k+m] + bTa[k][l];
						} // end for over m
						if(1 == ntrLs[2+k][l]) hTaPos1D[k][l]->Fill(taHitPos[k][l]);
						if(0 == k){	// PDCArrU
							t0_1Pos[l] = kTa[k][l] * zT0_1 + bTa[k][l];
							vetoPos[l] = kTa[k][l] * zVeto + bTa[k][l];
						}
					} // end if
				} // end loop over tracks
			} // end loop over X-Y
		} // end for over k

		// *** initialization *** //
		for(int ii = 0; ii < 2; ii++){ // loop over X-Y
//			if(1 == ii) continue;
			for(int jj = 0; jj < 3; jj++){ // loop over MWDCs
				DCRPos[jj][ii] = -9999.;
			}
			TOFWPos[ii] = -9999.;
		} // end for over i
		kDC_ = -9999.; bDC_ = -9999.;
		for(int j = 0; j < 6; j++) nuDCR[j] = -9999.;
		stripGap = -9999.;

		for(int j = 0; j < ntrT; j++){ // loop over tracks
			if(110 == type[j]){
				kDC_ = k_[j]; bDC_ = b[j];
				for(int k = 0; k < 3; k++){ // loop over DC0-1-2
					for(int l = 0; l < 2; l++){ // loop over X1-X2
						if(nu[j][k*2+l] >= 0 && 1 == ntrLs[1][0]){
							int NU = k*2+l;
							hNu[k][l]->Fill(nu[j][NU]);
							nuDCR[NU] = nu[j][NU];
						}
					} // end loop over X1-X2
					double zhit = (b[j] - bDC[k]) / (kDC[k] - k_[j]);
					double xhit = (b[j]*kDC[k] - bDC[k]*k_[j]) / (kDC[k] - k_[j]);
					// from hit to the detector center
					double sHit = sqrt((zhit-zDC[k])*(zhit-zDC[k]) + (xhit-xDC[k])*(xhit-xDC[k]));
					if(xhit < xDC[k]) sHit *= -1.; // on the beam side of the DC
					DCRPos[k][0] = sHit;
					if(k2[0] != -9999.) DCRPos[k][1] = k2[0]*zhit+b2[0]-yDC[k];
					hDCRPosX[k]->Fill(sHit);
				} // end loop over 3 DCs
				double zhit = (b[j] - bTOFW) / (kTOFW - k_[j]);
				double xhit = (b[j]*kTOFW - bTOFW*k_[j]) / (kTOFW - k_[j]);
				double sHit = sqrt((zhit-zTOFW)*(zhit-zTOFW) + (xhit-xTOFW)*(xhit-xTOFW));
				if(xhit < xTOFW) sHit *= -1.; // on the beam side of the DC
				TOFWPos[0] = sHit;				
				if(k2[0] != -9999.) TOFWPos[1] = k2[0]*zhit+b2[0]-yTOFW;
				hTOFWPosX->Fill(sHit);
				hTOFWFiredStrip->Fill(firedStripId[j]);
				
				// get the distance from hit point in the TOFWall to the nearest strip gap
				stripGap = 1E200;
				for(int m = firedStripId[j] - 15; m < firedStripId[j] - 13; m++){
					double tmp = fabs(sHit-(stripGapP0 + stripGapP1*m));
					if(tmp < stripGap) stripGap = tmp;
				} // end for over m
//				cout.setf(std::ios_base::fixed, std::ios_base::floatfield); // DEBUG
//				cout << "sHit: " << sHit << endl; // DEBUG
//				cout << "stripGap: " << stripGap << endl; // DEBUG
//				getchar(); // DEBUG
			} // end if
		} // end loop over tracks
		for(int j = 0; j < 2; j++){
			if(1 == ntrLs[2+j][0] && 1 == ntrLs[2+j][1]){ // j: U-D
				hTaPos2D[j]->Fill(taHitPos[j][0], taHitPos[j][1]);
				for(int m = 0; m < 2; m++){ // DC0-1
					hPDCPos[2*j+m]->Fill(PDCPos[2*j+m][0], PDCPos[2*j+m][1]);
					TOT_Avrg_Ta[j][m] = 0.; TOTcnt_Ta[j][m] = 0;
					for(int l = 0; l < 4; l++) if(-9999. != TOT_Ta[j][m][l]){
						TOT_Avrg_Ta[j][m] += TOT_Ta[j][m][l];
						TOTcnt_Ta[j][m]++;
//						cout << "index: " << index << endl; // DEBUG
//						cout << "TOT_Ta[j][m][l]: " << TOT_Ta[j][m][l] << endl; // DEBUG
//						cout << "TOT_Avrg_Ta[j][m]: " << TOT_Avrg_Ta[j][m] << endl; // DEBUG
//						cout << "TOTcnt_Ta[j][m]: " << TOTcnt_Ta[j][m] << endl; // DEBUG
					} // end for over l
					if(0 != TOTcnt_Ta[j][m]) TOT_Avrg_Ta[j][m] /= TOTcnt_Ta[j][m];
					else TOT_Avrg_Ta[j][m] = -9999.;
//					cout << "TOT_Avrg_Ta[j][m]: " << TOT_Avrg_Ta[j][m] << endl; // DEBUG
				} // end for over m
//				getchar(); // DEBUG
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


		//////// calculate derivatives of daq over time //////////
		// dsca1/dsca15
		int sca1ph, sca1mh; // +h, -h; trigger request count
		int sca15ph, sca15mh; // +h, -h; 1khz clock
		if(i < 30 || i > n - 31) sca1drv = 0.;
		else{
			vme->GetEntry(i); sca1ph = sca[1]; sca15ph = sca[15];
			vme->GetEntry(i-30); sca1mh = sca[1]; sca15mh = sca[15];
			if(sca15ph-sca15mh > 10000.){ // spill crossed
				vme->GetEntry(i); sca1ph = sca[1]; sca15ph = sca[15];
				vme->GetEntry(i+30); sca1mh = sca[1]; sca15mh = sca[15];
			}
			sca1drv = 1. * (sca1ph - sca1mh) / (sca15ph - sca15mh);
		}


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





