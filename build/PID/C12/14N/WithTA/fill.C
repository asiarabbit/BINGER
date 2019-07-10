void fill(){
	TH1F *h1 = new TH1F("h1", "h1", 500, -10., 10.);
	TH1F *h2 = new TH1F("h2", "h2", 500, -10, 10.);
	TH1F *h3 = new TH1F("h3", "h3", 500, -10, 10.);
	h2->SetLineColor(2);
	h3->SetLineColor(1);
	h3->SetFillColor(kYellow);

	double sigma = 0.4;
	double miu0 = 0., miu1 = 1.;

	const int n = 1000000;
	TRandom3 r;
	for(int i = 0; i < n; i++){
		double s = 0;
		for(int j = 0; j < 12; j++) s += r.Uniform();
		double y = s - 6.;
		double x1 = sigma*y + miu0;
		double x2 = sigma*y + miu1;
		h1->Fill(x1); h3->Fill(x1);
		h2->Fill(x2); h3->Fill(x2);
	}

	TCanvas *c = new TCanvas("c", "c", 800, 600);
	h3->Draw();
	h2->Draw("same");
	h1->Draw("same");
}
