// AnaG4Sim.C -- analyze the G4 simulation data to assess ETF PID capability
// Created: Feb. 2, 2019, by Asia SUN, asia.rabbit@163.com
// Last modified: Feb 2, 2019 by Asia SUN

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

#include "TAMath.h"
#include "TAPID.h"

using std::cout;
using std::endl;

struct data_t; // the tree branch struct

int main(int argc, char **argv){
	if(argc == 1){
		cout << "./anaG4 <rootfile>    ";
		cout << "Rootfile name has to be proveded as the argument" << endl;
	}
	std::string rootfile = argv[1];
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	TTree *treeETF = (TTree *)f->Get("treeETF");
	if(!treeETF){
		cout << "treeETF not found." << endl;
		exit(1);
	}
	// set branch address //
	data_t data;
	fTree->SetBranchAddress("index", &data.index);
	// particle definition
	fTree->SetBranchAddress("A", &data.A);
	fTree->SetBranchAddress("Z", &data.Z);
	// initial momentum
	fTree->SetBranchAddress("p0", data.p0);
	// Tstop physics
	fTree->SetBranchAddress("TstopNhit", &data.TstopNhit);
	fTree->SetBranchAddress("TstopTime", &data.TstopTime);
	fTree->SetBranchAddress("TstopVelocity", &data.TstopVelocity);
	fTree->SetBranchAddress("TstopKineticEnergy", &data.TstopKineticEnergy);
	fTree->SetBranchAddress("TstopPos", data.TstopPos);
	fTree->SetBranchAddress("TstopMomentum", data.TstopMomentum);
	// TOFW physics
	fTree->SetBranchAddress("TOFWNhit", &data.TOFWNhit);
	fTree->SetBranchAddress("TOFWStripId", &data.TOFWStripId);
	fTree->SetBranchAddress("TOFWTime", &data.TOFWTime);
	fTree->SetBranchAddress("TOFWVelocity", &data.TOFWVelocity);
	fTree->SetBranchAddress("TOFWKineticEnergy", &data.TOFWKineticEnergy);
	fTree->SetBranchAddress("TOFWPos", data.TOFWPos);
	fTree->SetBranchAddress("TOFWMomentum", data.TOFWMomentum);
	// PDC physics
	fTree->SetBranchAddress("PDCArrNhit", &data.PDCArrNhit);
	fTree->SetBranchAddress("PDCTime", data.PDCTime);
	fTree->SetBranchAddress("PDCVelocity", data.PDCVelocity);
	fTree->SetBranchAddress("PDCKineticEnergy", data.PDCKineticEnergy);
	fTree->SetBranchAddress("PDCPos", data.PDCPos);
	fTree->SetBranchAddress("PDCMomentum", data.PDCMomentum);
	// DC physics
	fTree->SetBranchAddress("DCArrNhit", &data.DCArrNhit);
	fTree->SetBranchAddress("DCTime", data.DCTime);
	fTree->SetBranchAddress("DCVelocity", data.DCVelocity);
	fTree->SetBranchAddress("DCKineticEnergy", data.DCKineticEnergy);
	fTree->SetBranchAddress("DCPos", data.DCPos);
	fTree->SetBranchAddress("DCMomentum", data.DCMomentum);

	// the result tree
	TTree *treeTrack = new TTree("treeTrack",
		"G4Sim Analysis Result Tree for ETF");
	int index; // event index
	double kPDC, bPDC, kDC, bDC; // track function: x=kz+b, tracking result
	double tof, beta, zz, aoz, aa; // particle identity, PID result
	double poz, brho, trkLenT, x2, dx2; // PID relevant
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("kPDC", &kPDC);
	treeTrack->SetBranchAddress("bPDC", &bPDC);
	treeTrack->SetBranchAddress("kDC", &kDC);
	treeTrack->SetBranchAddress("bDC", &bDC);
	treeTrack->SetBranchAddress("tof", &tof);
	treeTrack->SetBranchAddress("beta", &beta);
	treeTrack->SetBranchAddress("zz", &zz); // Z: charge
	treeTrack->SetBranchAddress("aoz", &aoz);
	treeTrack->SetBranchAddress("aa", &aa); // Z: charge
	treeTrack->SetBranchAddress("poz", &poz);
	treeTrack->SetBranchAddress("brho", &brho);
	treeTrack->SetBranchAddress("trkLenT", &trkLenT);
	treeTrack->SetBranchAddress("x2", &x2);
	treeTrack->SetBranchAddress("dx2", &dx2);


	// loop over events //
	const int n = treeETF->GetEntries();
	for(int i = 0; i < n; i++){
		treeETF->GetEntry(i);
		// LSM for particle tracking //
		
		// PID using RK or Uniform Mag method //



		treeTrack->Fill();
	} // end of loop over i
	// write the analysis result
	treeTrack->Write("", TObject::kOverwrite);
	f->Close();



	return 0;
}



struct data_t{
	G4int index;
	
	// particle identity
	G4double A;
	G4double Z;

	G4double p0[3]; // initial momentum for the primary

	G4int TstopNhit;
	G4double TstopTime;
	G4double TstopVelocity;
	G4double TstopKineticEnergy; // kineticEnergy
	G4double TstopPos[3]; // [x-y-z]
	G4double TstopMomentum[3];

	G4int TOFWNhit;
	G4int TOFWStripId;
	G4double TOFWTime;
	G4double TOFWVelocity;
	G4double TOFWKineticEnergy;
	G4double TOFWPos[3];
	G4double TOFWMomentum[3];

	G4int PDCArrNhit;
	G4double PDCTime[4];
	G4double PDCVelocity[4];
	G4double PDCKineticEnergy[4];
	G4double PDCPos[4][3]; // [wire_layer - (x, y, z)]
	G4double PDCMomentum[4][3];

	G4int DCArrNhit;
	G4double DCTime[6];
	G4double DCVelocity[6];
	G4double DCKineticEnergy[6];
	G4double DCPos[6][3]; // [wire_layer - (x, y, z)]
	G4double DCMomentum[6][3];

	data_t(){
		initialize();
	}
	void initialize(){
		index = -1;

		A = -9999.;
		Z = -9999.;

		for(G4double &x : p0) x = -9999.;

		TstopNhit = -1;
		TstopTime = -9999.;
		TstopVelocity = -9999.;
		TstopKineticEnergy = -9999.;
		for(G4double &x : TstopPos) x = -9999.;
		for(G4double &x : TstopMomentum) x = -9999.;

		TOFWNhit = -1;
		TOFWStripId = -1;
		TOFWTime = -9999.;
		TOFWVelocity = -9999.;
		TOFWKineticEnergy = -9999.;
		for(G4double &x : TOFWPos) x = -9999.;
		for(G4double &x : TOFWMomentum) x = -9999.;

		PDCArrNhit = -1;
		for(auto &x : PDCTime) x = -9999.;
		for(auto &x : PDCVelocity) x = -9999.;
		for(auto &x : PDCKineticEnergy) x = -9999.;
		for(auto &x : PDCPos) for(auto &t : x) t = -9999.;
		for(auto &x : PDCMomentum) for(auto &t : x) t = -9999.;

		DCArrNhit = -1;
		for(auto &x : DCTime) x = -9999.;
		for(auto &x : DCVelocity) x = -9999.;
		for(auto &x : DCKineticEnergy) x = -9999.;
		for(auto &x : DCPos) for(auto &t : x) t = -9999.;
		for(auto &x : DCMomentum) for(auto &t : x) t = -9999.;
	}
	void setP0(double x, double y, double z){
		p0[0] = x; p0[1] = y; p0[2] = z;
	}
};

