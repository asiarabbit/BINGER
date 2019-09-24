void entries(){
    TFile *f = new TFile("C16TA.root");
    TTree *treeTrack = (TTree*)f->Get("treeTrack");
    cout << "treeTrack->GetEntries(\"aoz[0] != -9999.\"): ";
    cout << treeTrack->GetEntries("aoz[0] != -9999.");
    
    const int n = treeTrack->GetEntries();
    double aoz[100];
    for(double &x : aoz) x = -9999.;
    int ntr;
    int m = 0;
    treeTrack->SetBranchAddress("aoz", aoz);
    treeTrack->SetBranchAddress("ntr", &ntr);
    for(int i = 0; i < n; i++){
        treeTrack->GetEntry(i);
        if(aoz[0] != -9999.) m++;
        for(int j = ntr; j--;) aoz[j] = -9999.;
    }
    cout << "count of aoz != -9999. by code is " << m << endl;
}
