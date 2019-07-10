/// ETFRunAction.cc -- implementation of the class
// Created: Jan. 27, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 27, 2019, by Asia SUN

#include <sstream>

#include "ETFRunAction.hh"
#include "ETFEventAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"

// ROOT include
#include "TFile.h"
#include "TTree.h"

ETFRunAction::ETFRunAction()
 : G4UserRunAction(), fFile(0), fTree(0){
	fFile = new TFile("ETF.root", "RECREATE");
	fTree = new TTree("treeETF", "data tree for ETF");
	fTree->Branch("index", &fData.index, "index/I");

	// particle definition
	fTree->Branch("A", &fData.A, "A/D");
	fTree->Branch("Z", &fData.Z, "Z/D");

	fTree->Branch("p0", fData.p0, "p0[3]/D"); // initial momentum

	fTree->Branch("TstopNhit", &fData.TstopNhit, "TstopNhit/I");
	fTree->Branch("TstopTime", &fData.TstopTime, "TstopTime/D");
	fTree->Branch("TstopVelocity", &fData.TstopVelocity, "TstopVelocity/D");
	fTree->Branch("TstopKineticEnergy", &fData.TstopKineticEnergy, "TstopKineticEnergy/D");
	fTree->Branch("TstopPos", fData.TstopPos, "TstopPos[3]/D");
	fTree->Branch("TstopMomentum", fData.TstopMomentum, "TstopMomentum[3]/D");

	fTree->Branch("TOFWNhit", &fData.TOFWNhit, "TOFWNhit/I");
	fTree->Branch("TOFWStripId", &fData.TOFWStripId, "stripId/I");
	fTree->Branch("TOFWTime", &fData.TOFWTime, "TOFWTime/D");
	fTree->Branch("TOFWVelocity", &fData.TOFWVelocity, "TOFWVelocity/D");
	fTree->Branch("TOFWKineticEnergy", &fData.TOFWKineticEnergy, "TOFWKineticEnergy/D");
	fTree->Branch("TOFWPos", fData.TOFWPos, "TOFWPos[3]/D");
	fTree->Branch("TOFWMomentum", fData.TOFWMomentum, "TOFWMomentum[3]/D");

	fTree->Branch("PDCArrNhit", &fData.PDCArrNhit, "PDCArrNhit/I");
	fTree->Branch("PDCTime", fData.PDCTime, "PDCTime[4]/D");
	fTree->Branch("PDCVelocity", fData.PDCVelocity, "PDCVelocity[4]/D");
	fTree->Branch("PDCKineticEnergy", fData.PDCKineticEnergy, "PDCKineticEnergy[4]/D");
	fTree->Branch("PDCPos", fData.PDCPos, "PDCPos[4][3]/D");
	fTree->Branch("PDCMomentum", fData.PDCMomentum, "PDCMomentum[4][3]/D");

	fTree->Branch("DCArrNhit", &fData.DCArrNhit, "DCArrNhit/I");
	fTree->Branch("DCTime", fData.DCTime, "DCTime[6]/D");
	fTree->Branch("DCVelocity", fData.DCVelocity, "DCVelocity[6]/D");
	fTree->Branch("DCKineticEnergy", fData.DCKineticEnergy, "DCKineticEnergy[6]/D");
	fTree->Branch("DCPos", fData.DCPos, "DCPos[6][3]/D");
	fTree->Branch("DCMomentum", fData.DCMomentum, "DCMomentum[6][3]/D");
}

ETFRunAction::~ETFRunAction(){
	fFile->Close();
}

void ETFRunAction::BeginOfRunAction(const G4Run */*run*/){
}

void ETFRunAction::EndOfRunAction(const G4Run *){
	G4cout << "fTree->GetEntries(): " << fTree->GetEntries() << G4endl;
	fTree->Write("", TObject::kOverwrite);
//	fFile->Close(); // DEBUG
}
