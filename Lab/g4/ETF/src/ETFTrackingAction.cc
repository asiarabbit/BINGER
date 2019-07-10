// B1TrackingAction.hh -- implementation of the class
// derived from G4UserTrackingAction
// Created: Asia SUN, asia.rabbit@163.com, Jan. 20, 2019
// Last modified: Jan. 20, 2019, by Asia SUN

#include "ETFTrackingAction.hh"

#include "ETFEventAction.hh"
#include "G4Track.hh"

ETFTrackingAction::ETFTrackingAction(ETFEventAction *eventAction)
 : G4UserTrackingAction(), fEventAction(eventAction){}

ETFTrackingAction::~ETFTrackingAction(){}

void ETFTrackingAction::PostUserTrackingAction(const G4Track */*track*/){
	// parentID - 0: primary; 1: secondary; 2: 3rd ...
//	G4int parentID = track->GetParentID();
//	G4int trackID = track->GetTrackID();
//	G4VPhysicalVolume *nextVolume = track->GetNextVolume();
//	if(0 == nextVolume && 0 == parentID && 1 == trackID){ // out of the world
//		G4double ef = track->GetKineticEnergy();
//		eventAction->SetFinalEnergyOfPrimary(ef);
//	}
}
