void CSGraph(){
	const int n = 2;
	const double x0[n] = {2., 1.};
	const double dx0[n] = {0., 0.};
	const double x[n] = {52.2, 33.5};
	const double dx[n] = {3.8, 2.5};

	// no pileUp calculation
	const double xNPU[n] = {48.4, 33.5};
	const double dxNPU[n] = {4.6, 3.5};
	const double x0NPU[n] = {2.05, 1.05};


	const char *nu[n] = {"13C", "14C"};
	
	TGraphErrors *g = new TGraphErrors(n, x0, x, dx0, dx);
	g->SetMinimum(0.);
	g->SetTitle("Cross Sections for C-fragments;\\Delta N;\\sigma [mb]");
	g->SetMarkerColor(2);
	g->SetMarkerStyle(21);

	TGraphErrors *gNPU = new TGraphErrors(n, x0NPU, xNPU, dx0, dxNPU);
	gNPU->SetMinimum(0.);
	gNPU->SetTitle("Cross Sections for C-fragments;\\Delta N;\\sigma [mb]");
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
	c->SaveAs("C_crossSectionsNPU.pdf");
}
