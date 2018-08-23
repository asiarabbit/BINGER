// strcali_pdc.C -- Space-Time Relation Self-Calibration
// Created: 2018/6/8, Lasted modified: 2018/6/8, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASTRCalibPDCArrU.h"
#include "TASTRCalibPDCArrD.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 4){
		cout << "./str_pdc <rootfile> <DCArrOption> <roundId>. rootfile name, dcArrOption (0: U, 1: D) and roundId have to be provided\n";
		exit(1);
	}
	bool isDCArrD = bool(atoi(argv[2])); int round = atoi(argv[3]);
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TASTRCalibDCArrTa *str = nullptr;
	if(isDCArrD) str = new TASTRCalibPDCArrD(argv[1]);
	else str = new TASTRCalibPDCArrU(argv[1]);
	// mark if the statistics is enough, then fill behavior would vary
	str->SetIsBigStatistics(true);
	str->ChiHistogramming(false); // true: using 3D cali; false: using trk-proj cali
	str->GenerateSTRCorFile(round);
	// adopt the calibration
	char cmd[128];
	sprintf(cmd, "mv STRCorrection/*.003 %s/STR_cor/", ep->GetCtrlPara()->ConfigExpDir());
	system(cmd);
	
	delete str; str = nullptr;
	return 0;
}
