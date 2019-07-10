// ETFDriftChamberSD.hh -- a brief definition of the class for Sensitive Detectors of DCs
// Created: Jan. 22, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 22, 2019 by Asia SUN

#ifndef ETFDriftChamberSD_h
#define ETFDriftChamberSD_h

#include "G4VSensitiveDetector.hh"

#include "ETFDriftChamberHit.hh" // ETFDriftChamberHitsCollection declaration

class ETFDriftChamberSD : public G4VSensitiveDetector{
public:
	ETFDriftChamberSD(G4String name);
	virtual ~ETFDriftChamberSD();

	// invoked at the beginning of each event
	virtual void Initialize(G4HCofThisEvent *hce);
	// query step info from G4Step. ROhist: history from ReadOut Geom.
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);
protected:
	// internally this is a std::vector
	ETFDriftChamberHitsCollection *fHitsCollection;
	G4int fHCID;
};

#endif
