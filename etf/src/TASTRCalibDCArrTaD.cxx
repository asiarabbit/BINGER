///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrTaD.C												 //
//   TASTRCalibDCArrTaD.C -- source file for class TASTRCalibDCArrTaD				 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArrTa, specifically for the DCTas downstream of the	 //
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

#include "TASTRCalibDCArrTaD.h"
#include "TAMWDCArrayD.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TASTRCalibDCArrTaD::TASTRCalibDCArrTaD(const string &rootfile, TAMWDCArrayD *dcArrTaD)
		: TASTRCalibDCArrTa(rootfile, dcArrTaD){
	if(dcArrTaD && dcArrTaD->GetDetId() != 7)
		TAPopMsg::Error("TASTRCalibDCArrTaD", "TASTRCalibDCArrTaD: input DC array is not dcArrTaD, detId: %d", dcArrTaD->GetDetId());
	if(!fDCArr){
		fDCArr = (TAMWDCArray2*)TAParaManager::Instance()->GetDetList()[7];
		if(!fDCArr) TAPopMsg::Error("TASTRCalibDCArrTaD", "TASTRCalibDCArrTaD: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TASTRCalibDCArrTaD::~TASTRCalibDCArrTaD(){}

