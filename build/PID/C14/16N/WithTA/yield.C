// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C14TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"N16", "N15", "N14", "C15", "C14", "C13", "C12", "B13", "B12", "B11", "B10", "Be10", "Be9"};
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
	const double x0[] = {2.31, 2.17, 2.03, 2.52, 2.36, 2.19, 2.03, 2.63, 2.43, 2.23, 2.03, 2.54, 2.27}; // aoz
	const double a[]  = {6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76, 6.76}; // aoz/100.
	const double y0[] = {2.31, 2.31, 2.31, 1.77, 1.77, 1.77, 1.77, 1.34, 1.34, 1.34, 1.34, 0.94, 0.94}; // dE1
	const double b[]  = {0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20}; // dE1
	const int nCut = cutgLs.size();
	
	for(int I = 0; I < nCut; I++){
		for(int i = 0; i <= n; i++){
			const double theta = 2.*pi*i/n;
			const double x = x0[I] + a[I]/100.*cos(theta);
			const double y = y0[I] + b[I]*sin(theta);
			TCutG *c = cutgLs[I];
			c->SetPoint(c->GetN(), x, y);
		} // end for over i
	} // end for over I

	for(TCutG *c : cutgLs) c->Write("", TObject::kOverwrite);
}


