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
	effC[0] = {0.908074, 0.00459552};
	effB[0] = {0.888889, 0.0062123};
	sigma0[0] = {904.6796, 23.7890};
	sigmacc[0] = {736.6989, 11.2545};
	sigman[0] = {81.2025, 8.7351};
	sigma2n[0] = {77.4694, 8.8144};
	sigmap[0] = {17.8052, 2.0108};
	sigmanp[0] = {9.7117, 1.6732};
	
	// for C15 setting //
	effC[1] = {0.908008, 0.00366687};
	effB[1] = {0.861679, 0.00935577};
	sigma0[1] = {920.9755, 21.0111};
	sigmacc[1] = {728.4689, 12.0324};
	sigman[1] = {110.6945, 11.8136};
	sigma2n[1] = {57.2544, 7.4777};
	sigmap[1] = {27.8133, 2.7355};
	sigmanp[1] = {38.5393, 3.4942};

	// for C14 setting //
	effC[2] = {0.912436, 0.00263738};
	effB[2] = {0.888859, 0.0122277};
	sigma0[2] = {822.2614, 11.9900};
	sigmacc[2] = {706.5993, 8.0318};
	sigman[2] = {84.4025, 7.8933};
	sigma2n[2] = {26.6249, 4.1669};
	sigmap[2] = {43.3632, 2.7830};
	sigmanp[2] = {24.2735, 2.0165};
	
	// for C13 setting //
	effC[3] = {0.934552, 0.00718666};
	effB[3] = {0.909749, 0.0198239};
	sigma0[3] = {834.6038, 28.4348};
	sigmacc[3] = {745.1893, 20.1873};
	sigman[3] = {75.6066, 16.4276};
	sigma2n[3] = {17.6609, 7.7544};
	sigmap[3] = {39.2473, 5.9426};
	sigmanp[3] = {62.1580, 6.7661};

	// for C12 setting //
	effC[4] = {0.923611, 0.00583145};
	effB[4] = {0.900669, 0.0165077};
	sigma0[4] = {770.8620, 17.7015};
	sigmacc[4] = {692.4887, 13.8558};
	sigman[4] = {60.5136, 11.0834};
	sigma2n[4] = {0.9897, 5.2382};
	sigmap[4] = {62.3704, 5.6687};
	sigmanp[4] = {35.2857, 4.7037};
	
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



