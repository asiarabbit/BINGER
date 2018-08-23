///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayL.h													 //
//   TAMWDCArrayL.h -- header file for class TAMWDCArrayL							 //
//   Introduction: the left arm of the MWDC arrays. A derived class from TAMWDCArray //
// with distinct features of the left arm.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/6.															     //
// Last modified: 2018/3/18, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCARRAYL_H_
#define _TAMWDCARRAYL_H_

#include "TAMWDCArray.h"

class TAMWDCArrayL : public TAMWDCArray{
public:
	TAMWDCArrayL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayL();

	virtual void Configure() override; // create detectors of the MWDC array
protected:
};

#endif

