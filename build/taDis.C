void taDis(){

	TFile *f = new TFile("tmp.root", "RECREATE");

	TH2F *hxk = new TH2F("hxk", "hxk;x;k", 500, -60., 60., 500, -0.02, 0.02);
	TH2F *hkb = new TH2F("hkb", "hkb;b;k", 500, -200., 200., 500, -0.02, 0.02);
	
	const double zTa = -1806.43;
	const int n = 10000;
	for(int i = 0; i < n; i++){
		const double k = (1. - 2.*i/n)*0.006;
		for(int j = 0; j < n; j++){
			const double b = (1. - 2.*j/n)*200;
			double x = k * zTa + b;
//			cout << "k: " << k << "\tb: " << b << endl; // DEBUG
//			cout << "x: " << x << endl; getchar(); // DEBUG
			if(fabs(x) < 25.){
				hxk->Fill(x, k);
				hkb->Fill(b, k);
			} // end if
		} // end for over j
	} // end for over i

	hxk->Write("", TObject::kOverwrite);
	hkb->Write("", TObject::kOverwrite);
} // end of the main function
