void fit(){
    TFile *f = new TFile("tmp.root", "update");
    TH1D *h = (TH1D*)f->Get("h2");
    TF1 *fun = new TF1("fun", "gaus(0)+gaus(3)", 2.43, 2.70);

    fun->SetParameter(0, 89.41);
    fun->SetParameter(1, 2.50);
    fun->SetParLimits(1, 2.49, 2.51);

    fun->SetParameter(2, 0.020);
    fun->SetParLimits(2, 0.018, 0.025);

    fun->SetParameter(3, 1.1315e4);
    fun->SetParameter(4, 2.66);
    fun->SetParLimits(4, 2.65, 2.67);

    fun->SetParameter(5, 0.020);
    fun->SetParLimits(5, 0.018, 0.025);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    c->SetGrid();

    h->Fit(fun, "R");

    double binWidth = (3.7-1.8)/500.;
    double p[6];
    fun->GetParameters(p);
    double yield = sqrt(2.*TMath::Pi())*p[2]*p[0]/binWidth;
    cout << "\n\n\033[1myield = sqrt(2Pi)*C0*sigma/binWidth" << endl;
    cout << "\033[31;1myield: " << yield << "\033[0m\n";
}
