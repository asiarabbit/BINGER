void CSGraph(){
	const int n = 3;
	const double x0[n] = {3., 2., 1.};
	const double dx0[n] = {0., 0., 0.};
	const double x[n] = {71.9, 19.4, 31.4};
	const double dx[n] = {4.4, 2.7, 2.9};
	const char *nu[n] = {"15N", "16N", "17N"};
	
	TGraphErrors *g = new TGraphErrors(n, x0, x, dx0, dx);
	g->SetMinimum(0.);
	g->SetTitle("Cross Sections for N-fragments;\\Delta N;\\sigma [mb]");
	g->SetMarkerColor(2);
	g->SetMarkerStyle(21);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	g->Draw("AP");
	c->SaveAs("N_crossSections.pdf");
}
