///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/t3DPIDInfo.C																 //
//   t3DPIDInfo.C -- source file for struct t3DPIDInfo								 //
//   Introduction: struct for storing particle identification results using refined	 //
// three-dimensional straight track information. This struct is initially coined for //
// parameter passing of TAEventProcessor::RefinePID(), yet has been seperated alone	 //
// for code extension considerations in the future.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/11/23.															     //
// Last modified: 2018/9/5, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "t3DPIDInfo.h"

t3DPIDInfo::t3DPIDInfo(){
	initialize();
}

void t3DPIDInfo::initialize(){
	aoz = -9999.; aozdmin = -9999.;
	angTaOut[0] = -9999.; angTaOut[1] = -9999.;
	trkLenT = -9999.; poz = -9999.; brho = -9999;
}
