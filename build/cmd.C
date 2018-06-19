void cmd(){
    TFile *f = new TFile("20180606_0826.dat_0.root", "update");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *vme = (TTree*)f->Get("vme");

	treeTrack->AddFriend(vme);
    treeTrack->AddFriend(treeshoot);
    treeshoot->AddFriend(treeTrack);
    treeshoot->AddFriend(vme);
    vme->AddFriend(treeTrack);
    vme->AddFriend(treeshoot);


	TCut *vetoCut = new TCut("-9999 == mtdc0[3][0] && -9999 == mtdc0[4][0]");
	TCut *uniTrkDXCut = new TCut("1 == ntrLs[3][0]");
	TCut *uniTrkRXCut = new TCut("1 == ntrLs[1][0]");
	TCut *uniTrkDYCut = new TCut("1 || 1 == ntrLs[3][1]");
	TCut *uniTrkDXYCut = new TCut(*uniTrkDXCut && *uniTrkDYCut);
	TCut *taPosXCut = new TCut("taHitPos[1][0] > 3.5 && taHitPos[1][0] < 25.");
	TCut *taPosYCut = new TCut("taHitPos[1][1] > -23. && taHitPos[1][1] < -5.1");
	TCut *taPosXYCut = new TCut(*taPosXCut && *taPosYCut);
	TCut *muPreTaCut = new TCut("(adc[7]+adc[8]) > 1384"); //  && (adc[7]+adc[8]) < 1700
	TCut *beta2Cut = new TCut("beta2 > 0.4 && beta2 < 0.8");
	TCut *matchXCut = new TCut("taHitPos[0][0]!=-9999. && taHitPos[1][0]!=-9999. && fabs(taHitPos[0][0] - taHitPos[1][0]) < 4.");
	TCut *matchYCut = new TCut("taHitPos[0][1]!=-9999. && taHitPos[1][1]!=-9999. && fabs(taHitPos[0][1] - taHitPos[1][1]) < 4.");
	TCut *matchXYCut = new TCut(*matchXCut && *matchYCut);
	TCut *t0_1_okCut = new TCut("t0_1_ok");

	TCut *goodCut = new TCut(*matchXYCut && *beta2Cut && *uniTrkDXCut && *uniTrkRXCut && *t0_1_okCut);


	TCutG *CUTZ8p = (TCutG*)f->Get("CUTZ8p_1");
	TCutG *CUTZ7p = (TCutG*)f->Get("CUTZ7p_1");
	TCutG *CUTZ6p = (TCutG*)f->Get("CUTZ6p_1");
	TCutG *CUTZ5p = (TCutG*)f->Get("CUTZ5p_1");

	const int n = 4;
    TCanvas *c = new TCanvas("c", "c", 1000, 600*n);
    c->ToggleEventStatus();
    c->ToggleEditor();
    c->ToggleToolBar();
    c->ToggleToolTips();
    c->Divide(2, n);


//	c->cd(1); treeTrack->Draw("aoz>>h1(500, 1.1, 3.1)");
//	c->cd(2); treeTrack->Draw("aoz>>h2(500, 1.1, 3.1)", *vetoCut);
//	c->cd(3); treeshoot->Draw("taHitPos[1][1]:taHitPos[1][0]>>h3(500, -60., 60., 500, -60., -60.)", *uniTrkDXYCut, "col");
//	c->cd(4); treeshoot->Draw("taHitPos[1][1]:taHitPos[1][0]>>h4(500, -60., 60., 500, -60., -60.)", *vetoCut && *uniTrkDXYCut, "col");
	c->cd(1); vme->Draw("adc[12]:(adc[9]+adc[10]+adc[11])>>h1(1000, 0., 3000., 1000, 0., 4000.)", "", "col");
	c->cd(2); vme->Draw("adc[12]:(adc[9]+adc[10]+adc[11])>>h2(1000, 0., 3000., 1000, 0., 4000.)", *vetoCut && *taPosXYCut && *muPreTaCut && *beta2Cut, "col");
	c->cd(3); vme->Draw("(adc[12]*pow(beta2, 2.)):((adc[9]+adc[10]+adc[11])*pow(beta2, 2.))>>h3(1000, 0., 1400., 1000, 0., 2000.)", *beta2Cut, "col");
	c->cd(4); treeTrack->Draw("tof2:poz>>h4(500, 1000., 3000, 500, 52., 56.)", *muPreTaCut && "CUTZ7p_1", "col");
	c->cd(5); treeTrack->Draw("tof2:poz>>h5(500, 1000., 3000, 500, 52., 56.)", "CUTZ6p_1", "col");
	c->cd(6); vme->Draw("beta2>>h6(1000, 0.5, 0.9)", *vetoCut && *muPreTaCut && *taPosXYCut, "col");
	c->cd(7); vme->Draw("aoz:((adc[9]+adc[10]+adc[11])*pow(beta2, 2.))>>h7(1000, 0., 3000., 1000, 0., 5.)", "aoz != -9999.", "col");
	c->cd(8); vme->Draw("aoz:((adc[9]+adc[10]+adc[11])*pow(beta2, 2.))>>h8(1000, 0., 4000., 1000, 0., 10.)", *vetoCut && *taPosXYCut && *muPreTaCut && *beta2Cut, "col");

	TObject *hh[2*n]{0};
	for(int i = 0; i < 2*n; i++){
		ostringstream ostr;
		ostr << "h" << i;
		hh[i] = gDirectory->Get(ostr.str().c_str());
	}
	const char *dir = "cmp";
	if(!f->FindObjectAny(dir)) f->mkdir(dir);
	f->cd(dir);
	for(TObject *&h : hh) if(h) h->Write("", TObject::kOverwrite);
} // end of cmd










