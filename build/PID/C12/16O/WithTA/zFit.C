void zFit(){
	const int n = 4;
	const double zPID[n] = {1.00346e+00, 8.86718e-01, 7.80114e-01, 6.73848e-01};
	const double zReal[n] = {8, 7, 6, 5};
	
	TGraph *g = new TGraph(n, zPID, zReal);
	g->SetMarkerStyle(20);
	g->SetMarkerSize(2);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	g->Draw("AP");
	g->Fit("pol1");
	
	const double p[2] = {-1.12786, 9.12385};
	
	treeTrack->Draw("sqrt(dE1)*beta2[0]*9.12385-1.12786:aoz[0]*1.01818-0.0378892>>h(500, 1.2, 3, 500, 3., 10.)", "dsca11==0&&A0&&B0&&C0&&(dx2[0]>-9&&dx2[0]<3.6)&&!StripGap&&sca1drv<0.3;aoz;dE1 arb.;", "col");
}
