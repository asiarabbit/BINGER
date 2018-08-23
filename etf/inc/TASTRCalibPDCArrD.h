///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibPDCArrD.h												 //
//   TASTRCalibPDCArrD.h -- header file for class TASTRCalibPDCArrD					 //
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

#ifndef _TASTRCALIBPDCARRD_H_
#define _TASTRCALIBPDCARRD_H_

#include "TASTRCalibDCArrTa.h"

class TAPDCArrayD;

class TASTRCalibPDCArrD : public TASTRCalibDCArrTa{
public:
	TASTRCalibPDCArrD(const string &rootfile = "", TAPDCArrayD *pdcArrD = nullptr);
	virtual ~TASTRCalibPDCArrD();
protected:
};

#endif
