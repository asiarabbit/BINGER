// t0cali.C -- fit the T0 constants
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAT0CalibDCArrL.h"
#include "TAT0CalibDCArrR.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "./str <rootfile> <DCArrOption>.\nrootfile name and dcArrOption (0: L, 1: R) have to be provided\n";
		exit(1);
	}
	bool isDCArrR = bool(atoi(argv[2]));
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TAT0CalibDCArr *t0 = nullptr;
	if(isDCArrR) t0 = new TAT0CalibDCArrR(argv[1]);
	else t0 = new TAT0CalibDCArrL(argv[1]);
	// virtual void Refine_DTHisto(bool isCalib = true);
	// isCalib: whether to store hdt histos and generate calibration file
	t0->Refine_DTHisto(true);
	// virtual void GenerateCalibFile(bool isShowFit = false);
	t0->GenerateCalibFile(false);
	// adopt the calibration
	char cmd[128];
//	sprintf(cmd, "mv T0Calibration/*.002 %s/T0/", ep->GetCtrlPara()->ConfigExpDir()); system(cmd);
}

