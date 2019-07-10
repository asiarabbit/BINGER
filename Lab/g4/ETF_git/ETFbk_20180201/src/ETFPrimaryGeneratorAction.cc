// ETFPrimaryGeneratorAction.cc -- implementation of the class
// Created: Jan. 26, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 26, 2019, by Asia SUN

#include "ETFPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "Randomize.hh"

ETFPrimaryGeneratorAction::ETFPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fMessenger(0),
   fKaon(0), fProton(0), fMomentum(1000.*MeV), fSigmaMomentum(50.*MeV),
   fSigmaAngle(2.*deg), fRandomizePrimary(true){
	G4int  nofParticles = 1;
	fParticleGun = new G4ParticleGun(nofParticles);
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	fPositron = particleTable->FindParticle("e+");
	fMuon = particleTable->FindParticle("mu+");
	fPion = particleTable->FindParticle("pi+");
	fKaon = particleTable->FindParticle("kaon+");
	fProton = particleTable->FindParticle("proton");

	// default particle kinematics
	fParticleGun->SetParticleDefinition(fPositron);
	fParticleGun->SetParticleCharge(6);
	fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -3.*m));
	
	// define commands for this class
	DefineCommands(); // construction of fMessenger
}

ETFPrimaryGeneratorAction::~ETFPrimaryGeneratorAction(){
	delete fParticleGun;
	delete fMessenger;
}

void ETFPrimaryGeneratorAction::GeneratePrimaries(G4Event *event){
	G4ParticleDefinition *particle;
	if(fRandomizePrimary){
		G4int opt = (int)(5.*G4UniformRand()); // 5.*(0~1)
		switch(opt){
			case 0: particle = fPositron; break;
			case 1: particle = fMuon;	  break;
			case 2: particle = fPion;	  break;
			case 3: particle = fKaon;	  break;
			default: particle = fProton;  break;
		} // end of switch
		fParticleGun->SetParticleDefinition(particle);
	}
	else{
		particle = fParticleGun->GetParticleDefinition();
	}

	G4double pp = fMomentum + (G4UniformRand() - 0.5)*fSigmaAngle;
	G4double mass = particle->GetPDGMass();
	// kinetic energy. Mass is in units of equivalent energy
	G4double ekin = std::sqrt(pp*pp+mass*mass) - mass;
	fParticleGun->SetParticleEnergy(ekin);

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
	auto& momentumCmd = fMessenger->DeclarePropertyWithUnit("momentum", "GeV",
		fMomentum, "Mean momentum of primaries");
	momentumCmd.SetParameterName("p", true);
	momentumCmd.SetRange("p>=0.");
	momentumCmd.SetDefaultValue("1.");
	
	// sigmaMomentum command
	G4GenericMessenger::Command& sigmaMomentumCmd =
		fMessenger->DeclarePropertyWithUnit("sigmaMomentum",
			"MeV", fSigmaMomentum, "sigma momentum of primaries");
	// (name, ommitable, currentAsDefault=true)
	sigmaMomentumCmd.SetParameterName("sp", true);
	sigmaMomentumCmd.SetRange("sp>=0.");
	sigmaMomentumCmd.SetDefaultValue("50.");

	// sigmaMomentum command
	G4GenericMessenger::Command& sigmaAngleCmd =
		fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle,
			"Sigma angle divergence of primaries");
	sigmaAngleCmd.SetParameterName("t", true);
	sigmaAngleCmd.SetRange("t>=0.");
	sigmaAngleCmd.SetDefaultValue("2.");

	// randomizePrimary command
	G4GenericMessenger::Command& randomCmd
		= fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
	G4String guidance
		= "Boolean flag for randomizing primary particle types.\n";
	guidance
		+= "In case this flag is false, you can select the primary particle.\n";
	guidance += "  with /gun/particle command.";
	randomCmd.SetGuidance(guidance);
	randomCmd.SetParameterName("flag", "true");
	randomCmd.SetDefaultValue("true");
}

