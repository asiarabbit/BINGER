// ETF.cc -- the main program for the External Target Facility
// Created: Jan. 28, 2019, by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 28, 2019, by Asia SUN



#include "ETFDetectorConstruction.hh"
#include "ETFActionInitialization.hh"

#include "G4RunManager.hh"

#include "G4UItcsh.hh"
#include "FTFP_BERT.hh"

#include "G4UIterminal.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

//#include "G4StepLimiterPhysics.hh"
#include "G4VisExecutive.hh"

int main(int argc, char **argv){
	// constrct the default manager
	G4RunManager *runManager = new G4RunManager;

	// mandatory user initialization classes
	runManager->SetUserInitialization(new ETFDetectorConstruction);
	runManager->SetUserInitialization(new FTFP_BERT);
	runManager->SetUserInitialization(new ETFActionInitialization);	
	
	// initialize G4 kernel
//	runManager->Initialize();
	
	G4UIsession *session = new G4UIterminal(new G4UItcsh);
	if(argc > 1){
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		G4UImanager::GetUIpointer()->ApplyCommand(command + fileName);
	}
	session->SessionStart();

	delete session;
	delete runManager;	
	return 0;
}
