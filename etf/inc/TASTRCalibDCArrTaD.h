///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrTaD.h												 //
//   TASTRCalibDCArrTaD.h -- header file for class TASTRCalibDCArrTaD				 //
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

#ifndef _TASTRCALIBDCARRTAD_H_
#define _TASTRCALIBDCARRTAD_H_

#include "TASTRCalibDCArrTa.h"

class TAMWDCArrayD;

class TASTRCalibDCArrTaD : public TASTRCalibDCArrTa{
public:
	TASTRCalibDCArrTaD(const string &rootfile = "", TAMWDCArrayD *dcArrTaD = nullptr);
	virtual ~TASTRCalibDCArrTaD();
protected:
};

#endif
