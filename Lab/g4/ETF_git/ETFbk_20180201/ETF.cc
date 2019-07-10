// ETF.cc -- the main program for the External Target Facility
// Created: Jan. 28, 2019, by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 28, 2019, by Asia SUN



#include "ETFDetectorConstruction.hh"
#include "ETFActionInitialization.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char **argv){
	// Detect interactive mode (if no arguments) and define UI session
	G4UIExecutive *ui = 0;
	if(1 == argc){ // no arguments
		ui = new G4UIExecutive(argc, argv);
	}

	// constrct the default manager
	G4RunManager *runManager = new G4RunManager;

	// mandatory user initialization classes
	runManager->SetUserInitialization(new ETFDetectorConstruction);
	auto physicsList = new FTFP_BERT;
	physicsList->RegisterPhysics(new G4StepLimiterPhysics);
	runManager->SetUserInitialization(physicsList);
	runManager->SetUserInitialization(new ETFActionInitialization);

	// visualization manager construction
	G4VisExecutive *visManager = new G4VisExecutive;
	visManager->Initialize();

	// get the pointer to the user interface manager
	G4UImanager *UImanager = G4UImanager::GetUIpointer();

	if(!ui){
		// execute an argument macro file if exist
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	else{
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		if(ui->IsGUI()){
			UImanager->ApplyCommand("/control/execute gui.mac");
		}
		// start interactive session
		ui->SessionStart();
		delete ui;
	} // end else

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main program
	delete visManager;
	delete runManager;

	return 0;
}
