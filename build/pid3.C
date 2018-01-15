// pid.C -- pid for March version of the ETF project
const double alpha = 0.61803398874989484820458683436565; // the Golden Cut ratio
const double Pi = 3.14159265358979323846;
const double DEGREE = Pi / 180.; // 0.0174532922
const double e0 = 1.60217733E-19; // unit charge, unit: Coulomb
const double u0 = 1.66053886E-27 ; // atomic mass unit. unit: kg
const double m0 = 39.9525 * u0; // mass of Argon specifically. unit: kg
const double muAr = m0 / 40.; // average nucleon mass of 40Ar
const double c0 = 299.792458; // unit: mm/ns

// transform the original SFE16 chip ids to contiguous serial numbers
int SFE16SerialId(int sfe16Id);
const double ccRDCSFE16TOTOffset[90] = {
	0., 0., 0., 0., -42.9082, 1.25967, 0., 9.92376, 0., 0., 0., 0., 0., 0., -37.3049, -17.5785, 0., 0., 0., 0., // 1 - 20
	0., 0., 0., 0., 25.7735, 48.1937, 0., 0., 0., 0., 0., 0., 0., 0., 26.2808, 19.4945, 30.9059, 36.8541, 0., 0., // 21 - 40
	0., 0., 0., 0., 22.9275, 19.4414, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., // 41 - 60
	0., 0., 0., 0., -23.5692, -29.3544, -19.8717, -20.689, 0., 0., 0., 0., 0., 0., 2.38534, -18.672, 0., 0., 0., 0., // 61 - 80
	0., 0., 0., 0., -12.8844, -3.14078, -17.467, 0., 0., 0. // 81 - 90
}; // checked, no mistakes found.

