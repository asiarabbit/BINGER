// ETFPrimaryGeneratorAction.cc -- implementation of the class
// Created: Jan. 26, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 26, 2019, by Asia SUN

#include "ETFPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include "G4GenericIon.hh"

ETFPrimaryGeneratorAction::ETFPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fMessenger(0),
   fKineticEnergyPerAmu(0.), fKineticEnergy(0.), fSigmaEkPerAmu(0.),
   fMomentum(0.), fSigmaAngle(2.*deg){

	fKineticEnergyPerAmu = 240.*MeV;

	G4int  nofParticles = 1;
	fParticleGun = new G4ParticleGun(nofParticles);

	// define commands for this class
	DefineCommands(); // construction of fMessenger
}

ETFPrimaryGeneratorAction::~ETFPrimaryGeneratorAction(){
	delete fParticleGun;
	delete fMessenger;
}

void ETFPrimaryGeneratorAction::ConstructIon(G4int Z, G4int A){
	static G4bool isCalled = false;
	if(isCalled) return;

	G4double ionCharge = Z*eplus;
	G4double exciteEnergy = 0.*keV;
	G4ParticleDefinition *ion
		= G4IonTable::GetIonTable()->GetIon(Z, A, exciteEnergy);
	fParticle = ion;
	fParticleGun->SetParticleDefinition(ion);
	fParticleGun->SetParticleCharge(ionCharge);
	fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -3.*m));

	isCalled = true;	
}

void ETFPrimaryGeneratorAction::GeneratePrimaries(G4Event *event){
	G4int Z = 9, A = 18;
	ConstructIon(Z, A);
	G4cout << "ETFPrimaryGeneratorAction::GeneratePrimaries: "
		<< "Particle Name: " << fParticle->GetParticleName() << G4endl;
	fParticle->DumpTable();

	G4double ekPerAmu =
		fKineticEnergyPerAmu + (G4UniformRand() - 0.5)*fSigmaEkPerAmu;
	// kinetic energy. Mass is in units of equivalent energy
	G4double mass = fParticle->GetPDGMass();
	G4double atomicMass = fParticle->GetAtomicMass();
	// A = (mass / amu_c2)
	fKineticEnergy = ekPerAmu * atomicMass;
	fParticleGun->SetParticleEnergy(fKineticEnergy);

	fMomentum = std::sqrt(fKineticEnergy * (fKineticEnergy + 2. * mass));

	G4double angle = (G4UniformRand() - 0.5)*fSigmaAngle;
	// (sinTheta*cosPhi, sinTheta*sinPhi, cosTheta), Phi = 0.
	fParticleGun->SetParticleMomentumDirection(
		G4ThreeVector(std::sin(angle), 0., std::cos(angle)));

	fParticleGun->GeneratePrimaryVertex(event);
}

void ETFPrimaryGeneratorAction::DefineCommands(){
	// momentum unit: GeV/c
	fMessenger =
		new G4GenericMessenger(this, "/ETF/generator/", "Primary generator control");
	auto& momentumCmd = fMessenger->DeclarePropertyWithUnit("KineticEnergyPerAmu", "MeV",
		fKineticEnergyPerAmu, "Ek/u for primaries");
	momentumCmd.SetParameterName("EkPerAmu/u", true);
	momentumCmd.SetRange("EkPerAmu>=0.");
	momentumCmd.SetDefaultValue("240.");

	// sigmaMomentum command
	G4GenericMessenger::Command& sigmaEkPerAmuCmd =
		fMessenger->DeclarePropertyWithUnit("sigmaEkPerAmu",
			"MeV", fSigmaEkPerAmu, "sigma EkPerAmu of primaries");
	// (name, ommitable, currentAsDefault=true)
	sigmaEkPerAmuCmd.SetParameterName("sigmaEkPerAmu", true);
	sigmaEkPerAmuCmd.SetRange("sigmaEkPerAmu>=0.");
	sigmaEkPerAmuCmd.SetDefaultValue("5.");

	// sigmaMomentum command
	G4GenericMessenger::Command& sigmaAngleCmd =
		fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle,
			"Sigma angle divergence of primaries");
	sigmaAngleCmd.SetParameterName("t", true);
	sigmaAngleCmd.SetRange("t>=0.");
	sigmaAngleCmd.SetDefaultValue("2.");
}

