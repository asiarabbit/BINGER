// ETFPlaHit.hh -- a breif definition of the class for plastic scintillators readout
// Created: Jan. 23, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 30, 2019 by Asia SUN

#ifndef ETFPlaSD_h
#define ETFPlaSD_h

#include "G4VSensitiveDetector.hh"

#include "ETFPlaHit.hh" // ETFPlaHitsCollection definition

class ETFPlaSD : public G4VSensitiveDetector{
public:
	ETFPlaSD(G4String name);
	virtual ~ETFPlaSD();

	// invoked at the beginning of each event
	virtual void Initialize(G4HCofThisEvent *hce);
	// query step info from G4Step. ROhist: history from ReadOut Geom.
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);
protected:
	ETFPlaHitsCollection *fHitsCollection; // internally this is a std::vector
	G4int fHCID;
};

#endif
