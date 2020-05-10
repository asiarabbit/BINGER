// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build1/C16TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"C16", "C15", "C14", "C13", "B15", "B14", "B13", "B12", "B11", "Be12", "Be11", "Be10", "Be9", "C16R", "C16U"};
	for(const char *c : nuclLs) cutgLs.push_back(new TCutG(c, n));

	for(TCutG *c : cutgLs){
		c->SetTitle("NUCLIDE CUT");
		c->SetVarX("aoz[0]");
		c->SetVarY("dE1");
		c->SetFillStyle(100);
		c->SetLineColor(2);
		c->SetLineWidth(2);
		while(c->GetN()) c->RemovePoint(0);
	} // end for over TCutG objects


	// a: aoz - half length of the ellipse's major axis
	// b: dE1 - half length of the ellipse's minor axis
	vector<double> x0 = {2.67, 2.50, 2.33, 2.17, 3.00, 2.80, 2.60, 2.40, 2.20, 3.00, 2.76, 2.51, 2.28}; // aoz
	vector<double> a  = {0.07, 0.06, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	vector<double> y0 = {1.73, 1.73, 1.73, 1.73, 1.27, 1.27, 1.27, 1.27, 1.27, 0.93, 0.93, 0.93, 0.93}; // dE1
	vector<double> b  = {0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17}; // dE1
	// for C16U and C16R //
	double daoz = fabs(x0[0] - x0[1]), ddE1 = fabs(y0[0] - 1.27);
	// C16R
	x0.push_back(x0[0] + daoz); y0.push_back(y0[0]);
	a.push_back(a[0]); b.push_back(b[0]);
	// C16U
	x0.push_back(x0[0]); y0.push_back(y0[0] + ddE1);
	a.push_back(a[0]); b.push_back(b[0]);

	
	const int nCut = cutgLs.size();
	
	for(int I = 0; I < nCut; I++){
		for(int i = 0; i <= n; i++){
			const double theta = 2.*pi*i/n;
			const double x = x0[I] + a[I]*cos(theta);
			const double y = y0[I] + b[I]*sin(theta);
			TCutG *c = cutgLs[I];
			c->SetPoint(c->GetN(), x, y);
		} // end for over i
	} // end for over I

	for(TCutG *c : cutgLs) c->Write("", TObject::kOverwrite);
}


