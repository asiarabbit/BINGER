/**
	\file pre.C
	\example pre.C
	\brief Input a PXI and a VME binary raw data file to be decoded and analyzed for Daq evaluation,
	 detector debugging, or PID output.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/12/1 Last revised: 2018/8/18, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#include <iostream>
#include <cstdlib>
#include "TAUI.h"

using std::cout;
using std::endl;

// pion: 0.24835; C16: 1.45; C15: 1.295; C14: 1.20; C12: 0.9535
//                  0-C16     1-C15      2-C14      3-C13,     4-C12      5-O18      6-O18-NOTA
const double B[] = {1.45,     1.295,     1.2006,    1.075,     0.9535,    1.2004,    1.2336}; // unit: Telsa

int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list

	//user options
	usr->SetMagneticIntensity(B[4]);
//	usr->BunchIdMisAlignCheck();
//	usr->Silent(); // don't show TAPopMsg::Info() printings
//	usr->CheckChannelId(403); // see channel with channelId
//	usr->CoarseFit();

	usr->Go(); // pattern recognition, track fit, and particle identification

	return 0;
} // end of the main function



