///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAParameter.h														 //
//   TAParameter.h -- header file for class TAParameter								 //
//   Introduction: base class for unit parameter storage, derived from TAStuff.		 //
// Direct instantiation of this class is supposed to store physical constants.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/9/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPARAMETER_H_
#define _TAPARAMETER_H_

#include "TAStuff.h"

class TAParameter : public TAStuff{
public:
	TAParameter(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAParameter();
	double GetValue() const;
	double SetValue(double value){ fValue = value; }
protected:
	double fValue;
};

#endif

