///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrTaD.C												 //
//   TAT0CalibDCArrTaD.C -- source file for class TAT0CalibDCArrTaD					 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArrTa, specifically//
// for the DC array downstream of the target and upstream of the magnet.			 //
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

#include "TAT0CalibDCArrTaD.h"
#include "TAMWDCArrayD.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TAT0CalibDCArrTaD::TAT0CalibDCArrTaD(const string &rootfile, TAMWDCArrayD *dcArrD)
		: TAT0CalibDCArrTa(rootfile, dcArrD){
	if(!fDCArr){
		fDCArr = (TAMWDCArray2*)TAParaManager::Instance()->GetDetList()[7];
		if(!fDCArr) TAPopMsg::Error("TAT0CalibDCArrTaD", "TAT0CaliDCArrTaD: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TAT0CalibDCArrTaD::~TAT0CalibDCArrTaD(){}

