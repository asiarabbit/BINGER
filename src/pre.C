// pre.C -- pre analysis of 2017 Pion experiment.
// Created: 2017/12/1, Lasted modified: 2018/1/1, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAUI.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	usr->SetConfigExpDir(dir[0]); // path of config files - chId, layout, etc.
	usr->SetMagneticIntensity(0.24835); // unit: Telsa 0.24835 1.456
//	usr->Silent(); // don't show TAPopMsg::Info() printings
	usr->Go(); // pattern recognition, track fit, and particle identification

	return 0;
} // end of the main function



