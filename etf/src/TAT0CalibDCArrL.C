///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrL.C													 //
//   TAT0CalibDCArrL.C -- source file for class TAT0CalibDCArrL						 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArr, specifically	 //
// for the left DC array arm.														 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/12/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAT0CalibDCArrL.h"
#include "TAMWDCArrayL.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TAT0CalibDCArrL::TAT0CalibDCArrL(const string &rootfile, TAMWDCArrayL *dcArrL)
		: TAT0CalibDCArr(rootfile, dcArrL){
	if(!fDCArr){
		fDCArr = (TAMWDCArray*)TAParaManager::Instance()->GetDetList()[3];
		if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArrL", "TAT0CaliDCArrL: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TAT0CalibDCArrL::~TAT0CalibDCArrL(){}

