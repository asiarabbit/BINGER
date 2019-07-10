// B1batch.cc -- B1 project in hard-coded batch mode
// Created: Jan. 18, 2019 by Asia SUN, asia.rabbit@163.com
// Last modified: jan. 18, 2019, by Asia SUN

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UItcsh.hh"
#include "QBBC.hh"

#include "G4UIterminal.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

int main(){
	// construct the default run manager
	G4RunManager *runManager = new G4RunManager();
	
	// set mandatory initialization classes
	runManager->SetUserInitialization(new B1DetectorConstruction);
	runManager->SetUserInitialization(new QBBC);
	runManager->SetUserInitialization(new B1ActionInitialization);

	// initialize G4 kernel
	runManager->Initialize();
	
	// apply some commands first
//	G4UImanager *UImanager = G4UImanager::GetUIpointer();
//	UImanager->ApplyCommand("/run/beamOn 1");

	// define UI terminal for interactive mode
	G4UIsession *session = new G4UIterminal(new G4UItcsh); // new G4UItcsh
	session->SessionStart();
	delete session;
	
//	int numberOfEvent = 1000;
//	runManager->BeamOn(numberOfEvent);
	
	// job termination
	delete runManager;
	return 0;
}

