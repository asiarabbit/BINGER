// ETFPlaHit.cc -- implementation of the class for plastic scintillators readout
// Created: Jan. 23, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 23, 2019 by Asia SUN

#include "ETFPlaSD.hh"

#include "ETFPlaHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"

ETFPlaSD::ETFPlaSD(G4String name)
 : G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1){
	// collectionName is a container class object and data member from the father class
	collectionName.insert("PlaColl");
}

ETFPlaSD::~ETFPlaSD(){}

void ETFPlaSD::Initialize(G4HCofThisEvent *hce){
	fHitsCollection = new ETFPlaHitsCollection(SensitiveDetectorName, collectionName[0]);
	
	if(fHCID < 0){
		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
	}
	hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool ETFPlaSD::ProcessHits(G4Step *step, G4TouchableHistory *){
	G4Track *track = step->GetTrack();
	const G4ParticleDefinition *particle = track->GetParticleDefinition();
	G4int parentID = track->GetParentID();
	G4int trackID = track->GetTrackID();
	G4String particleName = particle->GetParticleName();
	G4double charge = particle->GetPDGCharge();
	G4double mass = particle->GetPDGMass();
	if(0 == charge) return true;

	G4StepPoint *preStepPoint = step->GetPreStepPoint();
	const G4VTouchable *touchable = preStepPoint->GetTouchable(); // logVol+heirarchy+rot+pos
	G4VPhysicalVolume *physical = touchable->GetVolume(); // depth == 0
	G4int copyNo = physical->GetCopyNo();
	G4double hitTime = preStepPoint->GetGlobalTime();
	G4double velocity = preStepPoint->GetVelocity();
	G4double kineticEnergy = preStepPoint->GetKineticEnergy();
	G4ThreeVector worldPos = preStepPoint->GetPosition();
	G4ThreeVector localPos
		= touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
	G4ThreeVector momentum = preStepPoint->GetMomentum();
	

	// check if this strip already has a hit
	// because the earlist hit registered the first
	// the followers would be synchronised to the earliest one and skip registry
	G4int ix = -1;
	for(G4int i = 0; i < fHitsCollection->entries(); i++){
		if((*fHitsCollection)[i]->GetID() == copyNo){
			ix = i;
			break;
		}
	}
	if(ix >= 0){
		if((*fHitsCollection)[ix]->GetTime() > hitTime){
			(*fHitsCollection)[ix]->SetTime(hitTime);
		}
	}
	else{
		// if not, create a new hit and set it to the collection
		ETFPlaHit *hit = new ETFPlaHit(copyNo, hitTime);
		hit->SetVelocity(velocity);
		hit->SetKineticEnergy(kineticEnergy);
		hit->SetMomentum(momentum);
		hit->SetLogVol(physical->GetLogicalVolume()); // for volume name and drawing
		// auto would remove reference
		auto transform = touchable->GetHistory()->GetTopTransform();
		transform.Invert();
		hit->SetRot(transform.NetRotation());
//		hit->SetPos(transform.NetTranslation());
		hit->SetPos(worldPos);

		// store hits that belongs to the primaries //
		static G4int runVerbose = G4RunManager::GetRunManager()->GetVerboseLevel();
		if(0 == parentID && 1 == trackID){
			if(runVerbose > 0){
				G4cout << "--- ETFEventAction::ProcessOneHit() ---" << G4endl;
				G4cout << touchable->GetVolume(1)->GetName() << G4endl;
				G4cout << hit->GetLogVol()->GetName() << G4endl;
				G4cout << particleName << " Q: " << charge << " M: " << mass << G4endl;
				G4cout << "-------------------------------------------------" << G4endl;
			}
			fHitsCollection->insert(hit);
		}
	} // end else


	return true;	
}
