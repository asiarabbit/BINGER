// t0cali.C -- fit the T0 constants
// Created: 2017/12/1, Lasted modified: 2018/3/3, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAT0CalibDCArrTaU.h"
#include "TAT0CalibDCArrTaD.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "./t0 <rootfile> <DCArrOption>.\nrootfile name and dcArrOption (0: U, 1: D) have to be provided\n";
		exit(1);
	}
	bool isDCArrD = bool(atoi(argv[2]));
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TAT0CalibDCArrTa *t0 = nullptr;
	if(isDCArrD) t0 = new TAT0CalibDCArrTaD(argv[1]);
	else t0 = new TAT0CalibDCArrTaU(argv[1]);
	// virtual void Refine_DTHisto(bool isCalib = true);
	// isCalib: whether to store hdt histos and generate calibration file
	t0->Refine_DTHisto();
	// virtual void GenerateCalibFile(bool isShowFit = false);
	t0->GenerateCalibFile(true);
	// adopt the calibration
	char cmd[128];
	sprintf(cmd, "mv T0CalibrationTa/*.002 %s/T0/", ep->GetCtrlPara()->ConfigExpDir()); system(cmd);

	delete t0; t0 = nullptr;	
	return 0;
}

