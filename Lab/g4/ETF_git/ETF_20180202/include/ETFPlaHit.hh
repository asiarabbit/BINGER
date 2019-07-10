// ETFPlaHit.hh -- a brief definition of the class for hits of DCs
// Created: Jan. 23, 2019, by Asia SUN, asia.rabbit@163.com
// Last Modified: Jan. 23, 2019 by Asia SUN

#ifndef ETFPlaHit_h
#define ETFPlaHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4RotationMatrix.hh"

class G4LogicalVolume;

// class description
// This class is for plastic scintillator signal

class ETFPlaHit : public G4VHit{
public:
	ETFPlaHit(G4int id, G4double time);
	ETFPlaHit(const ETFPlaHit &right);
	virtual ~ETFPlaHit();
	
	const ETFPlaHit &operator=(const ETFPlaHit &right);
	int operator==(const ETFPlaHit &right) const;

	inline void *operator new(size_t);
	inline void operator delete(void *hit);
	
	virtual void Draw();
	virtual const std::map<G4String, G4AttDef> *GetAttDefs() const;
	virtual std::vector<G4AttValue> *CreateAttValues() const;
	virtual void Print();

	G4int GetID() const{ return fId; }
	
	void SetTime(G4double time){ fTime = time; }
	G4double GetTime() const{ return fTime; }

	void SetKineticEnergy(G4double energy){ fKineticEnergy = energy; }
	G4double GetKineticEnergy() const{ return fKineticEnergy; }

	void SetVelocity(G4double v){ fVelocity = v; }
	G4double GetVelocity() const{ return fVelocity; }
	
	void SetPos(G4ThreeVector pos){ fPos = pos; }
	G4ThreeVector GetPos() const{ return fPos; }
	
	void SetMomentum(G4ThreeVector momentum){ fMomentum = momentum; }
	G4ThreeVector GetMomentum() const{ return fMomentum; }
	
	void SetRot(G4RotationMatrix rmat){ fRot = rmat; }
	G4RotationMatrix GetRot() const{ return fRot; }

	void SetLogVol(G4LogicalVolume *logVol){ fLogVol = logVol; }
	G4LogicalVolume *GetLogVol() const{ return fLogVol; }
	
private:
	G4int fId;
	G4double fTime;
	G4double fKineticEnergy;
	G4double fVelocity; // velocity
	G4ThreeVector fPos;
	G4ThreeVector fMomentum;
	G4RotationMatrix fRot;
	G4LogicalVolume *fLogVol;
};

using ETFPlaHitsCollection = G4THitsCollection<ETFPlaHit>;

extern G4ThreadLocal G4Allocator<ETFPlaHit> *ETFPlaHitAllocator;

inline void *ETFPlaHit::operator new(size_t){
	if(!ETFPlaHitAllocator) ETFPlaHitAllocator = new G4Allocator<ETFPlaHit>;
	return (void*)ETFPlaHitAllocator->MallocSingle();
}

inline void ETFPlaHit::operator delete(void *hit){
	ETFPlaHitAllocator->FreeSingle((ETFPlaHit*)hit);
}

#endif
