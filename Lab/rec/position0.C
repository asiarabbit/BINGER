#include <time.h>
const double Pi = 3.14159265357;
void position0(){
	const int n = 4;
	double x[n], z[n];

	x[0] = -4.620875096;
	z[0] = -1730.026663347;
	x[1] = 5.365729120;
	z[1] = -1720.013358052;
	x[2] = -5.597015329;
	z[2] = -939.051817225;
	x[3] = 4.377232176;
	z[3] = -929.026217927;


	TGraph *gframe = new TGraph();
	gframe->SetPoint(4, -3000., -100.);
	gframe->SetPoint(5, -3000., 100.);
	gframe->SetPoint(6, 0., -100.);
	gframe->SetPoint(7, 0., 100.);
	gframe->GetXaxis()->SetTitle("z/mm");
	gframe->GetYaxis()->SetTitle("x/mm");

	TGraph *g = new TGraph();
	g->SetMarkerSize(1);
	g->SetMarkerStyle(24);
	g->SetMarkerColor(kBlack);
	g->SetLineColor(kBlue);
	for(int i = 4; i--;) g->SetPoint(i, z[i], x[i]);
	
	TGraph *gline = new TGraph();
	double k = -0.001081895, b = -1.557143074;
	const int nn = 1000;
	for(int i = 0; i < nn; i++){
		double x = -1500. + (2.*i/nn - 1.) * 1500.; // (-3000, 0)
		double y = k * x + b;
		gline->SetPoint(i, x, y);
	}
	gline->SetLineColor(kBlue);
	gline->SetLineWidth(2);



	
	TCanvas *c = new TCanvas("c", "position");
	gframe->Draw("AP");
	g->Draw("Psame");
	gline->Draw("PLsame");
} // end of function position






