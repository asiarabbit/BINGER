///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrL.h													 //
//   TAT0CalibDCArrL.h -- header file for class TAT0CalibDCArrL						 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArr, specifically	 //
// for the left DC array arm.														 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/11/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAT0CALIBDCARRL_H_
#define _TAT0CALIBDCARRL_H_

#include "TAT0CalibDCArr.h"

class TAMWDCArrayL;

class TAT0CalibDCArrL : public TAT0CalibDCArr{
public:
	TAT0CalibDCArrL(const string &rootfile = "", TAMWDCArrayL *dcArrL = nullptr);
	virtual ~TAT0CalibDCArrL();
protected:
};

#endif
