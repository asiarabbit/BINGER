static const double amu = 931.494; // atomic mass unit; unit: MeV
static const double mp = 1.007825; // unit: amu
static const double mn = 1.008665; // unit: amu

double nnCS(double *x, double *p){
	const double a = 1. + x[0]/amu;
	double beta = sqrt(a*a-1.) / a;
	return 13.73 - 15.04/beta + 8.76/pow(beta, 2) + 68.67*pow(beta, 4);
}
double ppCS(double *x, double *p){
	const double a = 1. + x[0]/amu;
	double beta = sqrt(a*a-1.) / a;
	return 13.73 - 15.04/beta + 8.76/pow(beta, 2) + 68.67*pow(beta, 4);
}
double npCS(double *x, double *p){
	const double a = 1. + x[0]/amu;
	double beta = sqrt(a*a-1.) / a;
	return -70.67 - 18.18/beta + 25.26/pow(beta, 2) + 113.85*beta;
}
double NNCS(double *x, double *p){
	return (nnCS(x, p) + ppCS(x, p) + 2*npCS(x, p))/4;
}

void nncs(){
	TF1 *fnn = new TF1("fnn", nnCS, 0.1, 1200);
	TF1 *fpp = new TF1("fpp", ppCS, 0.1, 1200);
	TF1 *fnp = new TF1("fnp", npCS, 0.1, 1200);
	TF1 *fNN = new TF1("fNN", NNCS, 0.1, 1200);
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	fnn->SetLineColor(1);
	fpp->SetLineColor(2);
	fnp->SetLineColor(3);
	fNN->SetLineColor(4);
	fnn->Draw();
	fpp->Draw("same");
	fnp->Draw("same");
	fNN->Draw("same");
	
	TLegend *lg = new TLegend(0.6, 0.7, 0.9, 0.9);
	lg->SetBorderSize(0);
	lg->AddEntry(fnn, "\\sigma_{nn}");
	lg->AddEntry(fpp, "\\sigma_{pp}");
	lg->AddEntry(fnp, "\\sigma_{np}");
	lg->AddEntry(fNN, "\\sigma_{NN}");
	lg->Draw();
}
