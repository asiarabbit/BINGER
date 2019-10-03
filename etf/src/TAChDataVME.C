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

#include "TAChDataVME.h"

TAChDataVME::TAChDataVME(const string &name, const string &title, unsigned uid)
	: TAChData(name, title, uid){
}
TAChDataVME::~TAChDataVME(){}

// get the n-th pulse
double TAChDataVME::GetLeadingTime(int n) const{
	double data = TAChData::GetLeadingTime(n);
	return -data;
}

// get the n-th pulse
double TAChDataVME::GetTrailingTime(int n) const{
	double data = TAChData::GetTrailingTime(n);
	return -data;
}
