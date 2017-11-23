///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayR.h													 //
//   TAMWDCArrayR.h -- header file for class TAMWDCArrayR							 //
//   Introduction: the right arm of the MWDC array. A derived class from TAMWDCArray //
// with distinct features of the right arm.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/6.															     //
// Last modified: 2017/10/7, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCArrayR_H_
#define _TAMWDCArrayR_H_

#include "TAMWDCArray.h"

class TAMWDCArrayR : public TAMWDCArray{
public:
	TAMWDCArrayR(const string &name = "", const string &title = "", int uid = 999999999);
	virtual ~TAMWDCArrayR();

	virtual void Configure() override; // create detectors of the MWDC array
protected:
};

#endif

