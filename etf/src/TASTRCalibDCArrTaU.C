///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrTaU.C												 //
//   TASTRCalibDCArrTaU.C -- source file for class TASTRCalibDCArrTaU				 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArrTa, specifically for the DCTas upstream of the	 //
// target. Note that this is for DCs around the target made by P.Ma.				 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/8.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TASTRCalibDCArrTaU.h"
#include "TAMWDCArrayU.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TASTRCalibDCArrTaU::TASTRCalibDCArrTaU(const string &rootfile, TAMWDCArrayU *dcArrTaU)
		: TASTRCalibDCArrTa(rootfile, dcArrTaU){
	if(dcArrTaU && dcArrTaU->GetDetId() != 6)
		TAPopMsg::Error("TASTRCalibDCArrTaU", "TASTRCalibDCArrTaU: input DC array is not dcArrTaU, detId: %d", dcArrTaU->GetDetId());
	if(!fDCArr){
		fDCArr = (TAMWDCArray2*)TAParaManager::Instance()->GetDetList()[6];
		if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArrTaU", "TASTRCalibDCArrTaU: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TASTRCalibDCArrTaU::~TASTRCalibDCArrTaU(){}

