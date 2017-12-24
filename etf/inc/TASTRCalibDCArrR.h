///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrR.h												 //
//   TASTRCalibDCArrR.h -- header file for class TASTRCalibDCArrR					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArr, specifically for the right DC array arm.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/12/23, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TASTRCALIBDCARRR_H_
#define _TASTRCALIBDCARRR_H_

#include "TASTRCalibDCArr.h"

class TAMWDCArrayR;

class TASTRCalibDCArrR : public TASTRCalibDCArr{
public:
	TASTRCalibDCArrR(const string &rootfile = "", TAMWDCArrayR *dcArrR = nullptr);
	virtual ~TASTRCalibDCArrR();
protected:
};

#endif
