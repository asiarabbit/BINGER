// dx2ratio.C -- this scriptlet is to claculate yield of the residue carbon isotopes 
// of 12C(16C, X)anything. X is 16-13C.

const double dx2[2] = {-2.5, -1.3};
const bool sGap = true; // true: residues in strip gaps will not be included in the PID spectrum;

void dx2ratio(){
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

	TCanvas *c = new TCanvas("c", "c", 800, 600);
	c->SetGrid();

	ostringstream cut("");
	cut << "dsca11==0&&A0&&B0&&C0&&(dx2[0]>" << dx2[0] << "&&dx2[0]<" << dx2[1] << ")";
	if(sGap) cut << "&&!stripgap";

	string cutDraw = cut.str() + ";aoz;dE1 arb.;";
	cout << "cut: " << cut.str() << "\ncutDraw: " << cutDraw << endl;
	treeTrack->Draw("dE1:aoz[0]>>h(500, 1.8, 3.7, 500, 0., 4.5)", cutDraw.c_str(), "colbox");

	// output the carbon isotope yields
	double *nC = new double[size];
	double *ratioC = new double[size];
	cut << "&&";
	for(int i = 0; i < size; i++){
		string cutC = cut.str() + nclLs[i];
		cout << "\033[33;1m" << cutC << "\033[0m" << endl;
		nC[i] = treeTrack->GetEntries(cutC.c_str());
		ratioC[i] = nC[i] / nC[0];
		cout << nclLs[i] << ": " << nC[i] << ", " << ratioC[i] << endl;
	}
}




