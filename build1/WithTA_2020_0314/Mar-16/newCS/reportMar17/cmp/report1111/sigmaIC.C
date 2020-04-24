// sigmaIC.C -- interaction and reaction cross sections of carbon isotopes from C12 to C16
// created: Nov. 18, 2019

struct t_data{
	double val;
	double err;
};

void sigmaIC(){

	t_data sigmaR83[4]={{957, 39}, {1075, 61}, {1319, 40}, {1237, 25}}; // reaction c.s. at 83MeV/u
	t_data sigmaI240[5] = {{770.8620, 17.7015}, {834.6038, 28.4348}, {822.2614, 11.9900}, {920.9755, 21.0111}, {904.6796, 23.7890}}; // interaction c.s. at 240MeV/u
	t_data sigmaI900[5] = {{853, 6}, {862, 12}, {880, 19}, {945, 10}, {1036, 11}}; // interaction c.s. at 900MeV/u

	TGraphErrors *gsigmaR83 = new TGraphErrors();
	TGraphErrors *gsigmaI240 = new TGraphErrors();
	TGraphErrors *gsigmaI900 = new TGraphErrors();
	
	int A[4] = {12, 14, 15, 16};
	for(int i = 0; i < 4; i++){
		gsigmaR83->SetPoint(i, A[i], sigmaR83[i].val);
		gsigmaR83->SetPointError(i, 0., sigmaR83[i].err);
	}
	for(int i = 0; i < 5; i++){
		gsigmaI240->SetPoint(i, 12+i, sigmaI240[i].val);
		gsigmaI240->SetPointError(i, 0., sigmaI240[i].err);

		gsigmaI900->SetPoint(i, 12+i, sigmaI900[i].val);
		gsigmaI900->SetPointError(i, 0., sigmaI900[i].err);
	} // end of loop over carbon isotopes
	gsigmaR83->SetNameTitle("gsigmaR83_0", "reaction and interaction c.s.");
	gsigmaI240->SetNameTitle("gsigmaI240_0", "interaction c.s.");
	gsigmaI900->SetNameTitle("gsigmaI900_0", "interaction c.s.");
	gsigmaR83->SetLineStyle(2);
	gsigmaI240->SetLineStyle(2);
	gsigmaI900->SetLineStyle(2);
	
	gsigmaR83->SetLineWidth(2); gsigmaR83->SetLineColor(1); gsigmaR83->SetMarkerColor(1); gsigmaR83->SetMarkerStyle(22);
	gsigmaI240->SetLineWidth(2); gsigmaI240->SetLineColor(2); gsigmaI240->SetMarkerColor(1); gsigmaI240->SetMarkerStyle(23);
	gsigmaI900->SetLineWidth(2); gsigmaI900->SetLineColor(3); gsigmaI900->SetMarkerColor(1); gsigmaI900->SetMarkerStyle(24);
	
	// draw the graphs //
	TCanvas *c = new TCanvas("c_0", "c_0", 800, 600);
	TLegend *lg = new TLegend(0.3, 0.5, 0.9, 0.9);
	lg->SetName("lg_0");
	c->SetGrid();


	TGraph *g = gsigmaR83;
	g->SetTitle("Reaction and Interaction C.S. for C->C12 @240MeV/u");
	g->SetMinimum(650); g->SetMaximum(1650.);
	g->GetXaxis()->SetTitle("A");
	g->GetYaxis()->SetTitle("\\sigma [mb]");
	g->GetYaxis()->SetTitleOffset(1.4);
	g->Draw("APL");
	gsigmaI240->Draw("PLsame");
	gsigmaI900->Draw("PLsame");

	lg->AddEntry(gsigmaR83, "83MeV/u, Reaction C.S. - Old");
	lg->AddEntry(gsigmaI240, "240MeV/u, Interaction C.S. - Old");
	lg->AddEntry(gsigmaI900, "900MeV/u, Interaction C.S. - Old");
	lg->SetY1(0.7);
	lg->Draw();

	c->Print("sigmaIC_0.pdf");
	c->Print("sigmaIC_0.root");

	TFile *f = new TFile("sigmaIC_0.root", "update");
	g->Write("", TObject::kOverwrite);
	gsigmaI240->Write("", TObject::kOverwrite);
	gsigmaI900->Write("", TObject::kOverwrite);
	lg->Write("", TObject::kOverwrite);
	f->Close();
	system("cp sigmaIC_0.root ..");
} // end of the main function



