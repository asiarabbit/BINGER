// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C12NOTA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum

	// objL-R-U: left, right and down side ellipse nuclide zone to calculate overlap rate
	const char *nuclLs[] = {"O16", "O15", "N15", "N14", "N13", "C14", "C13", "C12", "C11", "C10", "B11", "B10", "Be7", "Li6", "O16R", "O16U"};
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
	for(int i = 2; i--;) cutgLs.end()[-i-1]->SetLineColor(3); // set the last two green

	// a: aoz - half length of the ellipse's major axis
	// b: dE1 - half length of the ellipse's minor axis
	vector<double> x0 = {1.935, 1.815, 2.070, 1.938, 1.803, 2.252, 2.096, 1.935, 1.780, 1.611, 2.129, 1.938, 1.714, 1.935}; // aoz
	vector<double> a  = {0.07, 0.06, 0.068, 0.068, 0.068, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	vector<double> y0 = {3.022, 3.022, 2.412, 2.412, 2.412, 1.90, 1.89, 1.90, 1.91, 1.91, 1.386, 1.386, 1.102, 0.85}; // dE1
	vector<double> b  = {0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23}; // dE1
	// for incNucl_U and incNucl_R //
	double daoz = fabs(x0[0] - x0[1] + 0.02), ddE1 = fabs(3.10 - 2.45); // F - O
	// incNucl_R
	x0.push_back(x0[0] + daoz); y0.push_back(y0[0]);
	a.push_back(a[0]); b.push_back(b[0]);
	// incNucl_U
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


