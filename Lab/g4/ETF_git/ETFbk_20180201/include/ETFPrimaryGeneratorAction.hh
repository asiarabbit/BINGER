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
	
	void SetMomentum(G4double val){ fMomentum = val; }
	G4double GetMomentum() const{ return fMomentum; }
	
	void SetSigmaMomentum(G4double val){ fSigmaMomentum = val; }
	G4double GetSigmaMomentum() const{ return fSigmaMomentum; }

	void SetSigmaAngle(G4double val){ fSigmaAngle = val; }
	G4double GetSigmaAngle() const{ return fSigmaAngle; }
	
	void SetRandomize(G4double val){ fRandomizePrimary = val; }
	G4bool G4Randomize() const{ return fRandomizePrimary; }

private:
	void DefineCommands();

	G4ParticleGun *fParticleGun;
	G4GenericMessenger *fMessenger;
	G4ParticleDefinition *fPositron;
	G4ParticleDefinition *fMuon;
	G4ParticleDefinition *fPion;
	G4ParticleDefinition *fKaon;
	G4ParticleDefinition *fProton;

	G4double fMomentum;
	G4double fSigmaMomentum;
	G4double fSigmaAngle;
	G4bool fRandomizePrimary;
};

#endif
