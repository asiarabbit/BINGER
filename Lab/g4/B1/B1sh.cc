// B1.cc -- the main program of the B1 project

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

int main(int argc, char **argv){
	// detect interactive mode (if no arguments) and define UI session //
	G4UIExecutive *ui = 0;
	if(1 == argc){ // no arguments are provided
		ui = new G4UIExecutive(argc, argv, "tcsh");
	}

	// construct the default manager //
#ifdef G4MULTITHREADED
	G4MTRunManager *runManager = new G4MTRunManager;
#else
	G4RunManager *runManager = new G4RunManager;
#endif

	// choose the random engine //
	G4Random::setTheEngine(new CLHEP::RanecuEngine); // an embeded class of CLHEP

	// set mandatory initialization classes //
	// detector construction
	runManager->SetUserInitialization(new B1DetectorConstruction);
	// physics list
	// QBBC: recommended for medical and space physics simulations
	G4VModularPhysicsList *physicsList = new QBBC;
	physicsList->SetVerboseLevel(1);
	runManager->SetUserInitialization(physicsList);
	// user action initialization
	runManager->SetUserInitialization(new B1ActionInitialization);
//	runManager->Initialize();
	ui->SessionStart();

	// initialize visualization //
//	G4VisManager *visManager = new G4VisExecutive;
//	visManager->Initialize();
	// get the pointer to the User Interface manager
//	G4UImanager *UImanager = G4UImanager::GetUIpointer();

/*	// process macro or start UI session
	if(!ui){
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	else{
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}
*/
	// job termination //
	// free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main() program

//	delete visManager;
	delete runManager;
} // end of the main program
