// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/O18TA_L1.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"O18", "O17", "O16", "O15", "N17", "N16", "N15", "N14", "N13", "C16", "C15", "C14", "C13", "C12", "C11", "B13", "B12", "B11", "B10", "Be7", "Be9", "Be10", "objL", "objR", "objD"};
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
	// b: dE1 - half length of the ellipse's minor axis
	const double x0[] = {2.23, 2.105, 1.98, 1.86, 2.41, 2.27, 2.13, 1.99, 1.84, 2.66, 2.49, 2.32, 2.15, 1.99, 1.82, 2.56, 2.39, 2.18, 1.99, 1.75, 2.24, 2.47, 2.03, 2.43, 2.23}; // aoz
	const double a[]  = {5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73, 5.73}; // aoz/100
	const double y0[] = {1.70, 1.70, 1.71, 1.72, 1.34, 1.34, 1.34, 1.34, 1.34, 1.06, 1.06, 1.06, 1.06, 1.06, 1.06, 0.81, 0.81, 0.81, 0.81, 0.63, 0.59, 0.58, 1.70, 1.70, 2.06}; // dE1
	const double b[]  = {0.13, 0.12, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13, 0.13}; // dE1
	const int nCut = cutgLs.size();
	
	for(int I = 0; I < nCut; I++){
		for(int i = 0; i <= n; i++){
			const double theta = 2.*pi*i/n;
			const double x = x0[I] + a[I]*cos(theta)/100.;
			const double y = y0[I] + b[I]*sin(theta);
			TCutG *c = cutgLs[I];
			c->SetPoint(c->GetN(), x, y);
		} // end for over i
	} // end for over I

	for(TCutG *c : cutgLs) c->Write("", TObject::kOverwrite);
}


