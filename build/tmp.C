void tmp(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(vme);

	// PID
	treeTrack->Draw("dE0:tof1>>(500, 133., 137., 500, -0.5, 4.5)", "", "col");
//	treeTrack->Draw("dE1:dE0>>(500, -1., 5., 500, -1., 5.)", "", "col");
//	treeTrack->Draw("aoz>>(500, 1.4, 2.7)", "");
	// overall PID
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3., 500, 0., 4.5)", "", "col")
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3., 500, 0., 4.5)", "dsca11<=0&&dsca10<=1", "col")
//	treeTrack->Draw("dE1:aoz>>(500, 1.4, 3., 500, 0., 4.5)", "!(dsca11<=0&&dsca10<=1)", "col")

	// track count
//	double(treeTrack->GetEntries("ntr==0&&A04"))/treeTrack->GetEntries("A04")
//	double(treeTrack->GetEntries("ntrLs[3][0]==0&&A04"))/treeTrack->GetEntries("A04")


}
