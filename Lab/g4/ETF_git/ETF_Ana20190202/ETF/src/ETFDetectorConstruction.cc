// ETFDetectorConstruction.cc -- the implementation of the class
// Created: Jan. 21, 2019, by Asia SUN, asia.rabbit@163.com
// Last modified: Feb. 2, 2019 by Asia SUN

#include "ETFDetectorConstruction.hh"
#include "ETFMagneticField.hh"
#include "ETFDriftChamberSD.hh" // sensitive detectors for drift chambers
#include "ETFPlaSD.hh" // sensitive detectors of plastic scintillators

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh" // inherited from G4PhysicalVolume
#include "G4SystemOfUnits.hh" // units imported from CLHEP library
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4GenericMessenger.hh"
#include "G4ios.hh"
#include "G4PVReplica.hh" // why ?

G4ThreadLocal ETFMagneticField *ETFDetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager *ETFDetectorConstruction::fFieldMgr = 0;

ETFDetectorConstruction::ETFDetectorConstruction()
	: G4VUserDetectorConstruction(), fLogicalTstop(0), fLogicalWirePlanePDC(0),
	fLogicalWirePlaneDC(0), fLogicalStrip(0), fLogicalTOFW(0), fLogicalDipo(0){
	fVisAttributes.reserve(4);
}

ETFDetectorConstruction::~ETFDetectorConstruction(){}

