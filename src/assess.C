// assess.C -- assess the data analysis result
// Created: 2017/12/1, Lasted modified: 2018/5/24, Author: SUN Yazhou

#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAAssess.h"
#include "TAAssessTa.h"
#include "TAAssessPDC.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 4){
		cout << "./ass <rootfile> <isDCArrR> <round_id>. 3 Parameters have to be given\n";
		exit(1);
	}
	const int round = atoi(argv[3]);
	bool isDCArrR = bool(atoi(argv[2]));

	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TAAssess *ass = nullptr; TAAssessTa *assTa = nullptr; TAAssessPDC *assPDC = nullptr;
//	ass = TAAssess::Instance();
//	assTa = TAAssessTa::Instance();
	assPDC = TAAssessPDC::Instance();

	
	if(ass){
		ass->SetROOTFile(argv[1]);
		ass->EvalDCArr(round, isDCArrR);
		ass->PostEval(round, isDCArrR); // analyze hrt_04_sample for STRcor assess
	}
	if(assTa){
		assTa->SetROOTFile(argv[1]);
		assTa->EvalDCArr(round, isDCArrR);
		assTa->PostEval(round, isDCArrR); // analyze hrt_04_sample for STRcor assess
	}
	if(assPDC){
		assPDC->SetROOTFile(argv[1]);
		assPDC->EvalDCArr(round, isDCArrR);
		assPDC->PostEval(round, isDCArrR); // analyze hrt_04_sample for STRcor assess
	}

	return 0;
}

