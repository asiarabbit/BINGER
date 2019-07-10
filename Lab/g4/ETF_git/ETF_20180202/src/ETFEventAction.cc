// ETFEventAction.hh -- a brief definition for the class
// Created: Jan. 27, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 27, 2019, by Asia SUN

#include <array>

#include "ETFEventAction.hh"

#include "ETFDriftChamberHit.hh"
#include "ETFPlaHit.hh"
#include "ETFRunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "TTree.h"

using std::array;

// Utility function which finds a hit collection with the given Id
// and print warnings if not found
// unnamed namespace for declarations of static methods and variables
// used in the stead of "static"
G4VHitsCollection *ETFEventAction::GetHC(const G4Event *event, G4int collId){
	G4HCofThisEvent *hce = event->GetHCofThisEvent();
	if(!hce){
		G4ExceptionDescription msg;
		msg << " No hits collection of this event found." << G4endl;
		G4Exception("ETFEventAction::EndOfEventAction()",
					"ETFCode001", JustWarning, msg);
		return nullptr;
	}

	G4VHitsCollection *hc = hce->GetHC(collId);
	if(!hc){
		G4ExceptionDescription msg;
		msg << "Hits collection " << collId
			<< " of this event not found." << G4endl;
		G4Exception("ETFEventAction::EndOfEventAction()",
			"ETFCode001", JustWarning, msg);
	}

	return hc;
}

inline void copy3Vector(double *p, const G4ThreeVector &v){
	p[0] = v[0]; p[1] = v[1]; p[2] = v[2];
}


ETFEventAction::ETFEventAction(ETFRunAction *runAction)
 : G4UserEventAction(), fPlaHCID{-1, -1}, fDriftChamberHCID{-1, -1},
 fRunAction(runAction){
	G4RunManager::GetRunManager()->SetPrintProgress(1);
}

ETFEventAction::~ETFEventAction(){}

void ETFEventAction::BeginOfEventAction(const G4Event *){
	// find hit collections by name (just once)
	// and save them in the data members of this class
	
	if(-1 == fPlaHCID[0]){ // assign only once
		G4SDManager *sdManager = G4SDManager::GetSDMpointer();
		
		// hits collections names
		array<G4String, 2> pHCName // plastic scintillator hit collection name
			= {{"Tstop_SD/PlaColl", "TOFW_SD/PlaColl"}};
		array<G4String, 2> dHCName // drift chamber hit collection name
			= {{"PDC_SD/DriftChamberColl", "DC_SD/DriftChamberColl"}}; // SDname/HCName

		for(G4int i = 0; i < 2; i++){
			// hit collections IDs
			fPlaHCID[i] = sdManager->GetCollectionID(pHCName[i]);
			fDriftChamberHCID[i] = sdManager->GetCollectionID(dHCName[i]);
		}
	} // end if
}

