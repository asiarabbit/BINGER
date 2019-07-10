// B1SteppingAction.cc -- implementation of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

B1SteppingAction::B1SteppingAction(B1EventAction *eventAction)
	: G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(0){
}

B1SteppingAction::~B1SteppingAction(){}

void B1SteppingAction::UserSteppingAction(const G4Step *step){
	if(!fScoringVolume){
		const B1DetectorConstruction *detectorConstruction = 
			static_cast<const B1DetectorConstruction *>
				(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		fScoringVolume = detectorConstruction->GetScoringVolume();
	}

	// get volume of the current step
	G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()
		->GetVolume()->GetLogicalVolume();
	
	// check if we are in scoring volume
	if(volume != fScoringVolume) return;

	// collect if we are in scoring volume
	G4double edepStep = step->GetTotalEnergyDeposit();
	fEventAction->AddEdep(edepStep);
}
