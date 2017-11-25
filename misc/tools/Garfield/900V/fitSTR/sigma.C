// sigma.C -- spatial resolution of MWDC
const double DEGREE = 3.14159265358 / 180.;
int GetSTRid(double k);

void sigma(){
	TFile *file = new TFile("garfield.root", "update");
	// just for retrieving the bin center value of dca
	TH1F *hdca = (TH1F*)file->Get("anode0/hdca");
	TTree *tree = (TTree*)file->Get("tree");
	int layerId_tree, anodeId_tree, driftLineCnt_tree; // to specify the hit anode
	double dca_tree, driftTime_tree, x_ionization_tree, kl_tree, bl_tree;
	tree->SetBranchAddress("layerId", &layerId_tree);
	tree->SetBranchAddress("anodeId", &anodeId_tree);
	tree->SetBranchAddress("dca", &dca_tree);
	tree->SetBranchAddress("t", &driftTime_tree);
	tree->SetBranchAddress("x", &x_ionization_tree);
	tree->SetBranchAddress("kl", &kl_tree);
	tree->SetBranchAddress("bl", &bl_tree);
	tree->SetBranchAddress("DLcnt", &driftLineCnt_tree);

	TF1 *fSTR[6]; TGraph *gSTR[6];
	fSTR[0] = ((TGraph*)file->Get("anode0/gSTR_9"))->GetFunction("fSTR_9");
	fSTR[1] = ((TGraph*)file->Get("anode0/gSTR_10"))->GetFunction("fSTR_10");
	fSTR[2] = ((TGraph*)file->Get("anode0/gSTR_11"))->GetFunction("fSTR_11");
	fSTR[3] = ((TGraph*)file->Get("anode0/gSTR_12"))->GetFunction("fSTR_12");
	fSTR[4] = ((TGraph*)file->Get("anode0/gSTR_13"))->GetFunction("fSTR_13");
	fSTR[5] = ((TGraph*)file->Get("anode0/gSTR_14"))->GetFunction("fSTR_14");
	
	
	TH2F *hsigma[6];
	hsigma[0] = new TH2F("hsigma_9", "Spatial Resoution (r(t)-dca) - dca (-15, -10) \\degree;dca/mm;\\sigma/mm", 500, -0.1, 8, 100, -0.8, 0.8);
	hsigma[1] = new TH2F("hsigma_10", "Spatial Resoution (r(t)-dca) - dca (-10, -5) \\degree;dca/mm;\\sigma/mm", 500, -0.1, 8, 100, -0.8, 0.8);
	hsigma[2] = new TH2F("hsigma_11", "Spatial Resoution (r(t)-dca) - dca (-5, 0) \\degree;dca/mm;\\sigma/mm", 500, -0.1, 8, 100, -0.8, 0.8);
	hsigma[3] = new TH2F("hsigma_12", "Spatial Resoution (r(t)-dca) - dca (0, 5) \\degree;dca/mm;\\sigma/mm", 500, -0.1, 8, 100, -0.8, 0.8);
	hsigma[4] = new TH2F("hsigma_13", "Spatial Resoution (r(t)-dca) - dca (5, 10) \\degree;dca/mm;\\sigma/mm", 500, -0.1, 8, 100, -0.8, 0.8);
	hsigma[5] = new TH2F("hsigma_14", "Spatial Resoution (r(t)-dca) - dca (10, 15) \\degree;dca/mm;\\sigma/mm", 500, -0.1, 8, 100, -0.8, 0.8);

	// fill the deviation histograms
	const int n = tree->GetEntries();
	for(int i = 0; i < n; i++){
		tree->GetEntry(i);
		int id = GetSTRid(kl_tree) - 9; if(id > 5 || id < 0) continue;
		double dca = dca_tree*10., dt = driftTime_tree; // mm, ns
		double r = fSTR[id]->Eval(dt);
		hsigma[id]->Fill(dca,  dca-r);
	} // end for over i

	// evaluate the sigma and mean of every dca subsection
	TGraph *gsigma[6]; int gsigma_cnt[6]{};
	TGraph *gmean[6]; int gmean_cnt[6]{};
	char name1[64], name2[64]; string title1, title2;
	for(int i = 0; i < 6; i++){
		sprintf(name1, "gsigma_%d", i+9);
		title1 = hsigma[i]->GetTitle(); title1 += name1;
		gsigma[i] = new TGraph(); gsigma[i]->SetMarkerStyle(21); // solid square
		gsigma[i]->SetNameTitle(name1, title1.c_str());

		sprintf(name2, "gmean_%d", i+9);
		title2 = hsigma[i]->GetTitle(); title2 += name2;
		gmean[i] = new TGraph(); gmean[i]->SetMarkerStyle(22); // solid circle
		gmean[i]->SetNameTitle(name2, title2.c_str());
	}
	
	TF1 *fgaus = new TF1("fgaus", "gaus", -4., 4.);
	for(int i = 0; i < 6; i++){ // loop over six STRs
		for(int j = 0; j < 25; j++){
			TH1D *hproj = hsigma[i]->ProjectionY("hproj", j*20+1, j*20+1+10);
			if(hproj->GetEntries() < 30) continue;
			// confine the parameters to facilitate the fitting
			fgaus->SetParameter(1, 0.);
			fgaus->SetParLimits(1, -2., 2.); // mean, unit: mm
			fgaus->SetParameter(2, 0.1);
			fgaus->SetParLimits(2, 0., 2.); // sigma, unit: mm

			hproj->Fit(fgaus, "NQR");
			if(j*20+10 > 500) continue;
			double rm = hdca->GetBinCenter(j*20+1+10); // rm: mm
			double mean = fgaus->GetParameter("Mean");
			double sigma = fgaus->GetParameter("Sigma");
//			if(fabs(mean) > 0.5 || fabs(sigma) > 0.6) continue;
//			cout << "mean: " << mean << "\tsigma: " << sigma << endl; getchar(); // DEBUG
			gsigma[i]->SetPoint(gsigma_cnt[i]++, rm, sigma);
			gmean[i]->SetPoint(gmean_cnt[i]++, rm, mean);
			hproj->~TH1D();
//			cout << "Fitting histogram of STR " << i << " and bin " << j << "\r" << flush;
		} // end for over j
	} // end for over i

	file->cd("anode0");
	for(TH2F *&h : hsigma) h->Write("", TObject::kOverwrite);
	for(TGraph *&g : gsigma) g->Write("", TObject::kOverwrite);
	for(TGraph *&g : gmean) g->Write("", TObject::kOverwrite);

	file->Close();
} // end function sigma

// -nan     -10,     -5,     0,     5,     10,     nan
//      [0]      [1]     [2]    [3]   [4]     [5]    
int GetSTRid(double k){
	k = -1./k;
	double theta = atan(k) / DEGREE;
	if(theta <= -55.) return 0;
	else if(theta > 55.) return 23;
	else{
		for(int i = 0; i < 22; i++){
			if(theta > i*5-55. && theta <= i*5-50.)
				return i + 1;
		}
	}
	cout << "Error in GetSTRid: k out of range.\n";
	getchar();
	return -1;
} // end of function GetSTRid


