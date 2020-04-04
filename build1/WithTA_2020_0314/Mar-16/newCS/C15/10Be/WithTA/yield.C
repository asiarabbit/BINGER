// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/C15TA.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	const char *nuclLs[] = {"Be10", "Be9", "Be10R", "Be10U"};
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
	vector<double> x0 = {2.456, 2.220}; // aoz
	vector<double> a  = {0.08, 0.08}; // aoz
	vector<double> y0 = {0.81, 0.81}; // dE1
	vector<double> b  = {0.13, 0.13}; // dE1
	// for incNucl_U and incNucl_R //
	double daoz = fabs(x0[0] - x0[1]), ddE1 = fabs(1.17 - 0.80);
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


