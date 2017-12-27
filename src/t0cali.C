// t0cali.C -- fit the T0 constants
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TAT0CalibDCArrL.h"
#include "TAT0CalibDCArrR.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 2){
		cout << "./t0 <rootfile>. rootfile name has to be provided\n";
		exit(1);
	}
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TAT0CalibDCArr *t0 = new TAT0CalibDCArrR(argv[1]);
	// if T_tof and T_wire has been corrected for in pattern recognition stage
	t0->SetHasCorrected(true);
	// virtual void Refine_DTHisto(bool isCalib = true);
	// isCalib: whether to store hdt histos and generate calibration file
	t0->Refine_DTHisto(true);
	// virtual void GenerateCalibFile(bool isShowFit = false);
	t0->GenerateCalibFile(false);
}

