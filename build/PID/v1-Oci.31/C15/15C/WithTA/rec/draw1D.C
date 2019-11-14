// draw1D.C -- one dimensional dx2-ratio spectrum
void draw1D(){
    TFile *f = new TFile("dx2_1D_1000LL.root", "update");
    TGraph2D *gdx2RatioC14 = (TGraph2D*)f->Get("gdx2RatioC14");
    TGraph2D *gdx2RatioC13 = (TGraph2D*)f->Get("gdx2RatioC13");
    TGraph2D *gdx2RatioC12 = (TGraph2D*)f->Get("gdx2RatioC12");

    const int n = gdx2RatioC14->GetN();
    double *x = gdx2RatioC14->GetX();
    double *y = gdx2RatioC14->GetY();
    double *z = gdx2RatioC14->GetZ();
    TGraph *gC14 = new TGraph(n, x, z);
    gC14->SetName("gC14");
    gC14->SetTitle("gC14");

    x = gdx2RatioC13->GetX();
    y = gdx2RatioC13->GetY();
    z = gdx2RatioC13->GetZ();
    TGraph *gC13 = new TGraph(n, x, z);
    gC13->SetName("gC13");
    gC13->SetTitle("gC13");

    x = gdx2RatioC12->GetX();
    y = gdx2RatioC12->GetY();
	z = gdx2RatioC12->GetZ();
    TGraph *gC12 = new TGraph(n, x, z);
    gC12->SetName("gC12");
    gC12->SetTitle("gC12");

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    c->Divide(2,2);
    c->cd(1);
    gC14->SetMarkerStyle(20);
    gC14->Draw("apl");
    c->cd(2);
    gC13->SetMarkerStyle(20);
    gC13->Draw("apl");
    c->cd(3);
    gC12->SetMarkerStyle(20);
    gC12->Draw("apl");
    
    gC12->Write("", TObject::kOverwrite);
    gC13->Write("", TObject::kOverwrite);
    gC14->Write("", TObject::kOverwrite);
}


