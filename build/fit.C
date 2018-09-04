void fit(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update");
	TH1F *h0x = (TH1F*)f->Get("h0x");
	TF1 *g0 = new TF1("g0", "gaus", 133.8, 134.5);
	TF1 *g1 = new TF1("g1", "gaus", 134.6, 135.2);
	TF1 *total = new TF1("total", "gaus(0)+gaus(3)", 133.8, 135.2);
//	g0->SetParameters(7.44443e+03, 1.34379e+02, 1.00603e-01);
//	g1->SetParameters(2.09926e+03, 1.34607e+02, 8.35765e-02);
	h0x->Fit(g0, "R");
	h0x->Fit(g1, "R+");

	double par[6];
	const double px[6] = {7.42002e+03, 1.34376e+02, 9.91599e-02, 1.68136e+03, 1.34635e+02, 7.39848e-02};
	const double py[3] = {2.13325e+04, 2.11582e+00, 5.00133e-02};
	g0->GetParameters(&par[0]);
	g0->GetParameters(&par[3]);
	total->SetParameters(par);
//	total->SetParLimits(0, 7400, 7500);
//	total->SetParLimits(1, 134.348, 134.405);
//	total->SetParLimits(2, 0.099, 0.101);
//	total->SetParLimits(4, 134.542, 134.65);
//	total->SetParLimits(5, 0.08, 0.088);

	h0x->Fit(total, "R");
}
