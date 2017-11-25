// sim.C -- Generate simulation data.
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASimulation.h"
#include "TAPopMsg.h"

using std::cout;
using std::endl;
int main(int argc, char *argv[]){
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->SetConfigExpDir("pion_2017Oct"); ep->SetSTRROOTFile("STR.root");
	ep->Configure();
	TASimulation *sim = new TASimulation();
	sim->GenerateSim(10000, 1, 1.);

	return 0;
}

