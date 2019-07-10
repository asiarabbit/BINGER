// a clip board for cmd store and exchange

// for i in {1..3}; do ./ass 20180708_0153.dat_0.root $i; done

void tmp(){
	TFile *f = new TFile("~/pionExp2017/build/20180713_0920.dat_0.root", "update"); // 0920 0508
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treePDCPileUp = (TTree*)f->Get("treePDCPileUp");
	TTree *treeDCRPileUp = (TTree*)f->Get("treeDCRPileUp");
	TTree *treePID3D = (TTree*)f->Get("treePID3D");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeTOFWR);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treePDCPileUp);
	treeTrack->AddFriend(treeDCRPileUp);
	treeTrack->AddFriend(treePID3D);
	treeTrack->AddFriend(vme);

	// stored cuts //
	TCutG *A0 = (TCutG*)f->Get("A0");
	TCutG *B0 = (TCutG*)f->Get("B0");
	TCutG *B1 = (TCutG*)f->Get("B1");
	TCutG *B2 = (TCutG*)f->Get("B2");
	TCutG *B3 = (TCutG*)f->Get("B3");

	// PID
	bool pidShow = 0;
	string cut = "1"; // cut
	TCanvas *c = new TCanvas("c", "c", 800, 600);
//	treeTrack->Draw("dE0:tof1>>h(500, 120., 152., 500, -0.5, 4.5)", "dsca11==0;tof1 [ns];dE0;", "col");
//	treeTrack->Draw("dE1:PDCPos[2][1]>>h(500, -50., 50., 500, -0.5, 4.5)", "dsca11==0&&PDCPos[2][1]!=-9999.;PDCPos[2][1] [mm];dE1 arb.;", "");
//	treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):PDCPos[1][1]>>h(500, -50., 50., 500, -0.5, 4.5)", "dsca11==0&&PDCPos[1][1]!=-9999.;PDCPos[1][1] [mm];dE0-corrected arb.;", "col");
	treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):tof1>>h(500, 133., 159., 500, -0.5, 4.5)", "dsca11==0&&PDCPos[1][1]!=-9999.;tof1 [ns];dE0-corrected arb.;", "col");
//	treeTrack->Draw("dE1:dE0+(PDCPos[1][1]*(0.005*dE0-0.0016))>>h(500, -1., 5., 500, -1., 5.)", "1;dE0-corrected arb.;dE1 arb.;", "col");
//	treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):tof1>>h(500, 120., 152., 500, -0.5, 4.5)", "dsca11==0;tof1[ns];dE0-corrected arb.;", "col");

//	const char *cutOpt[] = {"dE1>2.65", "dE1>2.2&&dE1<2.65", "dE1>1.7&&dE1<2.15", "dE1>1.2&&dE1<1.6"}; // [0-3]: [O-N-C-B]
//	treeTrack->Draw("aoz[0]:TOFWPos[0]>>h(500, -700., 700., 500, 1.4, 3.)", "dsca11==0&&A0&&(dx2[0]>-9&&dx2[0]<4)&&dE1>2.2&&dE1<2.65;TOFWPosX [mm];aoz;", "colbox");
//	treeTrack->Draw("taHitPos[0][1]:taHitPos[0][0]>>h(500, -40., 40., 500, -40., 40.)", "dsca11==0&&A0;taHitPosX [mm];taHitPosY [mm];", "col");
//	treeTrack->Draw("poz[0]:tof2[0]>>h(500, 50., 80., 500, 800., 2200.)", "dsca11==0&&A0&&B0;tof2;poz;", "col");
//	treeTrack->Draw("dE1:aoz[0]>>h(500, 1.4, 3., 500, 0., 4.5)", "dsca11==0&&A0&&B0&&(dx2[0]>-9&&dx2[0]<4.);aoz;dE1 arb.;", "colbox");
//	treeTrack->Draw("dE1:aoz[0]>>h(500, 1.4, 3., 500, 0., 4.5)", "dsca11==0&&A0&&B0&&(dx2[0]>-9&&dx2[0]<4)&&sca1drv<0.5;aoz;dE1 arb;", "colbox"); // 



