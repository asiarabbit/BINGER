// sim.C -- Generate simulation data
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASimulation.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "./sim <nTrkPerEv> <eff%> (eff: 0~100). 2 Parameters have to be given\n";
		exit(1);
	}
	int nTrkPerEv = atoi(argv[1]); double eff = atoi(argv[2])/100.;
	cout << "\033[33;1mnTrkPerEv: " << nTrkPerEv << "\teff: " << eff*100. << "%\033[0m\n";
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TASimulation *sim = new TASimulation();
	sim->SetFixDCArr(1); sim->SetIsDebug(0); // MWDC array L or R only
	sim->GenerateSim(10000, nTrkPerEv, eff);

	return 0;
}


