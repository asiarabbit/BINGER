// ETFDetectorConstruction.hh -- a brief definition of the External Target Facility
// at the Institute of Modern Physics, Lanzhou, China.
// Created: Jan. 21, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 21, 2019 by Asia SUN

#ifndef ETFDetectorConstruction_h
#define ETFDetectorConstruction_h

#include <vector>

#include "G4VUserDetectorConstruction.hh"

// class description
// This class is to simulate the External Target Facility located at the middle
// of RIBLL2, originally written to explore the PID resolution limit and its relation
// with the individual detector resolutions of the facility


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4MagneticField;
class G4MagneticFieldManger;
class G4FieldManager;
class ETFMagneticField;
class G4VisAttributes;

class ETFDetectorConstruction : public G4VUserDetectorConstruction{
public:
	ETFDetectorConstruction();
	virtual ~ETFDetectorConstruction();

	// the pure virutal method of the abstract class
	virtual G4VPhysicalVolume *Construct();
	virtual void ConstructSDandField(); // construct sensitive detectors and mag field
protected:
	static G4ThreadLocal G4FieldManager *fFieldMgr;
	static G4ThreadLocal ETFMagneticField *fMagneticField;

	G4LogicalVolume *fLogicalTstop;
	G4LogicalVolume *fLogicalPDC;
	G4LogicalVolume *fLogicalDC;
	G4LogicalVolume *fLogicalWirePlanePDC;
	G4LogicalVolume *fLogicalWirePlaneDC;
	G4LogicalVolume *fLogicalStrip;
	G4LogicalVolume *fLogicalTOFW;
	G4LogicalVolume *fLogicalDipo;
	G4LogicalVolume *fLogicalBField; // dipo + fringe field
	
	// visulaization attributes of individual detectors
	std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
