// B1DetectorConstruction.hh -- a brief definition of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

// detector construction class to define materials and geometry
class B1DetectorConstruction : public G4VUserDetectorConstruction{
public:
	B1DetectorConstruction();
	virtual ~B1DetectorConstruction();
	
	virtual G4VPhysicalVolume *Construct();
	G4LogicalVolume *GetScoringVolume() const{ return fScoringVolume; }

protected:
	G4LogicalVolume *fScoringVolume;
};

#endif
