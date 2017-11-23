///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrR.h													 //
//   TAT0CalibDCArrR.h -- header file for class TAT0CalibDCArrR						 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArr, specifically	 //
// for the right DC array arm.														 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/10/18, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAT0CALIBDCARRR_H_
#define _TAT0CALIBDCARRR_H_

#include "TAT0CalibDCArr.h"

class TAMWDCArrayR;

class TAT0CalibDCArrR : public TAT0CalibDCArr{
public:
	TAT0CalibDCArrR(const string &rootfile = "", TAMWDCArrayR *dcArrR = nullptr);
	virtual ~TAT0CalibDCArrR();
protected:
};

#endif
