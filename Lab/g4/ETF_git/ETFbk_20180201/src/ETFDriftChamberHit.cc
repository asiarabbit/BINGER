// ETFDriftChamberHit.cc -- implementation of the class for hits of DCs
// Created: Jan. 22, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 31, 2019 by Asia SUN

#include <map>

#include "ETFDriftChamberHit.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh" // G4BestUnit
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


G4ThreadLocal G4Allocator<ETFDriftChamberHit> *ETFDriftChamberHitAllocator;

ETFDriftChamberHit::ETFDriftChamberHit()
 : G4VHit(), fDCID(-1), fWireLayerID(-1), fTime(0.), fLocalPos(), fWorldPos(){
}

ETFDriftChamberHit::ETFDriftChamberHit(G4int DCID)
 : G4VHit(), fDCID(DCID), fTime(0.), fLocalPos(), fWorldPos(){
}

ETFDriftChamberHit::~ETFDriftChamberHit(){}

ETFDriftChamberHit::ETFDriftChamberHit(const ETFDriftChamberHit &right)
 : G4VHit(), fDCID(right.fDCID), fTime(right.fTime),
   fLocalPos(right.fLocalPos), fWorldPos(right.fWorldPos){
}

ETFDriftChamberHit &ETFDriftChamberHit::operator=(const ETFDriftChamberHit &right){
	fDCID = right.fDCID;
	fTime = right.fTime;
	fLocalPos = right.fLocalPos;
	fWorldPos = right.fWorldPos;
	return *this;
}

int ETFDriftChamberHit::operator==(const ETFDriftChamberHit &) const{
	return 0;
}

void ETFDriftChamberHit::Draw(){
	G4VVisManager *visManager = G4VVisManager::GetConcreteInstance();
	if(!visManager) return;
	
	G4Circle circle(fWorldPos);
	circle.SetScreenSize(4);
	circle.SetFillStyle(G4Circle::filled);
	// milky green
	circle.SetVisAttributes(new G4VisAttributes(G4Colour(1., 0., 0.)));
	visManager->Draw(circle);
}

const std::map<G4String, G4AttDef> *ETFDriftChamberHit::GetAttDefs() const{
	G4bool isNew; // check if "ETFDriftChamberHit" is a new element in the map
	std::map<G4String, G4AttDef> *store
		= G4AttDefStore::GetInstance("ETFDriftChamberHit", isNew);
	
	if(isNew){
		// the following two constructor styles are equivalent
		(*store)["HitType"]
			= G4AttDef("HitType", "Hit Type", "Physics", "", "G4String");
		store->insert(std::pair<G4String, G4AttDef>("DCID",
			G4AttDef("DCID", "Drift Chamber Id", "Detector", "", "G4int")));
		store->insert(std::pair<G4String, G4AttDef>("WireLayerID",
			G4AttDef("WireLayerID", "Sense Wire Layer ID", "Detector", "", "G4int")));
		(*store)["Time"]
			= G4AttDef("Time", "Time", "Physics", "G4BestUnit", "G4double");
		(*store)["Velocity"]
			= G4AttDef("Velocity", "Velocity", "Physics", "", "G4double");
		(*store)["KineticEnergy"]
			= G4AttDef("KineticEnergy", "Kinetic Energy", "Physics", "G4BestUnit", "G4double");
		(*store)["Pos"]
			= G4AttDef("Pos", "Position", "Physics", "G4BestUnit", "G4ThreeVector");
		(*store)["Momentum"]
			= G4AttDef("Momentum", "Energy", "Physics", "G4BestUnit", "G4ThreeVector");
	}
	
	return store;
}

std::vector<G4AttValue> *ETFDriftChamberHit::CreateAttValues() const{
	std::vector<G4AttValue> *values = new std::vector<G4AttValue>;

	values->push_back(G4AttValue("HitType", "DriftCahmberHit", ""));
	values->push_back(G4AttValue("DCID", G4UIcommand::ConvertToString(fDCID), ""));
	values->push_back(G4AttValue("WireLayerID", G4UIcommand::ConvertToString(fWireLayerID), ""));
	values->push_back(G4AttValue("Time", G4BestUnit(fTime, "Time"), ""));
	values->push_back(G4AttValue("Velocity", G4UIcommand::ConvertToString(fVelocity), " mm/ns"));
	values->push_back(G4AttValue("KineticEnergy", G4BestUnit(fKineticEnergy, "Energy"), ""));
	values->push_back(G4AttValue("LocalPos", G4BestUnit(fLocalPos, "Length"), ""));
	values->push_back(G4AttValue("WorldPos", G4BestUnit(fWorldPos, "Length"), ""));
	values->push_back(G4AttValue("Momentum", G4BestUnit(fMomentum, "Energy"), ""));

	return values;
}

void ETFDriftChamberHit::Print(){
	G4cout << "  DC[" << fDCID << "], WireLayer[" << fWireLayerID << "]" << G4endl;
	G4cout << "  Time " << fTime/ns << " ns ---" << G4endl;
	G4cout << "  Velocity: " << fVelocity << " mm/ns" << G4endl;
	G4cout << "  KineticEnergy: " << G4BestUnit(fKineticEnergy, "Energy") << G4endl;
	G4cout << "  Local (x, y, z): (" << fLocalPos.x() << ", " << fLocalPos.y()
		<< ", " << fLocalPos.z() << ")" << G4endl;
	G4cout << "  World (x, y, z): (" << fWorldPos.x() << ", " << fWorldPos.y()
		<< ", " << fWorldPos.z() << ")" << G4endl;
	G4cout << "  Momentum(px, py, pz): (" << fMomentum.x() << ", " << fMomentum.y()
		   << ", " << fMomentum.z() << ")" << G4endl;
}

