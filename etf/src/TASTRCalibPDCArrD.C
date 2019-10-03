///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibPDCArrD.C												 //
//   TASTRCalibPDCArrD.C -- source file for class TASTRCalibPDCArrD					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibPDCArr, specifically for the DCTas downstream of the	 //
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

#include "TASTRCalibPDCArrD.h"
#include "TAPDCArrayD.h"
#include "TAParaManager.h"
#include "TAPopMsg.h"

TASTRCalibPDCArrD::TASTRCalibPDCArrD(const string &rootfile, TAPDCArrayD *pdcArrD)
		: TASTRCalibDCArrTa(rootfile, pdcArrD){
	if(pdcArrD && pdcArrD->GetDetId() != 9)
		TAPopMsg::Error("TASTRCalibPDCArrD", "TASTRCalibPDCArrD: input DC array is not pdcArrD, detId: %d", pdcArrD->GetDetId());
	if(!fDCArr){
		fDCArr = (TAMWDCArray2*)TAParaManager::Instance()->GetDetList()[9];
		if(!fDCArr) TAPopMsg::Error("TASTRCalibPDCArrD", "TASTRCalibPDCArrD: DC array from TAParaManager is null. TAEventProcessor::Configure() not run yet?");
	}
}
TASTRCalibPDCArrD::~TASTRCalibPDCArrD(){}
