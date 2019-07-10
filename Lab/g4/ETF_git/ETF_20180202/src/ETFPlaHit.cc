// ETFPlaHit.cc -- implementation of the class for hits of DCs
// Created: Jan. 23, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 23, 2019 by Asia SUN

#include "ETFPlaHit.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Transform3D.hh"
#include "G4LogicalVolume.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<ETFPlaHit> *ETFPlaHitAllocator;

ETFPlaHit::ETFPlaHit(G4int id, G4double time)
 : G4VHit(), fId(id), fTime(time), fKineticEnergy(-9999.),
   fPos(), fMomentum(), fRot(), fLogVol(0){}
 
ETFPlaHit::~ETFPlaHit(){}

// copy constructor
ETFPlaHit::ETFPlaHit(const ETFPlaHit &right)
 :  G4VHit(), fId(right.fId), fTime(right.fTime),
	fPos(right.fPos), fLogVol(right.fLogVol){}

const ETFPlaHit &ETFPlaHit::operator=(const ETFPlaHit &right){
  fId = right.fId;
  fTime = right.fTime;
  fPos = right.fPos;
  fRot = right.fRot;
  fLogVol = right.fLogVol;
  return *this;
}

int ETFPlaHit::operator==(const ETFPlaHit &) const{
	return 0;
}

void ETFPlaHit::Draw(){
	G4VVisManager *visManager = G4VVisManager::GetConcreteInstance();
	if(!visManager) return;

	G4Transform3D trans(fRot.inverse(), fPos);
	G4VisAttributes attribs;
	const G4VisAttributes *pVA = fLogVol->GetVisAttributes();
	if(pVA) attribs = *pVA;
	attribs.SetColour(G4Colour(0., 1., 1.));
	attribs.SetForceSolid(true);
	visManager->Draw(*fLogVol, attribs, trans);
}

const std::map<G4String, G4AttDef> *ETFPlaHit::GetAttDefs() const{
	G4bool isNew;
	std::map<G4String, G4AttDef> *store = G4AttDefStore::GetInstance("ETFPlaHit", isNew);
	
	if(isNew){
		(*store)["HitType"]
			//             name, description, category, extra, valueType
			= G4AttDef("HitType", "Hit Type", "Physics", "", "G4String");
		(*store)["ID"]
			= G4AttDef("ID", "ID", "Physics", "", "G4int");
		(*store)["Time"]
			= G4AttDef("Time", "Time", "Physics", "", "G4double");
		(*store)["Velocity"]
			= G4AttDef("Velocity", "Velocity", "Physics", "", "G4double");
		(*store)["KineticEnergy"]
			= G4AttDef("KineticEnergy", "KineticEnergy", "Physics", "", "G4double");
		(*store)["Pos"]
			= G4AttDef("Pos", "Position", "Physics", "G4BestUnit", "G4ThreeVector");
		(*store)["Momentum"]
			= G4AttDef("Momentum", "Energy", "Physics", "G4BestUnit", "G4ThreeVector");
		(*store)["LVol"]
			= G4AttDef("LVol", "Logical Volume", "Physics", "", "G4String");
	}

	return store;
}

std::vector<G4AttValue> *ETFPlaHit::CreateAttValues() const{
	std::vector<G4AttValue> *values = new std::vector<G4AttValue>;
	
	values->push_back(G4AttValue("HitType", "ETFHit", ""));
	values->push_back(G4AttValue("ID", G4UIcommand::ConvertToString(fId), ""));
	values->push_back(G4AttValue("Time", G4BestUnit(fTime, "Time"), ""));
	// (G4String &name, G4String &value, G4String &showLabel)
	values->push_back(G4AttValue("Velocity", G4UIcommand::ConvertToString(fVelocity), " mm/ns"));
	values->push_back(G4AttValue("KineticEnergy", G4BestUnit(fKineticEnergy, "Energy"), ""));
	values->push_back(G4AttValue("Pos", G4BestUnit(fPos, "Length"), ""));
	values->push_back(G4AttValue("Momentum", G4BestUnit(fMomentum, "Energy"), ""));
	
	if(fLogVol)
		values->push_back(G4AttValue("LVol", fLogVol->GetName(), ""));
	else
		values->push_back(G4AttValue("LVol", "", ""));

	return values;
}

void ETFPlaHit::Print(){
	if(fLogVol){
		G4cout << "  --" << fLogVol->GetName() << G4endl;
	}
	G4cout << "  Strip[" << fId << "] ------------" << G4endl;
	G4cout << "  Time: " << fTime/ns << " ns" << G4endl;
	G4cout << "  Velocity: " << fVelocity << " mm/ns" << G4endl;
	G4cout << "  KineticEnergy: " << G4BestUnit(fKineticEnergy, "Energy") << G4endl;
	G4cout << "  WorldPos(x, y, z):    (" << fPos.x() << ", " << fPos.y() << ", " << fPos.z() << ")" << G4endl;
	G4cout << "  Momentum(px, py, pz): (" << fMomentum.x() << ", " << fMomentum.y()
		   << ", " << fMomentum.z() << ")" << G4endl;
}

