// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C16NOTA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"B13", "B12", "B11", "Be12", "Be11", "Be10", "Be9", "Li7"};
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
	const double x0[] = {2.70, 2.49, 2.28, 3.10, 2.86, 2.60, 2.36, 2.43}; // aoz
	const double a[]  = {0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	const double y0[] = {1.19, 1.19, 1.19, 0.83, 0.83, 0.83, 0.83, 0.58}; // dE1
	const double b[]  = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15}; // dE1
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


