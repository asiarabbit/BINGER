// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C15NOTA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"N17", "N16", "N15", "N14", "C16", "C15", "C14", "C13", "C12", "B13", "B12", "B11", "B10", "Be10", "Be9", "N17R", "N17U"};
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
	vector<double> x0 = {2.394, 2.257, 2.117, 1.987, 2.622, 2.462, 2.304, 2.142, 1.982, 2.572, 2.366, 2.177, 1.998, 2.476, 2.232}; // aoz
	vector<double> a  = {0.068, 0.068, 0.068, 0.068, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}; // aoz
	vector<double> y0 = {2.22, 2.22, 2.22, 2.22, 1.70, 1.69, 1.69, 1.68, 1.68, 1.27, 1.27, 1.27, 1.27, 0.90, 0.90}; // dE1
	vector<double> b  = {0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22}; // dE1
	// for incNucl_U and incNucl_R //
	double daoz = fabs(x0[0] - x0[1]), ddE1 = fabs(y0[0] - 1.73);
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


