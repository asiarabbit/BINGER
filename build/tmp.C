void tmp(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update"); // 2052 1848 1649
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treePDCPileUp = (TTree*)f->Get("treePDCPileUp");
	TTree *treeDCPileUp = (TTree*)f->Get("treeDCPileUp");
	TTree *treePID3D = (TTree*)f->Get("treePID3D");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeTOFWR);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treePDCPileUp);
	treeTrack->AddFriend(treeDCPileUp);
	treeTrack->AddFriend(treePID3D);
	treeTrack->AddFriend(vme);

	// stored cuts //
	TCutG *A0t0 = (TCutG*)f->Get("A0t0");
	TCutG *A0t1 = (TCutG*)f->Get("A0t1");
	TCutG *B0 = (TCutG*)f->Get("B0");
	TCutG *B1 = (TCutG*)f->Get("B1");
	TCutG *B2 = (TCutG*)f->Get("B2");
	TCutG *B3 = (TCutG*)f->Get("B3");

	// PID
	string cut = "1"; // cut
	TCanvas *c = new TCanvas("c", "c", 800, 600);
//	treeTrack->Draw("dE0:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0;tof1 [ns];dE0-corrected arb.;", "col");
//	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0", "col");
//	treeTrack->Draw("dE1:dE0+0.00623053*PDCPos[1][1]>>h(500, -1., 5., 500, -1., 5.)", "dsca11==0&&aoz>0;dE0-corrected arb.;dE1 arb.;", "col");
//	treeTrack->Draw("aoz:TOFWPos>>h(500, -700., 700., 500, 1.4, 3.)", "CUTG", "colbox");
//	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0", "col");
//	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0;tof1[ns];dE0-corrected arb.;", "col");

//	treeTrack->Draw("taHitPos[0][1]:taHitPos[0][0]>>h(500, -40., 40., 500, -40., 40.)", "dsca11==0;taHitPosX [mm];taHitPosY [mm];", "col");
//	treeTrack->Draw("kTa[0][1]:kTa[0][0]>>h(500, -0.1, 0.1, 500, -0.1, 0.1)", "dsca11==0;kTa[0][0];kTa[0][1];", "col");
//	treeTrack->Draw("DCRPos[2][1]:DCRPos[2][0]>>h(500, -700., 700., 500, -500., 500.)", "dsca11==0&&(A0t0||A0t1)&&B0&&B2;DC2PosX [mm];DC2PosY [mm];", "col");
//	treeTrack->Draw("TOFWPos[1]:TOFWPos[0]>>h(500, -900., 900., 500, -700., 700.)", "", "col");
//	treeTrack->Draw("DCRPos[2][1]:TOFWPos[0]>>h(500, -700., 700., 500, -500., 500.)", "dsca11==0&&(A0t0||A0t1)&&B0&&B2;TOFWPosX [mm];DC2PosY [mm];", "col");
//	treeTrack->Draw("kTa[1][1]:kTa[1][0]>>h(500, -0.1, 0.1, 500, -0.1, 0.1)", "dsca11==0&&(A0t0||A0t1)&&B0&&B2;kTa[1][0];kTa[1][1];", "col");
//	treeTrack->Draw("kTa[1][0]:kTa[0][0]>>h(500, -0.1, 0.1, 500, -0.1, 0.1)", "dsca11==0;kTa[1][0];kTa[0][0];", "col");

//	treeTrack->Draw("poz[0]:beta2[0]>>h(500, 0.5, 0.66, 500, 1000., 2600.)", "dsca11==0&&(A0t0||A0t1)&&B0&&dE1>1.5;beta2;poz;", "col");
	treeTrack->Draw("dE1:aoz[0]>>h(500, 1.4, 3., 500, 0., 4.5)", "dsca11==0&&(A0t0||A0t1)&&B0&&B2;aoz;dE1 arb.;", "col");

