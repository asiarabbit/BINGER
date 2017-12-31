// pre.C -- pre analysis of 2017 Pion experiment.
#include <iostream>
#include <cstdlib>

#include "TAUI.h"
#include "TAAssess.h"
#include "TAT0CalibDCArrL.h"
#include "TAT0CalibDCArrR.h"
#include "TASTRCalibDCArrL.h"
#include "TASTRCalibDCArrR.h"

using std::cout;
using std::endl;

// wheher to implement STR and T0 calibraiton
// if calibration has been sufficiently done, and stored in calib-cofig-files,
// this can be switched off
bool hasCalibrated = false;
int main(int argc, char *argv[]){
	TAUI *usr = TAUI::Instance();
	usr->GetOpt(argc, argv); // parse the user input parameter list
	const char dir[2][64] = {"pion_2017Oct", "beamTest_2016Nov"};
	usr->SetConfigExpDir(dir[1]); // path of config files - chId, layout, etc.
	usr->SetMagneticIntensity(0.24835); // unit: Telsa 0.24835 1.456
//	usr->Silent(); // don't show TAPopMsg::Info() printings
	usr->Configure();
	usr->Go(); // pattern recognition, track fit, and particle identification

//	return 0;
	// 3D tracking has to be implemented for calibration procedures to work
	if(usr->Is3DTracking() && hasCalibrated) return 0;



	///////////////////// Calibration iteration //////////////////////
	const char *rf = usr->GetROOTFile();
	// Assess the tracking result
	TAAssess *ass = TAAssess::Instance(); ass->SetROOTFile(rf);
	// sense wire T0 calibration
	TAT0CalibDCArr *t0[2] = {new TAT0CalibDCArrL(rf), new TAT0CalibDCArrR(rf)};
	// STR self-calibration
	TASTRCalibDCArr *str[2] = {new TASTRCalibDCArrL(rf), new TASTRCalibDCArrR(rf)};
	char cmd[128];
	// T0 Calibration implementation //
	for(int i = 0; i < 2; i++){ // loop over the two MWDC arrays; 0: L; 1: R
		if(0 == i) continue; // skip MWDC Array L (close to CSRe)
		// if T_tof and T_wire has been corrected for in pattern recognition stage
		t0[i]->SetHasCorrected(true);
		// virtual void Refine_DTHisto(bool isCalib = true);
		// isCalib: whether to store hdt histos and generate calibration file
		t0[i]->Refine_DTHisto(true);
		// virtual void GenerateCalibFile(bool isShowFit = false);
		t0[i]->GenerateCalibFile(false);
		sprintf(cmd, "cp T0Calibration/*.002 %s/T0/", usr->GetCtrlPara()->ConfigExpDir());
		system(cmd);
//		getchar(); // DEBUG
	} // end loop over MWDC arrays
	// STR Calibration implementation //
	for(int i = 0; i < 4; i++){ // STR iteration
		for(int j = 0; j < 2; j++){ // loop over the two MWDC arrays; 0: L; 1: R
			if(0 == j) continue; // skip MWDC Array L (close to CSRe)
			ass->SetRunId(i);
			ass->EvalDCArr(j); // the implementation of the assess

			// BigSta -> true, false: mark if the statistics is enough, then fill behavior would vary
			str[j]->SetIsBigStatistics(true); // see header file of TASTRCalibDCArr for details
			str[j]->ChiHistogramming(true); // true: using 3D cali; false: using trk-proj cali
			str[j]->GenerateSTRCorFile(i); // i: round number, i.e. the iteration id
			sprintf(cmd, "cp STRCorrection/*.003 %s/STR_cor/", usr->GetCtrlPara()->ConfigExpDir());
			system(cmd);
//			getchar(); // DEBUG
		} // end for over MWDC arrays
		usr->Go(); // reimplement the pattern recognition, track fit, and particle identification
//		getchar(); // DEBUG
	} // end for over STR iteration
	delete str[0]; delete str[1];
	delete t0[0]; delete t0[1];
	delete usr;

	return 0;
} // end of the main function





