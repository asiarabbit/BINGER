// B1PrimaryGeneratorAction.hh -- a brief definition of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

// the primary generator action class with particle gun
// the default kinematic is a 6 MeV gamma, randomly distributed
// in front of the phantom across 80% of the (X, Y) phantom size

class B1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
public:
	B1PrimaryGeneratorAction();
	virtual ~B1PrimaryGeneratorAction();
	
	// method from the base class
	virtual void GeneratePrimaries(G4Event *);
	
	// method to access particle gun
	const G4ParticleGun *GetParticleGun() const{ return fParticleGun; }

private:
	G4ParticleGun *fParticleGun;
	G4Box *fEnvelopeBox;
};


#endif
