
double ST(double *x, double *p);

void tmp(){
    TF1 *f = new TF1("f1", "pol6", -40., 550.);
    double p[7] = {0.15774, 0.0212614, 0.000404477, -3.02031e-06, 8.88275e-09, -1.21066e-11, 6.31554e-15};
    f->SetParameters(p);

    TF1 *fST = new TF1("fST", ST, -5000., 4200., 5);
    double pp[5] = {0., 0.000629724, -2.44369e-07, 5.23574e-11, -4.07449e-15};
    fST->SetParameters(pp);

    new TCanvas();
    f->Draw();
    fST->SetLineColor(1);
    fST->Draw();
}

double ST(double *x, double *p){
    double t = x[0]/0.097656250-2050.;
    return t*p[1]+t*t*p[2]+t*t*t*p[3]+t*t*t*t*p[4];
}
