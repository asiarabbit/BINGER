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
// Last modified: 2018/4/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TADeployPara.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TACtrlPara.h"
#include "TAGPar.h"

static const TAGPar *gp = TAGPar::Instance();
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
	if(3 == type[0] || 4 == type[0]){ // MWDC Array L-R
		if(type[1] > 2) TAPopMsg::Error("TADeployPara", "GetNDCCable: Not an MWDC");
		return fNDCCable[type[0]-3][type[1]];
	}
	// there is only one cable for each SLayer in DCs around the target
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0]){
		if(type[1] > 1) TAPopMsg::Error("TADeployPara", "GetNDCCable: Not an MWDC");
		return 1;
	}
	TAPopMsg::Error("TADeployPara", "GetNDCCable: Not an MWDC array");
	return -1;
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
		-0.6605, -1.0634, -0.9669, -0.8210, -0.2714, -0.4465, // 0-5
		-0.3346, -0.0428, -0.5486, -1.0641, -0.9815, -0.7967, // 6-11
		-0.1450, -0.2909, -0.2277,  0.0690, -0.5000, -0.6410, // 12-17
		-0.6410, -0.5833,  0.0859, -0.0082,  0.1158,  0.3230, // 18-23
		-0.5050, -0.3984, -0.6136, -0.5733,  0.0893, -0.0349  // 24-29
	}; // mean: -0.424532ns, standard deviation: 0.410898ns.

	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0])
		TAPopMsg::Error("TADeployPara", "GetTOFWallStripDelay: Not an L-R MWDC array");
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

	const double ccDelayAvrg_TOFWall[2] = {gp->Val(89), gp->Val(93)};
	return ccDelayAvrg_TOFWall[type[0] - 3]; // [0-1]: DCArr[L-R]
}
double TADeployPara::GetMWDCDelay(unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid);
	if(3 != type[0] && 4 != type[0] && 6 != type[0] && 7 != type[0] && 8 != type[0] && 9 != type[0])
		TAPopMsg::Error("TADeployPara", "GetMWDCDelay: Not an MWDC array");
	if((3 == type[0] || 4 == type[0]) && type[1] >= 3)
		TAPopMsg::Error("TADeployPara", "GetMWDCDelay: Not an MWDC");
	if((6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0]) && type[1] >= 2)
		TAPopMsg::Error("TADeployPara", "GetMWDCDelay: Not an MWDC");

	const double offset0[6][3] = {
		{gp->Val(86), gp->Val(87), gp->Val(88)}, // MWDC Array L
		{gp->Val(90), gp->Val(91), gp->Val(92)}, // MWDC Array R
		{gp->Val(94), gp->Val(95), 0.		  }, // MWDC Array U
		{gp->Val(96), gp->Val(97), 0.		  }, // MWDC Array D
		{gp->Val(98), gp->Val(99), 0.		  }, //  PDC Array U
		{gp->Val(100), gp->Val(101), 0.		  }  //  PDC Array D
	}; // [3-4-6-7-8-9]

	int dcArrId = -9999;
	if(3 == type[0] || 4 == type[0]) dcArrId = type[0] - 3; // 0-1: L-R
	if(6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0])
		dcArrId = type[0] - 6 + 2; // 2-3-4-5: U-D-PDCU-D
	double delay = offset0[dcArrId][type[1]];
//	delay += -TACtrlPara::Instance()->T_wireMean(uid);
	return delay;
}

double TADeployPara::GetTargetZ0() const{
	static const double z = gp->Val(84);
	return z;
}

