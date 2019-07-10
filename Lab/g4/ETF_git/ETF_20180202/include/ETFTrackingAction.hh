// ETFTrackingAction.hh -- brief definition of the class
// derived from G4UserTrackingAction
// Created: Jan. 27, 2019, by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 27, 2019, by Asia

#ifndef ETFTrackingAction_h
#define ETFTrackingAction_h

#include "G4UserTrackingAction.hh"

class ETFEventAction;

class ETFTrackingAction : public G4UserTrackingAction{
public:
	ETFTrackingAction(ETFEventAction *eventAction);
	virtual ~ETFTrackingAction();
	
	virtual void PostUserTrackingAction(const G4Track *pTrack);
private:
	ETFEventAction *fEventAction;
};

#endif
