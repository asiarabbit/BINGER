// B1RunAction.cc -- the implementation of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

// RunAction calculates the accumulated dose in the scoring volume //

#include <sstream>
#include <iostream>
#include <cstdlib>

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h" // include from ROOT
#include "TH1F.h" // include from ROOT
#include "TTree.h" // include from ROOT

using std::cout;
using std::endl;

B1RunAction::B1RunAction()
	: G4UserRunAction(), fEdep(0.), fEdep2(0.),
	fFile(0), fHef(0), fTree(0), fEf(0.){
	// add new units for dose
//	const G4double milligray = 1.e-3*gray;
//	const G4double microgray = 1.e-6*gray;
//	const G4double nanogray  = 1.e-9*gray;
//	const G4double picogray  = 1.e-12*gray;
	
	// register accumulable to the accumulable manager
	G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->RegisterAccumulable(fEdep);
	accumulableManager->RegisterAccumulable(fEdep2);

	fFile = new TFile("he.root", "RECREATE");
	fHef = new TH1F("hef", "Final Energy from G4 Simulation;e_f [MeV];Counts", 500, 0., 10000.);
	fTree = new TTree("tree", "Remaining Energy");
	fTree->Branch("ef", &fEf, "ef/D");
}

B1RunAction::~B1RunAction(){
	fFile->Close();
}

void B1RunAction::BeginOfRunAction(const G4Run *){
	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	
	// reset accumulables to their initial values
	G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Reset();
}

void B1RunAction::EndOfRunAction(const G4Run *run){
	G4int nofEvents = run->GetNumberOfEvent();
	if(0 == nofEvents) return;

	// merge accumulables
	G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Merge();
	
	// compute dose = total energy deposit in a run and its variance
	G4double edep  = fEdep.GetValue();
	G4double edep2 = fEdep2.GetValue();

	G4double rms = edep2 - edep*edep/nofEvents; // /nofEvents
	if(rms > 0.) rms = std::sqrt(rms); else rms = 0.;

	const B1DetectorConstruction *detectorConstruction = 
		static_cast<const B1DetectorConstruction *>
			(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
	G4double dose = edep / mass;
	G4double rmsDose = rms / mass;
	
	// run conditions
	// note: there is no primary generator action object for "master"
	// run manager for multi-threaded mode
	const B1PrimaryGeneratorAction *generatorAction =
		static_cast<const B1PrimaryGeneratorAction *>
			(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
	G4String runCondition; // e.g. proton of 20MeV
	if(generatorAction){
		const G4ParticleGun *particleGun = generatorAction->GetParticleGun();
		runCondition += particleGun->GetParticleDefinition()->GetParticleName();
		runCondition += " of ";
		G4double particleEnergy = particleGun->GetParticleEnergy();
		runCondition += G4BestUnit(particleEnergy, "Energy");
	}

	// print
	if(IsMaster()){
		G4cout << G4endl << "-----------------End of Global Run----------------------";
	}
	else{
		G4cout << G4endl << "-----------------End of Local Run-----------------------";
	}
	G4cout << G4endl << " The run consists of " << nofEvents << " " << runCondition
		<< G4endl << " Cumulated dose per run, in scoring volume: "
		<< G4BestUnit(dose, "Dose") << " rms = " << G4BestUnit(rmsDose, "Dose") << G4endl
		<< "--------------------------------------------------------------------------"
		<< G4endl << G4endl;
		
	std::ostringstream hefName("");
	hefName << "hef_Run" << run->GetRunID();
	fHef->SetName(hefName.str().c_str());
	fHef->Write("", TObject::kOverwrite);
	fTree->Write("", TObject::kOverwrite);
}

void B1RunAction::AddEdep(G4double edep){
	fEdep  += edep;
	fEdep2 += edep*edep;
}

void B1RunAction::FillFinalEnergy(G4double ef){
	fEf = ef;
	fHef->Fill(fEf);
	fTree->Fill();
}

