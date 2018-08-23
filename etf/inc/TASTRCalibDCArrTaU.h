///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrTaU.h												 //
//   TASTRCalibDCArrTaU.h -- header file for class TASTRCalibDCArrTaU				 //
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

#ifndef _TASTRCALIBDCARRTAU_H_
#define _TASTRCALIBDCARRTAU_H_

#include "TASTRCalibDCArrTa.h"

class TAMWDCArrayU;

class TASTRCalibDCArrTaU : public TASTRCalibDCArrTa{
public:
	TASTRCalibDCArrTaU(const string &rootfile = "", TAMWDCArrayU *dcArrTaU = nullptr);
	virtual ~TASTRCalibDCArrTaU();
protected:
};

#endif
