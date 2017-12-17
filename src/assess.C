// assess.C -- assess the data analysis result
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAAssess.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->SetConfigExpDir("pion_2017Oct"); ep->Configure();
	TAAssess *ass = TAAssess::Instance();
	ass->SetROOTFile(argv[1]); ass->SetRunId(0);
	ass->EvalDCArr();

	return 0;
}

