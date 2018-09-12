// eff.C -- calculate efficiency

void rate(double N1, double N2, double *pp);
void errDiv(double x1, double dx1, double x2, double dx2);
void errProd(double x1, double dx1, double x2, double dx2);

void eff(){
	TFile *f = new TFile("20180707_2052.dat_0.root", "update"); // 2052 1848 1649
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


	cout << "treeTrack->GetEntries(): " << treeTrack->GetEntries() << endl;
	cout << "treeTrack->GetEntries(aoz>0): " << treeTrack->GetEntries("aoz>0") << endl;

	// the general cut
	string cut0 = "1"; // "(A0t0||A0t1)&&B0&&B2&&dsca11==0"; // 
	cut0 += "&&dE1>1.5"; // O
//	cut0 += "&&dE1>1.2&&dE1<1.5"; // N
//	cut0 += "&&dE1>0.93&&dE1<1.2"; // C
//	cut0 += "&&dE1>0.7&&dE1<0.93"; // B
	double n1, n2;
	double p[4]{}; // to pass result

	cout << endl << "e_eP: " << endl;
	string cut1 = "ntrLs[1][0]==1&&" + cut0;
	string cut2 = "ntrLs[1][0]==1&&ntrLs[3][0]==1&&" + cut0;
	n1 = treeTrack->GetEntries(cut1.c_str());
	n2 = treeTrack->GetEntries(cut2.c_str());
	rate(n1, n2, &p[0]);

	cout << endl << "e_eR: " << endl;
	string cut3 = "ntrLs[3][0]==1&&" + cut0;
	string cut4 = "ntrLs[3][0]==1&&ntrLs[1][0]==1&&" + cut0;
	n1 = treeTrack->GetEntries(cut3.c_str());
	n2 = treeTrack->GetEntries(cut4.c_str());
	rate(n1, n2, &p[2]);
	cout << endl;

	errProd(p[0], p[1], p[2], p[3]);
}


void rate(double N1, double N2, double *pp){ // pp: store result
    const double rate = N2 / N1;
    const double rel = sqrt(1. / N1 + 1. / N2);
    const double sigma = rate * rel;

	cout << "N1: " << N1 << "\tN2: " << N2 << endl;
    cout << "relative error: " << rel << endl;
    cout << rate << "(" << sigma << ")" << endl;

    pp[0] = rate; pp[1] = sigma;
}

void errDiv(double x1, double dx1, double x2, double dx2){
	const double rate = x1/x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = rate * rel;

	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
    cout << "relative error: " << rel << endl;
    cout << rate << "(" << sigma << ")" << endl;
}

void errProd(double x1, double dx1, double x2, double dx2){
	const double prod = x1*x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = prod * rel;

	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
    cout << "relative error: " << rel << endl;
    cout << prod << "(" << sigma << ")" << endl;
}











