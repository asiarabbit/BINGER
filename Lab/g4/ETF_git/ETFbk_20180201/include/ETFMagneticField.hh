// ETFDetectorConstruction.cc -- the implementation of the class
// Created: Jan. 23, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 23, 2019 by Asia SUN

#ifndef ETFMagneticField_h
#define ETFMagneticField_h

#include "G4MagneticField.hh"

class G4GenericMessenger;

class ETFMagneticField : public G4MagneticField{
public:
	ETFMagneticField();
	virtual ~ETFMagneticField();

	virtual void GetFieldValue(const G4double point[4], double *bField) const;

	void SetField(G4double val){ fBy = val; }
	G4double GetField() const{ return fBy; }

private:
	void DefineCommands();

	G4GenericMessenger *fMessenger;
	G4double fBy;
};

#endif
