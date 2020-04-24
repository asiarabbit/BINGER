void softEff(){
	TFile *f = new TFile("~/pionExp2017/build1/O18TA.root", "update");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treePDCPileUp = (TTree*)f->Get("treePDCPileUp");
	TTree *treeDCRPileUp = (TTree*)f->Get("treeDCRPileUp");
//	TTree *treePID3D = (TTree*)f->Get("treePID3D");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeTOFWR);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treePDCPileUp);
	treeTrack->AddFriend(treeDCRPileUp);
//	treeTrack->AddFriend(treePID3D);
	treeTrack->AddFriend(vme);


//	const char *cutOpt[] = {"dE1>2.65", "dE1>2.2&&dE1<2.65", "dE1>1.7&&dE1<2.15", "dE1>1.2&&dE1<1.6"}; // [0-3]: [O-N-C-B]
	const char *cutOpt[] = {"dE1>1.5", "dE1>2.2&&dE1<2.65", "dE1>1.7&&dE1<2.15", "dE1>1.2&&dE1<1.6"}; // [0-3]: [O-N-C-B]
	string cut = "aoz[0]>0&&";
	cut += cutOpt[0];
	const double naoz = treeTrack->GetEntries(cut.c_str());
	cout << "fileName: " << f->GetName() << "\t";
	cout << "n: " << treeTrack->GetEntries() << "\taoz: " << naoz << endl;;
	double eff[6] = { // DC#-X1X2
		treeTrack->GetEntries(string(cut+"&&nu[0][0]>=0").c_str())/naoz,
		treeTrack->GetEntries(string(cut+"&&nu[0][1]>=0").c_str())/naoz,
		treeTrack->GetEntries(string(cut+"&&nu[0][2]>=0").c_str())/naoz,
		treeTrack->GetEntries(string(cut+"&&nu[0][3]>=0").c_str())/naoz,
		treeTrack->GetEntries(string(cut+"&&nu[0][4]>=0").c_str())/naoz,
		treeTrack->GetEntries(string(cut+"&&nu[0][5]>=0").c_str())/naoz
	};

	for(double x : eff) cout << x << '\t'; cout << endl;
	// calculate the total DC efficiency from DC software effciency //
	// nFiredAnodeLayer = 6
	const double effT6 = eff[0]*eff[1]*eff[2]*eff[3]*eff[4]*eff[5];
	// nFiredAnodeLayer = 5
	double effT5 = 0.;
	for(int i = 6; i--;) effT5 += effT6/eff[i]*(1. - eff[i]);
	// nFiredAnodeLayer = 4
	double effT4 = 0.;
	for(int i = 0; i < 5; i++) for(int j = i + 1; j < 6; j++) effT4 += effT6/(eff[i]*eff[j])*(1. - eff[i])*(1. - eff[j]);
	// nFiredAnodeLayer = 3
	double effT3 = 0.;
	for(int i = 2; i--;) for(int j = 2; j--;) for(int k = 2; k--;)
		effT3 += eff[i]*(1.-eff[1-i])*eff[2+j]*(1.-eff[3-j])*eff[4+k]*(1.-eff[5-k]);
	// the total efficiency
	const double effT = effT6 + effT5 + effT4 + effT3;
	
	
	// output the final result
	cout << "cut:\033[32;1m " << cut << "\033[0m\neff: \033[1m" << endl;
	for(double x : eff) cout << x << "\t";
	cout << endl;
	cout << "\033[0mThe total efficiency: \033[31;1m" << effT << "\033[0m" << endl;
//	cout << "\033[33;1mDONE\033[0m\n\n" << endl;
}



