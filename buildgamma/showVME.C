void showVME(){
	TFile *f = new TFile("20200410_0310.dat_0.root", "UPDATE");
	TTree *vme = (TTree*)f->Get("vme");
	int mtdc1[128][10];
	
	vme->SetBranchAddress("mtdc1", mtdc1);
	
	const int n = vme->GetEntries();
	TH1F *hIDmtdc1 = new TH1F("hIDmtdc1", "Valid ID distribution of mtdc1", 132, -1.5, 130.5);
	for(int i = 0; i < n; i++){
		vme->GetEntry(i);
		for(int j = 0; j < 128; j++){
			if(mtdc1[j][0] != -9999){
				hIDmtdc1->Fill(j);
//				cout << "valid id: " << j << " ----" << endl; // DEBUG
//				cout << "mtdc1[" << j << "][0]: " << mtdc1[j][0] << endl; // DEBUG
//				getchar(); // DEBUG
			}
		} // end for over j
	} // end for over i
	
	hIDmtdc1->Write("", TObject::kOverwrite);
}
