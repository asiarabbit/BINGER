// subtract NOTA from TA to get the net momentum distribution of the residue
// NOTA is scaled to TA with a scale factor of N0_NOTA/N0_TA
// where N0 is the number of incident nuclei
// 2020-04-16

const double scale = 334664./252260.; // N0 of the two
void sub(){
	TFile *f = new TFile("ppl.root", "update");
	TH1F *hmomdisTA = (TH1F*)f->Get("hmomdisTA");
	TH1F *hmomdisNOTA = (TH1F*)f->Get("hmomdisNOTA");
	
	hmomdisNOTA->SetLineColor(2);
	hmomdisNOTA->Sumw2();
	hmomdisTA->Sumw2();

	hmomdisNOTA->Scale(scale);
	TH1F *hmomdisNET = new TH1F((*hmomdisTA) - (*hmomdisNOTA));
	hmomdisNET->SetName("hmomdisNET");
	hmomdisNET->SetLineColor(kCyan);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();
	hmomdisTA->Draw();
	hmomdisNOTA->Draw("same");
	hmomdisNET->Draw("same");

	hmomdisTA->Fit("gaus");
	hmomdisNOTA->Fit("gaus");
	hmomdisNET->Fit("gaus");
}
