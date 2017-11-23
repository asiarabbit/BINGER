// assess0L.C -- assess track qualities after step 0 - pattern recognition

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <string>
#include "TAEventProcessor.h"
#include "TAMWDCArray.h"
#include "TAParaManager.h"
#include "TAMWDC.h"
#include "TADetectorPara.h"
#include "TH1F.h"
#include "TTree.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH2F.h"
#include "TAMath.h"

using namespace std;

const double DEGREE = 3.1415926/180.;
const char *dir = "assess0L";
int main(int argc, char *argv[]){
	string ccrootfile = argv[1];
	cout << "Input rootfile name: " << ccrootfile << endl; // getchar();

	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Verbose(false); // don't show TAPopMsg::ConfigInfo() printings (detector information)
	ep->Silent(); // don't show TAPopMsg::Info() printings
	ep->CoarseFit(); // rough yet fast fit // May Not Use

	ep->SetConfigExpDir("pion_2017Oct");
	ep->SetSTRROOTFile("aaa1500.root");
	ep->SetMagneticIntensity(0.2484); // unit: Telsa
	ep->Configure();

	TAParaManager::ArrDet_t &dec_vec = ep->GetParaManager()->GetDetList();
	TAMWDCArray *dcArrL, *dcArrR;
	dcArrR = (TAMWDCArray*)dec_vec[3]; // dc array L
	dcArrL = (TAMWDCArray*)dec_vec[4]; // dc array R

	const double phiAvrg = dcArrR->GetPhiAvrg();
	double phiDC[3]{};
	for(int i = 3; i--;) phiDC[i] = dcArrR->GetMWDC(i)->GetDetPara()->GetPhi();
	const double zDC[3] = {dcArrR->GetMWDC(0)->GetDetPara()->GetZ(), dcArrR->GetMWDC(1)->GetDetPara()->GetZ(), dcArrR->GetMWDC(2)->GetDetPara()->GetZ()}; // for calculation of 3D coincidence deviation in x direction
	// read the track tree
	const int ntrMax = 200, ntrMax3D = ntrMax / 3;
	int ntr, index, type[ntrMax], id[ntrMax], nu[ntrMax][6], gGOOD[ntrMax];
	double t[ntrMax][6], r[ntrMax][6], k[ntrMax], b[ntrMax];
	double chi[ntrMax][6], chi2[ntrMax], Chi[ntrMax];
	double TOF[ntrMax], nStripStray[ntrMax], xMiss3D[ntrMax][3];
	int firedStripId[ntrMax];
	TFile *frt = new TFile("../config/STR/aaa1500.root");
	TF1 *rt = (TF1*)frt->Get("RT1");
	frt->Close();
	TH1F *hr_ = new TH1F("hr_", "hr_", 500, -6.5, 6.5);
	TFile *ff = new TFile(ccrootfile.c_str(), "UPDATE");
	TTree *treeTrack = (TTree*)ff->Get("treeTrack");
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrack->SetBranchAddress("firedStripId", firedStripId);
	treeTrack->SetBranchAddress("nStripStray", nStripStray);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("xMiss3D", xMiss3D);
	treeTrack->SetBranchAddress("chi", chi); // residuals for each hit
	treeTrack->SetBranchAddress("chi2", chi2); // sum of chi^2
	treeTrack->SetBranchAddress("Chi", Chi); // sqrt(chi2/nFiredAnodeLayer)
	treeTrack->SetBranchAddress("gGOOD", gGOOD);
	treeTrack->SetBranchAddress("k", k); // start for iterative fit, necessary
	treeTrack->SetBranchAddress("b", b); // start for iterative fit, necessary
	treeTrack->SetBranchAddress("type", type); // track type: 1[LR][XUV]
	treeTrack->SetBranchAddress("id", id); // 3-D track id
	char name[64], title[128], xuv[4] = "XUV";
	TH1F *hchi = new TH1F("hchi", "Overall Residual Distribution;dr/mm", 800, -4.0, 4.0);
	TH1F *hChi = new TH1F("hChi", "Track Averaged Residual Distribution(sqrt(Chi2/n));\\chi/mm", 400, 0., 4.);
	TH1F *hchi2 = new TH1F("hchi2", "Track chi2 Distribution(sum(chi^2));\\chi{^2}/mm{^2}", 1000., -0.5, 100.);
	TH2F *hrt01 = new TH2F("hrt01", "Overall Distribution of Calculated Drift Distance and Drift Time;t/ns;r/mm", 500, -100., 400., 800, -0.5, 7.5);
	TH2F *hrt02[3], *hrt03[3][3]; // [0-3]: [XUV]; [0-3][0-3]: [XUV][DC0-1-2]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hrt02_%d", i);
		sprintf(title, "Distribution of Calculated Drift Distance and Drift Time for %c-tracks;t/ns;r/mm", xuv[i]);
		hrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		for(int j = 0; j < 3; j++){ // loop over three DCs
			sprintf(name, "hrt03_%d_%d", i, j);
			sprintf(title, "Distribution of Calculated Drift Distance and Drift Time for %c-tracks-DC%d;t/ns;r/mm", xuv[i], j);
			hrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
		} // end for over j
	} // end for over i
	TH2F *hrt04 = new TH2F("hrt04", "Distribution of Calculated Drift Distance and Drift Time for DC0X1Anode40;t/ns;r/mm", 500, -100., 400., 800, -0.5, 7.5);
	TH2F *hrt04_STR[6];
	for(int i = 0; i < 6; i++){
		sprintf(name, "hrt04_STR_%d", i);
		sprintf(title, "Distribution of Calculated Drift Distance and Drift Time for DC0X1Anode40STR_%d;t/ns;r/mm", i);
		hrt04_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -0.5, 7.5);
	}
	// hdrt: chi v.s. t
	TH2F *hdrt01 = new TH2F("hdrt01", "Overall Distribution of Residual and Drift Time;t/ns;dr/mm", 500, -100., 400., 800, -4.0, 4.0);
	TH2F *hdrt02[3], *hdrt03[3][3]; // [0-3]: [XUV]; [0-3][0-3]: [XUV][DC0-1-2]
	for(int i = 0; i < 3; i++){ // loop over XUV
		sprintf(name, "hdrt02_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for %c-tracks;t/ns;dr/mm", xuv[i]);
		hdrt02[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		for(int j = 0; j < 3; j++){ // loop over three DCs
			sprintf(name, "hdrt03_%d_%d", i, j);
			sprintf(title, "Distribution of Residual and Drift Time for %c-tracks-DC%d;t/ns;dr/mm", xuv[i], j);
			hdrt03[i][j] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
		} // end for over j
	} // end for over i
	TH2F *hdrt04 = new TH2F("hdrt04", "Distribution of Residual and Drift Time for DC0X1Anode40;t/ns;dr/mm", 500, -100., 400., 800, -4.0, 4.0);
	TH2F *hdrt04_STR[6];
	for(int i = 0; i < 6; i++){
		sprintf(name, "hdrt04_STR_%d", i);
		sprintf(title, "Distribution of Residual and Drift Time for DC0X1Anode40STR_%d;t/ns;dr/mm", i);
		hdrt04_STR[i] = new TH2F(name, title, 500, -100., 400., 800, -4.0, 4.0);
	}

	TH1F *hSTRid = new TH1F("hSTRid", "STR id Distribution;STRid", 9, -1.5, 7.5);
	TH1F *hntrTot = new TH1F("hntrTot", "Total Track Count;nSec-X-U-V-3D", 8, -2.5, 5.5); // -10123
	TH1F *hntrPerSec[3];
	for(int i = 0; i < 3; i++){
		sprintf(name, "hntrPerSec%c", xuv[i]);
		sprintf(title, "Number of %c Tracks Per Data Section;ntracks/Secion", xuv[i]);
		hntrPerSec[i] = new TH1F(name, title, 11, -1.5, 9.5);
	}
	TH1F *hntrPerSec3D = new TH1F("hntrPerSec3D", "Number of 3D Tracks Per Data Section;ntracks/secion", 11, -1.5, 9.5);
	TH1F *hHAng = new TH1F("hHAng", "3D Track Horizontal Angle Distribution;angle/degree", 5000, 12., 16.);
	TH1F *hVAng = new TH1F("hVAng", "3D Track Vertical Angle Distribution;angle/degree", 600, -30., 30.);
	// statistics of number of fired anode layers per section
	TH1F *hnF[3], *hgGOOD[3]; // [XUV]
	TH2F *htt[3][3], *hrr[3][3]; // [XUV][DC0-1-2]
	TH1F *h3DMissxPre[3], *h3DMissxPost[3], *hdt[3]; // [DC0-1-2]
	for(int i = 0; i < 3; i++){
		sprintf(name, "hnF_%d", i);
		sprintf(title, "Number of Fired %c Anode Layers Per 3D Track;nF%c", xuv[i], xuv[i]);
		hnF[i] = new TH1F(name, title, 10, -1.5, 8.5);
		sprintf(name, "Hdt%d", i);
		sprintf(title, "Drift Time Distribution of MWDC%d;drift time/ns", i);
		hdt[i] = new TH1F(name, title, 500, -100, 400);
		sprintf(name, "hgGOOD%c", xuv[i]);
		sprintf(title, "gGOOD-Fired %c Anode Layers per Track", xuv[i]);
		hgGOOD[i] = new TH1F(name, title, 11, -1.5, 9.5);
		sprintf(name, "h3DMissx%dPre", i);
		sprintf(title, "x Deviation in 3D Coincidence at z of MWDC%d (Pre-Correction)", i);
		h3DMissxPre[i] = new TH1F(name, title, 5000, -100., 100.);
		sprintf(name, "h3DMissx%dPost", i);
		sprintf(title, "x Deviation in 3D Coincidence at z of MWDC%d (Post-Correction)", i);
		h3DMissxPost[i] = new TH1F(name, title, 5000, -100., 100.);
		for(int j = 0; j < 3; j++){
			sprintf(name, "htt%c%d", xuv[i], j);
			sprintf(title, "t_{X1} vs. t_{X2} for Vertical %c Tracks for MWDC%d;t_{X1}/ns;t_{X2}/ns", xuv[i], j);
			htt[i][j] = new TH2F(name, title, 500, -100., 400., 500, -100., 400.);
			sprintf(name, "hrr%c%d", xuv[i], j);
			sprintf(title, "r_{X1} vs. r_{X2} for Vertical %c Tracks for MWDC%d;r_{X1}/mm;r_{X2}/mm", xuv[i], j);
			hrr[i][j] = new TH2F(name, title, 500, -0.2, 6., 500, -0.2, 6.);
		} // end for over j
	} // end for over i
	TH1F *hnF3D = new TH1F("hnF3D", "Number of All Fired Anode Layers Per 3D Track;nF", 22, -1.5, 20.5);
	TH1F *h3DMissxTotPre = new TH1F("h3DMissxTotPre", "x Deviation in 3D Coincidence at z of all MWDCs (Pre-Correction)", 5000, -100., 100.);
	TH1F *h3DMissxTotPost = new TH1F("h3DMissxTotPost", "x Deviation in 3D Coincidence at z of all MWDCs (Post-Correction)", 5000, -100., 100.);
	TH1F *hX0 = new TH1F("hX0", "X at the Entrance of the Magnetic Field", 1000, -500., 500.);
	TH1F *hY0 = new TH1F("hY0", "Y at the Entrance of the Magnetic Field", 500, -250., 250.);


	const int n = treeTrack->GetEntries();
	int n3DtrX, n3DtrU, n3DtrV, n3Dtr;
	int trkId[3][ntrMax3D]; // track id [XUV] <-> 3D track id
	int ntrTot[3]{}, n3DtrTot = 0; // total number of tracks of all kinds of type
	int ntrPerSec[3][50]{}, n3DtrPerSec[50]{}; // [XUV][ntr]
	cout << "Totally " << n << " data sections\n";
	int hasAllCnt = 0; // count of sections that have X, U and V tracks.
	int hasXUVCnt[3]{}; // count of section that have X, U or V tracks.
	int effTot = 0, eff[3][3][2]{}; // efficiency[DC][type][layer]
	for(int i = 0; i < n; i++){
		treeTrack->GetEntry(i);

		// identify 3-D tracks
		n3DtrX = 0; n3DtrU = 0; n3DtrV = 0;
		// loop over grouped track projections
		for(int j = 0; j < ntr; j++) if(id[j] != -1){
				if(type[j] == 110){
					trkId[0][id[j]] = j; // DCArrR-X
					n3DtrX++;
				}
				if(type[j] == 111){
					trkId[1][id[j]] = j; // DCArrR-U
					n3DtrU++;
				}
				if(type[j] == 112){
					trkId[2][id[j]] = j; // DCArrR-V
					n3DtrV++;
				}
		} // end for over j and if
		if(n3DtrX != n3DtrU || n3DtrX != n3DtrV){
			cout << "This is odd... track projections of X, U and V are not consistent.\n";
			cout << "n3DtrX: " << n3DtrX << " n3DtrU: " << n3DtrU << " n3DtrV: " << n3DtrV << endl;
			getchar();
		} // end if
		n3Dtr = n3DtrX; n3DtrTot += n3Dtr; n3DtrPerSec[n3Dtr]++;
		// cache the last value of array ntrTot
		int ntrTot_pre[3] = {ntrTot[0], ntrTot[1], ntrTot[2]};
		for(int j = 0; j < ntr; j++) if(type[j]/10 == 11) ntrTot[type[j]%110]++;
		for(int j = 0; j < 3; j++)
			ntrPerSec[j][ntrTot[j]-ntrTot_pre[j]]++; // n tracks per section
		// loop over 3D tracks
		n3Dtr = 0;
		for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section.
			int nFX = 0, nFU = 0, nFV = 0; // fired anode layers in 
			bool BINGO = false;
			double dd = 5.;
			if(fabs(xMiss3D[trkId[0][jj]][0]) < dd && fabs(xMiss3D[trkId[0][jj]][1]) < dd && fabs(xMiss3D[trkId[0][jj]][2]) < dd){ BINGO = true; effTot++; } // a valid 3D track.
			if(!BINGO) continue;
			for(int j = 0; j < 6; j++){ // count effective measurements
				if(nu[trkId[0][jj]][j] != -1){ nFX++; if(BINGO) eff[j/2][0][j%2]++; }
				if(nu[trkId[1][jj]][j] != -1){ nFU++; if(BINGO) eff[j/2][1][j%2]++; }
				if(nu[trkId[2][jj]][j] != -1){ nFV++; if(BINGO) eff[j/2][2][j%2]++; }
			} // end for over j
			const int nF = nFX + nFU + nFV; // number of measured points
			hnF[0]->Fill(nFX); hnF[1]->Fill(nFU); hnF[2]->Fill(nFV); hnF3D->Fill(nF);
			double p[4]; // [0-3]: k1, k2, b1, b2
			p[0] = k[trkId[0][jj]]; // k1
			p[2] = b[trkId[0][jj]]; // b1
			p[1] = TAMath::kUV_Y(phiAvrg, k[trkId[1][jj]], k[trkId[2][jj]]); // k2
			p[3] = TAMath::bUV_Y(phiAvrg, k[trkId[1][jj]], k[trkId[2][jj]], b[trkId[1][jj]], b[trkId[2][jj]]); // b2
			hHAng->Fill(atan(p[0])/DEGREE);
			hVAng->Fill(atan(p[1])/DEGREE);
			hX0->Fill(p[0]*1050.+p[2]);
			if(fabs(atan(p[1])/DEGREE) < 0.1) hY0->Fill(p[1]*4000.+p[3]);
			//// calculate and fill the 3d track coincidence index: x deviations
			double k1t = TAMath::kUV_X(phiAvrg, k[trkId[1][jj]], k[trkId[2][jj]]); // k1
			double b1t = TAMath::bUV_X(phiAvrg, k[trkId[1][jj]], k[trkId[2][jj]], b[trkId[1][jj]], b[trkId[2][jj]]); // b1
//			cout << "k1: " << p[0] << " b1: " << p[2] << endl; // DEBUG
//			cout << "k1t: " << k1t << " b1t: " << b1t << endl; // DEBUG
			for(int k = 0; k < 3; k++){
				double xt = k1t*zDC[k]+b1t, x = p[0]*zDC[k]+p[2];
				h3DMissxPost[k]->Fill(x-xt); h3DMissxTotPost->Fill(x-xt);
				h3DMissxPre[k]->Fill(xMiss3D[trkId[0][jj]][k]);
				h3DMissxTotPre->Fill(xMiss3D[trkId[0][jj]][k]);
//				cout << "x: " << x << " xt: " << xt << endl; // DEBUG
//				cout << "x-xt: " << x-xt << endl; // DEBUG
//				cout << "x-xt _pre: " << xMiss3D[trkId[0][jj]][k] << endl; // DEBUG
//				getchar(); // DEBUG
			}
		} // end for over jj (3D tracks)
		bool has_X = false, has_U = false, has_V = false;
		for(int j = 0; j < ntr; j++){
//			cout << "type[j]: " << type[j] << endl; getchar();
			if(type[j] == 100) has_X = true;
			if(type[j] == 101) has_U = true;
			if(type[j] == 102) has_V = true;
			if(type[j]/10 == 11) continue;
			effTot++;
//			cout << "_____________________effTot: " << effTot << endl;
			int dcType = type[j]%100;
			int STRid = 0;
			for(int l = 0; l < 6; l++){
				if(nu[j][l] != -1){
					// rc: distance of andoe to track
					double tt = t[j][l], dr = chi[j][l];
					double rc = r[j][l]+dr;
//					cout << "ntr: " << ntr; // DEBUG
//					cout << " dcType: " << dcType << " nu[j][l]: " << nu[j][l]; // DEBUG
//					cout << "\ntt: " << tt << " dr: " << dr << " rc: " << rc; // DEBUG
//					getchar(); // DEBUG

					int DCid = l/2;
					hchi->Fill(chi[j][l]);
					hrt01->Fill(tt, rc);
					hrt02[dcType]->Fill(tt, rc);
					hrt03[dcType][DCid]->Fill(tt, rc);
					hdrt01->Fill(tt, dr);
					hdrt02[dcType]->Fill(tt, dr);
					hdrt03[dcType][DCid]->Fill(tt, dr);

					eff[DCid][dcType][l%2]++;
//					cout << "l: " << l << endl;
//					cout << "\n\nDCid: " << DCid << "\tdcType: " << dcType << "\tl%2: " << l%2 << endl;
//					cout << "eff[DCid][dcType][l%2]: " << eff[DCid][dcType][l%2] << endl; getchar();
					
					if(dcType == 0 && l == 2 && nu[j][l] == 40){
						hrt04->Fill(tt, rc);
						hrt04_STR[STRid]->Fill(tt, rc);
						hdrt04->Fill(tt, dr);
						hdrt04_STR[STRid]->Fill(tt, dr);
					} // end if
				} // end of double for and if
			} // end for over l
			if(dcType == 0){ // U
//				cout << "t[j][0]: " << t[j][0] << endl; getchar(); // DEBUG
				for(int l = 0; l < 3; l++){
					if(t[j][2*l] != -9999.) hdt[l]->Fill(t[j][2*l]);
					if(t[j][2*l+1] != -9999.) hdt[l]->Fill(t[j][2*l+1]);
				}
				for(int l = 0; l < 6; l++){
//					if(t[j][l] != -9999.) hr_->Fill(rt->Eval(t[j][l]));
				}
			} // end if
			hChi->Fill(Chi[j]);
			hchi2->Fill(chi2[j]);
//			cout << "chi2: " << chi2[j] << endl; getchar(); // DEBUG
			hSTRid->Fill(STRid);
			hgGOOD[dcType]->Fill(gGOOD[j]);

			// fill the time loop from approximate vertical tracks
			double ang = atan(k[j]);
//			if(dcType == 0) ang -= phiAvrg;
			if(dcType == 0) ang -= phiDC[1] + 0.0 * DEGREE;
			hr_->Fill(ang/DEGREE);
			if(ang > -1. * DEGREE && ang < 2. * DEGREE){
				for(int l = 0; l < 3; l++){ // loop over three DCs
					if(nu[j][2*l] >= 30 && nu[j][2*l] <= 50){
						htt[dcType][l]->Fill(t[j][2*l], t[j][2*l+1]);
						hrr[dcType][l]->Fill(r[j][2*l], r[j][2*l+1]);
					} // end if
				} // end for
			} // end if
		} // end for over j
		if(has_X && has_U && has_V) hasAllCnt++;
		if(has_X) hasXUVCnt[0]++; if(has_U) hasXUVCnt[1]++; if(has_V) hasXUVCnt[2]++;
		cout << "Data section " << i << " processed.\r" << flush;
	} // end for over i

	hntrTot->SetBinContent(2, n); // nsec
	hntrTot->SetBinContent(6, n3DtrTot); // n 3D tracks
	for(int i = 0; i < 3; i++) hntrTot->SetBinContent(i+3, ntrTot[i]); // V-U-X
	for(int i = 0; i < 3; i++) for(int j = 0; j < 10; j++)
		hntrPerSec[i]->SetBinContent(j+2, ntrPerSec[i][j]); // X-U-V
	for(int i = 0; i < 10; i++) hntrPerSec3D->SetBinContent(i+2, n3DtrPerSec[i]);

	// write
	if(!ff->FindObjectAny(dir)) ff->mkdir(dir);
	ff->cd(dir);
	hchi->Write("", TObject::kOverwrite);
	hChi->Write("", TObject::kOverwrite);
	hchi2->Write("", TObject::kOverwrite);
	hrt01->Write("", TObject::kOverwrite);
	hdrt01->Write("", TObject::kOverwrite);
	for(int i = 0; i < 3; i++){
		hrt02[i]->Write("", TObject::kOverwrite);
		hdrt02[i]->Write("", TObject::kOverwrite);
		for(int j = 0; j < 3; j++){
			hrt03[i][j]->Write("", TObject::kOverwrite);
			hdrt03[i][j]->Write("", TObject::kOverwrite);
		} // end for over j
		hntrPerSec[i]->Write("", TObject::kOverwrite);
		hnF[i]->Write("", TObject::kOverwrite);
	} // end for over i
	hrt04->Write("", TObject::kOverwrite);
	hdrt04->Write("", TObject::kOverwrite);
	for(int i = 0; i < 6; i++){
		hrt04_STR[i]->Write("", TObject::kOverwrite);
		hdrt04_STR[i]->Write("", TObject::kOverwrite);
	}
	hntrTot->Write("", TObject::kOverwrite);
	hntrPerSec3D->Write("", TObject::kOverwrite);
	hHAng->Write("", TObject::kOverwrite);
	hVAng->Write("", TObject::kOverwrite);
	hnF3D->Write("", TObject::kOverwrite);
	hSTRid->Write("", TObject::kOverwrite);
	for(int i = 0; i < 3; i++){
		hdt[i]->Write("", TObject::kOverwrite);
		hgGOOD[i]->Write("", TObject::kOverwrite);
		h3DMissxPre[i]->Write("", TObject::kOverwrite);
		h3DMissxPost[i]->Write("", TObject::kOverwrite);
		for(int j = 0; j < 3; j++){
			htt[i][j]->Write("", TObject::kOverwrite);
			hrr[i][j]->Write("", TObject::kOverwrite);
		} // end for over j
	} // end for over i
	h3DMissxTotPre->Write("", TObject::kOverwrite);
	h3DMissxTotPost->Write("", TObject::kOverwrite);
	hX0->Write("", TObject::kOverwrite);
	hY0->Write("", TObject::kOverwrite);
	hr_->Write("", TObject::kOverwrite);
	ff->Close();


	cout << "\n\nhasXCnt: \033[1m" << hasXUVCnt[0];
	cout << "\t\033[0mhasUCnt: \033[1m" << hasXUVCnt[1];
	cout << "\t\033[0mhasVCnt: \033[1m" << hasXUVCnt[2];
	cout << "\n\nhasAllCnt: \033[1m" << hasAllCnt;
	cout << "\033[0m\tTotal 3D track count: \033[1m" << effTot;
	cout << "\033[0m\nCoincidence Success rate: \033[1m";
	cout << double(effTot) / hasXUVCnt[0] << "\033[0m\n";
	cout << "\n__________ software efficiency __________________\n";
	cout << setw(7) << "" << setw(7) << "DC0" << setw(10) << "DC1" << setw(10) << "DC2" << endl;
	for(int i = 0; i < 3; i++) for(int j = 0; j < 2; j++){
		cout << xuv[i] << j + 1 << setw(12);
		cout << "\033[32;1m" << double(eff[0][i][j]) / effTot;
		cout << setw(10) << double(eff[1][i][j]) / effTot;
		cout << setw(10) << double(eff[2][i][j]) / effTot << "\033[0m\n";
	}
//	hrr[0][1]->SaveAs("pdf/0918/03.pdf"); // [x][DC1]
	cout << "\n\n\033[33;1mDONE\033[0m\n\n";
} // end of the main function








