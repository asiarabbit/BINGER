///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayU.h													 //
//   TAMWDCArrayU.h -- header file for class TAMWDCArrayU							 //
//   Introduction: the MWDC array upstream of the target. A derived class from		 //
// TAMWDCArray2. Note that there's only two DCs in the DC array.					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/22.															     //
// Last modified: 2018/3/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCARRAYU_H_
#define _TAMWDCARRAYU_H_

#include "TAMWDCArray2.h"

class TAMWDCArrayU : public TAMWDCArray2{
public:
	TAMWDCArrayU(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayU();

	virtual void Configure() override; // create detectors of the MWDC array
protected:
};

#endif

