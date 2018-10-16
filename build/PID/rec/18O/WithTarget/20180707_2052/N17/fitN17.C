void fitN17(){
	TFile *f = new TFile("~/pionExp2017/build/20180707_2052.dat_0.root", "update");
	TH1F *haoz = (TH1F*)f->Get("hN17");
	TF1 *total = new TF1("total", "gaus(0)", 1.17, 1.52);
	double p[3] = {2.22108e+01, 1.32484e+00, 4.53322e-02};
	total->SetParameters(p);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();
	haoz->Fit(total, "R");

	const double cnt = total->Integral(0., 4.)/0.009;
	cout << "N17 count: " << cnt << "(" << sqrt(cnt) << ")" << endl;

	c->SaveAs("N17.pdf");
}




