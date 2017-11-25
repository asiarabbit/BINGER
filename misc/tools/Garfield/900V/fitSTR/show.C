// show.C -- for drawing purposes to objects stored in garfield*.root :)

void show(){
	TFile *f = new TFile("garfield.root", "update");
	TH1F *h0 = (TH1F*)f->Get("anode0/hSTR_9");
	TH1F *h1 = (TH1F*)f->Get("anode0/hSTR_10");
	TH1F *h2 = (TH1F*)f->Get("anode0/hSTR_11");
	TH1F *h3 = (TH1F*)f->Get("anode0/hSTR_12");
	TH1F *h4 = (TH1F*)f->Get("anode0/hSTR_13");
	TH1F *h5 = (TH1F*)f->Get("anode0/hSTR_14");
	TH1F *h6 = (TH1F*)f->Get("anode0/hSTR_17");
	TH1F *h7 = (TH1F*)f->Get("anode0/hSTR_20");
	h0->SetMarkerColor(1); h0->SetLineColor(1);
	h1->SetMarkerColor(2); h1->SetLineColor(2);
	h2->SetMarkerColor(3); h2->SetLineColor(3);
	h3->SetMarkerColor(4); h3->SetLineColor(4);
	h4->SetMarkerColor(5); h4->SetLineColor(5);
	h5->SetMarkerColor(6); h5->SetLineColor(6);
	h6->SetMarkerColor(2); h6->SetLineColor(2);
	h7->SetMarkerColor(4); h7->SetLineColor(4);


	TCanvas *c = new TCanvas("c", "STR Comparison", 1000, 800);
	h2->SetTitle("STR Comparison");
//	h0->Draw();
//	h1->Draw("same");
	h2->Draw("same");
//	h3->Draw("same");
//	h4->Draw("same");
//	h5->Draw("same");
	h6->Draw("same");
	h7->Draw("same");
	cout << h0->GetTitle() << endl;

	TLegend *lg = new TLegend(0.2, 0.65, 0.5, 0.9, "incident angle/degree");
//	lg->AddEntry(h0, "(-15,-10)", "plf");
//	lg->AddEntry(h1, "(-10,-5)", "plf");
	lg->AddEntry(h2, "(-5,0)", "plf");
//	lg->AddEntry(h3, "(0,5)", "plf");
//	lg->AddEntry(h4, "(5,10)", "plf");
//	lg->AddEntry(h5, "(10,15)", "plf");
	lg->AddEntry(h6, "(25,30)", "plf");
	lg->AddEntry(h7, "(40,45)", "plf");
	lg->Draw();
	c->SaveAs("hybrid.pdf");
}
