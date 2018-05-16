void sigmar(){
    TFile *f = new TFile("SIMDLNF6.root", "update");
    TTree *treeTrack = (TTree*)f->Get("treeTrack");
    TH1F *h = new TH1F("h", "h", 500, -3., 3.);
    const int n = treeTrack->GetEntries();
    const int ntrMax = 10;
    int ntr;
    double chi[ntrMax][6];
    treeTrack->SetBranchAddress("chi", chi);
    treeTrack->SetBranchAddress("ntr", &ntr);

    for(int i = 0; i < n; i++){
        treeTrack->GetEntry(i);
        for(int j = 0; j < ntr; j++){
            for(int k = 0; k < 6; k++)
                h->Fill(chi[j][k]);
        }
    }

    cout << "h->GetEntries(): " << h->GetEntries() << endl;
    TF1 *f1 = new TF1("f1", "gaus", -3., 3.);
    h->Fit(f1, "R");
}
