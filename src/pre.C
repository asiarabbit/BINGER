// pre.C -- pre analysis of 2017 Pion experiment.
// Created: 2017/12/1, Lasted modified: 2018/8/18, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAUI.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list

	//user options
	// 0.24835 1.456 1.0755 1.27 1.2001 1.449825
	usr->SetMagneticIntensity(0.9535); // unit: Telsa // 1.234 // 0.9535
//	usr->BunchIdMisAlignCheck();
//	usr->Silent(); // don't show TAPopMsg::Info() printings
//	usr->CheckChannelId(403); // see channel with channelId

	usr->Go(); // pattern recognition, track fit, and particle identification

	return 0;
} // end of the main function



