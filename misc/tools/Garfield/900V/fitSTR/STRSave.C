void STRSave(){
	TFile *f = new TFile("garfield.root");
	TFile *ff = new TFile("STR/STRMerged.root", "UPDATE");
	ff->mkdir("1000"); ff->cd("/1000");
	TF1 *fSTR[6]; char name[64];
	for(int i = 0; i < 6; i++){
		sprintf(name, "STR_1000V/fSTR_%d", i+9);
		fSTR[i] = (TF1*)f->Get(name);
		sprintf(name, "RT%d", i);
		fSTR[i]->SetName(name);
		fSTR[i]->Write("", TObject::kOverwrite);
	}
	ff->Close();
	f->Close();
}