//	treeTrack->Draw("kTa[0][1]:kTa[0][0]>>h(500, -0.1, 0.1, 500, -0.1, 0.1)", "dsca11==0;kTa[0][0];kTa[0][1];", "col");
//	treeTrack->Draw("DCRPos[0][1]:DCRPos[0][0]>>h(500, -700., 700., 500, -500., 500.)", "dsca11==0&&A0&&B0&&B2;DC2PosX [mm];DC2PosY [m];", "col");
//	treeTrack->Draw("TOFWPos[1]:TOFWPos[0]>>h(500, -900., 900., 500, -700., 700.)", "", "col");
//	treeTrack->Draw("DCRPos[2][1]:TOFWPos[0]>>h(500, -700., 700., 500, -500., 500.)", "dsca11==0&&A0&&B0;TOFWPosX [mm];DC2PosY [mm];", "colbox");
//	treeTrack->Draw("kTa[1][1]:kTa[1][0]>>h(500, -0.1, 0.1, 500, -0.1, 0.1)", "dsca11==0&&A0&&B0;kTa[1][0];kTa[1][1];", "col");
//	treeTrack->Draw("kTa[1][0]:kTa[0][0]>>h(500, -0.1, 0.1, 500, -0.1, 0.1)", "dsca11==0;kTa[1][0];kTa[0][0];", "col");

//	treeTrack->Draw("poz[0]:beta2[0]>>h(500, 0.5, 0.66, 500, 1000., 2600.)", "dsca11==0&&A0&&B0&&dE1>1.5;beta2;poz;", "col");
//	treeTrack->Draw("tof2[0]>>h(500, 55., 66.)", "dsca11==0&&A0&&B0&&B2;tof2 [ns];;");

//	treeTrack->Draw("TOF_posY_refine[0]:firedStripId[0]>>h(33, -1.5, 31.5, 500, -10, 1300.)", "dsca11==0&&A0&&B0", "col");
//	treeTrack->Draw("TOF_posY_refine[0]:TOFWPos>>h(500, -700., 700., 500, -10, 1300.)", "dsca11==0&&A0&&B0&&aoz[0]>0", "col");
//	treeTrack->Draw("(TOT_DC_Avrg[0]+TOT_DC_Avrg[1]+TOT_DC_Avrg[2])/3.:dE1>>h(500, -0.1, 4.5, 500, 0., 700.)", "ntrLs[1][0]>=1&&ntrLs[1][1]==1&&ntrLs[1][2]==1&&dsca11==0;dE1 arb.;TOT [ns];", "col");
//	treeTrack->Draw("treePID3D.TOT_DC_Avrg[0]:dE1>>h(500, -0.1, 4.5, 500, 0., 700.)", "n3Dtr==1;dE1 arb.;TOT [ns];", "col");
//	treeTrack->Draw("treePID3D.TOT_DC_Avrg[0]:aoz[0]>>h(500, 1.9, 3.5, 500, 0., 700.)", "n3Dtr==1;aoz;TOT [ns];", "col");
//	treeTrack->Draw("(TOT_Avrg_Ta[1][0]*TOTcnt_Ta[1][0]+TOT_Avrg_Ta[1][1]*TOTcnt_Ta[1][1])/(TOTcnt_Ta[1][0]+TOTcnt_Ta[1][1])+treePID3D.TOT_DC_Avrg[0]:dE1>>h(500, -1., 4.5, 500, 0., 1000.)", "n3Dtr==1", "col");
//	treeTrack->Draw("(TOT_Avrg_Ta[0][0]+TOT_Avrg_Ta[0][1])/2.:dE0+(PDCPos[1][1]*(0.005*dE0-0.0016))>>h(500, -1., 4.5, 500, 0., 500.)", "", "col");
//	treeTrack->Draw("(TOT_Avrg_Ta[1][0]+TOT_Avrg_Ta[1][1])/2.:dE1>>h(500, -1., 4.5, 500, 0., 500.)", "", "col");
//	treeTrack->Draw("(TOT_DC_Avrg[0]+TOT_DC_Avrg[1]+TOT_DC_Avrg[2])/3.:dE1>>h(500, -1., 4.5, 500, 0., 500.)", "ntrLs[1][0]==1&&ntrLs[1][1]==1&&ntrLs[1][2]==1&&A0", "col");
//	treeTrack->Draw("(TOT_DC_Avrg[0]+TOT_DC_Avrg[1]+TOT_DC_Avrg[2])/300.:aoz[0]>>h(500, 1.4, 4., 500, 0., 6.)", "ntrLs[1][0]==1&&ntrLs[1][1]==1&&ntrLs[1][2]==1&&A0", "col");

	cut = "dsca11 == 1";
	cut += " && tRef_vme0ul[0] != -9999. && tRef_vme0ul[1] != -9999.";
	cut += " && tRef_vme0dl[0] != -9999. && tRef_vme0dl[1] != -9999.";
	cut += " && tRef_vme1ul[0] != -9999. && tRef_vme1ul[1] != -9999.";
	cut += " && tRef_vme1dl[0] != -9999. && tRef_vme1dl[1] != -9999.";
	cut += ";tl[1]-tl[0] [ns];dE1 arb.;";
