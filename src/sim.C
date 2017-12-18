// sim.C -- Generate simulation data
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASimulation.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TASimulation *sim = new TASimulation();
//	sim->SetFixDCArr(1); sim->SetIsDebug(1); // MWDC array L or R only
	sim->GenerateSim(100000, 1, 1.);

	return 0;
}


