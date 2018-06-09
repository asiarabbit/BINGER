///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrL.C												 //
//   TASTRCalibDCArrL.C -- source file for class TASTRCalibDCArrL					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArr, specifically for the left DC array arm.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TASTRCalibDCArrL.h"
#include "TAMWDCArrayL.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TASTRCalibDCArrL::TASTRCalibDCArrL(const string &rootfile, TAMWDCArrayL *dcArrL)
		: TASTRCalibDCArr(rootfile, dcArrL){
	// very the identity of the dcArray
	if(dcArrL && dcArrL->GetDetId() != 6)
		TAPopMsg::Error("TASTRCalibDCArrL", "TASTRCalibDCArrL: input DC array is not dcArrL, detId: %d", dcArrL->GetDetId());

	if(!fDCArr){
		fDCArr = (TAMWDCArray*)TAParaManager::Instance()->GetDetList()[3];
		if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArrL", "TASTRCaliDCArrL: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TASTRCalibDCArrL::~TASTRCalibDCArrL(){}

