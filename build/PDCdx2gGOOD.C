void PDCdx2gGOOD(){
	TFile *f = new TFile("C16TA.root", "update");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	
	const int ntrMax = 100;
	int type[ntrMax], gGOOD[ntrMax];
	double dx2[ntrMax], aoz[ntrMax];
	int ntrT; // total number of tracks
	treeTrack->SetBranchAddress("ntrT", &ntrT);
	treeTrack->SetBranchAddress("type", type);
	treeTrack->SetBranchAddress("gGOOD", gGOOD);
	treeTrack->SetBranchAddress("dx2", dx2);
	treeTrack->SetBranchAddress("aoz", aoz);
	
	TH2F *h = new TH2F("hdx2gGOOD", "dx2 v.s. gGOOD for PDC Tracking", 500, -50., 50., 9, -1.5, 7.5);
	
	const int n = treeTrack->GetEntries();
	cout << "Totally " << n << " events." << endl;
	cout << "treeTrack->GetEntries(\"aoz[0] != -9999.\"): ";
	cout << treeTrack->GetEntries("aoz[0] != -9999.") << endl;
	int m = 0;
	for(int i = 0; i < n; i++){
		cout << "Processing event " << i << "\r" << flush;
		aoz[0] = -9999.; // initialization
		treeTrack->GetEntry(i);

		if(-9999. == aoz[0]) continue;
		m++;
		if(aoz[0] != -9999.) for(int j = 0; j < ntrT; j++){
			int cnt = 0;
			if(130 == type[j]){
				h->Fill(dx2[0], gGOOD[j]); break;
				cnt++;
				if(cnt >= 2){
					cout << "More than 1 PDC-D X track in event " << i << endl;
					getchar();
				}
			} // end outer if
		} // end for over j
	} // end for over i
	cout << "m: " << m << endl;
	cout << endl;
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	h->Draw("col");
}
