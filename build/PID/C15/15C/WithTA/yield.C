// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C15TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"C15", "C14", "C13", "C12", "B14", "B13", "B12", "B11", "Be11", "Be10", "Be9"};
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
	const double x0[] = {2.54, 2.37, 2.21, 2.03, 2.85, 2.65, 2.44, 2.24, 2.82, 2.55, 2.29}; // aoz
	const double a[]  = {0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	const double y0[] = {1.73, 1.73, 1.73, 1.73, 1.30, 1.30, 1.30, 1.30, 0.93, 0.93, 0.93}; // dE1
	const double b[]  = {0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17}; // dE1
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


