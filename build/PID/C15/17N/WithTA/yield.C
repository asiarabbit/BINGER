// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C15TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"N17", "N16", "N15", "N14", "C16", "C15", "C14", "C13", "C12", "B13", "B12", "B11", "B10", "Be10", "Be9"};
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
	const double x0[] = {2.48, 2.33, 2.19, 2.04, 2.72, 2.55, 2.38, 2.21, 2.04, 2.65, 2.45, 2.25, 2.05, 2.55, 2.32}; // aoz
	const double a[]  = {0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	const double y0[] = {2.27, 2.27, 2.27, 2.27, 1.73, 1.73, 1.73, 1.73, 1.73, 1.30, 1.30, 1.30, 1.30, 0.93, 0.93}; // dE1
	const double b[]  = {0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22}; // dE1
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


