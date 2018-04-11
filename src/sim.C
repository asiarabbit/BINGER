// sim.C -- Generate simulation data
// Created: 2017/12/1, Lasted modified: 2018/3/3, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASimulation.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 4){
		cout << "./sim <nTrkPerEv> <eff%> (eff: 0~100) <rootfilename>.\n";
		cout << "3 Parameters have to be given\n";
		exit(1);
	}
	int nTrkPerEv = atoi(argv[1]); double eff = atoi(argv[2])/100.;
	cout << "\033[33;1mnTrkPerEv: " << nTrkPerEv << "\teff: " << eff*100. << "%\033[0m\n";
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TASimulation *sim = new TASimulation();
	sim->SetFixDCArr(1); sim->SetIsDebug(0); // MWDC array L or R only
	sim->SetBeta(0.45, 0.6); // DCArrL, DCArrR
	sim->GenerateSim(1250, nTrkPerEv, eff, argv[3]);

	return 0;
}


