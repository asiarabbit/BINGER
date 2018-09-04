void tmp(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update"); // 2052 1848 1649
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treePDCPileUp = (TTree*)f->Get("treePDCPileUp");
	TTree *treeDCPileUp = (TTree*)f->Get("treeDCPileUp");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeTOFWR);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treePDCPileUp);
	treeTrack->AddFriend(treeDCPileUp);
	treeTrack->AddFriend(vme);

	// stored cuts //
	TCutG *A0t0 = (TCutG*)f->Get("A0t0");
	TCutG *A0t1 = (TCutG*)f->Get("A0t1");
	TCutG *B0 = (TCutG*)f->Get("B0");
	TCutG *B1 = (TCutG*)f->Get("B1");

	// PID
	TCanvas *c = new TCanvas("c", "c", 800, 600);
//	treeTrack->Draw("dE0:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0;tof1 [ns];dE0-corrected arb.;", "col");
//	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0", "col");
	treeTrack->Draw("dE1:dE0+0.00623053*PDCPos[1][1]>>h(500, -1., 5., 500, -1., 5.)", "dsca11==0&&aoz>0;dE0-corrected arb.;dE1 arb.;", "col");
//	treeTrack->Draw("aoz:TOFWPos>>h(500, -700., 700., 500, 1.4, 3.)", "CUTG", "colbox");
////	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0", "col");
//	treeTrack->Draw("dE0+0.00623053*PDCPos[1][1]:tof1>>h(500, 133., 137., 500, -0.5, 4.5)", "dsca11==0;tof1[ns];dE0-corrected arb.;", "col");
//	treeTrack->Draw("taHitPos[0][1]:taHitPos[0][0]>>h(500, -40., 40., 500, -40., 40.)", "dsca11==0;taHitPosX [mm];taHitPosY [mm];", "col");
//	treeTrack->Draw("dE1:aoz>>h(500, 1.4, 3., 500, 0., 4.5)", "dsca11==0&&(A0t0||A0t1)&&B0;aoz;dE1 arb.;", "col");
	TNamed *h = (TNamed*)f->Get("h");
	h->SetTitle((string(h->GetTitle())+"}").c_str());
//	A0t0->Draw("same");
//	A0t1->Draw("same");
//	B0->Draw("same");
//	B1->Draw("same");
	//c->SaveAs("31.pdf");
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




