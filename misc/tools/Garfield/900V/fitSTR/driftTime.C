// driftTime.C -- extract STRs from garfield simulation results.

const double DEGREE = 3.14159265358 / 180.;
int GetSTRid(double k);

void driftTime(){
	TFile *file = new TFile("garfield.root", "update");
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

	char name[64], title[128];
	char range[24][64] = {
		"(-nan, -55)", "(-55, -50)", "(-50, -45)", "(-45, -40)", // 0-3
		"(-40, -35)", "(-35, -30)", "(-30, -25)", "(-25, -20)", // 4-7
		"(-20, -15)", "(-15, -10)", "(-10, -5)", "(-5, 0)", // 8-11 **9, 10, 11**
		"(0, 5)", "(5, 10)", "(10, 15)", "(15, 20)", // 12-15 **12, 13, 14**
		"(20, 25)", "(25, 30)", "(30, 35)", "(35, 40)", // 16-19
		"(40, 45)", "(45, 50)", "(50, 55)", "(55, nan)" // 20-23
	};
	TH1F *hdca = new TH1F("hdca", "Distance of Closest Approach;dca/mm", 500, -0.1, 8.);
	TH1F *hdriftTime = new TH1F("hdriftTime", "Drift Time Distribution;t/ns", 500, -100., 400.);
	TH2F *hSTR[24]; // [STR_id]
	for(int i = 0; i < 24; i++){
		sprintf(name, "hSTR_%d", i);
		sprintf(title, "Space(dca)-Time Relation for angle %s;t/ns;r/mm", range[i]);
		hSTR[i] = new TH2F(name, title, 500, -100., 400., 500, -0.1, 8.);
	}

	const int nCells = 8; // have to be even
	const int hnCells = nCells/2;
	double sx[8]; // abscissas of sense wires
	sx[0] = 0.; sx[nCells-1] = hnCells;
	for(int i = 1; i < hnCells; i++){
		sx[2*i-1] = i*1.;
		sx[2*i] = -i*1.;
	}
	const int n = tree->GetEntries();
	int anode_id = -1, STR_id = -1;
	for(int i = 0; i < n; i++){
		tree->GetEntry(i);
		double dca = dca_tree*10., dt = driftTime_tree;
		
		int id = layerId_tree*5+anodeId_tree;
		double sx_t = sx[anodeId_tree], sy_t = -0.5; // X2
		if(layerId_tree == 0){ // X1
			sx_t += 0.4; sy_t += 1.;
		}
		double dca_c = (kl_tree*sx_t-sy_t+bl_tree)/sqrt(1.+kl_tree*kl_tree);
//		cout << "dca_c/dca: " << dca_c/dca_tree << endl; getchar();
//		if(dca_c > 0.) // id == 0 && 
//		if(layerId_tree == 1)
		{
			int id = GetSTRid(kl_tree);
			hdca->Fill(dca); // mm
			if(id >= 9 && id <= 14) hdriftTime->Fill(dt); //  
			hSTR[id]->Fill(dt, dca); // GetSTRid(kl_tree)
		} // end if
	} // end for over entries

	char dirName[] = "anode0";
	if(!file->FindObjectAny(dirName)) file->mkdir(dirName);
	file->cd(dirName);
	hdca->Write("", TObject::kOverwrite);
	hdriftTime->SetTitle("Drift Time Distribution(Global) 1000V;t/ns");
	hdriftTime->Write("", TObject::kOverwrite);
	for(TH2F *&h : hSTR) h->Write("", TObject::kOverwrite);
	file->cd("/");
	hdriftTime->SetName("Hdt0"); hdriftTime->Write("", TObject::kOverwrite);
	hdriftTime->SetName("Hdt1"); hdriftTime->Write("", TObject::kOverwrite);
	hdriftTime->SetName("Hdt2"); hdriftTime->Write("", TObject::kOverwrite);

	TCanvas *c = new TCanvas("c", "show", 800, 600);
	hdriftTime->Draw();
	c->SaveAs("driftTime_Global_1000V.pdf");
//	file->Close();
} // end the main function

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








