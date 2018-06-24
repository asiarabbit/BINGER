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
		cout << "./ass <rootfile> <isDCArrR> <round_id> [<DCArrOption>] (0-1-2: LR-TaUD-PDCUD)." << endl;
		cout << "At least 3 Parameters have to be given\n";
		exit(1);
	}
	const int DCArrOption = atoi(argv[2]);
	if(0 != DCArrOption && 1 != DCArrOption){
		cout << "./ass <rootfile> <isDCArrR> <round_id>. isDCArrR is not 0 or 1\n";
		exit(1);
	}
	const int round = atoi(argv[3]);
	const bool isDCArrR = bool(DCArrOption);

	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TAAssess *ass = nullptr; TAAssessTa *assTa = nullptr; TAAssessPDC *assPDC = nullptr;
	short dcArrOption = 0;
	if(argc >= 5) dcArrOption = atoi(argv[4]);
	switch(dcArrOption){
		case 0:
			cout << "\n\033[1mdcArr Option: DCArrLR\033[0m" << endl;
			ass = TAAssess::Instance(); break;
		case 1:
			cout << "\n\033[1mdcArr Option: DCArrTa\033[0m" << endl;
			assTa = TAAssessTa::Instance(); break;
		case 2:
			cout << "\n\033[1mdcArr Option: DCArrPDC\033[0m" << endl;
			assPDC = TAAssessPDC::Instance(); break;
		default: ass = TAAssess::Instance(); break;
	} // end of switch

	
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

