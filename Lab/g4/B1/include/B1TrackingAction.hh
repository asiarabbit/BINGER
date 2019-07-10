// B1TrackingAction.hh -- a brief definition of the class
// derived from G4UserTrackingAction
// Created: Asia SUN, asia.rabbit@163.com, Jan. 20, 2019
// Last modified: Jan. 20, 2019, by Asia SUN

#ifndef B1TrackingAction_h
#define B1TrackingAction_h

#include "G4UserTrackingAction.hh"

class B1RunAction;
class TFile;
class TH1F;

class B1TrackingAction : public G4UserTrackingAction{
public:
	B1TrackingAction(B1RunAction *runAction);
	virtual ~B1TrackingAction();
	
	virtual void PostUserTrackingAction(const G4Track *pTrack);
private:
	B1RunAction *fRunAction;
};

#endif
