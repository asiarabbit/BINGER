void CSGraph(){
	const int n = 2;
	const double x0[n] = {2., 1.};
	const double dx0[n] = {0., 0.};
	const double x[n] = {52.2, 33.5};
	const double dx[n] = {3.8, 2.5};
	const char *nu[n] = {"13C", "14C"};
	
	TGraphErrors *g = new TGraphErrors(n, x0, x, dx0, dx);
	g->SetMinimum(0.);
	g->SetTitle("Cross Sections for C-fragments;\\Delta N;\\sigma [mb]");
	g->SetMarkerColor(2);
	g->SetMarkerStyle(21);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	g->Draw("AP");
	c->SaveAs("C_crossSections.pdf");
}
