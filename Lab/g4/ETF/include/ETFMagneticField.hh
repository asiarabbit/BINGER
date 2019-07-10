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
	
	void SetIsUniformMagneticField(G4bool opt){
		fIsUniformMagneticField = opt;
		if(!fIsUniformMagneticField)
			LoadMagneticFieldFile("magnetic.txt");
	}
	G4bool IsUniformMagneticField(){ return fIsUniformMagneticField; }

protected:
	void DefineCommands();
	// read the magnetic field measurement file, generated from measurements using Hall sensor.
	void LoadMagneticFieldFile(const G4String &file);
	// assign B with the magnetic intensity vector at position p
	// return false if particle hit the iron wall of the dipo
	G4bool GetMagneticIntensity(const G4double p[4], G4double *B) const;

	G4GenericMessenger *fMessenger;
	G4bool fIsUniformMagneticField; // whether use uniform or real magnetic field

	// the magnetic field
	// this is the one eighth of the whole magnetic field for the dipole magnet.
	// x: -1000, 0; y: 0, 90; z: 0, 1050         10mm spacing
	G4double fBy; // the B for uniform or Bmax for real mag
	G4double fB[3][101][10][106]; // [BxByBz][x][y][z]
	G4bool fIsFileLoaded; // to mark if the magnetic field file is loaded
};

#endif
