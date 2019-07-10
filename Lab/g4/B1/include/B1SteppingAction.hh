// B1SteppingAction.hh -- a brief definition of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#ifndef B1SteppingAction_h
#define B1SteppingAction_h

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class B1EventAction;

class G4LogicalVolume;

// stepping action class

class B1SteppingAction : public G4UserSteppingAction{
public:
	B1SteppingAction(B1EventAction *eventAction);
	virtual ~B1SteppingAction();
	
	// method from the base class
	virtual void UserSteppingAction(const G4Step *);

private:
	B1EventAction *fEventAction;
	G4LogicalVolume *fScoringVolume;
};

#endif
