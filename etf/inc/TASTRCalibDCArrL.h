///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrL.h												 //
//   TASTRCalibDCArrL.h -- header file for class TASTRCalibDCArrL					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArr, specifically for the left DC array arm.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/12/23, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TASTRCALIBDCARRL_H_
#define _TASTRCALIBDCARRL_H_

#include "TASTRCalibDCArr.h"

class TAMWDCArrayL;

class TASTRCalibDCArrL : public TASTRCalibDCArr{
public:
	TASTRCalibDCArrL(const string &rootfile = "", TAMWDCArrayL *dcArrL = nullptr);
	virtual ~TASTRCalibDCArrL();
protected:
};

#endif
