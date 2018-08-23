void opf(){
	TFile *f = new TFile("20180713_2001.dat_0.root", "UPDATE");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeOpticFiber = (TTree*)f->Get("treeOpticFiber");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeOpticFiber);
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(vme);

//	treeTrack->Draw("dE1:tof1>>(500, 130., 180., 500, -0.03, 4.5)", "dsca10<=1&&dsca11==0", "col");
	treeTrack->Draw("tRef-(ul[10][0]+dl[10][0])/2.:(kTa[0][0]*(-1981.08)+bTa[0][0])>>(500, -30, 0, 500, 90, 110)", "multi==2", "col");
	treeTrack->Draw("multi>>(10, -1.5, 8.5)");
}
