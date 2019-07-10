// B1ActionInitialization.cc -- the implementation for the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#include "B1ActionInitialization.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1SteppingAction.hh"
#include "B1TrackingAction.hh"

B1ActionInitialization::B1ActionInitialization()
	: G4VUserActionInitialization(){}

B1ActionInitialization::~B1ActionInitialization(){}

void B1ActionInitialization::BuildForMaster() const{
	B1RunAction *runAction = new B1RunAction;
	SetUserAction(runAction);
}

// well, it seems that BuildForMaster and Build excludes each other
// runAction -> eventAction -> steppingAction
void B1ActionInitialization::Build() const{
	SetUserAction(new B1PrimaryGeneratorAction);
	
	B1RunAction *runAction = new B1RunAction();
	SetUserAction(runAction);

	B1EventAction *eventAction = new B1EventAction(runAction); // runAction: info collector
	SetUserAction(eventAction);

	SetUserAction(new B1TrackingAction(runAction));

	SetUserAction(new B1SteppingAction(eventAction)); // eventAction: info collector
}