//	treeTrack->Draw("(tRef_vme0ul[1]-tRef_vme0ul[0]):dE1>>h(500, -0.1, 4.5, 500, -2000., 8000.)", "tRef_vme0ul[1]!=-9999", "col");
//	treeTrack->Draw("(tRefLT_U[1]-tRefLT_U[0]):dE1>>h(500, -0.1, 4.5, 500, -2000., 8000.)", "tRefLT_U[1]!=-9999", "col");
//	cout << "cut: " << cut << endl; // DEBUG
//	treeTrack->Draw("tRefLT_U[1]-tRefLT_U[0]:dE0>>h(500, -1., 4.5, 500, -500., 1200.)", "dsca11==1", "col");

	TH1 *h = (TH1*)f->Get("h");
	h->GetYaxis()->SetTitleOffset(1.2);
	h->SetTitle((string(h->GetTitle())+"}").c_str());


//	A0->Draw("same");
//	B0->Draw("same");
//	B1->Draw("same");
//	B2->Draw("same");
//	B3->Draw("same");

	vector<TCutG *> cutgLs;
	cutgLs.push_back((TCutG*)f->Get("O16"));
	cutgLs.push_back((TCutG*)f->Get("N15"));
	cutgLs.push_back((TCutG*)f->Get("N14"));
	cutgLs.push_back((TCutG*)f->Get("N13"));
	cutgLs.push_back((TCutG*)f->Get("C13"));
	cutgLs.push_back((TCutG*)f->Get("C12"));
	cutgLs.push_back((TCutG*)f->Get("C11"));
	cutgLs.push_back((TCutG*)f->Get("C10"));
	cutgLs.push_back((TCutG*)f->Get("B11"));
	cutgLs.push_back((TCutG*)f->Get("B10"));
	cutgLs.push_back((TCutG*)f->Get("Be8"));
	cutgLs.push_back((TCutG*)f->Get("Li6"));

	if(pidShow) for(TCutG *c : cutgLs) c->Draw("same");

	c->SetGrid();
	c->ToggleToolBar();
	c->ToggleEventStatus();
//	c->SaveAs("4.pdf");
//	h0->Write("", TObject::kOverwrite);
//	treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):PDCPos[1][1]>>(500, -30, 30, 500, -0.2, 4.5)", "", "col")
//	treeTrack->Draw("dE1:dE0>>(500, -1., 5., 500, -1., 5.)", "", "col");
//	treeTrack->Draw("aoz[0]>>(500, 1.4, 2.7)", "")
	// overall PID
