// B1RunAction.hh -- a brief definition of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, 2019, by Asia SUN

#ifndef B1RunAction_h
#define B1RunAction_h

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;
class TFile;
class TH1F;
class TTree;

// run action class //
// in EndOfRunAction(), it calculates the dose in the selected volume
// from the energy deposit accumulated via stepping and event actions
// The computed dose is then printed on the screen

class B1RunAction : public G4UserRunAction{
public:
	B1RunAction();
	virtual ~B1RunAction();

	virtual void BeginOfRunAction(const G4Run *);
	virtual void EndOfRunAction(const G4Run *);

	void AddEdep(G4double edep);
	void FillFinalEnergy(G4double ef);

private:
	G4Accumulable<G4double> fEdep;
	G4Accumulable<G4double> fEdep2;
	TFile *fFile; // pointer to a root file for result storage
	TH1F *fHef; // pointer to histogram of final kinetic energy
	TTree *fTree; // the tree to store the final energy
	G4double fEf; // the final energy
};

#endif
