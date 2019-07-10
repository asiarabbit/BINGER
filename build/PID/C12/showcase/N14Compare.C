// N14Compare.C -- cross section comparison: 230MeV/u N14 -> carbon targets with results at other incidenct energies from other labs

void N14Compare(){
	const int nE = 2;
	const double E[nE]      = {230.,   600.};
	const double dE[nE]     = {  0.,     0.};
	// C12(N14, C13)X
	const double csC13[nE]  = {22.77, 21.3};
	const double dcsC13[nE] = {4.30, 0.32};
	// C12(N14, C12)X
	const double csC12[nE]  = {132.51, 117.2};
	const double dcsC12[nE] = {11.90, 1.76};
	// C12(N14, C11)X
	const double csC11[nE]  = {16.39, 27.5};
	const double dcsC11[nE] = {4.72, 0.41};
	// C12(N14, B11)X
	const double csB11[nE]  = {39.13, 39.8};
	const double dcsB11[nE] = {4.66, 0.60};
	// C12(N14, B10)X
	const double csB10[nE]  = {22.15, 23.9};
	const double dcsB10[nE] = {4.25, 0.36};
	// C12(N14, Be7)X
	const double csBe7[nE]  = {20.07, 21.6};
	const double dcsBe7[nE] = {3.95, 0.65};

	TGraphErrors *geC13 = new TGraphErrors(nE, E, csC13, dE, dcsC13);
	TGraphErrors *geC12 = new TGraphErrors(nE, E, csC12, dE, dcsC12);
	TGraphErrors *geC11 = new TGraphErrors(nE, E, csC11, dE, dcsC11);
	TGraphErrors *geB11 = new TGraphErrors(nE, E, csB11, dE, dcsB11);
	TGraphErrors *geB10 = new TGraphErrors(nE, E, csB10, dE, dcsB10);
	TGraphErrors *geBe7 = new TGraphErrors(nE, E, csBe7, dE, dcsBe7);
	
	TGraphErrors *g = nullptr; // temporary variable
	g = geC13; g->SetMarkerStyle(20); g->SetMarkerColor(1); g->SetLineWidth(2); g->SetLineColor(1);
	g = geC12; g->SetMarkerStyle(21); g->SetMarkerColor(2); g->SetLineWidth(2); g->SetLineColor(2);
	g = geC11; g->SetMarkerStyle(22); g->SetMarkerColor(7); g->SetLineWidth(2); g->SetLineColor(7);
	g = geB11; g->SetMarkerStyle(23); g->SetMarkerColor(4); g->SetLineWidth(2); g->SetLineColor(4);
	g = geB10; g->SetMarkerStyle(24); g->SetMarkerColor(8); g->SetLineWidth(2); g->SetLineColor(8);
	g = geBe7; g->SetMarkerStyle(25); g->SetMarkerColor(6); g->SetLineWidth(2); g->SetLineColor(6);
	
	gStyle->SetEndErrorSize(10);
	
	TLegend *lg = new TLegend(0.5, 0.4, 0.9, 0.7);
	lg->SetBorderSize(0); lg->SetFillStyle(0);	

	TCanvas *c = new TCanvas("c", "cs compare", 800, 600);
	geC13->SetMinimum(0.); geC13->SetMaximum(150.); geC13->SetTitle("230MeV/u ^{12}C(^{14}N, X)anything;E [MeV/u];\\sigma\\hbox{ [mb]}");
	lg->AddEntry(geC13, "C13"); geC13->Draw("APL");
	lg->AddEntry(geC12, "C12"); geC12->Draw("PLsame");
	lg->AddEntry(geC11, "C11"); geC11->Draw("PLsame");
	lg->AddEntry(geB11, "B11"); geB11->Draw("PLsame");
	lg->AddEntry(geB10, "B10"); geB10->Draw("PLsame");
	lg->AddEntry(geBe7, "Be7"); geBe7->Draw("PLsame");

	lg->Draw();
	c->SetGrid();
	c->SaveAs("N14Cmp.pdf");
}
