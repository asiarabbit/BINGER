// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C12TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"N14", "N13", "C13", "C12", "C11", "B11", "B10", "Be7"};
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
	const double x0[] = {2.00, 1.87, 2.15, 2.00, 1.84, 2.20, 2.00, 1.77}; // aoz
	const double a[]  = {0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	const double y0[] = {2.37, 2.37, 1.82, 1.82, 1.82, 1.33, 1.33, 1.06}; // dE1
	const double b[]  = {0.21, 0.21, 0.21, 0.21, 0.21, 0.21, 0.21, 0.21}; // dE1
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


