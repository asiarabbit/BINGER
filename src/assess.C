// assess.C -- assess the data analysis result
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAAssess.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 4){
		cout << "./ass <rootfile> <isDCArrR> <round_id>. 3 Parameters have to be given\n";
		exit(1);
	}
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TAAssess *ass = TAAssess::Instance();
	ass->SetROOTFile(argv[1]);
	const int round = atoi(argv[3]);
//	ass->EvalDCArr(round, bool(atoi(argv[2])));
	ass->PostEval(round); // analyze hrt_04_sample for evaluation of recursive STR correction

	return 0;
}