G4VPhysicalVolume *ETFDetectorConstruction::Construct(){
	// get nist material manager //
	G4NistManager *nist = G4NistManager::Instance();
	const G4bool checkOverlaps = true;

	// -- Material list -- //
	// gas condition in Lanzhou lab, typical pressure: 840mb, temperature: 20. C
	const G4double temperature = 293.15*kelvin;
	const G4double pressureLanzhou = 0.841*atmosphere;
//	G4Material *water = nist->FindOrBuildMaterial("G4_WATER");
	G4Material *airLanzhou = nist->ConstructNewGasMaterial("airLanzhou",
		"G4_AIR", temperature, pressureLanzhou);
	G4Material *arLanzhou = nist->ConstructNewGasMaterial("arLanzhou",
		 "G4_Ar", temperature, pressureLanzhou);
	const G4double arLanzhouDensity = arLanzhou->GetDensity();
	G4Material *c2h6Lanzhou = nist->ConstructNewGasMaterial("c2h6Lanzhou",
		 "G4_ETHANE", temperature, pressureLanzhou);
	const G4double c2h6LanzhouDensity = c2h6Lanzhou->GetDensity();
	G4Material *co2Lanzhou = nist->ConstructNewGasMaterial("co2Lanzhou",
		 "G4_CARBON_DIOXIDE", temperature, pressureLanzhou);
	const G4double co2LanzhouDensity = co2Lanzhou->GetDensity();

	// DC Gas 0: 80%Ar + 20%CO2 //
	const G4int nComptDCGas0 = 2; // number of gas components
	const G4double dcGas0Density = 0.8*arLanzhouDensity + 0.2*co2LanzhouDensity;
	const G4double fracAr_dcGas0 = 0.8*arLanzhouDensity / dcGas0Density;
	const G4double fracCO2_dcGas0 = 0.2*co2LanzhouDensity / dcGas0Density;
	G4Material *dcGas0 = new G4Material("dcGas0", dcGas0Density, nComptDCGas0,
		kStateGas, temperature, pressureLanzhou);
	dcGas0->AddMaterial(arLanzhou, fracAr_dcGas0);
	dcGas0->AddMaterial(co2Lanzhou, fracCO2_dcGas0);
	// DC Gas 1: 50%Ar + 50%C2H6 //
	const G4int nComptDCGas1 = 2; // number of gas components
	const G4double dcGas1Density = 0.5*arLanzhouDensity + 0.5*c2h6LanzhouDensity;
	const G4double fracAr_dcGas1 = 0.5*arLanzhouDensity / dcGas1Density;
	const G4double fracC2H6_dcGas1 = 0.5*c2h6LanzhouDensity / dcGas1Density;
	G4Material *dcGas1 = new G4Material("dcGas1", dcGas1Density, nComptDCGas1,
		kStateGas, temperature, pressureLanzhou);
	dcGas1->AddMaterial(arLanzhou, fracAr_dcGas1);
	dcGas1->AddMaterial(c2h6Lanzhou, fracC2H6_dcGas1);
	// plastic scintillators for Tstop (EJ-232Q) and TOFWall (BICRON BC408)
	G4Element *elH = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
	G4Element *elC = new G4Element("Carbon", "C", 6., 12.01*g/mole);
	const G4int nComptSci = 2; // tow elements
	G4Material *BC408 = new G4Material("BC408", 1.023*g/cm3, nComptSci);
	BC408->AddElement(elC, 9); // (elements, numberOfAtoms)
	BC408->AddElement(elH, 10);
	G4Material *EJ232Q = new G4Material("EJ232Q", 1.023*g/cm3, nComptSci);
	EJ232Q->AddElement(elC, 9); // (elements, numberOfAtoms)
	EJ232Q->AddElement(elH, 10);

	// ----- DETECTOR CONSTRUCTION ----- //
	// along beam direction: Tstop -> PDC2-3 -> Dipole -> MWDC0-1-2-TOFW
	// each of them is composed of box, or boxes

	// the World //
	const G4double worldSizeX = 8000.*mm; // horizontal
	const G4double worldSizeY = 2000.*mm; // vertical
	const G4double worldSizeZ = 20000.*mm; // beam direction
	const G4ThreeVector worldPos(0., 0., 0.);
	G4Material *worldMat = airLanzhou;
	G4Box *solidWorld = new G4Box("World", 0.5*worldSizeX, 0.5*worldSizeY, 0.5*worldSizeZ);
	G4LogicalVolume *logicalWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
	G4PVPlacement *physicalWorld = new G4PVPlacement(0, worldPos, logicalWorld, "World",
		0, false, 0, checkOverlaps);


	// Tstop //
	const G4double tstopSizeXY = 50.*mm;
	const G4double tstopSizeZ = 3.*mm; // thickness
	const G4ThreeVector tstopPos(0., 0., -2699.08);
	G4Box *solidTstop = new G4Box("Tstop", 0.5*tstopSizeXY, 0.5*tstopSizeXY,
		0.5*tstopSizeZ);
	fLogicalTstop = new G4LogicalVolume(solidTstop, EJ232Q, "Tstop");
	new G4PVPlacement(0, tstopPos, fLogicalTstop, "Tstop", logicalWorld, false, 0,
		checkOverlaps);


	// PDC2-3 //
	const G4double pdcSizeXY = 150.*mm;
	const G4double pdcSizeZ = 70.*mm;
	// PDC0-1-Target-PDC2-3. Only Post-target facility would be created here.
	const G4ThreeVector pdc2Pos(0.25, -0.79, -1725.15);
	const G4ThreeVector pdc3Pos(-0.65, 0.86, -933.64);
	G4Material *pdcMat = dcGas0;
	G4Box *solidPDC = new G4Box("PDC", 0.5*pdcSizeXY, 0.5*pdcSizeXY, 0.5*pdcSizeZ);
	fLogicalPDC = new G4LogicalVolume(solidPDC, pdcMat, "PDC");
	new G4PVPlacement(0, pdc2Pos, fLogicalPDC, "PDC2", logicalWorld, false, 0,
		checkOverlaps);
	new G4PVPlacement(0, pdc3Pos, fLogicalPDC, "PDC3", logicalWorld, false, 1,
		checkOverlaps);
	// "virtual" wire plane for PDCs
	G4Box *solidWirePlanePDC = new G4Box("WirePlanePDC", 0.5*pdcSizeXY, 0.5*pdcSizeXY, 0.5*0.08*mm);
	fLogicalWirePlanePDC = new G4LogicalVolume(solidWirePlanePDC, pdcMat,
		"WirePlanePDC");
	new G4PVPlacement(0, G4ThreeVector(0., 0., -15.*mm), fLogicalWirePlanePDC,
	 	"WirePlanePDCX1", fLogicalPDC, false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(0., 0.,  -5.*mm), fLogicalWirePlanePDC,
	 	"WirePlanePDCX2", fLogicalPDC, false, 1, checkOverlaps);

	// The BIG DIPOLE MAGNET //
	// specifications definition
	const G4double dipoSizeX = 980.*mm;
	const G4double dipoSizeY = 270.*mm;
	const G4double dipoSizeZ = 1000.*mm;
	const G4double bFieldSizeZ = 1800.*mm;
	const G4ThreeVector dipoPos(); // used as the origin of the lab frame
	G4Material *dipoMat = airLanzhou; // water; // 
	// field environment
	G4Box *solidBField = new G4Box("BField", 0.5*dipoSizeX, 0.5*dipoSizeY, 0.5*bFieldSizeZ);
	fLogicalBField = new G4LogicalVolume(solidBField, dipoMat, "BField");
	new G4PVPlacement(0, G4ThreeVector(), fLogicalBField, "BField", logicalWorld,
		false, 0, checkOverlaps);
	fLogicalBField->SetUserLimits(new G4UserLimits(1.*m));
	// the dipo magnet
	G4Box *solidDipo = new G4Box("Dipo", 0.5*dipoSizeX, 0.5*dipoSizeY, 0.5*dipoSizeZ);
	fLogicalDipo = new G4LogicalVolume(solidDipo, dipoMat, "Dipo");
	new G4PVPlacement(0, G4ThreeVector(), fLogicalDipo, "Dipo", fLogicalBField,
		false, 0, checkOverlaps);


	// DC0-1-2 //limit
	const G4double dcSizeX = 800.*mm;
	const G4double dcSizeY = 600.*mm;
	const G4double dcSizeZ = 130.*mm;
	const G4ThreeVector dc0Pos(473.58,  23.92, 1961.02);
	const G4ThreeVector dc1Pos(998.29,   8.78, 3499.40);
	const G4ThreeVector dc2Pos(1335.77, 10.16, 4999.67);
	G4RotationMatrix *dc0Rotation =  new G4RotationMatrix();
	G4RotationMatrix *dc1Rotation =  new G4RotationMatrix();
	G4RotationMatrix *dc2Rotation =  new G4RotationMatrix();
	dc0Rotation->rotateY(-14.27*deg);
	dc1Rotation->rotateY(-14.47*deg);
	dc2Rotation->rotateY(-14.87*deg);
	G4Material *dcMat = dcGas1;
	G4Box *solidDC = new G4Box("DC", 0.5*dcSizeX, 0.5*dcSizeY, 0.5*dcSizeZ);
	fLogicalDC = new G4LogicalVolume(solidDC, dcMat, "DC");
	new G4PVPlacement(dc0Rotation, dc0Pos, fLogicalDC, "DC0", logicalWorld, false, 0,
		checkOverlaps);
	new G4PVPlacement(dc1Rotation, dc1Pos, fLogicalDC, "DC1", logicalWorld, false, 1,
		checkOverlaps);
	new G4PVPlacement(dc2Rotation, dc2Pos, fLogicalDC, "DC2", logicalWorld, false, 2,
		checkOverlaps);
	// "virtual" wire plane for DCs
	G4Box *solidWirePlaneDC = new G4Box("WirePlaneDC", 0.5*dcSizeX, 0.5*dcSizeY, 0.5*0.1*mm);
	fLogicalWirePlaneDC = new G4LogicalVolume(solidWirePlaneDC, dcMat,
		"WirePlaneDC");
	new G4PVPlacement(0, G4ThreeVector(0., 0.,  0.*mm), fLogicalWirePlaneDC,
	 	"WirePlaneDCX1", fLogicalDC, false, 0, checkOverlaps); // X1
	new G4PVPlacement(0, G4ThreeVector(0., 0., 10.*mm), fLogicalWirePlaneDC,
	 	"WirePlaneDCX2", fLogicalDC, false, 1, checkOverlaps); // X2

	// Time-of-Flight Wall (TOFW) //
	const G4int nStripTOFW = 30; // 30 strips
	const G4double tofwSizeX = 1250.*mm;
	const G4double stripSizeX = 4.*cm;
	const G4double stripSizeY = 1.2*m;
	const G4double stripSizeZ = 1.*cm; // thickness
	const G4ThreeVector tofwPos(2238.89, 0.0, 8275.43);
	G4RotationMatrix *tofwRotation =  new G4RotationMatrix();
	tofwRotation->rotateY(-14.9*deg);
	const G4double stripGapX = (tofwSizeX - nStripTOFW*stripSizeX)/(nStripTOFW - 1);
	// TOFWall //
	G4Box *solidTOFW = new G4Box("TOFW", 0.5*tofwSizeX, 0.5*stripSizeY, 0.5*stripSizeZ);
	fLogicalTOFW = new G4LogicalVolume(solidTOFW, airLanzhou, "TOFW");
	new G4PVPlacement(tofwRotation, tofwPos, fLogicalTOFW,
		"TOFW", logicalWorld, false, 0, checkOverlaps);
	// 30 strips in TOFWall //
	G4Box *solidStrip = new G4Box("strip", 0.5*stripSizeX, 0.5*stripSizeY, 0.5*stripSizeZ);
	fLogicalStrip = new G4LogicalVolume(solidStrip, BC408, "Strip");
	// center X of the 0-th strip in the TOFW frame
	G4double stripStepX = stripSizeX+stripGapX;
	G4double x0 = -(stripStepX*(nStripTOFW/2)-0.5*stripGapX-0.5*stripSizeX);
	for(G4int i = 0; i < nStripTOFW; i++){
		new G4PVPlacement(0, G4ThreeVector(x0+i*stripStepX, 0., 0.), fLogicalStrip,
			"Strip", fLogicalTOFW, false, i, checkOverlaps);
	} // end for over i


	// visualization attributes -----------------------------------------
	// (1., 1., 1.): white; (0.5, 0.5, 0.5): gray; (0., 0., 0.): black
	// (R,G,B); (0., 1., 1.): cyan; (1., 0., 1): magenta; (1., 1., 0): yellow
	// - logical World - //
	G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour(1., 1., 1.));
	visAttributes->SetVisibility(false);
	logicalWorld->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
	// - logical Tstop and TOFW - //
	visAttributes = new G4VisAttributes(G4Colour(0.8, 0.8, 0.)); // light yellow
	fLogicalTstop->SetVisAttributes(visAttributes);
	fLogicalStrip->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
	// for TOFW
	visAttributes = new G4VisAttributes(G4Colour(1., 1., 0.)); // yellow
	fLogicalTOFW->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
	// - logical PDC and DC - //
	visAttributes = new G4VisAttributes(G4Colour(0., 1., 0.)); // green
	fLogicalPDC->SetVisAttributes(visAttributes);
	fLogicalDC->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
	// - logical wirePlanePDC - //
	visAttributes = new G4VisAttributes(G4Colour(1., 0., 1.)); // magenta
	fLogicalWirePlanePDC->SetVisAttributes(visAttributes);
	fLogicalWirePlaneDC->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
	// - logical field - //
	visAttributes = new G4VisAttributes(G4Colour(0.3, 0.3, 0.)); // light yellow
	visAttributes->SetVisibility(false);
	fLogicalBField->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
	// - logical dipo - //
	visAttributes = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9)); // light gray
	fLogicalDipo->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);

	return physicalWorld;
} // end of member function Construct

