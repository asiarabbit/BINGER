void mag(){
	TFile *f = new TFile("magTree.root");
	
	TTree *treeMag = (TTree*)f->Get("treeMag");
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	treeMag->Draw("B:(h+min/60.)", "d==7");
}
