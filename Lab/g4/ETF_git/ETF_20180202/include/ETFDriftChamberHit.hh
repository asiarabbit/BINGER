// ETFDriftChamberHit.hh -- a brief definition of the class for hits of DCs
// Created: Jan. 22, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 23, 2019 by Asia SUN

#ifndef ETFDriftChamberHit_h
#define ETFDriftChamberHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

// class description
// This class records time and position of a particle's passage through wire plane

class ETFDriftChamberHit : public G4VHit{
public:
	ETFDriftChamberHit();
	ETFDriftChamberHit(G4int DCID);
	ETFDriftChamberHit(const ETFDriftChamberHit &right);
	virtual ~ETFDriftChamberHit();
	
	ETFDriftChamberHit &operator=(const ETFDriftChamberHit &right);
	int operator==(const ETFDriftChamberHit &right) const;

	inline void *operator new(size_t);
	inline void operator delete(void *hit);
	
	virtual void Draw();
	virtual const std::map<G4String, G4AttDef> *GetAttDefs() const;
	virtual std::vector<G4AttValue> *CreateAttValues() const;
	virtual void Print();

	void SetDCID(G4int id){ fDCID = id; }
	G4int GetDCID() const{ return fDCID; }

	void SetWireLayerID(G4int id){ fWireLayerID = id; }
	G4int GetWireLayerID() const{ return fWireLayerID; }

	void SetTime(G4double t){ fTime = t; }
	G4double GetTime() const{ return fTime; }
	
	void SetKineticEnergy(G4double energy){ fKineticEnergy = energy; }
	G4double GetKineticEnergy() const{ return fKineticEnergy; }
	
	void SetVelocity(G4double v){ fVelocity = v; }
	G4double GetVelocity() const{ return fVelocity; }
	
	void SetLocalPos(const G4ThreeVector p){ fLocalPos = p; }
	G4ThreeVector GetLocalPos() const{ return fLocalPos; }
	
	void SetWorldPos(const G4ThreeVector p){ fWorldPos = p; }
	G4ThreeVector GetWorldPos() const{ return fWorldPos; }
	
	void SetMomentum(const G4ThreeVector p){ fMomentum = p; }
	G4ThreeVector GetMomentum() const{ return fMomentum; }
protected:
	G4int fDCID; // DC0-1-2 -- PDC2-3
	G4int fWireLayerID; // X1 or X2
	G4double fTime;
	G4double fKineticEnergy;
	G4double fVelocity; // velocity
	G4ThreeVector fLocalPos;
	G4ThreeVector fWorldPos;
	G4ThreeVector fMomentum;
};

using ETFDriftChamberHitsCollection = G4THitsCollection<ETFDriftChamberHit>;

extern G4ThreadLocal G4Allocator<ETFDriftChamberHit> *ETFDriftChamberHitAllocator;

inline void *ETFDriftChamberHit::operator new(size_t){
	if(!ETFDriftChamberHitAllocator){
		ETFDriftChamberHitAllocator = new G4Allocator<ETFDriftChamberHit>;
	}
	return (void*)ETFDriftChamberHitAllocator->MallocSingle();
}

inline void ETFDriftChamberHit::operator delete(void *hit){
	ETFDriftChamberHitAllocator->FreeSingle((ETFDriftChamberHit*)hit);
}

#endif
