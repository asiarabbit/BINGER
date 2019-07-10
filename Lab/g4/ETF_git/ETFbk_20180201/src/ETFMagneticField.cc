// ETFMagnetciField.cc -- implementation of the class
// created: Jan. 25, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 25, 2019, by Aisa SUN

#include "ETFMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"

ETFMagneticField::ETFMagneticField()
 : G4MagneticField(), fMessenger(nullptr), fBy(-1.0*tesla){
	// define commands for this clas
	DefineCommands();
}

ETFMagneticField::~ETFMagneticField(){
	delete fMessenger;
}

// position and time
void ETFMagneticField::GetFieldValue(const G4double[4], double *bField) const{
	bField[0] = 0.;
	bField[1] = fBy;
	bField[2] = 0.;
}

void ETFMagneticField::DefineCommands(){
	// define /ETF/field command directory using generic messenger class
	fMessenger = new G4GenericMessenger(this, "/ETF/field/", "Field control");
	
	// field value command. Type: embedded class of G4
	auto& valueCmd = fMessenger->DeclareMethodWithUnit("value", "tesla", &ETFMagneticField::SetField, "set field strength");
	valueCmd.SetParameterName("field", true);
	valueCmd.SetDefaultValue("-1.");
}
