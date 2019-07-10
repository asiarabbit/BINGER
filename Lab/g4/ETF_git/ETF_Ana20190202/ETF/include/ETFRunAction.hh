// ETFRunAction.hh -- a brief definition for the class
// Created: Jan. 27, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 27, 2019, by Asia SUN

#ifndef ETFRunAction_h
#define ETFRunAction_h

#include "G4UserRunAction.hh"

// ROOT class forward declarations
class TFile;
class TTree;

// data for each event
struct data_t{
	G4int index;
	
	// particle identity
	G4double A;
	G4double Z;

	G4double p0[3]; // initial momentum for the primary

	G4int TstopNhit;
	G4double TstopTime;
	G4double TstopVelocity;
	G4double TstopKineticEnergy; // kineticEnergy
	G4double TstopPos[3]; // [x-y-z]
	G4double TstopMomentum[3];

	G4int TOFWNhit;
	G4int TOFWStripId;
	G4double TOFWTime;
	G4double TOFWVelocity;
	G4double TOFWKineticEnergy;
	G4double TOFWPos[3];
	G4double TOFWMomentum[3];

	G4int PDCArrNhit;
	G4double PDCTime[4];
	G4double PDCVelocity[4];
	G4double PDCKineticEnergy[4];
	G4double PDCPos[4][3]; // [wire_layer - (x, y, z)]
	G4double PDCMomentum[4][3];

	G4int DCArrNhit;
	G4double DCTime[6];
	G4double DCVelocity[6];
	G4double DCKineticEnergy[6];
	G4double DCPos[6][3]; // [wire_layer - (x, y, z)]
	G4double DCMomentum[6][3];

	data_t(){
		initialize();
	}
	void initialize(){
		index = -1;

		A = -9999.;
		Z = -9999.;

		for(G4double &x : p0) x = -9999.;

		TstopNhit = -1;
		TstopTime = -9999.;
		TstopVelocity = -9999.;
		TstopKineticEnergy = -9999.;
		for(G4double &x : TstopPos) x = -9999.;
		for(G4double &x : TstopMomentum) x = -9999.;

		TOFWNhit = -1;
		TOFWStripId = -1;
		TOFWTime = -9999.;
		TOFWVelocity = -9999.;
		TOFWKineticEnergy = -9999.;
		for(G4double &x : TOFWPos) x = -9999.;
		for(G4double &x : TOFWMomentum) x = -9999.;

		PDCArrNhit = -1;
		for(auto &x : PDCTime) x = -9999.;
		for(auto &x : PDCVelocity) x = -9999.;
		for(auto &x : PDCKineticEnergy) x = -9999.;
		for(auto &x : PDCPos) for(auto &t : x) t = -9999.;
		for(auto &x : PDCMomentum) for(auto &t : x) t = -9999.;

		DCArrNhit = -1;
		for(auto &x : DCTime) x = -9999.;
		for(auto &x : DCVelocity) x = -9999.;
		for(auto &x : DCKineticEnergy) x = -9999.;
		for(auto &x : DCPos) for(auto &t : x) t = -9999.;
		for(auto &x : DCMomentum) for(auto &t : x) t = -9999.;
	}
	void setP0(double x, double y, double z){
		p0[0] = x; p0[1] = y; p0[2] = z;
	}
};

// class description
// run action class

class ETFRunAction : public G4UserRunAction{
public:
	ETFRunAction();
	virtual ~ETFRunAction();

	virtual void BeginOfRunAction(const G4Run *);
	virtual void   EndOfRunAction(const G4Run *);
	
	friend class ETFEventAction;
private:
	TFile *fFile;
	TTree *fTree;
	// data for each event, would be assigned by ETFEventAction per event
	data_t fData;
};

#endif
