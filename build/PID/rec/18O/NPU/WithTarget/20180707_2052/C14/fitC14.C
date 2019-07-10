void fitC14(){
	TFile *f = new TFile("~/pionExp2017/build/20180707_2052.dat_0.root", "update");
	TH1F *haoz = (TH1F*)f->Get("hC14");
	TF1 *total = new TF1("total", "gaus(0)", 0.9, 1.2);
	double p[3] = {1.52722e+01, 1.04083e+00, 5.74699e-02};
	total->SetParameters(p);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();
	haoz->Fit(total, "R");

	const double cnt = total->Integral(0., 4.)/0.009;
	cout << "C14 count: " << cnt << "(" << sqrt(cnt) << ")" << endl;

	c->SaveAs("C14.pdf");
}




