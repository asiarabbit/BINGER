void align(){
	const int split = 350000;
    TFile *f = new TFile("20180606_1136.dat_0.root");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *vme = (TTree*)f->Get("vme");

	cout << "vme->GetEntries(): " << vme->GetEntries() << endl;
	cout << "treeTrack->GetEntries(): " << treeTrack->GetEntries() << endl;
	treeTrack->AddFriend(vme);
    vme->AddFriend(treeTrack);

	TCanvas *c = new TCanvas("c", "c", 800, 1200);
	c->Divide(1, 2);
	ostringstream cut1, cut2;
	cut1 << "index > 0 && index < " << split;
	cut2 << "index > " << split << " && index < 1e10";

	c->cd(1);
	vme->Draw("((mtdc0[1]-mtdc0[2])*0.097656250):treeTrack.tRef_pos>>h1(500, -10., 10., 500, -10., 10.)", cut1.str().c_str());
	
	c->cd(2);
	vme->Draw("((mtdc0[1]-mtdc0[2])*0.097656250):treeTrack.tRef_pos>>h2(500, -10., 10., 500, -10., 10.)", cut2.str().c_str());
	
}
