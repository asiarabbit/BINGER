// sys.C -- This code is to verify the repercurssion of detector misalignment to rigidity resolution

const double u0MeV = 931.494; // MeV
const double c0 = 299.792458; // mm/ns

void sys(){
	const double B = 1.2004; // Tesla
	const double L = 11000.; // flight path, mm
	const double z0 = 7;
	const double aoz[4] = {14/z0, 15/z0, 16/z0, 17/z0}; // nitrogen isotopes
	TTree *tree = new TTree("tree", "system error to aoz resolution");
	double aozReal, aozCal, rhoReal, rhoCal, betaReal, betaCal, gammaReal, gammaCal;
	double tofReal, tofMeas, pozReal, pozCal;
	tree->Branch("aozReal", &aozReal);
	tree->Branch("betaReal", &betaReal);
	tree->Branch("gammaReal", &gammaReal);
	tree->Branch("pozReal", &pozReal);
	tree->Branch("tofReal", &tofReal);
	tree->Branch("tofMeas", &tofMeas);
	tree->Branch("aozCal", &aozCal);
	tree->Branch("betaCal", &betaCal);
	tree->Branch("gammaCal", &gammaCal);
	tree->Branch("pozCal", &pozCal);

	TRandom3 r;
	const int n = 10000;
	for(int i = 0; i < n; i++){

//		fAoZ = B * (rho/1000.) * 0.321840605 / (fBeta * fGamma);
//		fPoZ = B * (rho/1000.) * 0.321840605 * u0MeV;

		aozReal = aoz[int(r.Uniform(10))%4];
		betaReal = r.Gaus(0.62, 0.01);
		gammaReal = 1./sqrt(1-betaReal*betaReal);
		pozReal = aozReal*betaReal*gammaReal*u0MeV; // MeV/c
		rhoReal = pozReal/(B*0.321840605*u0MeV)*1000.; // mm
		
		const double ofst = 1.; // ns
		tofReal = L / (betaReal*c0);
		tofMeas = tofReal + ofst;
		betaCal = L / tofMeas / c0;
		gammaCal = 1./sqrt(1.-betaCal*betaCal);
		rhoCal = rhoReal;
		pozCal = B*(rhoCal/1000.)*0.321840605*u0MeV;
		aozCal = pozCal / u0MeV / (betaCal*gammaCal);
		
		tree->Fill();
	} // end for over i
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	tree->Draw("aozCal");

	TFile *f = new TFile("a.root", "RECREATE");
	tree->Write();
}
