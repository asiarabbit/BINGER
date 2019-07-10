// ETFDriftChamberSD.cc -- implementation of the class for Sensitive Detectors of DCs
// Created: Jan. 22, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 22, 2019 by Asia SUN

#include "ETFDriftChamberSD.hh"

#include "ETFDriftChamberHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4HCofThisEvent.hh"

ETFDriftChamberSD::ETFDriftChamberSD(G4String name)
 : G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1){
	// collectionName is a container class object and data mamber from the father class
	collectionName.insert("DriftChamberColl");
}

ETFDriftChamberSD::~ETFDriftChamberSD(){}

void ETFDriftChamberSD::Initialize(G4HCofThisEvent *hce){
	// SensitiveDetectorName and collectionName are all data member from the father class
	fHitsCollection
		 = new ETFDriftChamberHitsCollection(SensitiveDetectorName, collectionName[0]);
	if(fHCID < 0){
		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
	}
	hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool ETFDriftChamberSD::ProcessHits(G4Step *step, G4TouchableHistory *){
	G4double charge = step->GetTrack()->GetParticleDefinition()->GetPDGCharge();
	if(0 == charge) return true;

	G4StepPoint *preStepPoint = step->GetPreStepPoint();
	// logVol+heirarchy+rot+pos
	const G4VTouchable *touchable = preStepPoint->GetTouchable();
	// mother volume: the mother for the wire plane
	G4int wireLayerID = touchable->GetVolume(0)->GetCopyNo(); // the wire plane
	G4int dcid = touchable->GetVolume(1)->GetCopyNo(); // the mother volume
	G4double hitTime = preStepPoint->GetGlobalTime();
	G4double velocity = preStepPoint->GetVelocity();
	G4double kineticEnergy = preStepPoint->GetKineticEnergy();
	G4ThreeVector worldPos = preStepPoint->GetPosition();
	G4ThreeVector localPos
		= touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
	G4ThreeVector momentum = preStepPoint->GetMomentum();

	ETFDriftChamberHit *hit = new ETFDriftChamberHit(dcid);
	hit->SetWireLayerID(wireLayerID);
	hit->SetTime(hitTime);
	hit->SetVelocity(velocity);
	hit->SetKineticEnergy(kineticEnergy);
	hit->SetWorldPos(worldPos);
	hit->SetLocalPos(localPos);
	hit->SetMomentum(momentum);

	// hits belonging to the primaries
	G4Track *track = step->GetTrack();
	G4int parentID = track->GetParentID();
	G4int trackID = track->GetTrackID();
	if(0 == parentID && 1 == trackID){
		fHitsCollection->insert(hit); // push_back
	}


	return true;
}

