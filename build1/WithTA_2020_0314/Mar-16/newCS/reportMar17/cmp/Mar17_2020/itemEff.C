// included by csShow.C

	TGraph *g = geffB;
	g->SetTitle("Detecting Efficiency of C and B @240MeV/u at ETF");
	g->SetMinimum(0.83); g->SetMaximum(0.97);
	g->GetXaxis()->SetTitle("A");
	g->GetYaxis()->SetTitle("effB"); // \\sigma_{0} [mb]
	g->GetYaxis()->SetTitleOffset(1.);
	g->Draw("APL");
	geffC->Draw("PLsame");
	
	lg->AddEntry(geffC, "eff C");
	lg->AddEntry(geffB, "eff B");
	lg->Draw();

	c->Print("eff.pdf");
	c->Print("eff.root");

	TFile *f = new TFile("eff.root", "update");
	geffB->SetLineStyle(1); geffB->SetName("gEffB_1");
	geffC->SetLineStyle(1); geffC->SetName("gEffC_1");
	geffB->Write("", TObject::kOverwrite);
	geffC->Write("", TObject::kOverwrite);
	f->Close();

	system("cp eff.root ../eff_1.root");

