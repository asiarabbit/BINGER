// included by csShow.C

	TGraph *g = gsigma0;
	g->SetTitle("Interaction and Charge-Changing C.S. for C->C12 @240MeV/u");
	g->SetMinimum(630.); g->SetMaximum(1000.);
	g->GetXaxis()->SetTitle("A");
	g->GetYaxis()->SetTitle("\\sigma_{0} [mb]");
	g->GetYaxis()->SetTitleOffset(1.4);
	g->Draw("APL");
	gsigmacc->Draw("PLsame");
	
	lg->AddEntry(gsigma0, "\\sigma_{0}");
	lg->AddEntry(gsigmacc, "\\sigma_{cc}");
	lg->Draw();

	c->Print("sigma0.pdf");
	c->Print("sigma0.root");

