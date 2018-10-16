void fitN16(){
	TFile *f = new TFile("~/pionExp2017/build/20180707_2052.dat_0.root", "update");
	TH1F *haoz = (TH1F*)f->Get("hN16");
	TF1 *total = new TF1("total", "gaus(0)+gaus(3)+gaus(6)", 0.67, 1.5);
	double p[11] = {6.39282e+00, 8.00963e-01, 6.14364e-02, 1.04607e+01, 1.04220e+00, 5.80843e-02, 1.05834e+01, 1.33706e+00, 6.39897e-02};
	total->SetParameters(p);
//	total->SetParLimits(1, 0.80, 0.84);
//	total->SetParLimits(4, 1.04, 1.08);
//	total->SetParLimits(7, 1.337, 1.3);
//	total->SetParLimits(8, 0.081, 0.082);
//	total->SetParLimits(10, 1.66, 1.76);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();
	haoz->Fit(total, "R");
	total->GetParameters(p);
	TF1 *g0 = new TF1("g0", "gaus", 0.5, 1.);
	TF1 *g1 = new TF1("g1", "gaus", 0.8, 1.4);
	TF1 *g2 = new TF1("g2", "gaus", 1.1, 1.6);
//	TF1 *gb = new TF1("gb", "pol1", 0.5, 1.6);
	g0->SetLineStyle(2); g0->SetLineColor(kRed);
	g1->SetLineStyle(2); g1->SetLineColor(kBlue);
	g2->SetLineStyle(2); g2->SetLineColor(kGreen);
//	gb->SetLineStyle(2); gb->SetLineColor(kBlack);

	g0->SetParameters(p);
	g1->SetParameters(p+3);
	g2->SetParameters(p+6);
//	gb->SetParameters(p+9);
	
	g0->Draw("same");
	g1->Draw("same");
	g2->Draw("same");
//	gb->Draw("same");

	const double cnt = g1->Integral(0., 4.)/0.009;
	cout << "N16 count: " << cnt << "(" << sqrt(cnt) << ")" << endl;
	
	c->SaveAs("N16.pdf");
}




