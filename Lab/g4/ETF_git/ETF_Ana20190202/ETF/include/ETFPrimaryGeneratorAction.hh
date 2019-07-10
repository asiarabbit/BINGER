// ETFPrimaryGeneratorAction.hh -- a brief definition for particle generation
// Created: Jan. 26, 2019, transcribed by Asia SUN
// Last modified: Jan. 26, 2019

#ifndef ETFPrimaryGeneratorAction_h
#define ETFPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;

// class description
// a single particle is generated
// usr can select
// - the initial momentum and angle
// - the momentum and angle spreads

class ETFPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
public:
	ETFPrimaryGeneratorAction();
	virtual ~ETFPrimaryGeneratorAction();
	
	virtual void GeneratePrimaries(G4Event *);
	
	void SetKineticEnergyPerAmu(G4double val){ fKineticEnergyPerAmu = val; }
	G4double GetKineticEnergyPerAmu() const{ return fKineticEnergyPerAmu; }
	
	void SetSigmaEkPerAmu(G4double val){ fSigmaEkPerAmu = val; }
	G4double GetSigmaEkPerAmu() const{ return fSigmaEkPerAmu; }

	void SetSigmaAngle(G4double val){ fSigmaAngle = val; }
	G4double GetSigmaAngle() const{ return fSigmaAngle; }

	void ConstructIon(G4int Z, G4int A); // naked nucleus
private:
	void DefineCommands();

	G4ParticleGun *fParticleGun;
	G4GenericMessenger *fMessenger;

	G4ParticleDefinition *fParticle;

	G4double fKineticEnergyPerAmu;
	G4double fKineticEnergy;
	G4double fSigmaEkPerAmu;
	G4double fMomentum;
	G4double fSigmaAngle;
};

#endif
