// eff.C -- calculate efficiency

void rate(double N1, double N2, double *pp);
void errDiv(double x1, double dx1, double x2, double dx2, double *pp);
void errProd(double x1, double dx1, double x2, double dx2, double *pp);

void effF(){
	TFile *f = new TFile("~/pionExp2017/build1/O18TA.root", "update"); // 2052 1848 1649 0920
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeTOFWR = (TTree*)f->Get("treeTOFWR");
	TTree *treeMulti = (TTree*)f->Get("treeMulti");
	TTree *treePDCPileUp = (TTree*)f->Get("treePDCPileUp");
	TTree *treeDCPileUp = (TTree*)f->Get("treeDCPileUp");
	TTree *treePID3D = (TTree*)f->Get("treePID3D");
	TTree *vme = (TTree*)f->Get("vme");
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeTOFWR);
	treeTrack->AddFriend(treeMulti);
	treeTrack->AddFriend(treePDCPileUp);
	treeTrack->AddFriend(treeDCPileUp);
	treeTrack->AddFriend(treePID3D);
	treeTrack->AddFriend(vme);

	// cuts //
	TCutG *A0 = (TCutG*)f->Get("A0");
	TCutG *B0 = (TCutG*)f->Get("B0");


	double n0 = treeTrack->GetEntries(), n0aoz = treeTrack->GetEntries("aoz[0]>0");
	cout << "treeTrack->GetEntries(): " << n0 << endl;
	cout << "treeTrack->GetEntries(\"aoz[0]>0\"): " << n0aoz << endl;
	cout << "aoz rate: \033[31;1m" << n0aoz / n0 << "\033[m" << endl;
	// selected events
	n0 = treeTrack->GetEntries("dsca11==0&&A0&&B0"), n0aoz = treeTrack->GetEntries("dsca11==0&&A0&&B0&&aoz[0]>0");
	cout << "____treeTrack->GetEntries(\"dsca11==0&&A0&&B0\"): " << n0 << endl;
	cout << "treeTrack->GetEntries(\"dsca11==0&&A0&&B0&&aoz[0]>0\"): " << n0aoz << endl;
	cout << "aoz rate: \033[31;1m" << n0aoz / n0 << "\033[m" << endl;

	// the general cut
	string cut0 = "1"; // "(A0t0||A0t1)&&B0&&B2&&dsca11==0"; // 
	const char *cutOpt[] = {"dE1>1.50"};
	cut0 = cutOpt[0];

	string cut1, cut2; // temporary varibles
	double n1, n2; // temporary variables
	double p[4]{}; // to pass result

	cout << endl << "PDC detection effciency - e_eP: " << endl;
	cut1 = "dsca11==0&&A0&&B0&&ntrLs[1][0]==1&&" + cut0;
	cut2 = "dsca11==0&&A0&&B0&&ntrLs[1][0]==1&&ntrLs[3][0]==1&&" + cut0;
	cout << "cut1: " << cut1 << endl;
	n1 = treeTrack->GetEntries(cut1.c_str());
	n2 = treeTrack->GetEntries(cut2.c_str());
	rate(n1, n2, &p[0]);

	cout << endl << "DCR detection effciency - e_eR: " << endl;
	cut1 = "dsca11==0&&A0&&B0&&ntrLs[3][0]==1&&" + cut0;
	cut2 = "dsca11==0&&A0&&B0&&ntrLs[3][0]==1&&ntrLs[1][0]==1&&" + cut0;
	n1 = treeTrack->GetEntries(cut1.c_str());
	n2 = treeTrack->GetEntries(cut2.c_str());
	rate(n1, n2, &p[2]);
	cout << endl;

	double eff_Final[2];
	errProd(p[0], p[1], p[2], p[3], eff_Final);
	cout << "Detecting efficiency: \033[32;1m" << eff_Final[0] << "(" << eff_Final[1] << ")\033[0m" << endl;
	cout << endl;

	double pdx2[4]{};
	cout << "dx2 efficiency - e_dx2 (-6, 2): " << endl;
	cut1 = "dsca11==0&&A0&&B0&&dx2[0]!=-9999.&&" + cut0;
	cut2 = "dsca11==0&&A0&&B0&&(dx2[0]>-6&&dx2[0]<2)&&" + cut0;
	n1 = treeTrack->GetEntries(cut1.c_str());
	n2 = treeTrack->GetEntries(cut2.c_str());
	rate(n1, n2, &pdx2[0]);
	cout << endl;

	cout << "dx2 efficiency - e_dx2 (-9, 4): " << endl;
	cut1 = "dsca11==0&&A0&&B0&&dx2[0]!=-9999.&&dsca4==1&&" + cut0;
	cut2 = "dsca11==0&&A0&&B0&&(dx2[0]>-9&&dx2[0]<4)&&dsca4==1&&" + cut0;
	n1 = treeTrack->GetEntries(cut1.c_str());
	n2 = treeTrack->GetEntries(cut2.c_str());
	rate(n1, n2, &pdx2[2]);
	cout << endl;

	// penetration rate
	double pPEN[4];
	cout << "Penetration rate: " << endl;
	cut1 = "dsca11==0&&A0&&B0";
	cut2 = "dsca11==0&&A0&&B0&&aoz[0]>0&&" + cut0;
	n1 = treeTrack->GetEntries(cut1.c_str());
	n2 = treeTrack->GetEntries(cut2.c_str());
	rate(n1, n2, &pPEN[0]);
	errDiv(pPEN[0], pPEN[1], eff_Final[0], eff_Final[1], &pPEN[2]);
	cout << "Penetration rate: \033[33;1m" << pPEN[2] << "(" << pPEN[3] << ")\033[0m" << endl;
	cout << endl;
}


void rate(double N1, double N2, double *pp){ // pp: store result
    const double rate = N2 / N1;
    const double rel = sqrt(1. / N1 + 1. / N2);
    const double sigma = rate * rel;

//	cout << "N1: " << N1 << "\tN2: " << N2 << endl;
//  cout << "relative error: " << rel << endl;
	cout << rate << "(" << sigma << ")" << endl;

    pp[0] = rate; pp[1] = sigma;
}

void errDiv(double x1, double dx1, double x2, double dx2, double *pp){
	const double rate = x1/x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = rate * rel;

//	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
//	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
//	cout << "relative error: " << rel << endl;
	cout << rate << "(" << sigma << ")" << endl;

    pp[0] = rate; pp[1] = sigma;
}

void errProd(double x1, double dx1, double x2, double dx2, double *pp){
	const double prod = x1*x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = prod * rel;

//	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
//	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
//  cout << "relative error: " << rel << endl;
	cout << prod << "(" << sigma << ")" << endl;

    pp[0] = prod; pp[1] = sigma;
}











