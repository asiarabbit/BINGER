// B1TrackingAction.cc -- the implementation of the class
// Created: Asia SUN, asia.rabbit@163.com, Jan. 20, 2019
// Last modified: Jan. 20, 2019, by Asia SUN

#include "B1TrackingAction.hh"
#include "B1RunAction.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"

B1TrackingAction::B1TrackingAction(B1RunAction *runAction)
 : G4UserTrackingAction(), fRunAction(runAction){}

B1TrackingAction::~B1TrackingAction(){}
 
void B1TrackingAction::PostUserTrackingAction(const G4Track *pTrack){
	G4int parentID = pTrack->GetParentID();
	G4int trackID = pTrack->GetTrackID();
	G4double ef = pTrack->GetKineticEnergy();
	G4VPhysicalVolume *nextVolume = pTrack->GetNextVolume();

//	const G4ParticleDefinition* particle = pTrack->GetParticleDefinition();
//	G4TrackStatus sta = pTrack->GetTrackStatus();
//	const G4Step *step = pTrack->GetStep();
//	G4int currentStepNumber = pTrack->GetCurrentStepNumber();

	if(0 == parentID && 1 == trackID && 0 == nextVolume){
		fRunAction->FillFinalEnergy(ef);
	}
}
