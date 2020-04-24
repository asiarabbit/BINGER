// to present nucleon removal cross sections of carbon isotopes at 240MeV/u
// in a graphical way
// The data were taken from the External Target Facility at IMP, Lanzhou, China
// c.s. unit: mb

struct t_data{
	double val;
	double err;
};

void csShow(){
	const unsigned short n = 5;
	const char *nucl[n] = {"C16", "C15", "C14", "C13", "C12"};
	t_data effC[n], effB[n]; // the det eff of C and B for the 5 settings in order
	t_data sigman[n], sigma2n[n], sigmap[n], sigmanp[n]; // nucleon-removal c.s.
	t_data sigma0[n], sigmacc[n]; // total interaction c.s. and charge-changing c.s.

	// for C16 setting //
	effC[0] = {0.908396, 0.00459323};
	effB[0] = {0.889325, 0.00621069};
	sigma0[0] = {908.9607, 23.1691};
	sigmacc[0] = {735.3861, 11.2367};
	sigman[0] = {74.3993, 8.1672};
	sigma2n[0] = {80.3357, 8.6324};
	sigmap[0] = {17.8239, 1.9682};
	sigmanp[0] = {10.0175, 1.6836};
	
	// for C15 setting //
	effC[1] = {0.90813, 0.00366526};
	effB[1] = {0.861485, 0.0110811};
	sigma0[1] = {909.6742, 20.5359};
	sigmacc[1] = {731.8413, 11.9774};
	sigman[1] = {97.4712, 11.0334};
	sigma2n[1] = {59.6716, 7.2540};
	sigmap[1] = {27.9671, 2.7490};
	sigmanp[1] = {38.1052, 3.5024};

	// for C14 setting //
	effC[2] = {0.912677, 0.00263612};
	effB[2] = {0.889314, 0.012222};
	sigma0[2] = {819.1418, 11.5962};
	sigmacc[2] = {704.8952, 8.0146};
	sigman[2] = {61.8642, 7.4820};
	sigma2n[2] = {22.1041, 4.0781};
	sigmap[2] = {41.9044, 2.7253};
	sigmanp[2] = {23.2590, 1.9744};
	
	// for C13 setting //
	effC[3] = {0.93463, 0.00718597};
	effB[3] = {0.909749, 0.0198239};
	sigma0[3] = {821.8231, 27.3148};
	sigmacc[3] = {743.2405, 20.1773};
	sigman[3] = {70.2847, 14.1221};
	sigma2n[3] = {16.8475, 7.1694};
	sigmap[3] = {37.8250, 5.8557};
	sigmanp[3] = {61.3866, 6.8880};

	// for C12 setting //
	effC[4] = {0.923763, 0.00582709};
	effB[4] = {0.900669, 0.0165077};
	sigma0[4] = {762.1677, 18.1800};
	sigmacc[4] = {691.2592, 13.7905};
	sigman[4] = {60.4032, 9.3306};
	sigma2n[4] = {3.6601, 4.4786};
	sigmap[4] = {63.5801, 5.5182};
	sigmanp[4] = {33.9513, 4.6457};
	
	TGraphErrors *geffC = new TGraphErrors();
	TGraphErrors *geffB = new TGraphErrors();
	TGraphErrors *gsigma0 = new TGraphErrors();
	TGraphErrors *gsigmacc = new TGraphErrors();
	TGraphErrors *gsigman = new TGraphErrors();
	TGraphErrors *gsigma2n = new TGraphErrors();
	TGraphErrors *gsigmap = new TGraphErrors();
	TGraphErrors *gsigmanp = new TGraphErrors();

	
	// fill the graphs
//	int chro[n] = {1, 3, 2, 5, 4};
	for(int j = n; j--;){
		int i = 16-j; // atomic number A
//		int i = chro[j]; // time sequence
		geffC->SetPoint(j, i, effC[j].val);
		geffC->SetPointError(j, 0., effC[j].err);
		geffB->SetPoint(j, i, effB[j].val);
		geffB->SetPointError(j, 0., effB[j].err);
		gsigma0->SetPoint(j, i, sigma0[j].val);
		gsigma0->SetPointError(j, 0., sigma0[j].err);
		gsigmacc->SetPoint(j, i, sigmacc[j].val);
		gsigmacc->SetPointError(j, 0., sigmacc[j].err);
		gsigman->SetPoint(j, i, sigman[j].val);
		gsigman->SetPointError(j, 0., sigman[j].err);
		gsigma2n->SetPoint(j, i, sigma2n[j].val);
		gsigma2n->SetPointError(j, 0., sigma2n[j].err);
		gsigmap->SetPoint(j, i, sigmap[j].val);
		gsigmap->SetPointError(j, 0., sigmap[j].err);
		gsigmanp->SetPoint(j, i, sigmanp[j].val);
		gsigmanp->SetPointError(j, 0., sigmanp[j].err);
	} // end for over carbon isotopes from C16 to C12


	// set graph attribute //
	geffC->SetNameTitle("geffC", "C Detecting Efficiency over Different Settings");
	geffC->SetMarkerStyle(20); geffC->SetLineWidth(2);
	geffC->SetLineColor(1); geffC->SetMarkerColor(1);
	geffB->SetNameTitle("geffB", "B Detecting Efficiency over Different Settings");
	geffB->SetMarkerStyle(21); geffB->SetLineWidth(2);
	geffB->SetLineColor(2); geffB->SetMarkerColor(2);
	gsigma0->SetNameTitle("gsigma0", "Interaction Cross Section");
	gsigma0->SetMarkerStyle(22); gsigma0->SetLineWidth(2);
	gsigma0->SetLineColor(3); gsigma0->SetMarkerColor(3);
	gsigmacc->SetNameTitle("gsigmacc", "Charge-Changing Cross Section");
	gsigmacc->SetMarkerStyle(23); gsigmacc->SetLineWidth(2);
	gsigmacc->SetLineColor(4); gsigmacc->SetMarkerColor(4);
	gsigman->SetNameTitle("gsigman", "1n Removal Cross Section");
	gsigman->SetMarkerStyle(24); gsigman->SetLineWidth(2);
	gsigman->SetLineColor(9); gsigman->SetMarkerColor(9);
	gsigma2n->SetNameTitle("gsigma2n", "2n Removal Cross Section");
	gsigma2n->SetMarkerStyle(25); gsigma2n->SetLineWidth(2);
	gsigma2n->SetLineColor(6); gsigma2n->SetMarkerColor(6);
	gsigmap->SetNameTitle("gsigmap", "1p Removal Cross Section");
	gsigmap->SetMarkerStyle(26); gsigmap->SetLineWidth(2);
	gsigmap->SetLineColor(7); gsigmap->SetMarkerColor(7);
	gsigmanp->SetNameTitle("gsigmanp", "np Removal Cross Section");
	gsigmanp->SetMarkerStyle(27); gsigmanp->SetLineWidth(2);
	gsigmanp->SetLineColor(8); gsigmanp->SetMarkerColor(8);


	// draw the graphs //
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	TLegend *lg = new TLegend(0.7, 0.8, 0.9, 0.9);
	c->SetGrid();
	
//	#include "itemEff.C"
//	#include "itemSigma0.C"
	#include "itemSigma.C"
} // end of the main function



