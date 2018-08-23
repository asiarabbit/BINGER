///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrR.C												 //
//   TASTRCalibDCArrR.C -- source file for class TASTRCalibDCArrR					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArr, specifically for the right DC array arm.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TASTRCalibDCArrR.h"
#include "TAMWDCArrayR.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TASTRCalibDCArrR::TASTRCalibDCArrR(const string &rootfile, TAMWDCArrayR *dcArrR)
		: TASTRCalibDCArr(rootfile, dcArrR){
	// very the identity of the dcArray
	if(dcArrR && dcArrR->GetDetId() != 6)
		TAPopMsg::Error("TASTRCalibDCArrR", "TASTRCalibDCArrR: input DC array is not dcArrR, detId: %d", dcArrR->GetDetId());

	if(!fDCArr){
		fDCArr = (TAMWDCArray*)TAParaManager::Instance()->GetDetList()[4];
		if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArrR", "TASTRCaliDCArrR: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TASTRCalibDCArrR::~TASTRCalibDCArrR(){}