//	treeTrack->Draw("dE1:aoz[0]>>(500, 1.9, 3.5, 500, 0., 4.5)", "", "col")
//	treeTrack->Draw("sqrt(dE1)*pow(beta2, 0.8)*1.4:aoz[0]>>(500, 1.9, 3.5, 500, 0., 2.2)", "dsca11==0", "col");
//	treeTrack->Draw("dE1:aoz[0]>>(500, 1.9, 3.5, 500, 0., 4.5)", "dsca11<=0&&dsca10<=1", "col")
//	treeTrack->Draw("dE1:aoz[0]>>(500, 1.9, 3.5, 500, 0., 4.5)", "!(dsca11<=0&&dsca10<=1)", "col")
//	treeTrack->Draw("adc[22]:adc[23]>>(500, 0, 4500, 500, 0, 4500)", "dsca11==0&&A2", "col")
//	treeTrack->Draw("adc[24]:adc[17]>>(500, 0, 4500, 500, 0, 4500)", "dsca11==0&&A2", "col")
//	treeTrack->Draw("dE1:aoz[0]>>(500, 1.9, 3.55, 500, -0.03, 4.5)", "!(dsca11==0&&dsca10==1&&tRef_UV_NL==1&&tRef_DV_NL==1&&CUTG&&multi_pre==1)&&CUTG", "col")

	// track count
//	double(treeTrack->GetEntries("ntr==0&&A04"))/treeTrack->GetEntries("A04")
//	double(treeTrack->GetEntries("ntrLs[3][0]==0&&A04"))/treeTrack->GetEntries("A04")
//	treeTrack->Draw("dE1:dE0>>(500, -1., 5., 500, -1., 5.)", "multi_PDC[0][0][0][0]==1&&multi_PDC[0][0][0][1]==1&&multi_PDC[0][0][1][0]==1&&multi_PDC[0][0][1][1]==1&&multi_PDC[0][1][0][0]==1&&multi_PDC[0][1][0][1]==1&&multi_PDC[0][1][1][0]==1&&multi_PDC[0][1][1][1]==1&&dsca11==0&&dsca10==1&&multi_PDC[1][0][0][0]==1&&multi_PDC[1][0][0][1]==1&&multi_PDC[1][0][1][0]==1&&multi_PDC[1][0][1][1]==1&&multi_PDC[1][1][0][0]==1&&multi_PDC[1][1][0][1]==1&&multi_PDC[1][1][1][0]==1&&multi_PDC[1][1][1][1]==1&&multi_DC[1][0][0][0]==1&&multi_DC[1][0][0][1]==1&&multi_DC[1][0][1][0]==1&&multi_DC[1][0][1][1]==1&&multi_DC[1][0][2][0]==1&&multi_DC[1][0][2][1]==1&&multi_DC[1][1][0][0]==1&&multi_DC[1][1][0][1]==1&&multi_DC[1][1][1][0]==1&&multi_DC[1][1][1][1]==1&&multi_DC[1][1][2][0]==1&&multi_DC[1][1][2][1]==1&&multi_DC[1][2][0][0]==1&&multi_DC[1][2][0][1]==1&&multi_DC[1][2][1][0]==1&&multi_DC[1][2][1][1]==1&&multi_DC[1][2][2][0]==1&&multi_DC[1][2][2][1]==1", "col")


	const double mainBeam = treeTrack->GetEntries("dsca11==0&&A0&&B0&&(dx2[0]>-9&&dx2[0]<4.)");
	const double bullsEye = treeTrack->GetEntries("dsca11==0&&A0&&B0&&(dx2[0]>-9&&dx2[0]<4.)&&O16");
	cout << "tof2_PU eff: " << bullsEye << "/" << mainBeam << " = " << bullsEye/mainBeam << endl;
	return;

	const double n0 = treeTrack->GetEntries("dsca11==0&&A0&&B0"); // &&sca1drv<0.5
	cout << "n0: " << n0 << endl;
	const char *CUTOPT[4] = {"dsca11==0&&A0&&B0&&dx2[0]!=-9999.", "dsca11==0&&A0&&B0&&(dx2[0]>-6.&&dx2[0]<2.)", "dsca11==0&&A0&&B0&&(dx2[0]>-9.&&dx2[0]<4.)", "dsca11==0&&A0&&B0&&(dx2[0]>-9.&&dx2[0]<4.)&&sca1drv<0.5"};
	string cut0 = CUTOPT[2]; cut0 += "&&";
	double yield[10]{};
	for(TCutG *c : cutgLs){
		const double tmp = treeTrack->GetEntries((cut0 + c->GetName()).c_str());
		cout << c->GetName() << ": " << tmp << endl;
		if(!strcmp(c->GetName(), "N15")) yield[0] = tmp;
		if(!strcmp(c->GetName(), "N14")) yield[1] = tmp;
		if(!strcmp(c->GetName(), "N13")) yield[2] = tmp;
		if(!strcmp(c->GetName(), "C13")) yield[3] = tmp;
		if(!strcmp(c->GetName(), "C12")) yield[4] = tmp;
		if(!strcmp(c->GetName(), "C11")) yield[5] = tmp;
		if(!strcmp(c->GetName(), "B11")) yield[6] = tmp;
		if(!strcmp(c->GetName(), "B10")) yield[7] = tmp;
	}
	// 					   {  N15,   N14,   N13,   C13,   C12,   C11,  B11,    B10};
	const double e_g[]   = {   1.,    1.,  0.97,  0.95,    1.,   0.9, 0.95,     1.}; // for emptyTA 0920
	const double e_pdc[] = {94.22, 94.22, 94.22, 93.75, 93.75, 93.75, 86.00, 86.00};
	const double e_dcr[] = {95.61, 95.61, 95.61, 95.61, 95.61, 95.61, 95.61, 95.61};
