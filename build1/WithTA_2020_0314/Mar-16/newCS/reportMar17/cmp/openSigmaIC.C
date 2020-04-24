void openSigmaIC(){
    TFile *f = new TFile("./sigmaIC.root", "update");
    TCanvas *c = new TCanvas("c", "c", 800, 600);

    TGraphErrors *gsigmaR83_1 = (TGraphErrors*)f->Get("gsigmaR83_1");
    TGraphErrors *gsigmaI240_0 = (TGraphErrors*)f->Get("gsigmaI240_0");
    TGraphErrors *gsigmaI240_1 = (TGraphErrors*)f->Get("gsigmaI240_1");
    TGraphErrors *gsigmaI900_1 = (TGraphErrors*)f->Get("gsigmaI900_1");

		TLegend *lg = new TLegend(0.3, 0.5, 0.9, 0.9);
		lg->SetName("lg");
		c->SetGrid();
		lg->AddEntry(gsigmaR83_1, "83MeV/u, Reaction C.S.");
		lg->AddEntry(gsigmaI240_0, "240MeV/u, Interaction C.S. - Old");
		lg->AddEntry(gsigmaI240_1, "240MeV/u, Interaction C.S. - New");
		lg->AddEntry(gsigmaI900_1, "900MeV/u, Interaction C.S.");


    gsigmaR83_1->Draw("APL");
    gsigmaI240_0->Draw("PLsame");
    gsigmaI240_1->Draw("PLsame");
    gsigmaI900_1->Draw("PLsame");
		lg->SetY1(0.7);
		lg->Draw("same");

    c->Write();
    c->SaveAs("sigmaIC.pdf");
    
}
