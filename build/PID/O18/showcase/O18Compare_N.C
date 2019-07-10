// O18Compare.C -- cross section comparison: 230MeV/u O18 -> carbon targets with results at other incidenct energies from other labs

void O18Compare_N(){
	const int nE = 2;
	const double E[nE]      = {260., 573.};
	const double dE[nE]     = {0.,     0.};
	// C12(O18, N17)X
	const double csN17[nE]  = {33.65, 29.6};
	const double dcsN17[nE] = {3.04, 2.2};
	// C12(O18, N16)X
	const double csN16[nE]  = {20.25, 27.0};
	const double dcsN16[nE] = {2.68, 3.7};
	// C12(O18, N15)X
	const double csN15[nE]  = {73.49, 57.3};
	const double dcsN15[nE] = {5.48, 3.9};
	// C12(O18, N14)X
	const double csN14[nE]  = {15.60, 18.7};
	const double dcsN14[nE] = {2.28, 1.8};
	// C12(O18, C14)X
	const double csC14[nE]  = {34.05, 20.3};
	const double dcsC14[nE] = {2.84, 1.6};
	// C12(O18, C13)X
	const double csC13[nE]  = {51.11, 27.5};
	const double dcsC13[nE] = {4.47, 2.2};
	// C12(O18, C12)X
	const double csC12[nE]  = {41.82, 17.4};
	const double dcsC12[nE] = {3.53, 1.6};
	// C12(O18, O16)X
	const double csO16[nE]  = {47.17, 41.4};
	const double dcsO16[nE] = {5.10, 3.0};

	TGraphErrors *geN17 = new TGraphErrors(nE, E, csN17, dE, dcsN17);
	TGraphErrors *geN16 = new TGraphErrors(nE, E, csN16, dE, dcsN16);
	TGraphErrors *geN15 = new TGraphErrors(nE, E, csN15, dE, dcsN15);
	TGraphErrors *geN14 = new TGraphErrors(nE, E, csN14, dE, dcsN14);
	TGraphErrors *geC14 = new TGraphErrors(nE, E, csC14, dE, dcsC14);
	TGraphErrors *geC13 = new TGraphErrors(nE, E, csC13, dE, dcsC13);
	TGraphErrors *geC12 = new TGraphErrors(nE, E, csC12, dE, dcsC12);
	TGraphErrors *geO16 = new TGraphErrors(nE, E, csO16, dE, dcsO16);

	TGraphErrors *g = nullptr; // temporary variable
	g = geN17; g->SetMarkerStyle(20); g->SetMarkerColor(1); g->SetLineWidth(2); g->SetLineColor(1);
	g = geN16; g->SetMarkerStyle(21); g->SetMarkerColor(2); g->SetLineWidth(2); g->SetLineColor(2);
	g = geN15; g->SetMarkerStyle(22); g->SetMarkerColor(3); g->SetLineWidth(2); g->SetLineColor(3);
	g = geN14; g->SetMarkerStyle(23); g->SetMarkerColor(4); g->SetLineWidth(2); g->SetLineColor(4);
	g = geC14; g->SetMarkerStyle(25); g->SetMarkerColor(6); g->SetLineWidth(2); g->SetLineColor(6);
	g = geC13; g->SetMarkerStyle(26); g->SetMarkerColor(7); g->SetLineWidth(2); g->SetLineColor(7);
	g = geC12; g->SetMarkerStyle(27); g->SetMarkerColor(8); g->SetLineWidth(2); g->SetLineColor(8);
	g = geO16; g->SetMarkerStyle(28); g->SetMarkerColor(9); g->SetLineWidth(2); g->SetLineColor(9);
	
	gStyle->SetEndErrorSize(10);
	
	TLegend *lg = new TLegend(0.7, 0.6, 0.9, 0.9);
	lg->SetBorderSize(0); lg->SetFillStyle(0);	

	TCanvas *c = new TCanvas("c", "cs compare", 800, 600);
	geN17->SetMinimum(0.); geN17->SetMaximum(100.);
	geN17->SetTitle("260MeV/u ^{12}C(^{18}O, ^{X}N)anything;E [MeV/u];\\sigma\\hbox{ [mb]}");
	lg->AddEntry(geN17, "N17"); geN17->Draw("APL");
	lg->AddEntry(geN16, "N16"); geN16->Draw("PLsame");
	lg->AddEntry(geN15, "N15"); geN15->Draw("PLsame");
	lg->AddEntry(geN14, "N14"); geN14->Draw("PLsame");
//	lg->AddEntry(geC14, "C14"); geC14->Draw("PLsame");
//	lg->AddEntry(geC13, "C13"); geC13->Draw("PLsame");
//	lg->AddEntry(geC12, "C12"); geC12->Draw("PLsame");
	lg->AddEntry(geO16, "O16"); geO16->Draw("PLsame");

	lg->Draw();
	c->SetGrid();
	c->SaveAs("O18Cmp_N.pdf");
}
