// ETFEventAction.hh -- a brief definition for the class
// Created: Jan. 27, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 28, 2019, by Asia SUN

#ifndef ETFEventAction_h
#define ETFEventAction_h

#include "globals.hh"
#include "G4UserEventAction.hh"

class G4VHitsCollection;
class ETFRunAction;

// class description
// event action class

class ETFEventAction : public G4UserEventAction{
public:

	ETFEventAction(ETFRunAction *runAction);
	virtual ~ETFEventAction();

	virtual void BeginOfEventAction(const G4Event *);
	virtual void   EndOfEventAction(const G4Event *);
	static G4VHitsCollection *GetHC(const G4Event *event, G4int collId);
	
private:
	// hit collection ID array
	G4int fPlaHCID[2]; // [Tstop - TOFWstrip]
	G4int fDriftChamberHCID[2]; // [PDC-DC]
	G4int fPlaHCIDPrimary[2]; // [Tstop - TOFWstrip]
	G4int fDriftChamberHCIDPrimary[2]; // [PDC-DC]
	
	ETFRunAction *fRunAction;
};

#endif
