///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADeployPara.C												     //
//   TADeployPara.C -- source file for class TADeployPara						     //
//   Introduction: an information class for storing and conveying detector design	 //
// parameters, mainly used to pass specifications when creating detector objects.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/12.															     //
// Last modified: 2017/10/20, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TADeployPara.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TACtrlPara.h"

TADeployPara* TADeployPara::fInstance = nullptr;

TADeployPara::TADeployPara(){
	for(int i = 2; i--;) for(int j = 3; j--;) fNDCCable[i][j] = 5;
	fNDCCable[0][2] = 6;

	fNTOFWallStrip = 30;
}
TADeployPara::~TADeployPara(){}

TADeployPara* TADeployPara::Instance(){
	if(!fInstance) fInstance = new TADeployPara();
	return fInstance;
}

short TADeployPara::GetNDCCable(unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not an MWDC array");
	if(type[1] > 3) TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not an MWDC");
	return fNDCCable[type[0]-3][type[1]];
}
short TADeployPara::GetNTOFWallStrip(unsigned uid) const{
	return fNTOFWallStrip;
}

double TADeployPara::GetTOFWallStripWidth(unsigned uid) const{
	return 41.67; // mm
}
double TADeployPara::GetTOFWallStripLength(unsigned uid) const{
	return 1200.; // mm
}
double TADeployPara::GetTOFWallStripDelay(unsigned uid) const{
	static const double ccDelay_TOFWallR[30] = { // calibrated by 40Ar, no target. Nov.25 night, 2016.
		-0.858497, -1.16066,  -1.16797,   -0.970526, -0.389165,
		-0.487886, -0.429385, -0.136876,  -0.615859, -1.01806,
		-0.996121, -0.707268, -0.144188,  -0.261192, -0.155158,
		 0.100788, -0.506168, -0.656079,  -0.561014, -0.586608,
		 0.0898187, 0.0271849, 0.104444,   0.305544, -0.462292,
		-0.447666, -0.64511,   1E-10,         1E-10,     1E-10,
	}; // mean: -0.424532ns, standard deviation: 0.410898ns.

	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not an MWDC array");
	if(3 != type[1]) TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not a TOFWall");
	if(type[2] > 30)
		TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Strip Id out of range: %d", type[2]);
	if(3 == type[0]) return 1E-10; // MWDC array L // DO NOT SET TO 0, or ERROR would be triggered
	if(4 == type[0]) return ccDelay_TOFWallR[type[2]]; // MWDC array R
	return 1E-10; // DO NOT SET TO 0, or ERROR would be triggered
}
// speed at which light travels in TOF wall plastic scintillator strips
double TADeployPara::GetTOFWallStripVeff(unsigned uid) const{
	return 1200. / 7.8; // mm/ns
}
double TADeployPara::GetTOFWallDelayAvrg(unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TADeployPara", "GetTOFWallDelayAvrg: Not an MWDC array");

	static double ccDelayAvrg_TOFWall[2] = {2.33, -4.67};
	return ccDelayAvrg_TOFWall[type[0] - 3]; // [0-1]: DCArr[L-R]
}
double TADeployPara::GetMWDCDelay(unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not an MWDC array");
	if(type[1] >= 3)
		TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not an MWDC");

	double offset0[2] = {15.33, 10.33}; // delay between TOFWall and DCs
	double delay = GetTOFWallDelayAvrg(uid) + offset0[type[0] - 3];
	delay += -TACtrlPara::Instance()->T_wireMean(uid);
	return delay;
}











