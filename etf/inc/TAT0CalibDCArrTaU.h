///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrTaU.h												 //
//   TAT0CalibDCArrTaU.h -- header file for class TAT0CalibDCArrTaU					 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArrTa, specifically//
// for the DC array upstream of the target.											 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/3.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAT0CALIBDCARRTAU_H_
#define _TAT0CALIBDCARRTAU_H_

#include "TAT0CalibDCArrTa.h"

class TAMWDCArrayU ;

class TAT0CalibDCArrTaU : public TAT0CalibDCArrTa{
public:
	TAT0CalibDCArrTaU(const string &rootfile = "", TAMWDCArrayU *dcArrU = nullptr);
	virtual ~TAT0CalibDCArrTaU();
protected:
};

#endif

