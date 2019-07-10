void tmp(){
	TFile *f = new TFile("ETFRand.root", "UPDATE");
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	TTree *treeETF = (TTree*)f->Get("treeETF");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	treeTrack->AddFriend(treeETF);

	// particle selection //
//	treeETF->Draw("A>>(11, 9.5, 20.5)");
//	treeETF->Draw("Z>>(11, -0.5, 10.5)");

	// initial momentum //
//	treeETF->Draw("p0[0]>>(500, 6000, 12000)", "Z==6&&A==12");
//	treeETF->Draw("p0[1]>>(500, 6000, 12000)", "Z==6&&A==12");
//	treeETF->Draw("p0[2]>>(500, 6000, 12000)", "Z==6&&A==12");

	// Tstop statistics //
	// Position
//	treeETF->Draw("TstopPos[0]>>(500, -0.1, 0.1)", "Z==6&&A==12");
//	treeETF->Draw("TstopPos[1]>>(500, -0.1, 0.1)", "Z==6&&A==12");
//	treeETF->Draw("TstopPos[2]>>(500, -0.1, 0.1)", "Z==6&&A==12");
	// momentum
//	treeETF->Draw("TstopMomentum[0]", "Z==6&&A==12");
//	treeETF->Draw("TstopMomentum[1]", "Z==6&&A==12");
//	treeETF->Draw("TstopMomentum[2]", "Z==6&&A==12");

	// PDC statistics //
//	treeTrack->Draw("PDCPos[0][0]>>(500, -20., 0.)", "Z==6&&A==12");
//	treeETF->Draw("PDCKineticEnergy[0]", "Z==6&&A==12");
//	treeTrack->Draw("d2DC/6");

	// tracking statistics
//	treeTrack->Draw("chiDC>>(500, -2., 2.)");

	// PID
//	treeTrack->Draw("zz:aoz>>(500, 1.7, 2.5, 500, 3., 8.)", "", "col");
	treeTrack->Draw("aoz>>(500, 1.7, 2.5)");
//	treeTrack->Draw("dx2>>(500, -12., 12.)");
	
	
}
