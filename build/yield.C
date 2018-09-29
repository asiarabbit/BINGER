// yield.C -- create the TCutG objects to mark different nuclides
void yield(){
	const double pi = TMath::Pi();

	TFile *f = new TFile("~/pionExp2017/build/20180713_0920.dat_0.root", "update");
	vector<TCutG *> cutgLs;

	const int n = 100; // number of points for TCutG in the dE1-aoz spectrum
	cutgLs.push_back(new TCutG("O16", n));
	cutgLs.push_back(new TCutG("N15", n));
	cutgLs.push_back(new TCutG("N14", n));
	cutgLs.push_back(new TCutG("N13", n));
	cutgLs.push_back(new TCutG("C13", n));
	cutgLs.push_back(new TCutG("C12", n));
	cutgLs.push_back(new TCutG("C11", n));
	cutgLs.push_back(new TCutG("C10", n));
	cutgLs.push_back(new TCutG("B11", n));
	cutgLs.push_back(new TCutG("B10", n));
	cutgLs.push_back(new TCutG("Be8", n));
	cutgLs.push_back(new TCutG("Li6", n));

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
	const double x0[] = {2.02, 2.16, 2.02, 1.86, 2.19, 2.02, 1.86, 1.7 , 2.03, 2.22, 1.784, 2.036}; // aoz
	const double a[]  = {0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06 , 0.06 }; // aoz
	const double y0[] = {3.10, 2.43, 2.43, 2.43, 1.91, 1.91, 1.91, 1.91, 1.41, 1.41, 1.10 , 0.81 }; // dE1
	const double b[]  = {0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20 , 0.20 }; // dE1
	const int nCut = cutgLs.size();
	
	for(int I = 0; I < nCut; I++){
		for(int i = 0; i <= n; i++){
			const double theta = 2.*pi*i/n;
			const double x = x0[I] + a[I]*cos(theta);
			const double y = y0[I] + b[I]*sin(theta) - 0.12;
			TCutG *c = cutgLs[I];
			c->SetPoint(c->GetN(), x, y);
		} // end for over i
	} // end for over I

	for(TCutG *c : cutgLs) c->Write("", TObject::kOverwrite);
}


