void tmp(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *vme = (TTree*)f->Get("shoot");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(shoot);
	treeTrack->AddFriend(vme);


	treeTrack->Draw("dE0:tof1>>(500, 133., 137., 500, -0.5, 4.5)", "", "col");
	treeTrack->Draw("dE1:dE0>>(500, -1., 5., 500, -1., 5.)", "", "col");

	treeTrack->Draw("aoz>>(500, 1.4, 2.7)", "CUTG0");
}
