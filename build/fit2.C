void fit2(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update");
	TH1F *haoz = (TH1F*)f->Get("haoz_18O");
	TF1 *total = new TF1("total", "gaus(0)+gaus(3)+gaus(6)+gaus(9)+gaus(12)+pol1(15)", 1.4, 3);
	total->SetNpx(6000);
	double p[17] = {9e+00, 1.87340e+00, 4.87248e-02, 5.23930e+01, 1.97376e+00, 2.40678e-02, 1.81206e+02, 2.12678e+00, 4.53559e-02, 1.11572e+04, 2.21490e+00, 2.10386e-02, 2.99575e+02, 2.33654e+00, 7.19797e-02, 0., 0.};
	total->SetParameters(p);
//	total->SetParLimits(1, 0.82, 0.84);
//	total->SetParLimits(4, 1.06, 1.08);
//	total->SetParLimits(7, 1.2, 1.3);
//	total->SetParLimits(8, 0.081, 0.082);
//	total->SetParLimits(0, 4, 15);
	total->SetParLimits(13, 2.3, 2.45);


	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();
	c->SetLogy();
	haoz->Fit(total, "R");
	total->GetParameters(p);
	TF1 *g0 = new TF1("g0", "gaus", 1.4, 3);
	TF1 *g1 = new TF1("g1", "gaus", 1.4, 3);
	TF1 *g2 = new TF1("g2", "gaus", 1.4, 3);
	TF1 *g3 = new TF1("g3", "gaus", 1.4, 3);
	TF1 *g4 = new TF1("g4", "gaus", 1.4, 3);
	TF1 *gb = new TF1("gb", "pol1", 1.4, 3);
	g0->SetLineStyle(2); g0->SetLineColor(kRed);
	g1->SetLineStyle(2); g1->SetLineColor(kBlue);
	g2->SetLineStyle(2); g2->SetLineColor(kGreen);
	g3->SetLineStyle(1); g3->SetLineColor(kYellow);
	g4->SetFillColor(kPink);
	g4->SetLineStyle(2); g4->SetLineColor(kMagenta);
	gb->SetLineStyle(2); gb->SetLineColor(kBlack);

	g0->SetParameters(p);
	g1->SetParameters(p+3);
	g2->SetParameters(p+6);
	g3->SetParameters(p+9);
	g4->SetParameters(p+12);
	gb->SetParameters(p+15);
	
	g0->Draw("same");
	g1->Draw("same");
	g2->Draw("same");
	g3->Draw("same");
	g4->Draw("same");
	gb->Draw("same");

//	const double cnt = g1->Integral(0, 2)/0.009;
//	cout << "15C count: " << cnt << endl;
	
//	c->SaveAs("");
}




