void CSGraph(){
	const int n = 3;
	const double x0[n] = {3., 2., 1.};
	const double dx0[n] = {0., 0., 0.};
	const double x[n] = {71.9, 19.4, 31.4};
	const double dx[n] = {4.4, 2.7, 2.9};
	const char *nu[n] = {"15N", "16N", "17N"};

	// no pileUp calculation
	const double xNPU[n] = {71.0, 23.5, 35.9};
	const double dxNPU[n] = {5.6, 3.3, 3.8};
	const double x0NPU[n] = {3.05, 2.05, 1.05};
	
	TGraphErrors *g = new TGraphErrors(n, x0, x, dx0, dx);
	g->SetMinimum(0.);
	g->SetTitle("Cross Sections for N-fragments;\\Delta N;\\sigma [mb]");
	g->SetMarkerColor(2);
	g->SetMarkerStyle(21);
	
	TGraphErrors *gNPU = new TGraphErrors(n, x0NPU, xNPU, dx0, dxNPU);
	gNPU->SetMinimum(0.);
	gNPU->SetTitle("Cross Sections for N-fragments;\\Delta N;\\sigma [mb]");
	gNPU->SetMarkerColor(4);
	gNPU->SetMarkerSize(2);
	gNPU->SetMarkerStyle(22);
	
	TLegend *lg = new TLegend(0.4, 0.6, 0.7, 0.7);
	lg->AddEntry(g, "WithPU");
	lg->AddEntry(gNPU, "No Pile Up");

	TCanvas *c = new TCanvas("c", "c", 800, 600);
	g->Draw("AP");
	gNPU->Draw("Psame");
	lg->Draw();
	c->SaveAs("N_crossSectionsNPU.pdf");
}
