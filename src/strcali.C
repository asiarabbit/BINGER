// strcali.C -- Space-Time Relation Self-Calibration
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASTRCalibDCArrR.h"
#include "TASTRCalibDCArrR.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 2){
		cout << "./str <rootfile>. rootfile name has to be provided\n";
		exit(1);
	}
	TAEventProcessor *ep = TAEventProcessor::Instance();
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	ep->SetConfigExpDir(dir[1]); ep->Configure();
	TASTRCalibDCArrR *str = new TASTRCalibDCArrR(argv[1]);
	str->SetIsBigStatistics(true); // mark if the statistics is enough, then fill behavior would vary
	str->ChiHistogramming(true); // true: using 3D cali; false: using trk-proj cali
	str->GenerateSTRCorFile(0);
}
