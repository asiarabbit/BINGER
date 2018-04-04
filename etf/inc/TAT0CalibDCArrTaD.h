///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrTaD.h												 //
//   TAT0CalibDCArrTaD.h -- header file for class TAT0CalibDCArrTaD					 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArrTa, specifically//
// for the DC array downstream of the target and upstream of the magnet.			 //
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

#ifndef _TAT0CALIBDCARRTAD_H_
#define _TAT0CALIBDCARRTAD_H_

#include "TAT0CalibDCArrTa.h"

class TAMWDCArrayD;

class TAT0CalibDCArrTaD : public TAT0CalibDCArrTa{
public:
	TAT0CalibDCArrTaD(const string &rootfile = "", TAMWDCArrayD *dcArrD = nullptr);
	virtual ~TAT0CalibDCArrTaD();
protected:
};

#endif

