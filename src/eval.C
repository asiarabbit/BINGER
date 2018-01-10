// assess.C -- assess the data analysis result
// Created: 2017/12/1, Lasted modified: 2018/1/1, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASimulation.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc <= 1){
		cout << "./eval <rootfile>; rootfile name have to be given\n";
		exit(1);
	}
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TASimulation::Evaluate(argv[1]);
	return 0;
}

