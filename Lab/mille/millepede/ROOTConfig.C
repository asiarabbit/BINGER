// ROOTConfig.C -- included by millepede.C, mainly to make the main program look more slim

	TFile *file = new TFile(ccrootfile.c_str(), "update");
	cout << "Input ROOT file name: \033[1m" << file->GetName() << "\033[0m\n";
	TTree *treeTrack = (TTree*)file->Get("treeTrack");
	if(!file->FindObjectAny("treeTrackT")){
		cout << "treeTrackT not found. RDCT0Cali1 not run yet? Exiting...";
		getchar(); exit(1);
	}
	TTree *treeTrackT1 = (TTree*)file->Get("treeTrackT"); // the rectified data
	treeTrack->AddFriend(treeTrackT1);
	int ntr = 0; // number of track projections in a data section(3*3D track)
	int index; // data section index
	const int ntrMax = 200, ntrMax3D = ntrMax / 3;
	int type[ntrMax]; //  // track type: : 1[LR][XUV] <=> 1[01][012]
	int id[ntrMax]; // tracks with the same track Id and different track type are projections of the same 3-D track.
	int nu[ntrMax][6];
	double t[ntrMax][6], r[ntrMax][6], k[ntrMax], b[ntrMax], TOF[ntrMax];
	int trkId[3][ntrMax3D]; // track id [XUV] <-> 3D track id
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("TOF", TOF);
	treeTrackT1->SetBranchAddress("t", t); // T1
	treeTrackT1->SetBranchAddress("r", r); // T1
	treeTrack->SetBranchAddress("k", k); // start for iterative fit, necessary
	treeTrack->SetBranchAddress("b", b); // start for iterative fit, necessary
	treeTrack->SetBranchAddress("type", type); // track type: 1[LR][XUV]
	treeTrack->SetBranchAddress("id", id); // 3-D track id
	const int n = treeTrack->GetEntries();

	char name[64], title[128];
//	TH1F **htmpd = new TH1F*[NITERLCL];
//	for(int i = 0; i < NITERLCL; i++){ // DEBUG
//		sprintf(name, "htmpd%d", i); // DEBUG
//		sprintf(title, "reduced residuals of iteration %d", i); // DEBUG
//		htmpd[i] = new TH1F(name, title, 500, -10., 10.); // DEBUG
//	} // end for // DEBUG
	TH1F **hpull = new TH1F*[NITERGLB];
	TH2F **hrt = new TH2F*[NITERGLB];
	TTree **treeTrackMil = new TTree*[NITERGLB];
	double k1[ntrMax3D], b1[ntrMax3D], k2[ntrMax3D], b2[ntrMax3D];
	double dr[ntrMax][6], pull_[ntrMax][6], initializer[ntrMax][6];
	double chi2Loc[ntrMax3D], chi2LocOverNdf[ntrMax3D];
	for(int i = 0; i < ntrMax; i++) for(double &x : initializer[i]) x = -9999.;
	int n3DtrX = 0, n3DtrU = 0, n3DtrV = 0, n3Dtr;
	for(int i = 0; i < NITERGLB; i++){
		sprintf(name, "hpull%d", i); sprintf(title, "The Pull Function-iteration_%d", i);
		hpull[i] = new TH1F(name, title, 1005, -7., 7.);
		sprintf(name, "hrt%d", i);
		sprintf(title, "r-\\sigma_r relation-iteration_%d;r/mm;\\sigma_r/mm", i);
		hrt[i] = new TH2F(name, title, 500, -2.05, 9., 500, -1.2, 1.2);

		// store refined results of global iterations
		sprintf(name, "treeTrackMil_%d", i); sprintf(title, "Refined Fit via Millepede_%d", i);
		treeTrackMil[i] = new TTree(name, title);
		treeTrackMil[i]->Branch("index", &index, "index/I");
		treeTrackMil[i]->Branch("ntr", &ntr, "ntr/I");
		treeTrackMil[i]->Branch("n3Dtr", &n3Dtr, "n3Dtr/I");
		treeTrackMil[i]->Branch("nu", nu, "nu[ntr][6]/I");
		treeTrackMil[i]->Branch("t", t, "t[ntr][6]/D");
		treeTrackMil[i]->Branch("r", r, "r[ntr][6]/D");
		treeTrackMil[i]->Branch("dr", dr, "dr[ntr][6]/D"); // residuum
		treeTrackMil[i]->Branch("pull", pull_, "pull[ntr][6]/D"); // residuum
		treeTrackMil[i]->Branch("chi2Loc", chi2Loc, "chi2Loc[n3Dtr]/D"); // k_x
		treeTrackMil[i]->Branch("chi2LocOverNdf", chi2LocOverNdf, "chi2LocOverNdf[n3Dtr]/D");
		treeTrackMil[i]->Branch("type", type, "type[ntr]/I"); // track type: 1[LR][XUV]
		treeTrackMil[i]->Branch("id", id, "id[ntr]/I"); // 3-D track id
		treeTrackMil[i]->Branch("k1", k1, "k1[n3Dtr]/D"); // k_x
		treeTrackMil[i]->Branch("b1", b1, "b1[n3Dtr]/D"); // b_x
		treeTrackMil[i]->Branch("k2", k2, "k2[n3Dtr]/D"); // k_y
		treeTrackMil[i]->Branch("b2", b2, "b2[n3Dtr]/D"); // b_y
	} // end for over i





