void MUShow(){
	TFile *f1 = new TFile("MU1.root");
	TFile *f2 = new TFile("MU2.root");
	
	TH1F *hMU1 = (TH1F*)f1->Get("hMU1");
	TH1F *hMU2 = (TH1F*)f2->Get("hMU2");
	
	TCanvas *c = new TCanvas("c", "MUCmp", 800, 1000);
	c->SetGrid();
	c->Divide(1,2);

	c->cd(1);
	hMU1->Draw();
	c->cd(2);
	hMU2->Draw();
}

//treeTrack->Draw("-0.439744+7.51908*sqrt(dE1)*pow(beta2[0],0.8):tof1>>h(500, 130., 155., 500, 0., 12)", "dsca11==0&&B0&&C0;tof1 [ns];dE0-corrected arb.;", "col")

//treeTrack->Draw("-0.640743+8.60835*sqrt(dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)))*pow(beta,0.8):tof1>>h(500, 130., 155., 500, 0., 12)", "dsca11==0&&PDCPos[1][1]!=-9999.&&C0;tof1 [ns];dE0-corrected arb.;", "col")
