// this is for 13C
void yieldN15(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/20180707_1649.dat_0.root", "update");
	// dE1: 36 bins; aoz: 41 bins
	const double paoz[3] = {3.55987e+01, 2.14844e+00, 2.27927e-02}; // constant, mean, sigma
	const double pdE1[3] = {4.22494e+01, 1.34217e+00, 5.26811e-02}; // constant, mean, sigma
//	const double bdE1[2] = {1.14198e+00, 2.19480e-01}; // dE1 background
	const double x0 = paoz[1], y0 = pdE1[1] + 0.02;
	const double a = 3.*paoz[2], b = 2.7*pdE1[2];
	
	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	TCutG *C0 = new TCutG("N15", n); // for selecting 12C
	C0->SetTitle("N15");
	C0->SetVarX("aoz");
	C0->SetVarY("dE1");
	C0->SetFillStyle(100);
	C0->SetLineColor(2);
	C0->SetLineWidth(2);

	while(C0->GetN()) C0->RemovePoint(0);
	for(int i = 0; i <= n; i++){
		const double theta = 2.*pi*i/n;
		const double x = x0 + a*cos(theta);
		const double y = y0 + b*sin(theta);
		C0->SetPoint(C0->GetN(), x, y);
	} // end for over i
	
	C0->Write("", TObject::kOverwrite);
} // end the main function