//	const double e_dx2[] = {100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0}; // (-inf, inf)
//	const double e_dx2[] = {44.29, 44.29, 44.29, 54.76, 54.76, 54.76, 60.00, 60.00}; // (-6, 2)
	const double e_dx2[] = {63.33, 63.33, 63.33, 71.43, 71.43, 71.43, 72.50, 72.50}; // (-9, 4)
	const double e_PU_tof2 = 0.8816;

//	const double e_g[]   = {   1.,    1.,  0.97,  0.95,    1.,   0.9, 0.95,     1.}; // for TA 0508
//	const double e_pdc[] = {94.25, 94.25, 94.25, 93.67, 93.67, 93.67, 91.06, 91.06};
//	const double e_dcr[] = {95.84, 95.84, 95.84, 95.84, 95.84, 95.84, 95.84, 95.84};
//	const double e_dx2[] = {100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0}; // (-inf, inf)
//	const double e_dx2[] = {48.59, 48.05, 44.29, 54.76, 54.76, 54.76, 47.58, 47.58}; // (-6, 2)
//	const double e_dx2[] = {67.92, 67.92, 67.92, 68.78, 68.78, 68.78, 66.66, 66.66}; // (-9, 4)
//	const double e_PU_tof2 = 0.8627;

	ostringstream cmd("");
	const int nNcl = sizeof(e_g)/sizeof(double);
	for(int i = 0; i < nNcl; i++){
		const double e_e = e_pdc[i] / 100 * e_dcr[i] / 100;
		cmd.str(""); cmd << "./cs " << n0 << " " << e_e * e_dx2[i] * e_PU_tof2 << " 0. " << yield[i] / e_g[i];
		cout << "\033[1m" << i << cmd.str().c_str() << "\033[0m" << endl;
		system(cmd.str().c_str());
	} // end for over i
} // end of the main function




