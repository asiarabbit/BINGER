// strcali.C -- Space-Time Relation Self-Calibration
// Created: 2017/12/1, Lasted modified: 2018/3/3, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASTRCalibDCArrR.h"
#include "TASTRCalibDCArrL.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 4){
		cout << "./str <rootfile> <DCArrOption> <roundId>. rootfile name, dcArrOption (0: L, 1: R) and roundId have to be provided\n";
		exit(1);
	}
	bool isDCArrR = bool(atoi(argv[2])); int round = atoi(argv[3]);
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TASTRCalibDCArr *str = nullptr;
	if(isDCArrR) str = new TASTRCalibDCArrR(argv[1]);
	else str = new TASTRCalibDCArrL(argv[1]);
	str->SetIsBigStatistics(false); // mark if the statistics is enough, then fill behavior would vary
	str->ChiHistogramming(false); // true: using 3D cali; false: using trk-proj cali
	str->GenerateSTRCorFile(round);
	// adopt the calibration
	char cmd[128];
	sprintf(cmd, "mv STRCorrection/*.003 %s/STR_cor/", ep->GetCtrlPara()->ConfigExpDir());
	system(cmd);
	
	delete str; str = nullptr;
	return 0;
}
