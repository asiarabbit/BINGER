// eff.C -- calculate efficiency

void rate(double N1, double N2, double *pp);
void errDiv(double x1, double dx1, double x2, double dx2, double *pp);
void errProd(double x1, double dx1, double x2, double dx2, double *pp);
void errSqrt(double x, double dx, double *pp);

const char *dE1Cut = "dE1>2.113";

void eff(){
	TFile *f = new TFile("~/pionExp2017/build/C13NOTA.root", "update"); // 2052 1848 1649 0920
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
	TCutG *C0 = (TCutG*)f->Get("C0");
	TCutG *C16Ori = (TCutG*)f->Get("C13Ori");


	string cut0 = "dsca11==0&&A0&&B0&&C0";
	string cut1 = "dsca11==0&&A0&&B0&&C0&&aoz>0.";
//	cut1 += "&&"; cut1 += dE1Cut;
	const int n1 = treeTrack->GetEntries(cut0.c_str());
	const int n2 = treeTrack->GetEntries(cut1.c_str());
	cout << "cut0: " << cut0 << endl;
	cout << "cut1: " << cut1 << endl;

	double p[2];
	rate(n1, n2, p);
	cout << "\033[1mDetecting rate using NOTA run: ";
	cout << p[0] << "(" << p[1] << ")" << "\n\033[0m";


	// include -p products //
	cout << "\n\n----------- include -p products $$$$$$$$$$$$$$$ ------------------\n";
	cut1 += "&&"; cut1 += dE1Cut;
	const int n3 = treeTrack->GetEntries(cut0.c_str());
	const int n4 = treeTrack->GetEntries(cut1.c_str());
	cout << "cut0: " << cut0 << endl;
	cout << "cut1: " << cut1 << endl;
	rate(n3, n4, p);
	cout << "\033[33;1mDetecting rate using NOTA run including -p products: ";
	cout << p[0] << "(" << p[1] << ")" << "\n\033[0m";

	// include -p products, and exclude -p incident nuclei -> the correct one //
	cout << "\n\n----------- include -p products $$$$$$$$$$$$$$$ ------------------\n";
	cut0 += "&&"; cut0 += dE1Cut;
	const int n5 = treeTrack->GetEntries(cut0.c_str());
	const int n6 = treeTrack->GetEntries(cut1.c_str());
	cout << "cut0: " << cut0 << endl;
	cout << "cut1: " << cut1 << endl;
	rate(n5, n6, p);
	cout << "\033[33;1mDetecting rate using NOTA run including -p products: ";
	cout << p[0] << "(" << p[1] << ")" << "\n\033[0m";
}


void rate(double N1, double N2, double *pp){ // pp: store result
    const double rate = N2 / N1;
    const double rel = sqrt(1. / N1 + 1. / N2);
    const double sigma = rate * rel;

//	cout << "N1: " << N1 << "\tN2: " << N2 << endl;
//  cout << "relative error: " << rel << endl;
	cout << N1 << "/" << N2 << ": ";
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
//  cout << "relative error: " << rel << endl;
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

void errSqrt(double x, double dx, double *pp){
	pp[0] = sqrt(x);
	pp[1] = dx / (2.*pp[0]);
	cout << "sqrt, " << pp[0] << "(" << pp[1] << ")" << endl;
}