void ETFEventAction::EndOfEventAction(const G4Event *event){
	// acquire the data for one event //
	// access the drift chamber data
	for(G4int i = 0; i < 2; i++){
		G4VHitsCollection *hc = GetHC(event, fDriftChamberHCID[i]);
		if(!hc) return;
		
		G4int nhit = hc->GetSize();
		for(G4int j = 0; j < nhit; j++){
			ETFDriftChamberHit *hit =
				static_cast<ETFDriftChamberHit *>(hc->GetHit(j));
			G4ThreeVector localPos = hit->GetLocalPos();
			G4ThreeVector worldPos = hit->GetWorldPos();
//			G4double time = hit->GetTime();
		}
	}
	// access the plastic scintillator data
	for(G4int i = 0; i < 2; i++){
		G4VHitsCollection *hc = GetHC(event, fPlaHCID[i]);
		if(!hc) return;

		G4int nhit = hc->GetSize();
		for(G4int j = 0; j < nhit; j++){
			ETFPlaHit *hit =
				static_cast<ETFPlaHit *>(hc->GetHit(j));
			G4ThreeVector pos = hit->GetPos();
//			G4double time = hit->GetTime();
		}
	}

	// hits belonging to the primaries //
	static G4int index = 0;
	auto &data_run = fRunAction->fData;
	data_run.initialize();
	data_run.index = index++;
	G4VHitsCollection *hcPrimary[4] =
		{GetHC(event, fPlaHCID[0]), GetHC(event, fPlaHCID[1]),
		GetHC(event, fDriftChamberHCID[0]),
		GetHC(event, fDriftChamberHCID[1])};
	G4int nhit_dc[4]{};
	for(G4int i = 4; i--;) nhit_dc[i] = hcPrimary[i]->GetSize();
	// PIDable event check
	if(nhit_dc[0] >= 1 && nhit_dc[1] >= 1 &&
	   nhit_dc[2] >= 3 && nhit_dc[3] >= 4){
		// plastic scintillators // only use the first hit
		// Tstop
		ETFPlaHit *hitTstop = static_cast<ETFPlaHit *>(hcPrimary[0]->GetHit(0));
		data_run.TstopTime = hitTstop->GetTime();
		data_run.TstopVelocity = hitTstop->GetVelocity();
		data_run.TstopKineticEnergy = hitTstop->GetKineticEnergy();
		copy3Vector(data_run.TstopMomentum, hitTstop->GetMomentum());
		copy3Vector(data_run.TstopPos, hitTstop->GetPos());
		// TOF Wall
		ETFPlaHit *hitTOFW = static_cast<ETFPlaHit *>(hcPrimary[1]->GetHit(0));
		data_run.TOFWStripId = hitTOFW->GetID();
		data_run.TOFWTime = hitTOFW->GetTime();
		data_run.TOFWVelocity = hitTOFW->GetVelocity();
		data_run.TOFWKineticEnergy = hitTOFW->GetKineticEnergy();
		copy3Vector(data_run.TOFWMomentum, hitTOFW->GetMomentum());
		copy3Vector(data_run.TOFWPos, hitTOFW->GetPos());
		// drift chambers //
		// PDCs
		for(int i = 0; i < nhit_dc[2]; i++){
			ETFDriftChamberHit *hitPDC =
				static_cast<ETFDriftChamberHit *>(hcPrimary[2]->GetHit(i));
			G4int dcid = hitPDC->GetDCID();
			G4int wireLayerID = hitPDC->GetWireLayerID(); // wire layer id in the DC
			G4int wid = dcid*2+wireLayerID; // wire layer id in the DC array
			data_run.PDCTime[wid] = hitPDC->GetTime();
			data_run.PDCVelocity[wid] = hitPDC->GetVelocity();
			data_run.PDCKineticEnergy[wid] = hitPDC->GetKineticEnergy();
			copy3Vector(data_run.PDCPos[wid], hitPDC->GetWorldPos());
			copy3Vector(data_run.PDCMomentum[wid], hitPDC->GetMomentum());
		}
		// DCs
		for(int i = 0; i < nhit_dc[3]; i++){
			ETFDriftChamberHit *hitDC =
				static_cast<ETFDriftChamberHit *>(hcPrimary[3]->GetHit(i));
			G4int dcid = hitDC->GetDCID();
			G4int wireLayerID = hitDC->GetWireLayerID(); // wire layer id in the DC
			G4int wid = dcid*2+wireLayerID; // wire layer id in the DC array
			data_run.DCTime[wid] = hitDC->GetTime();
			data_run.DCVelocity[wid] = hitDC->GetVelocity();
			data_run.DCKineticEnergy[wid] = hitDC->GetKineticEnergy();
			copy3Vector(data_run.DCPos[wid], hitDC->GetWorldPos());
			copy3Vector(data_run.DCMomentum[wid], hitDC->GetMomentum());
		}
	}


	// print diagnostics //
	// particle info
	G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
	if(0 == printModulo || event->GetEventID() % printModulo != 0) return;
	
	G4PrimaryParticle *primary = event->GetPrimaryVertex(0)->GetPrimary(0);
	G4ThreeVector p0 = primary->GetMomentum(); // the initial momentum
	G4cout << G4endl
		<< ">>> Event " << event->GetEventID() << " >>> Simulation truth: "
		<< primary->GetG4code()->GetParticleName() << " "
		<< p0 << G4endl;
	data_run.setP0(p0[0], p0[1], p0[2]);
	// plastic scintillators
	for(G4int i = 0; i < 2; i++){
		G4VHitsCollection *hc = GetHC(event, fPlaHCID[i]);
		if(!hc) return;
		G4int nhit = hc->GetSize();
		G4cout << "Pla " << i << " has " << nhit
			<< " hits." << G4endl;
		for(G4int j = 0; j < nhit; j++) hc->GetHit(j)->Print();
	} // end for over i
	// drift chambers
	for(G4int i = 0; i < 2; i++){ // loop over two DC arrays
		G4VHitsCollection *hc = GetHC(event, fDriftChamberHCID[i]);
		if(!hc) return;
		G4int nhit = hc->GetSize();
		G4cout << "Drift Chamber array " << i << " has " << nhit
			<< " hits." << G4endl;
		for(G4int j = 0; j < nhit; j++) hc->GetHit(j)->Print();
	} // end for over i

	fRunAction->fTree->Fill();
} // end member function EndOfEventAction