int main(){

	// extract the calibration graphs
	TFile *fSFE16TOTCali = new TFile("sfecali.root");
	TGraph *gsfe[90];
	char name[64];
	// SFE16 chips are numbered as 0-89
	for(int i = 90; i--;){
		gsfe[i] = nullptr;
		sprintf(name, "gsfe%d", i);
		if(fSFE16TOTCali->FindObjectAny(name))
		gsfe[i] = (TGraph*)fSFE16TOTCali->Get(name);
	} // end for over i


	const char *rootfile = "20161123_2247.dat_0.root"; // "merged_M.root"; // 
//	const char *rootfile = "merged_M.root"; // "20161123_2247.dat.root"; // 
	cout << "rootfile name: " << rootfile << endl; // DEBUG
	TFile *f = new TFile(rootfile, "update"); // merged_M
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeVME_M = (TTree*)f->Get("treeVME_M"); // particle information recorded by VME daq.
	treeTrack->AddFriend(treeVME_M);

	cout << treeTrack->GetEntries() << " entries.\n"; // DEBUG

	const int ntrMax = 200, ntrMax3D = ntrMax / 3;
	int index, sca2, ntr, n3Dtr, type[ntrMax], id[ntrMax], SFE16Id[ntrMax][6], nu[ntrMax][6];
	double Chi[ntrMax], xMiss3D[ntrMax][3], nStripStray[ntrMax]; // track quality
	double TOF[ntrMax], TOF_T1; // TOF: TOF wall time tag; TOF_T1: time tag of scintillator T1
	double t[ntrMax][6], TOT_DC[ntrMax][6], r[ntrMax][6], chi[ntrMax][6]; // Time over threshold for DC channels
	double chit[ntrMax][6]; // chit: drift time error, suggested by fitted tracks
	double XP[3], YP[3], k[ntrMax], b[ntrMax]; // particle position before and after the magnet
	bool isVeto[2], isPileUp, isMWPCMiss[3], isFutile; // isFutile: mark fuitle VME entries.
	double TaX, TaY, thetaIn, phiIn, thetaOut, phiOut, zin, zout, tof, Ain, beta;
	// MWPC position along the beamline [#]+30.-3205.82, Target z position: -1815.92

	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("beta", &beta);
	treeTrack->SetBranchAddress("TOF_T1", &TOF_T1);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("type", type);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("id", id);
	treeTrack->SetBranchAddress("xMiss3D", xMiss3D);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrack->SetBranchAddress("t", t);
	treeTrack->SetBranchAddress("TOT_DC", TOT_DC);
	treeTrack->SetBranchAddress("SFE16Id", SFE16Id); // the SFE16 chip the andoe belongs to
	treeTrack->SetBranchAddress("r", r);
	treeTrack->SetBranchAddress("chi", chi);
	treeTrack->SetBranchAddress("chit", chit);
	treeTrack->SetBranchAddress("k", k);
	treeTrack->SetBranchAddress("b", b);
	treeTrack->SetBranchAddress("Chi", Chi); // sqrt(chi2/nFiredAnodeLayer)
	treeVME_M->SetBranchAddress("sca2", &sca2);
	treeVME_M->SetBranchAddress("TaX", &TaX);
	treeVME_M->SetBranchAddress("TaY", &TaY);
	treeVME_M->SetBranchAddress("thetaIn", &thetaIn);
	treeVME_M->SetBranchAddress("phiIn", &phiIn);
	treeVME_M->SetBranchAddress("thetaOut", &thetaOut);
	treeVME_M->SetBranchAddress("phiOut", &phiOut);
	treeVME_M->SetBranchAddress("zin", &zin);
	treeVME_M->SetBranchAddress("zout", &zout);
	treeVME_M->SetBranchAddress("tof1VME", &tof); // tof over the 25.88m flight
	treeVME_M->SetBranchAddress("Ain", &Ain);
	treeVME_M->SetBranchAddress("XP", XP);
	treeVME_M->SetBranchAddress("YP", YP);
	treeVME_M->SetBranchAddress("isPileUp", &isPileUp);
	treeVME_M->SetBranchAddress("isVeto", isVeto);
	treeVME_M->SetBranchAddress("isMWPCMiss", isMWPCMiss);
	treeVME_M->SetBranchAddress("isFutile", &isFutile);

	// pid using a new formula
	double brho = 6.0933; // brho of the RIBLL2, unit: mm
	TH2F *hTOTvsdEout = new TH2F("hTOTvsdEout", "TOT_DC v.s. dEout;dEout/channel;TOT/channel", 800, 0., 2200., 800, 10., 1000.);
	TH2F *hTOTvsZout = new TH2F("hTOTvsZout", "TOT_DC v.s. Zout;zout/channel;TOT/channel", 800, 5., 20., 800, 10., 1000.);
	TH2F *hsqrtTOTvsZout = new TH2F("hsqrtTOTvsZout", "sqrt(TOT_DC) v.s. Zout;zout/channel;\\sqrt{TOT}/channel", 800, 5., 20., 800, 8., 20.);
	TH2F *hsqrtTOTvsChi = new TH2F("hsqrtTOTvsChi", "sqrt(TOT_DC) v.s. chi;chi/mm;\\sqrt{TOT}/channel", 800, -2.5, 2.5, 800, 5., 35.);
	TH2F *hTOTvsChi = new TH2F("hTOTvsChi", "TOT_DC v.s. chi;chi/mm;TOT/channel", 800, -2.5, 2.5, 800, -100., 1000.);
	TH2F *hTOTavrgvsChi = new TH2F("hTOTavrgvsChi", "TOT_DC v.s. chi;chi/mm;TOT/channel", 800, -2.5, 2.5, 800, -100., 1000.);

	TH2F *hsqrtTOTvsChit = new TH2F("hsqrtTOTvsChit", "sqrt(TOT_DC) v.s. chit;chit/ns;\\sqrt{TOT}/channel", 800, -40., 40., 800, 5., 35.);
	TH2F *hTOTvsChit = new TH2F("hTOTvsChit", "TOT_DC v.s. chit;chit/ns;TOT/channel", 800, -40., 40., 800, -100., 1000.);
	TH2F *hTOTavrgvsChit = new TH2F("hTOTavrgvsChit", "TOT_DC v.s. chit;chit/ns;TOT/channel", 800, -40., 40., 800, -100., 1000.);
	TH1F *hr_avg = new TH1F("hr_avg", "Average Drift Distance Along One Track", 500, -0.1, 7.0);
	TH2F *hSFE16vsTOT = new TH2F("hSFE16vsTOT", "SFE16 v.s. TOT_DC for fixed zout and v; SFE16Id;TOT_DC/channel", 91, -0.5, 90.5, 500, 10., 1000.);
	TH2F *hPIDPXI = new TH2F("hPIDPXI", "PID Uising Only PXI data;TOF/ns;TOT_DC/Channel", 2000, 57., 68., 2000, 10., 1000.);

	const int n = treeTrack->GetEntries();
	int cntMinus = 0; // anomaly count while calculating aozNew
	int cntNormal = 0; // count of normally identified particles
	int trkId[3][ntrMax3D]; // track id [XUV] <-> 3D track id
	bool isEntryGood = true;
	for(int i = 0; i < n; i++){
		treeTrack->GetEntry(i);
		n3Dtr = 0;
		for(int j = 0; j < ntr; j++) if(id[j] != -1){
			if(type[j] == 110) trkId[0][id[j]] = j; // DCArrR-X
			if(type[j] == 111) trkId[1][id[j]] = j; // DCArrR-U
			if(type[j] == 112) trkId[2][id[j]] = j; // DCArrR-V
			n3Dtr++;
		} // end for over j and if
		if(n3Dtr%3 != 0){ cout << "n3Dtr%3 != 0, odd..."; getchar(); }
		n3Dtr /= 3;
		isEntryGood = true;
//		cout << "\nisEntry_good: " << isEntry_good << endl; // DEBUG
//		cout << "n3Dtr: " << n3Dtr << endl; getchar(); // DEBUG
		// initialization
		double TOT_DC_avg[ntrMax3D]{}, tof2[ntrMax3D]{};
		int cntTOT[ntrMax3D]{};
		double r_avg[ntrMax3D]{};
		int cnt_r[ntrMax3D]{};
		for(int jj = 0; jj < n3Dtr; jj++){
			tof2[jj] = TOF[trkId[0][jj]] - TOF_T1; // from T0_T1 to TOF Wall
			for(int j = 0; j < 3; j++){ // loop over XUV
				for(int k = 0; k < 6; k++){ // loop over six layers in the three MWDCs
//					if(r[j][k]>2.5&&r[j][k]<3.5)
//					if(beta>0.64&&beta>0.66)

					////////////////////// Correct TOT ////////////////////////
					int jt = trkId[j][jj];
					if(nu[jt][k] == -1) continue;
					int id = SFE16SerialId(SFE16Id[jt][k]);
					double TOTCor = 0.;
					if(gsfe[id]) TOTCor = gsfe[id]->Eval(TOT_DC[jt][k]);
					if(TOTCor > 200.) TOTCor = 200.;
					if(TOTCor < -200.) TOTCor = -200.;
//					TOT_DC[jt][k] -= TOTCor;
//					if(TOTCor != 0.) // DEBUG
//						cout << "\nTOTCor: " << TOTCor << endl; getchar(); // DEBUG
					///////////////// end of TOT Correction ///////////////////


					if(TOT_DC[jt][k] >= 250.){ // 250.
						TOT_DC_avg[jj] += TOT_DC[jt][k];
						cntTOT[jj]++;
//						cout << "j: " << j << "\tk: " << k << endl; // DEBUG
//						cout << "cntTOT[" << jj << "]: " << cntTOT[jj] << endl; // DEBUG
//						getchar(); // DEBUG
					} // end if
				} // end for over k
			} // end for over j
			TOT_DC_avg[jj] /= cntTOT[jj]; // get the average, temporary, for the following filtering
//			cout << "before filtering, TOT_DC_avg[jj]: " << TOT_DC_avg[jj] << endl; getchar(); // DEBUG

			// remove nasty signals
			double TOT_DC_avg_tmp = TOT_DC_avg[jj]; // a temporary variable
			TOT_DC_avg[jj] = 0.; cntTOT[jj] = 0; // initialize
			r_avg[jj] = 0.; cnt_r[jj] = 0; // initialize
			for(int j = 0; j < 3; j++){ // loop over XUV
				for(int k = 0; k < 6; k++){ // loop over six layers in the three MWDCs
//					if(r[j][k]>2.5&&r[j][k]<3.5)
//					if(beta>0.64&&beta>0.66)
					if(TOT_DC[trkId[j][jj]][k] >= TOT_DC_avg_tmp*0.6){ // 250
						TOT_DC_avg[jj] += TOT_DC[trkId[j][jj]][k];
						cntTOT[jj]++;
//						cout << "j: " << j << "\tk: " << k << endl; // DEBUG
//						cout << "cntTOT[" << jj << "]: " << cntTOT[jj] << endl; // DEBUG
//						getchar(); // DEBUG
					} // end if
					else TOT_DC[trkId[j][jj]][k] = -9999.; // DEBUG
					if(r[trkId[j][jj]][k] >= 0.){
						r_avg[jj] += r[trkId[j][jj]][k];
						cnt_r[jj]++;
					} // end if
				} // end for over k
			} // end for over j
			TOT_DC_avg[jj] /= cntTOT[jj]; // get the average
			r_avg[jj] /= cnt_r[jj]; // get the average
//			cout << "after the filtering, TOT_DC_avg[jj]: " << TOT_DC_avg[jj] << endl; getchar(); // DEBUG




		} // end for over jj
		if(n3Dtr > 1) isEntryGood = false;
		if(fabs(beta) < 0.4) isEntryGood = false;
		if(zout < 0.) isEntryGood = false;
//		cout << "n3Dtr: " << n3Dtr << "\tbeta: " << beta << "\tzout: " << zout << endl;
//		cout << "isEntryGood: " << isEntryGood << endl; getchar(); // DEBUG
		double coeZOut = 0.42, ofstZOut = 2.29; // to extract dEout
		for(int jj = 0; jj < n3Dtr; jj++){
//			if(Ain > 39.4 && Ain < 40.6 && zin > 17.5 && zin < 18.6)
			if(isEntryGood)
			{
				double dEout = (zout - ofstZOut) / coeZOut / pow(beta, 0.8);
				dEout *= dEout;
				hTOTvsdEout->Fill(dEout, TOT_DC_avg[0]);
				hTOTvsZout->Fill(zout, TOT_DC_avg[0]);
				hsqrtTOTvsZout->Fill(zout, sqrt(TOT_DC_avg[0])*beta);
				hr_avg->Fill(r_avg[0]);
				hPIDPXI->Fill(tof2[0], TOT_DC_avg[0]);
				for(int j = 0; j < 3; j++) for(int k = 0; k < 6; k++)
				if(TOT_DC[trkId[j][jj]][k] >= 0.){ // 250.
					int jt = trkId[j][jj]; // track id
					hsqrtTOTvsChi->Fill(chi[jt][k], sqrt(TOT_DC[jt][k]));
					hTOTvsChi->Fill(chi[jt][k], TOT_DC[jt][k]);
					hTOTavrgvsChi->Fill(chi[jt][k], TOT_DC_avg[0]);

					hsqrtTOTvsChit->Fill(chit[jt][k], sqrt(TOT_DC[jt][k]));
					hTOTvsChit->Fill(chit[jt][k], TOT_DC[jt][k]);
					hTOTavrgvsChit->Fill(chit[jt][k], TOT_DC_avg[0]);
					// {10.7, 11.5, 133.7, 136.5}; // Ne23 OK
					// {11.5, 13.3, 133.0, 135.4}; // Na25 OK
					// {12.6, 13.8, 132.1, 135.1}; // Mg27 OK
					// {13.7, 14.8, 134.5, 136.8}; // Al30 OK
					// {14.7, 15.8, 133.8, 136.2}; // Si32 OK

					// {6.6, 7.6, 100.0, 200.0}; // C		0
					// {7.6, 8.6, 100.0, 200.0}; // N		1
					// {8.6, 9.6, 100.0, 200.0}; // O		2
					// {9.6, 10.7, 100.0, 200.0}; // F		3
					// {10.6, 11.6, 100.7, 150.5}; // Ne OK		4
					// {11.4, 12.6, 100.0, 150.4}; // Na OK		5
					// {12.5, 13.9, 100.9, 150.1}; // Mg OK		6
					// {13.8, 14.7, 100.9, 150.1}; // Al OK		7
					// {14.6, 15.9, 100.9, 150.1}; // Si OK		8
					double aa[4] = 
						{12.5, 13.9, 100.9, 150.1}; // Mg OK		6
					if(zout > aa[0] && zout < aa[1]
					&& tof > aa[2] && tof < aa[3]
					)
					{
						// using sfe16 correction
						int id = SFE16SerialId(SFE16Id[jt][k]);
						double tot = TOT_DC[jt][k];
//						tot -= ccRDCSFE16TOTOffset[id];
						hSFE16vsTOT->Fill(id, tot);
					} // end if
				} // end the double loop and if
//				cout << "TOT_DC_avg[0]: " << TOT_DC_avg[0] << "cntTOT[0]: " << cntTOT[0] << endl; // DEBUG
//				cout << "TOT_DC_avg[1]: " << TOT_DC_avg[1] << "cntTOT[1]: " << cntTOT[1] << endl; // DEBUG
//				getchar(); // DEBUG
			} // end if			
		} // end for over jj
		cout << "Processing entry " << i << "\r" << flush;
	} // end for over i

	const char *pid3dir = "PID3";
	if(!f->FindObjectAny(pid3dir)) f->mkdir(pid3dir); f->cd(pid3dir);
	hTOTvsdEout->Write("", TObject::kOverwrite);
	hTOTvsZout->Write("", TObject::kOverwrite);
	hsqrtTOTvsZout->Write("", TObject::kOverwrite);
	hTOTvsChi->Write("", TObject::kOverwrite);
	hsqrtTOTvsChi->Write("", TObject::kOverwrite);
	hTOTavrgvsChi->Write("", TObject::kOverwrite);

	hTOTvsChit->Write("", TObject::kOverwrite);
	hsqrtTOTvsChit->Write("", TObject::kOverwrite);
	hTOTavrgvsChit->Write("", TObject::kOverwrite);
	hr_avg->Write("", TObject::kOverwrite);
	hSFE16vsTOT->Write("", TObject::kOverwrite);
	hPIDPXI->Write("", TObject::kOverwrite);
//	TCanvas *c = new TCanvas("c", "TOT v.s. zout", 1000, 600);
//	c->Divide(2);
//	c->cd(1); hTOTvsZout->Draw("col");
//	c->cd(2); hsqrtTOTvsZout->Draw("col");
	cout << "\n\n\033[33;1mDONE\033[0m\n\n";

	return 0;
} // end of function pid()



