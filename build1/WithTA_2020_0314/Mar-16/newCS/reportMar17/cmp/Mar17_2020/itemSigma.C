// included by csShow.C

	TGraph *g = gsigman;
	g->SetTitle("Nucleon Removal C.S. for C->C12 @240MeV/u");
	g->SetMinimum(-10.); g->SetMaximum(150.);
	g->GetXaxis()->SetTitle("A");
	g->GetYaxis()->SetTitle("\\sigma [mb]");
	g->GetYaxis()->SetTitleOffset(1.4);
	g->Draw("APL");
	gsigma2n->Draw("PLsame");
	gsigmap->Draw("PLsame");
	gsigmanp->Draw("PLsame");
	
	lg->AddEntry(gsigman, "-n");
	lg->AddEntry(gsigma2n, "-2n");
	lg->AddEntry(gsigmap, "-p");
	lg->AddEntry(gsigmanp, "-np");
	lg->SetY1(0.7);
	lg->Draw();

	c->Print("sigma.pdf");
	c->Print("sigma.root");

	TFile *f = new TFile("sigma.root", "update");
	gsigman->SetName("gsigman_1");
	gsigma2n->SetName("gsigma2n_1");
	gsigmap->SetName("gsigmap_1");
	gsigmanp->SetName("gsigmanp_1");
	gsigman->Write("", TObject::kOverwrite);
	gsigma2n->Write("", TObject::kOverwrite);
	gsigmap->Write("", TObject::kOverwrite);
	gsigmanp->Write("", TObject::kOverwrite);
	f->Close();

	system("cp sigma.root ../sigma_1.root");

