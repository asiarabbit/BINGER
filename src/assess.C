// assess.C -- assess the data analysis result
// Created: 2017/12/1, Lasted modified: 2018/3/3, Author: SUN Yazhou
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
	ep->Configure();
	TAAssess *ass = TAAssess::Instance();
	ass->SetROOTFile(argv[1]);
	const int round = atoi(argv[3]);
	bool isDCArrR = bool(atoi(argv[2]));
	ass->EvalDCArr(round, isDCArrR);
	ass->PostEval(round, isDCArrR); // analyze hrt_04_sample for STRcor assess

	return 0;
}

