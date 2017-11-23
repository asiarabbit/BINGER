// pre.C -- pre analysis of 2017 Pion experiment.
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"

using std::cout;
using std::endl;
//										1          2   3    4
int main(int argc, char *argv[]){ // datafilename id0 id1 runId
	if(argc < 5){
		cout << "Incomplete input detected.\n";
		cout << "[datafilename] [index0] [index1] [runId] Exiting...\n";
		return 0;
	}
	int arg[3]{}; // id0 id1 No#
	for(int i = 2; i <= 4; i++){
		arg[i-2] = atoi(argv[i]);
		if(arg[i-2] < 0){
			cout << "Invalid input\n";
			cout << "[datafilename] [index0] [index1] [runId] [isPID] Exiting...\n";
			return 0;
		}
	}
	bool isPID = true;
	if(argc >= 5) isPID = bool(atoi(argv[5]));
	// data file has to be put in ../config/data
	TAEventProcessor *ep = TAEventProcessor::Instance(argv[1], arg[2]);
	ep->Verbose(false); // don't show TAPopMsg::ConfigInfo() printings (detector information)
//	ep->Silent(); // don't show TAPopMsg::Info() printings
//	ep->CoarseFit(); // rough yet fast fit // May Not Use

	ep->SetConfigExpDir("pion_2017Oct");
	ep->SetSTRROOTFile("STR.root");
	ep->SetMagneticIntensity(0.24835); // unit: Telsa 0.24835 1.456
	ep->SetIsPID(isPID);
	// void Run(int id0 = 0, int id1 = INT_MAX, int secLenLim = INT_MAX);
	ep->Run(arg[0], arg[1], 2500000);
	return 0;
}

