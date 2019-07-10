// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C12TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum

	// objL-R-U: left, right and down side ellipse nuclide zone to calculate overlap rate
	const char *nuclLs[] = {"O16", "N15", "N14", "N13", "C14", "C13", "C12", "C11", "C10", "B11", "B10", "Be7", "Li6", "objL", "objR", "objD"};
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
	for(int i = 3; i--;) cutgLs.end()[-i-1]->SetLineColor(3); // set the last three green

	// a: aoz - half length of the ellipse's major axis
	// b: dE1 - half length of the ellipse's minor axis											   * L     R     D *
	const double x0[] = {2.00, 2.14, 2.00, 1.86, 2.33, 2.17, 2.00, 1.84, 1.67, 2.20, 2.00, 1.76, 2.00,   2.20, 1.80, 2.00}; // aoz
	const double a[]  = {0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07,   0.07, 0.07, 0.07}; // aoz
	const double y0[] = {3.10, 2.45, 2.45, 2.45, 1.87, 1.90, 1.91, 1.91, 1.91, 1.44, 1.44, 1.01, 0.85,   3.10, 3.10, 3.53}; // dE1
	const double b[]  = {0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23, 0.23,   0.23, 0.23, 0.23}; // dE1
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


