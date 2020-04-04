// draw1D.C -- one dimensional dx2-ratio spectrum
void draw1D(){
    TFile *f = new TFile("dx2_1D_1000.root", "update");
    TGraph2D *gdx2RatioC15 = (TGraph2D*)f->Get("gdx2RatioC15");
    TGraph2D *gdx2RatioC14 = (TGraph2D*)f->Get("gdx2RatioC14");
    TGraph2D *gdx2RatioC13 = (TGraph2D*)f->Get("gdx2RatioC13");

    const int n = gdx2RatioC15->GetN();
    double *x = gdx2RatioC15->GetX();
    double *y = gdx2RatioC15->GetY();
    double *z = gdx2RatioC15->GetZ();
    TGraph *gC15 = new TGraph(n, x, z);
    gC15->SetName("gC15");
    gC15->SetTitle("gC15");

    x = gdx2RatioC14->GetX();
    y = gdx2RatioC14->GetY();
    z = gdx2RatioC14->GetZ();
    TGraph *gC14 = new TGraph(n, x, z);
    gC14->SetName("gC14");
    gC14->SetTitle("gC14");

    x = gdx2RatioC13->GetX();
    y = gdx2RatioC13->GetY();
    z = gdx2RatioC13->GetZ();
    TGraph *gC13 = new TGraph(n, x, z);
    gC13->SetName("gC13");
    gC13->SetTitle("gC13");

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    c->Divide(2,2);
    c->cd(1);
    gC15->SetMarkerStyle(20);
    gC15->Draw("apl");
    c->cd(2);
    gC14->SetMarkerStyle(20);
    gC14->Draw("apl");
    c->cd(3);
    gC13->SetMarkerStyle(20);
    gC13->Draw("apl");
}


