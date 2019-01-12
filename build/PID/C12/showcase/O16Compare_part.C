// O16Compare.C -- cross section comparison: 230MeV/u O16 -> carbon targets with results at other incidenct energies from other labs

void O16Compare_part(){
	const int nE = 3;
	const double E[nE]      = {230.,   600., 2100.};
	const double dE[nE]     = {  0.,     0.,    0.};
	// C12(O16, N15)X
	const double csN15[nE]  = {67.99, 73.2, 54.2};
	const double dcsN15[nE] = {3.69, 1.10, 2.9};
	// C12(O16, N14)X
	const double csN14[nE]  = {49.05, 66.8, 41.8};
	const double dcsN14[nE] = {3.48, 1.00, 3.3};
	// C12(O16, N13)X
	const double csN13[nE]  = {8.94, 12.6, 8.06};
	const double dcsN13[nE] = {1.37, 0.38, 0.42};
	// C12(O16, C13)X
	const double csC13[nE]  = {35.53, 41.0, 27.7};
	const double dcsC13[nE] = {2.76, 0.62, 1.4};
	// C12(O16, C12)X
	const double csC12[nE]  = {62.23, 79.2, 65.1};
	const double dcsC12[nE] = {3.58, 1.19, 5.2};
	// C12(O16, C11)X
	const double csC11[nE]  = {16.75, 26.5, 18.46};
	const double dcsC11[nE] = {1.99, 0.40, 0.92};
	// C12(O16, B11)X
	const double csB11[nE]  = {31.72, 36.0, 26.0};
	const double dcsB11[nE] = {2.73, 0.54, 1.3};
	// C12(O16, B10)X
	const double csB10[nE]  = {19.23, 24.8, 20.3};
	const double dcsB10[nE] = {1.89, 0.37, 1.6};

	TGraphErrors *geN15 = new TGraphErrors(nE, E, csN15, dE, dcsN15);
	TGraphErrors *geN14 = new TGraphErrors(nE, E, csN14, dE, dcsN14);
	TGraphErrors *geN13 = new TGraphErrors(nE, E, csN13, dE, dcsN13);
	TGraphErrors *geC13 = new TGraphErrors(nE, E, csC13, dE, dcsC13);
	TGraphErrors *geC12 = new TGraphErrors(nE, E, csC12, dE, dcsC12);
	TGraphErrors *geC11 = new TGraphErrors(nE, E, csC11, dE, dcsC11);
	TGraphErrors *geB11 = new TGraphErrors(nE, E, csB11, dE, dcsB11);
	TGraphErrors *geB10 = new TGraphErrors(nE, E, csB10, dE, dcsB10);
	
	TGraphErrors *g = nullptr; // temporary variable
	g = geN15; g->SetMarkerStyle(20); g->SetMarkerColor(1); g->SetLineWidth(2); g->SetLineColor(1);
	g = geN14; g->SetMarkerStyle(21); g->SetMarkerColor(2); g->SetLineWidth(2); g->SetLineColor(2);
	g = geN13; g->SetMarkerStyle(22); g->SetMarkerColor(3); g->SetLineWidth(2); g->SetLineColor(3);
	g = geC13; g->SetMarkerStyle(23); g->SetMarkerColor(4); g->SetLineWidth(2); g->SetLineColor(4);
	g = geC12; g->SetMarkerStyle(24); g->SetMarkerColor(9); g->SetLineWidth(2); g->SetLineColor(9);
	g = geC11; g->SetMarkerStyle(25); g->SetMarkerColor(6); g->SetLineWidth(2); g->SetLineColor(6);
	g = geB11; g->SetMarkerStyle(26); g->SetMarkerColor(7); g->SetLineWidth(2); g->SetLineColor(7);
	g = geB10; g->SetMarkerStyle(27); g->SetMarkerColor(8); g->SetLineWidth(2); g->SetLineColor(8);
	
	gStyle->SetEndErrorSize(10);
	
	TLegend *lg = new TLegend(0.7, 0.6, 0.9, 0.9);
	lg->SetBorderSize(0); lg->SetFillStyle(0);	

	TCanvas *c = new TCanvas("c", "cs compare", 800, 600);
	geN15->SetMinimum(0.); geN15->SetMaximum(120.); geN15->SetTitle(";E [MeV/u];\\sigma\\hbox{ [mb]}"); // 230MeV/u ^{12}C(^{16}O, ^{x}N)anything
	lg->AddEntry(geN15, "N15"); geN15->Draw("APL");
	lg->AddEntry(geN14, "N14"); geN14->Draw("PLsame");
	lg->AddEntry(geN13, "N13"); geN13->Draw("PLsame");
//	lg->AddEntry(geC13, "C13"); geC13->Draw("PLsame");
//	lg->AddEntry(geC12, "C12"); geC12->Draw("PLsame");
//	lg->AddEntry(geC11, "C11"); geC11->Draw("PLsame");
//	lg->AddEntry(geB11, "B11"); geB11->Draw("PLsame");
//	lg->AddEntry(geB10, "B10"); geB10->Draw("PLsame");

	lg->Draw();
	c->SetGrid();
	c->SaveAs("O16NCmp.eps");
	system("epstopdf O16NCmp.eps");
}
