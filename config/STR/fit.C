// fit STR for PDCs
double myfunc(double *x, double *p);
const double poln = 2;

void fit(){
    TFile *f = new TFile("STR_spline.root", "update");
    const int n = 6;
    const double t[n] = {-0.1, 0., 124.5, 450., 470., 500.};
    const double r[n] = {0.,   0., 5.,    10.,  10.,  10.};


    TGraph *g = new TGraph(n, t, r);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    g->SetMarkerColor(2); g->SetMarkerSize(2); g->SetMarkerStyle(20);
    g->Draw();

    TF1 *rt0 = new TF1("RT0", myfunc, -100., 600., 0);
//    rt0->SetParameters(0., 0.0498214, -6.18568e-05);
 //   rt0->SetParLimits(0, -10., 10.);
  //  rt0->SetParLimits(1, 0.02, 0.1);
   // rt0->SetParLimits(2, 2e-5, 1e-4);
    g->Fit("pol2", "R", "", -100., 500.);
    rt0->SetLineColor(kMagenta); rt0->SetLineWidth(3);
    rt0->Draw("same");
   
    const char *dir = "1150P";
    if(!f->FindObjectAny(dir)) f->mkdir(dir);
	f->cd(dir);
	char name[64];
	for(int i = 0; i < 6; i++){
		sprintf(name, "RT%d", i);
		rt0->SetName(name);
//		rt0->Write("", TObject::kOverwrite);
	}
}

double myfunc(double *x, double *pp){
    double xx = x[0];
    if(xx <= 0.) return 0.;
    if(xx > 450.) return 10.;
    double result = 0.;
    static const double p[3] = {4.10232e-15, 0.0470497, -5.53337e-05};
    for(int i = 0; i <= poln; i++){
        result += pow(xx, i) * p[i];
    }
    return result;
}