void ETFDetectorConstruction::ConstructSDandField(){
	// sensitive detectors ----------------------------------------------
	G4SDManager *sdManager = G4SDManager::GetSDMpointer();

	ETFPlaSD *Tstop = new ETFPlaSD("Tstop_SD");
	sdManager->AddNewDetector(Tstop);
	fLogicalTstop->SetSensitiveDetector(Tstop);

	ETFDriftChamberSD *PDC = new ETFDriftChamberSD("PDC_SD");
	sdManager->AddNewDetector(PDC);
	fLogicalWirePlanePDC->SetSensitiveDetector(PDC);

	ETFDriftChamberSD *DC = new ETFDriftChamberSD("DC_SD");
	sdManager->AddNewDetector(DC);
	fLogicalWirePlaneDC->SetSensitiveDetector(DC);

	ETFPlaSD *TOFW = new ETFPlaSD("TOFW_SD");
	sdManager->AddNewDetector(TOFW);
	fLogicalStrip->SetSensitiveDetector(TOFW);
	
	// magnetic field -----------------------------------------------------
	fMagneticField = new ETFMagneticField();
	fFieldMgr = new G4FieldManager();
	fFieldMgr->SetDetectorField(fMagneticField);
	fFieldMgr->CreateChordFinder(fMagneticField);
	G4bool forceToAllDaughters = true;
//	fLogicalDipo->SetFieldManager(fFieldMgr, forceToAllDaughters);
	fLogicalBField->SetFieldManager(fFieldMgr, forceToAllDaughters);
}

