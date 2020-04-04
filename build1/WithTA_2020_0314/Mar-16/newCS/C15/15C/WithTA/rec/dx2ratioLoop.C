// dx2ratioLoop.C -- Loop over dx2 ranges and claculate yield of the residue carbon isotopes 
// of 12C(16C, X)anything. X is 16-13C.

void dx2ratio(const double *dx2, const bool sGap, double *nC, double *ratioC);

void dx2ratioLoop(){
	const bool sGap = true;
	const double dx2_0[2] = {-10., -2.}, dx2_1[2] = {-1.5, 8.};
	const int dx2_nStep = 20;
	const double dx2_0Step = (dx2_0[1] - dx2_0[0]) / dx2_nStep;
	const double dx2_1Step = (dx2_1[1] - dx2_1[0]) / dx2_nStep;
	double dx2[2]{}; // dx2 range

	// array to cache results of each looop
	const int size = 4;
	double *nC = new double[size];
	double *ratioC = new double[size];
	// graph to visualize the result
	TGraph2D *g2[size]; // C16-15-14-13
	for(int i = 4; i--;) g2[i] = new TGraph2D();
	g2[0]->SetNameTitle("gdx2RatioC16", "ratio v.s. dx2 distributions - C16");
	g2[1]->SetNameTitle("gdx2RatioC15", "ratio v.s. dx2 distributions - C15");
	g2[2]->SetNameTitle("gdx2RatioC14", "ratio v.s. dx2 distributions - C14");
	g2[3]->SetNameTitle("gdx2RatioC13", "ratio v.s. dx2 distributions - C13");

	// Loop begins
	int point[4]{};
	for(int i = 0; i <= dx2_nStep; i++){
		dx2[0] = dx2_0[0] + i * dx2_0Step;
		for(int j = 0; j <= dx2_nStep; j++){
			dx2[1] = dx2_1[0] + j * dx2_1Step;
			dx2ratio(dx2, sGap, nC, ratioC);
			for(int k = 1; k < size; k++)
				g2[k]->SetPoint(point[k]++, dx2[0], dx2[1], ratioC[k]);
			cout << "\n\n----------- i: " << i << " &&&&&& j: " << j << " ----------------\n\n";
		} // end for over j
	} // end for over i
	
	TFile *f = new TFile("dx2_.root", "recreate");
	for(auto g : g2) g->Write("", TObject::kOverwrite);
	f->Close();
	delete f;
	delete [] nC;
	delete [] ratioC;
	for(auto g : g2) delete g;
}


//sGap: true-> residues in strip gaps will not be included in the PID spectrum;
// nC: number of residue carbon isotopes; ratioC: nC / nC16, where C16 is the projectile
void dx2ratio(const double *dx2, const bool sGap, double *nC, double *ratioC){
	TFile *f = new TFile("~/pionExp2017/build/C15TA.root", "update"); // 0920 0508
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treePDCPileUp = (TTree*)f->Get("treePDCPileUp");
	TTree *treeDCRPileUp = (TTree*)f->Get("treeDCRPileUp");
	TTree *treePID3D = (TTree*)f->Get("treePID3D");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeTOFWR);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treePDCPileUp);
	treeTrack->AddFriend(treeDCRPileUp);
	treeTrack->AddFriend(treePID3D);
	treeTrack->AddFriend(vme);

	// stored cuts //
	vector<const char*> nclLs = {"C16", "C15", "C14", "C13"};
	vector<TCutG *> cutgLs;
	TCutG *A0 = (TCutG*)f->Get("A0");
	TCutG *B0 = (TCutG*)f->Get("B0");
	TCutG *C0 = (TCutG*)f->Get("C0");
	TCutG *B1 = (TCutG*)f->Get("B1");
	TCutG *B2 = (TCutG*)f->Get("B2");
	TCutG *B3 = (TCutG*)f->Get("B3");
	TCutG *stripGap = (TCutG*)f->Get("stripgap");
	const int size = nclLs.size();
	for(auto cut : nclLs){
		cutgLs.push_back((TCutG*)f->Get(cut));
	}

//	TCanvas *c = new TCanvas("c", "c", 800, 600);
//	c->SetGrid();

	ostringstream cut("");
	cut << "dsca11==0&&A0&&B0&&C0&&(dx2[0]>" << dx2[0] << "&&dx2[0]<" << dx2[1] << ")";
	if(sGap) cut << "&&!stripgap";

	string cutDraw = cut.str() + ";aoz;dE1 arb.;";
	cout << "cut: " << cut.str() << "\ncutDraw: " << cutDraw << endl;
//	treeTrack->Draw("dE1:aoz[0]>>h(500, 1.8, 3.7, 500, 0., 4.5)", cutDraw.c_str(), "colbox");

	// output the carbon isotope yields
	cut << "&&";
	for(int i = 0; i < size; i++){
		string cutC = cut.str() + nclLs[i];
		cout << "\033[33;1m" << cutC << "\033[0m" << endl;
		nC[i] = treeTrack->GetEntries(cutC.c_str());
		ratioC[i] = nC[i] / nC[0];
		cout << nclLs[i] << ": " << nC[i] << ", " << ratioC[i] << endl;
	}
	
	f->Close();
	delete f;
}




