///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPDCArrayU.h														 //
//   TAPDCArrayU.h -- header file for class TAPDCArrayU								 //
//   Introduction: The PDC array upstream of the target. A derived class from		 //
// TAMWDCArray2. Note that there's only two DCs in the DC array.					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/20.															     //
// Last modified: 2018/4/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPDCARRAYU_H_
#define _TAPDCARRAYU_H_

#include "TAMWDCArray2.h"

class TAPDCArrayU : public TAMWDCArray2{
public:
	TAPDCArrayU(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPDCArrayU();

	virtual void Configure() override; // create detectors of the MWDC array
protected:
};

#endif

