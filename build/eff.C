void eff(){
	TFile *f = new TFile("20180707_1623.dat_3.root");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *vme = (TTree*)f->Get("vme");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	treeTrack->AddFriend(vme);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treeTOFWR);

	TCanvas *c = new TCanvas("c", "c", 1500, 1200);
	c->Divide(2, 2);
	

	// efficiency
	double eff = treeTrack->GetEntries("multi_DC[1][0][0][0]>0&&treeTOFWR.multi_pre>0")/double(treeTrack->GetEntries("treeTOFWR.multi_pre>0"));

	c->cd(1);
//	treeTrack->Draw("(mtdc0[1][0]+mtdc0[2][0])/2.*0.09765625-0.09765625*mtdc0[0][0]+141.3>>h0(500, 120., 140.)"); // tof1
	treeTrack->Draw("(adc[16]+adc[17])/2.:((mtdc0[1][0]+mtdc0[2][0])/2.*0.09765625-0.09765625*mtdc0[0][0]+141.3)>>h0(500, 120., 140., 500, 0., 6000.)"); // tof1:dEb

	c->cd(2);
	treeTrack->Draw("0.5*(-0.010217*adc[0]-0.0104695*adc[1])+158.3>>h1(500, 120., 140.)"); // tof2

	c->cd(3);
	treeTrack->Draw("25881/(0.09765625*(mtdc0[1][0]+mtdc0[2][0])/2.-0.09765625*mtdc0[0][0]+141.3)/300.>>h2(500, 0.5, 0.8)"); // beta1

	c->cd(4);
//	treeTrack->Draw("25881/(0.5*(-0.010217*adc[0]-0.0104695*adc[1])+158.3)/300>>h3(400, 0.5, 0.8)"); // beta2
}
