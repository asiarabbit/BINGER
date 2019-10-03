///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrTaU.C												 //
//   TAT0CalibDCArrTaU.C -- source file for class TAT0CalibDCArrTaU					 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArrTa, specifically//
// for the DC array upstream of the target.											 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/3.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAT0CalibDCArrTaU.h"
#include "TAMWDCArrayU.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TAT0CalibDCArrTaU::TAT0CalibDCArrTaU(const string &rootfile, TAMWDCArrayU *dcArrU)
		: TAT0CalibDCArrTa(rootfile, dcArrU){
	if(!fDCArr){
		fDCArr = (TAMWDCArray2*)TAParaManager::Instance()->GetDetList()[8];
		if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArrTaU", "TAT0CaliDCArrTaU: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TAT0CalibDCArrTaU::~TAT0CalibDCArrTaU(){}

