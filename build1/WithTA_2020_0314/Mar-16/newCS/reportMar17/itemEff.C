// included by csShow.C

	TGraph *g = geffB;
	g->SetTitle("Detecting Efficiency of C and B @240MeV/u at ETF");
	g->SetMinimum(0.83); g->SetMaximum(0.97);
	g->GetXaxis()->SetTitle("A"); g->GetXaxis()->CenterTitle();
	g->GetYaxis()->SetTitle("effB"); // \\sigma_{0} [mb]
	g->GetYaxis()->CenterTitle();
	g->GetYaxis()->SetTitleOffset(1.1);
	g->Draw("AP");
	geffC->Draw("Psame");
	
	lg->AddEntry(geffC, "eff C", "p");
	lg->AddEntry(geffB, "eff B", "p");
	lg->Draw();

	c->Print("eff.pdf");
	c->Print("eff.root");

