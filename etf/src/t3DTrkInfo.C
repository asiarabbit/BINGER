///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/t3DTrkInfo.C																 //
//   t3DTrkInfo.C -- source file for struct t3DTrkInfo								 //
//   Introduction: struct for storing three-dimensional straight track information.	 //
// This struct is initially coined for parameter passing of TAEventProcessor::Refine-//
// Tracks() and TAEventProcessor::RefinePID(), yet has been seperated alone for	code //
// extension considerations in the future.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/11/23.															     //
// Last modified: 2018/1/14, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "t3DTrkInfo.h"

t3DTrkInfo::t3DTrkInfo(){
	initialize();
}

void t3DTrkInfo::initialize(){
	k1 = -9999.; b1 = -9999.;
	k2 = -9999.; b2 = -9999.;
	tof2 = -9999.; taHitX = -9999.;
	TOF_posY = -9999.; TOF_posY_refine = -9999.;
	for(double &x : chi) x = -9999.;
	chi2 = -9999.; Chi = -9999.; dcTOTAvrg = -9999.;
	isDCArrR = true; firedStripId = -1;
}


