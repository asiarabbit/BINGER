
void aozFit(){
	const int n = 6;
	const double aozPID[6] = {2.140, 2.000, 1.858, 2.167, 2.003, 1.841};
	const double aozReal[6] = {15./7, 14./7, 13./7, 13./6, 12./6, 11./6};
	TGraph *g = new TGraph(n, aozPID, aozReal);
	g->SetMarkerStyle(20);
	g->SetMarkerSize(2);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	g->Draw("AP");
	g->Fit("pol1");

	const double p[2] = {-0.0378892, 1.01818};
}
	
