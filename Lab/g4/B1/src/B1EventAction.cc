// B1EventAction.cc -- implementation of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

B1EventAction::B1EventAction(B1RunAction *runAction)
	: G4UserEventAction(), fRunAction(runAction), fEdep(0.){
}

B1EventAction::~B1EventAction(){}

void B1EventAction::BeginOfEventAction(const G4Event *){
	fEdep = 0.;
}

void B1EventAction::EndOfEventAction(const G4Event *){
	fRunAction->AddEdep(fEdep);
}
