// pre.C -- pre analysis of 2017 Pion experiment.

#include "TAUI.h"
// data file has to be put in ../config/data
int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list
	// beamTest_2016Nov, pion_2017Oct
	usr->SetConfigExpDir("beamTest_2016Nov"); // path of config files - chId, layout, etc.
	usr->SetMagneticIntensity(0.24835); // unit: Telsa 0.24835 1.456
//	usr->Silent(); // don't show TAPopMsg::Info() printings
	usr->Run();
	return 0;
}

