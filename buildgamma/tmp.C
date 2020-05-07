// tmp.C -- ROOT script for analysis of 2020April data

void tmp(){
	TFile *f = new TFile("~/pionExp2017/buildgamma/20200428_0432.dat_0.root", "update");
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

//	TCutG *A0 = (TCutG*)f->Get("A0");

	// PID
	bool pidShow = 1;
	string cut = "1"; // cut
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();
	c->ToggleToolBar();
	c->ToggleEventStatus();

	treeTrack->Draw("(adc[1]+adc[2]+adc[3])/3000.:aoz[0]>>(500, 1., 2.8, 500, 0, 5.)", "dsca[9]==0", "colbox");
}
