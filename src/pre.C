// pre.C -- pre analysis of 2017 Pion experiment.

#include "TAUI.h"
// data file has to be put in ../config/data
int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list
	usr->SetMagneticIntensity(0.24835); // unit: Telsa 0.24835 1.456
	usr->Verbose(false); // don't show TAPopMsg::ConfigInfo() printings (detector information)
//	usr->Silent(); // don't show TAPopMsg::Info() printings
//	usr->CoarseFit(); // rough yet fast fit // May Not Use
	usr->Run();
	return 0;
}