// transform the original SFE16 chip ids to contiguous serial numbers
// from 0 on.
int SFE16SerialId(int sfe16Id){
	int serialId = 0;
	int dns[4]{}; // five digits to resolute sfe16id
	dns[0] = sfe16Id >> 6 & 0x7; // DC#, 9 bits
	dns[1] = sfe16Id >> 9 & 0x3; // X-U-V, 2 bits
	dns[2] = sfe16Id >> 11 & 0x7; // cable ID, 3 bits
	dns[3] = sfe16Id >> 14 & 0x1; // L1 or L2, 1 bit
//	cout << "sfe16Id: " << sfe16Id << endl; // DEBUG
//	for(int x : dns) cout << x << " "; // DEBUG
	getchar(); // DEBUG
	for(int i = 0; i < 3; i++){ // loop over DCs
		for(int j = 0; j < 3; j++){ // loop over X-U-V
//			if(i == 1 && j == 2) continue; // DC1 V // DEBUG
			for(int k = 0; k < 5; k++){ // loop over cables
				for(int l = 0; l < 2; l++){ // loop over L#
					if(dns[0] == i && dns[1] == j && dns[2] == k && dns[3] == l){
						return serialId;
					} // end if
					else serialId++;
				} // end for over L#
			} // end for over cables
		} // end for over X-U-V
	} // end for over DCs
	cout << "ERROR: SFE16 Id abnormal..." << endl;
	cout << "SFE16Id: " << sfe16Id << endl;
	getchar();
	return -1;
} // end the bloody function


