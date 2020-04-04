void fit(){
    TFile *f = new TFile("tmp.root", "update");
    TH1D *h = (TH1D*)f->Get("h2");
    TF1 *fun = new TF1("fun", "gaus(0)+gaus(3)", 2.2, 2.50);

    fun->SetParameter(0, 89.41);
    fun->SetParameter(1, 2.296);
    fun->SetParLimits(1, 2.28, 2.32);

    fun->SetParameter(2, 0.020);
    fun->SetParLimits(2, 0.018, 0.025);

    fun->SetParameter(3, 1.23955e+02);
    fun->SetParameter(4, 2.456);
    fun->SetParLimits(4, 2.44, 2.48);

    fun->SetParameter(5, 0.020);
    fun->SetParLimits(5, 0.018, 0.025);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    c->SetGrid();

    h->Fit(fun, "R");

    double binWidth = (3.7-1.5)/500.;
    double p[6];
    fun->GetParameters(p);
    double yield = sqrt(2.*TMath::Pi())*p[2]*p[0]/binWidth;
    cout << "\n\n\033[1myield = sqrt(2Pi)*C0*sigma/binWidth" << endl;
    cout << "\033[31;1myield: " << yield << "\033[0m\n";
}
