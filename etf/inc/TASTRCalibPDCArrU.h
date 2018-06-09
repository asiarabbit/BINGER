///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibPDCArrU.h												 //
//   TASTRCalibPDCArrU.h -- header file for class TASTRCalibPDCArrU					 //
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

#ifndef _TASTRCALIBPDCARRU_H_
#define _TASTRCALIBPDCARRU_H_

#include "TASTRCalibDCArrTa.h"

class TAPDCArrayU;

class TASTRCalibPDCArrU : public TASTRCalibDCArrTa{
public:
	TASTRCalibPDCArrU(const string &rootfile = "", TAPDCArrayU *pdcArrU = nullptr);
	virtual ~TASTRCalibPDCArrU();
protected:
};

#endif
