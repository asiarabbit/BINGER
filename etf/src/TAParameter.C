///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAParameter.C														 //
//   TAParameter.C -- source file for class TAParameter								 //
//   Introduction: base class for unit parameter storage, derived from TAStuff.		 //
// Direct instantiation of this class is supposed to store physical constants.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/9/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAParameter.h"
#include "TAPopMsg.h"

TAParameter::TAParameter(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid){
	fValue = -9999.;
}
TAParameter::~TAParameter(){};

double TAParameter::GetValue() const{
	if(-9999. == fValue)
		TAPopMsg::Error(fName.c_str(), "Parameter may have not been assigned.");
	return fValue;
}

