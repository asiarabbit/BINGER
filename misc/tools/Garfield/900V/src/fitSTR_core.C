double myfunc_i(double *x, double *p); // (-nan, ccx1]
double myfunc_f(double *x, double *p); // (ccx1, nan)
double myfunc(double *x, double *p); // (-nan, nan)

void fitSTR(int strid = STRid, int poln_ = poln){
	if(strid < 9 || strid > 14){
		cout << "STR Id has to be within 9 and 14 (ends included)" << endl;
		exit(1);
	}
	STRid = strid; poln = poln_;
	STRarr = STRid - 9;
	TFile *f = new TFile("garfield.root", "update");

	char name[128], title[128];
	sprintf(name, "anode0/hSTR_%d", STRid);
	TH2F *hSTR = (TH2F*)f->Get(name);
	TH1F *hdca = (TH1F*)f->Get("anode0/hdca");
	TGraph *gSTR = new TGraph(); gSTR->SetMarkerSize(1.5);
	sprintf(name, "gSTR_%d", STRid);
	sprintf(title, "Space Time Relation from Simualtion - STR_%d;t/ns;r/mm", STRid);
	gSTR->SetNameTitle(name, title);
	int gSTR_cnt = 0;
	gSTR->SetMarkerStyle(22); gSTR->SetMarkerColor(4);

	for(int i = 0; i < 100; i++){
		TH1D *hproj = hSTR->ProjectionX("hpro", i*5, i*5+4); // 5 bins
		if(hproj->GetEntries() < 10) continue;
		int offset = 3;
#ifdef FIX
		if(i < 33) offset = 6;
#endif
		double rm = hdca->GetBinCenter(i*5+offset); // rm: mm
		double tm = hproj->GetMean(); // tm: ns
		if(rm > 0. && rm < 5.) tm = hproj->GetBinCenter(hproj->GetMaximumBin());
		if(rm > 5.2) continue;
		gSTR->SetPoint(gSTR_cnt++, tm, rm);
		hproj->~TH1D();
	}

	sprintf(name, "fSTR_%d_i", STRid);
	TF1 *fSTR_i = new TF1(name, myfunc_i, -100., 500., poln+1);
	fSTR_i->SetParLimits(0, -0.6, 0.6);
	fSTR_i->SetParameter(0, 0.);
	sprintf(name, "fSTR_%d_f", STRid);
	TF1 *fSTR_f = new TF1(name, myfunc_f, -100., 500., 2);
	sprintf(name, "fSTR_%d", STRid);
	TF1 *fSTR = new TF1(name, myfunc, -200., 1500., poln+3);
	fSTR->SetTitle(title); fSTR->SetNpx(6000);
	fSTR->SetLineColor(kGreen);
	
	gSTR->Fit(fSTR_i, "Q", "", ccx0[STRarr], ccx1[STRarr]+5.);
	gSTR->Fit(fSTR_i, "Q", "", ccx0[STRarr], ccx1[STRarr]+5.);
	gSTR->Fit(fSTR_f, "Q+", "", ccx1[STRarr], ccx2[STRarr]);
	double p[poln+3];
	fSTR_i->GetParameters(p);
	fSTR_f->GetParameters(p+poln+1);
	fSTR->SetParameters(p);

	TCanvas *c = new TCanvas("c", "STR FINAL", 800, 800);
	c->ToggleEventStatus();
	gSTR->Draw("AP"); fSTR->Draw("same");
	hSTR->SetMarkerColor(1); hSTR->Draw("same");
//	cout << "fSTR->Eval(1.) = " << fSTR->Eval(1.) << endl;
	sprintf(name, "pdf/fSTR_%d_Extended.pdf", STRid);
	c->SaveAs(name);

	if(!f->FindObjectAny(dir)) f->mkdir(dir); f->cd(dir);
	fSTR->Write("", TObject::kOverwrite);
	gSTR->Write("", TObject::kOverwrite);
	cout << "fSTR->Eval(100.) = " << fSTR->Eval(100.) << endl;

	f->Close();
	getchar();
	delete fSTR_i; delete fSTR_f; delete fSTR; delete gSTR; delete f;
	delete c;
}

double myfunc_i(double *x, double *p){
	double result = 0.;
	for(int i = 0; i <= poln; i++)
		result += p[i]*pow(x[0], i);
	return result;
}
double myfunc_f(double *x, double *p){
	return p[0] + x[0] * p[1];
}

double myfunc(double *x, double *p){
	if(x[0] < ccx0[STRarr]) return 0.;
	if(x[0] > ccx1[STRarr]) return 5.;
	double r = myfunc_i(x, p);
	if(r < 0.) r = 0.;
	return r < 5. ? r : 5.;
}





