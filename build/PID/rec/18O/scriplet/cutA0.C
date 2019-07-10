// cutA0.C -- a graphical cut made of two ellipses, for selecting incident 18O from dE0-tof spectrum

void cutA0(){
	TFile *f = new TFile("~/pionExp2017/build/20180707_1649.dat_0.root", "update");
	const double pi = TMath::Pi();

	// ellipse parameter: (x0, y0), a, b
	const double px[6] = {7.42002e+03, 1.34376e+02, 9.91599e-02, 1.68136e+03, 1.34635e+02, 7.39848e-02};
	const double py[3] = {2.13325e+04, 2.11582e+00, 5.00133e-02};
	const double x0[2] = {px[1], px[4]}, y0[2] = {py[1], py[1]}; // ellipse position
	const double a[2] = {3.5*px[2], 3.*px[5]}, b[2] = {3.5*py[2], 3.*py[2]}; // ellipse axis length: 3 sigma
	const double cp[2][2] = {{134.573, 1.97097},{134.574, 2.25948}};
	
	const int n = 100;
	TCutG *A0t[2], *A0;
	A0t[0] = new TCutG("A0t0", n);
	A0t[1] = new TCutG("A0t1", n);
	A0 = new TCutG("A0");
	for(int i = 0; i < 2; i++){
		A0t[i]->SetVarX("tof1");
		A0t[i]->SetVarY("dE0+0.00623053*PDCPos[1][1]");
		A0t[i]->SetTitle("A0t");
		A0t[i]->SetFillStyle(100);
		A0t[i]->SetLineColor(2);
		A0t[i]->SetLineWidth(2);
	} // end for over i
	A0->SetVarX("tof1");
	A0->SetVarY("dE0+0.00623053*PDCPos[1][1]");
	A0->SetTitle("A0");
	A0->SetFillStyle(100);

	int cutA0 = 0;
	for(int j = 0; j < 2; j++){
		for(int i = 0; i < n; i++){
			double theta = 2.*pi*i/n;
			double x = x0[j] + a[j]*cos(theta);
			double y = y0[j] + b[j]*sin(theta);
			A0t[j]->SetPoint(i, x, y);
			if((0 == j && x < cp[0][0]) || (1 == j && x > cp[0][0]))
				A0->SetPoint(A0->GetN(), x, y);
//			cout << "i: " << i << "\tx: " << x << "\ty: " << y << endl; getchar();
		}
	}
	for(int i = 0; i < 2; i++) A0t[i]->Write("", TObject::kOverwrite);
	A0->Write("", TObject::kOverwrite);
	
	TCutG *B0 = new TCutG("B0", 5);
	B0->SetLineColor(2);
	B0->SetLineWidth(2);
	B0->SetVarX("taHitPos[0][0]");
	B0->SetVarY("taHitPos[0][1]");
	B0->SetTitle("B0");
	B0->SetFillStyle(100);
	B0->SetPoint(0,-15.,-15.);
	B0->SetPoint(1,15.,-15.);
	B0->SetPoint(2,15.,15.);
	B0->SetPoint(3,-15.,15.);
	B0->SetPoint(4,-15.,-15.);
	B0->Write("", TObject::kOverwrite);

	TCutG *B1 = new TCutG("B1", 5);
	B1->SetLineColor(2);
	B1->SetLineWidth(2);
	B1->SetVarX("taHitPos[0][0]");
	B1->SetVarY("taHitPos[0][1]");
	B1->SetTitle("B1");
	B1->SetFillStyle(100);
	B1->SetPoint(0,-5.,-5.);
	B1->SetPoint(1,5.,-5.);
	B1->SetPoint(2,5.,5.);
	B1->SetPoint(3,-5.,5.);
	B1->SetPoint(4,-5.,-5.);
	B1->Write("", TObject::kOverwrite);

	TCutG *B2 = new TCutG("B2", 5);
	B2->SetLineColor(2);
	B2->SetLineWidth(2);
	B2->SetVarX("kTa[0][0]");
	B2->SetVarY("kTa[0][1]");
	B2->SetTitle("B2");
	B2->SetFillStyle(100);
	B2->SetPoint(0,-0.01,0.008);
	B2->SetPoint(1,-0.01,-0.008);
	B2->SetPoint(2,0.005,-0.008);
	B2->SetPoint(3,0.005,0.008);
	B2->SetPoint(4,-0.01,0.008);
	B2->Write("", TObject::kOverwrite);

	TCutG *B3 = new TCutG("B3", 5);
	B3->SetLineColor(2);
	B3->SetLineWidth(1);
	B3->SetLineStyle(2);
	B3->SetVarX("TOFWPos[0]");
	B3->SetVarY("DCRPos[2][1]");
	B3->SetTitle("B3");
	B3->SetFillStyle(100);
	B3->SetPoint(0,-625., 300.);
	B3->SetPoint(1,-625.,-300.);
	B3->SetPoint(2, 625.,-300.);
	B3->SetPoint(3, 625., 300.);
	B3->SetPoint(4,-625., 300.);
	B3->Write("", TObject::kOverwrite);

	TCutG *B4 = new TCutG("B4", 5);
	B4->SetLineColor(2);
	B4->SetLineWidth(1);
	B4->SetLineStyle(2);
	B4->SetVarX("PDCPos[3][0]");
	B4->SetVarY("PDCPos[3][1]");
	B4->SetTitle("B4");
	B4->SetFillStyle(100);
	B4->SetPoint(0,-75., 75.);
	B4->SetPoint(1,-75.,-75.);
	B4->SetPoint(2, 75.,-75.);
	B4->SetPoint(3, 75., 75.);
	B4->SetPoint(4,-75., 75.);
	B4->Write("", TObject::kOverwrite);
}















