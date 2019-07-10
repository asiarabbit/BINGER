// assess.C -- assess the data analysis result
// Created: 2017/12/1, Lasted modified: 2018/5/24, Author: SUN Yazhou

#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAGPar.h"
#include "TAAssess.h"
#include "TAAssessTa.h"
#include "TAAssessPDC.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "./ass <rootfile> <DCArrOption> (0-1-2-3-4-5: L-R--PDCU-D--TaU-D) [<round_id>]" << endl;
		exit(1);
	}
	const char *rootfile = argv[1];
	int dcArrOption = atoi(argv[2]);
	if(dcArrOption < 0 || dcArrOption > 5){
		cout << "./ass <rootfile> <DCArrOption> [<round_id>]. Abnormal value for DCArrOption\n";
		exit(1);
	}
	int round = 0;
	if(argc > 3) round = atoi(argv[3]);

	TAEventProcessor::Instance()->Configure();
	bool usingPDC = TAGPar::Instance()->Val(83);
	if(!usingPDC){
		if(2 == dcArrOption) dcArrOption = 4;
		if(3 == dcArrOption) dcArrOption = 5;
		if(4 == dcArrOption) dcArrOption = 2;
		if(5 == dcArrOption) dcArrOption = 3;
	}

	TAAssess *ass; TAAssessTa *assTa; TAAssessPDC *assPDC;
	switch(dcArrOption){
		case 0: case 1:
			ass = TAAssess::Instance();
			ass->SetROOTFile(rootfile);
			ass->EvalDCArr(round, dcArrOption%2);
			ass->PostEval(round, dcArrOption%2); // analyze hrt_04_sample for STRcor assess
			break;
		case 2: case 3:
			assTa = TAAssessTa::Instance();
			assTa->SetROOTFile(rootfile);
			assTa->EvalDCArr(round, dcArrOption%2);
			assTa->PostEval(round, dcArrOption%2);
			break;
		case 4: case 5:
			assPDC = TAAssessPDC::Instance();
			assPDC->SetROOTFile(rootfile);
			assPDC->EvalDCArr(round, dcArrOption%2);
			assPDC->PostEval(round, dcArrOption%2);
			break;
		default: break;	
	}

	return 0;
}

