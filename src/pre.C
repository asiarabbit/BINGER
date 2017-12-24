// pre.C -- pre analysis of 2017 Pion experiment.

#include "TAUI.h"
// data file has to be put in ../config/data
int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	usr->SetConfigExpDir(dir[0]); // path of config files - chId, layout, etc.
	usr->SetMagneticIntensity(0.24835); // unit: Telsa 0.24835 1.456
//	usr->Silent(); // don't show TAPopMsg::Info() printings
	usr->Go();
	return 0;
}

