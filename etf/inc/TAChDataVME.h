///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChDataVME.h														 //
//   TAChDataVME.h -- header file for class TAChDataVME								 //
//   Introduction: Derived from TAChData. This class is mainly for transformation of //
// measured data, by polymorphic implementation of GetLead(Trail)ingTime(...).		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/21.															     //
// Last modified: 2018/4/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TACHDATAVME_H_
#define _TACHDATAVME_H_

#include "TAChData.h"

class TAChDataVME : public TAChData{
public:
	TAChDataVME(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAChDataVME();

	virtual double GetLeadingTime(int n = 0) const; // get the n-th pulse
	virtual double GetTrailingTime(int n = 0) const; // get the n-th pulse
protected:
};

#endif
