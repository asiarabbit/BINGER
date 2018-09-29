void gGOOD(){
	TFile *f = new TFile("~/pionExp2017/build/20180708_0153.dat_0.root", "update");
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

	TCutG *A0 = (TCutG*)f->Get("A0");
	TCutG *C13 = (TCutG*)f->Get("C13");
	TCutG *C14 = (TCutG*)f->Get("C14");
	TCutG *C15 = (TCutG*)f->Get("C15");
	TCutG *B11 = (TCutG*)f->Get("B11");
	TCutG *B12 = (TCutG*)f->Get("B12");
	TCutG *B13 = (TCutG*)f->Get("B13");
	TCutG *B14 = (TCutG*)f->Get("B14");
	TCutG *B15 = (TCutG*)f->Get("B15");

	TCanvas *c = new TCanvas("c", "gGOOD Distribution", 800, 600);
	c->SetGrid();
	treeTrack->Draw("gGOOD>>h0(10, -1.5, 8.5)", "dsca11==0&&A0&&(dx2[0]>-9&&dx2[0]<4)");
	treeTrack->Draw("gGOOD>>h1(10, -1.5, 8.5)", "dsca11==0&&A0&&(dx2[0]>-9&&dx2[0]<4)&&C15", "same");
	treeTrack->Draw("gGOOD>>h2(10, -1.5, 8.5)", "dsca11==0&&A0&&(dx2[0]>-9&&dx2[0]<4)&&C14", "same");
	
	TH1F *h0 = (TH1F*)f->Get("h0"); h0->SetLineColor(4);
	TH1F *h1 = (TH1F*)f->Get("h1"); h1->SetLineColor(2);
	TH1F *h2 = (TH1F*)f->Get("h2"); h2->SetLineColor(3);
//	h1->Scale(h0->GetEntries()/h1->GetEntries());
//	h2->Scale(h0->GetEntries()/h2->GetEntries());
}
