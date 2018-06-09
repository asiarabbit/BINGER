///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibPDCArrU.C												 //
//   TASTRCalibPDCArrU.C -- source file for class TASTRCalibPDCArrU					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibPDCArr, specifically for the DCTas upstream of the	 //
// target. Note that this is for DCs around the target made in Japan.				 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/8.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TASTRCalibPDCArrU.h"
#include "TAPDCArrayU.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TASTRCalibPDCArrU::TASTRCalibPDCArrU(const string &rootfile, TAPDCArrayU *pdcArrU)
		: TASTRCalibDCArrTa(rootfile, pdcArrU){
	if(pdcArrU && pdcArrU->GetDetId() != 8)
		TAPopMsg::Error("TASTRCalibPDCArrU", "TASTRCalibPDCArrU: input DC array is not pdcArrU, detId: %d", pdcArrU->GetDetId());
	if(!fDCArr){
		fDCArr = (TAMWDCArray2*)TAParaManager::Instance()->GetDetList()[8];
		if(!fDCArr) TAPopMsg::Error("TASTRCalibPDCArrU", "TASTRCalibPDCArrU: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TASTRCalibPDCArrU::~TASTRCalibPDCArrU(){}
