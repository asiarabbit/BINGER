// cutA0.C -- a graphical cut made of two ellipses, for selecting incident 16C from dE0-tof spectrum

void cutA0(){
	TFile *f = new TFile("~/pionExp2017/build/C13NOTA.root", "update");
	const double pi = TMath::Pi();

	// ellipse parameter: (x0, y0), a, b
	const double px[3] = {1., 138.42, 0.3055};
	const double py[3] = {1., 1.2368, 0.0419};
	const double x0 = px[1], y0 = py[1]; // ellipse position
	const double a = 3.*px[2], b = 3.*py[2]; // ellipse axis length: 3 sigma
	
	const int n = 100;
	TCutG *A0 = new TCutG("A0", n);
	A0->SetLineColor(2);
	A0->SetLineWidth(2);
	A0->SetVarX("tof1");
	A0->SetVarY("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016))");
	A0->SetTitle("A0");
	A0->SetFillStyle(100);

	for(int i = 0; i <= n; i++){
		double theta = 2.*pi*i/n;
		double x = x0 + a*cos(theta);
		double y = y0 + b*sin(theta);
		A0->SetPoint(i, x, y);
	} // end for over i
	A0->Write("", TObject::kOverwrite);
	
	// target zone //
	TCutG *B0 = new TCutG("B0", 5);
	B0->SetLineColor(2);
	B0->SetLineWidth(2);
	B0->SetVarX("taHitPos[0][0]");
	B0->SetVarY("taHitPos[0][1]");
	B0->SetTitle("B0");
	B0->SetFillStyle(100);
	const double rTA = 22.; // r = 25 mm
	for(int i = 0; i <= n; i++){
		double theta = 2.*pi*i/n;
		double x = rTA*cos(theta) + 0.;
		double y = rTA*sin(theta) - 1.;
		B0->SetPoint(i, x, y);
	}
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
	B2->SetPoint(2,0.01,-0.008);
	B2->SetPoint(3,0.01,0.008);
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

	TCutG *C0 = new TCutG("C0", 5);
	C0->SetLineColor(2);
	C0->SetLineWidth(2);
	C0->SetVarX("t0_1Pos[0]");
	C0->SetVarY("t0_1Pos[1]");
	C0->SetTitle("C0");
	C0->SetFillStyle(1000);
	C0->SetPoint(0,-27., 21.);
	C0->SetPoint(1,-27.,-25.);
	C0->SetPoint(2, 24.,-25.);
	C0->SetPoint(3, 24., 21.);
	C0->SetPoint(4,-27., 21.);
	C0->Write("", TObject::kOverwrite);
}

