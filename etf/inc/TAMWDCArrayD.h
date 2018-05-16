///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayD.h													 //
//   TAMWDCArrayD.h -- header file for class TAMWDCArrayD							 //
//   Introduction: the MWDC array downstream of the target. A derived class from	 //
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

#ifndef _TAMWDCARRAYD_H_
#define _TAMWDCARRAYD_H_

#include "TAMWDCArray2.h"

class TAMWDCArrayD : public TAMWDCArray2{
public:
	TAMWDCArrayD(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayD();

	virtual void Configure() override; // create detectors of the MWDC array
protected:
};

#endif