//	treeTrack->Draw("TOF_posY_refine[0]:firedStripId[0]>>h(33, -1.5, 31.5, 500, -10, 1300.)", "dsca11==0&&(A0t0||A0t1)&&B0", "col");
//	treeTrack->Draw("TOF_posY_refine[0]:TOFWPos>>h(500, -700., 700., 500, -10, 1300.)", "dsca11==0&&(A0t0||A0t1)&&B0&&aoz>0", "col");
//	treeTrack->Draw("(TOT_DC_Avrg[0]+TOT_DC_Avrg[1]+TOT_DC_Avrg[2])/3.:dE1>>h(500, -0.1, 4.5, 500, 0., 700.)", "ntrLs[1][0]>=1&&ntrLs[1][1]==1&&ntrLs[1][2]==1&&dsca11==0;dE1 arb.;TOT [ns];", "col");
//	treeTrack->Draw("treePID3D.TOT_DC_Avrg[0]:dE1>>h(500, -0.1, 4.5, 500, 0., 700.)", "n3Dtr==1;dE1 arb.;TOT [ns];", "col");
//	treeTrack->Draw("treePID3D.TOT_DC_Avrg[0]:aoz>>h(500, 1.4, 3., 500, 0., 700.)", "n3Dtr==1;aoz;TOT [ns];", "col");
//	treeTrack->Draw("(TOT_Avrg_Ta[1][0]*TOTcnt_Ta[1][0]+TOT_Avrg_Ta[1][1]*TOTcnt_Ta[1][1])/(TOTcnt_Ta[1][0]+TOTcnt_Ta[1][1])+treePID3D.TOT_DC_Avrg[0]:dE1>>h(500, -1., 4.5, 500, 0., 1000.)", "n3Dtr==1", "col");
//	treeTrack->Draw("(TOT_Avrg_Ta[0][0]+TOT_Avrg_Ta[0][1])/2.:dE0+0.00623053*PDCPos[1][1]>>h(500, -1., 4.5, 500, 0., 500.)", "", "col");
//	treeTrack->Draw("(TOT_Avrg_Ta[1][0]+TOT_Avrg_Ta[1][1])/2.:dE1>>h(500, -1., 4.5, 500, 0., 500.)", "", "col");
//	treeTrack->Draw("(TOT_DC_Avrg[0]+TOT_DC_Avrg[1]+TOT_DC_Avrg[2])/3.:dE1>>h(500, -1., 4.5, 500, 0., 500.)", "ntrLs[1][0]==1&&ntrLs[1][1]==1&&ntrLs[1][2]==1&&(A0t0||A0t1)", "col");
//	treeTrack->Draw("(TOT_DC_Avrg[0]+TOT_DC_Avrg[1]+TOT_DC_Avrg[2])/300.:aoz>>h(500, 1.4, 4., 500, 0., 6.)", "ntrLs[1][0]==1&&ntrLs[1][1]==1&&ntrLs[1][2]==1&&(A0t0||A0t1)", "col");

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
//	A0t0->Draw("same");
//	A0t1->Draw("same");
//	B0->Draw("same");
//	B1->Draw("same");
//	B2->Draw("same");
//	B3->Draw("same");
	c->SetGrid();
	c->ToggleToolBar();
	c->ToggleEventStatus();
//	c->SaveAs("B3.pdf");
//	h0->Write("", TObject::kOverwrite);
//	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:PDCPos[1][1]>>(500, -30, 30, 500, -0.2, 4.5)", "", "col")
//	treeTrack->Draw("dE1:dE0>>(500, -1., 5., 500, -1., 5.)", "", "col");
//	treeTrack->Draw("aoz>>(500, 1.4, 2.7)", "")
	// overall PID
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3., 500, 0., 4.5)", "", "col")
//	treeTrack->Draw("sqrt(dE1)*pow(beta2, 0.8)*1.4:aoz>>(500, 1.4, 3., 500, 0., 2.2)", "dsca11==0", "col");
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3., 500, 0., 4.5)", "dsca11<=0&&dsca10<=1", "col")
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3., 500, 0., 4.5)", "!(dsca11<=0&&dsca10<=1)", "col")
//	treeTrack->Draw("adc[22]:adc[23]>>(500, 0, 4500, 500, 0, 4500)", "dsca11==0&&A2", "col")
//	treeTrack->Draw("adc[24]:adc[17]>>(500, 0, 4500, 500, 0, 4500)", "dsca11==0&&A2", "col")
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3.5, 500, -0.03, 4.5)", "!(dsca11==0&&dsca10==1&&tRef_UV_NL==1&&tRef_DV_NL==1&&CUTG&&multi_pre==1)&&CUTG", "col")

	// track count
//	double(treeTrack->GetEntries("ntr==0&&A04"))/treeTrack->GetEntries("A04")
//	double(treeTrack->GetEntries("ntrLs[3][0]==0&&A04"))/treeTrack->GetEntries("A04")
//	treeTrack->Draw("dE1:dE0>>(500, -1., 5., 500, -1., 5.)", "multi_PDC[0][0][0][0]==1&&multi_PDC[0][0][0][1]==1&&multi_PDC[0][0][1][0]==1&&multi_PDC[0][0][1][1]==1&&multi_PDC[0][1][0][0]==1&&multi_PDC[0][1][0][1]==1&&multi_PDC[0][1][1][0]==1&&multi_PDC[0][1][1][1]==1&&dsca11==0&&dsca10==1&&multi_PDC[1][0][0][0]==1&&multi_PDC[1][0][0][1]==1&&multi_PDC[1][0][1][0]==1&&multi_PDC[1][0][1][1]==1&&multi_PDC[1][1][0][0]==1&&multi_PDC[1][1][0][1]==1&&multi_PDC[1][1][1][0]==1&&multi_PDC[1][1][1][1]==1&&multi_DC[1][0][0][0]==1&&multi_DC[1][0][0][1]==1&&multi_DC[1][0][1][0]==1&&multi_DC[1][0][1][1]==1&&multi_DC[1][0][2][0]==1&&multi_DC[1][0][2][1]==1&&multi_DC[1][1][0][0]==1&&multi_DC[1][1][0][1]==1&&multi_DC[1][1][1][0]==1&&multi_DC[1][1][1][1]==1&&multi_DC[1][1][2][0]==1&&multi_DC[1][1][2][1]==1&&multi_DC[1][2][0][0]==1&&multi_DC[1][2][0][1]==1&&multi_DC[1][2][1][0]==1&&multi_DC[1][2][1][1]==1&&multi_DC[1][2][2][0]==1&&multi_DC[1][2][2][1]==1", "col")

} // end of the main function




