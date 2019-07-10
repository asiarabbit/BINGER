// B1G4DetectorConstruction.cc -- the implementation of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh" // a phi segment of a cone
#include "G4Orb.hh" // an orb
#include "G4Sphere.hh"
#include "G4Trd.hh" // a trapezoid
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh" // all kinds of units imported from CLHEP library

B1DetectorConstruction::B1DetectorConstruction()
	: G4VUserDetectorConstruction(), fScoringVolume(0){}

B1DetectorConstruction::~B1DetectorConstruction(){}

G4VPhysicalVolume *B1DetectorConstruction::Construct(){
	// get nist material manager
	G4NistManager *nist = G4NistManager::Instance();


	// -- World -> envelope -> shape1 + shape2; shape2 is the scoring volume -- //
	// option to switch on/off checking of volumes overlaps
	G4bool checkOverlaps = true;
	// envelope parameters
	G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;

	// world -- solid->logic->physical
	G4double world_sizeXY = 1.2*env_sizeXY;
	G4double world_sizeZ  = 1.2*env_sizeZ;
	G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Box *solidWorld = 
		new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
	// a name is madatory for the construction of a logical volume
	G4LogicalVolume *logicalWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), 
		logicalWorld, "World", 0, false, 0, checkOverlaps);

	// envelope -- solid->logic->physical
	G4Material *env_mat = nist->FindOrBuildMaterial("G4_WATER");
	G4Box *solidEnv = new G4Box("Envelope", 0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");
	new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope",
		logicalWorld, false, 0, checkOverlaps);

	// shape 1 -- solid->logic->physical
	G4Material *shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
	G4ThreeVector pos1 = G4ThreeVector(0., 2.*cm, -7.*cm);
	// Conical section shape
	G4double shape1_rmina = 0.*cm, shape1_rmaxa = 2.*cm;
	G4double shape1_rminb = 0.*cm, shape1_rmaxb = 4.*cm;
	G4double shape1_hz = 3.*cm; // half of the dimension along z-axis
	G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
	G4Cons *solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa,
		shape1_rminb, shape1_rmaxb, shape1_hz, shape1_phimin, shape1_phimax);
	G4LogicalVolume *logicShape1 = new G4LogicalVolume(solidShape1, shape1_mat, "Shape1");
	new G4PVPlacement(0, pos1, logicShape1, "Shape1", logicEnv, false, 0, checkOverlaps);

	// shape2
	G4Material *shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
	G4ThreeVector pos2 = G4ThreeVector(0, -1.*cm, 7.*cm);
	// Trapezoid shape
	G4double shape2_dxa = 12.*cm, shape2_dxb = 12.*cm;
	G4double shape2_dya = 10.*cm, shape2_dyb = 16.*cm;
	G4double shape2_dz  = 6.*cm;
	G4Trd *solidShape2 = new G4Trd("Shape2", 0.5*shape2_dxa, 0.5*shape2_dxb,
		0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz);
	G4LogicalVolume *logicShape2 = new G4LogicalVolume(solidShape2, shape2_mat, "Shape2");
	new G4PVPlacement(0, pos2, logicShape2, "Shape2", logicEnv, false, 0, checkOverlaps);
	
	// set Shape2 as scoring volume
	fScoringVolume = logicShape2;
	
	// always return the physical World
	return physWorld;
} // end of the member function Construct
    


