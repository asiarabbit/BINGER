// assess.C -- assess the data analysis result
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAAssess.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc <= 1){
		cout << "ROOT file name not provided. Exit...\n";
		exit(1);
	}
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TAAssess *ass = TAAssess::Instance();
	ass->SetROOTFile(argv[1]); ass->SetRunId(0);
	ass->EvalDCArr(true, 2);

	return 0;
}

