// C12Compare.C -- cross section comparison: 230MeV/u C12 -> carbon targets with results at other incidenct energies from other labs

void C12Compare(){
	const int nE = 5;
	const double E[nE]      = {230.,   250., 600., 1050., 2100.};
	const double dE[nE]     = {  0.,     0.,   0.,    0.,    0.};
	// C12(C12, B11)X
	const double csB11[nE]  = {66.60, 65.61, 70.7,  48.6, 53.8};
	const double dcsB11[nE] = { 6.00,  2.55, 1.06,   2.4,  2.7};
	// C12(C12, B10)X
	const double csB10[nE]  = {31.82, 47.50, 38.6,  27.9, 35.1};
	const double dcsB10[nE] = { 5.01,  2.42, 0.58,   2.2,  3.4};
	// C12(C12, Be7)X
	const double csBe7[nE]  = {25.51, 22.64, 15.5, 18.60, 18.61};
	const double dcsBe7[nE] = { 4.42,  1.49, 0.47,  0.90,  0.93};

	TGraphErrors *geB11 = new TGraphErrors(nE, E, csB11, dE, dcsB11);
	TGraphErrors *geB10 = new TGraphErrors(nE, E, csB10, dE, dcsB10);
	TGraphErrors *geBe7 = new TGraphErrors(nE, E, csBe7, dE, dcsBe7);
	
	geB11->SetMarkerStyle(20); geB11->SetMarkerColor(1); geB11->SetLineWidth(2); geB11->SetLineColor(1);
	geB10->SetMarkerStyle(21); geB10->SetMarkerColor(2); geB10->SetLineWidth(2); geB10->SetLineColor(2);
	geBe7->SetMarkerStyle(22); geBe7->SetMarkerColor(4); geBe7->SetLineWidth(2); geBe7->SetLineColor(4);
	
	gStyle->SetEndErrorSize(10);
	
	TLegend *lg = new TLegend(0.7, 0.7, 0.9, 0.9);
	lg->SetBorderSize(0); lg->SetFillStyle(0);	
	lg->AddEntry(geB11, "B11");
	lg->AddEntry(geB10, "B10");
	lg->AddEntry(geBe7, "Be7");
	
	TCanvas *c = new TCanvas("c", "cs compare", 800, 600);
	geB11->SetMinimum(10.); geB11->SetMaximum(80.); geB11->SetTitle("230MeV/u ^{12}C(^{12}C, X)anything;E [MeV/u];\\sigma\\hbox{ [mb]}");
	geB11->Draw("APL");
	geB10->Draw("PLsame");
	geBe7->Draw("PLsame");

	lg->Draw();
	c->SetGrid();
	c->SaveAs("C12Cmp.pdf");
}
