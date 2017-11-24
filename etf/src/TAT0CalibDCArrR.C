///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrR.C													 //
//   TAT0CalibDCArrR.C -- source file for class TAT0CalibDCArrR						 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArr, specifically	 //
// for the right DC array arm.														 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/11/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAT0CalibDCArrR.h"
#include "TAMWDCArrayR.h"

TAT0CalibDCArrR::TAT0CalibDCArrR(const string &rootfile, TAMWDCArrayR *dcArrR)
		: TAT0CalibDCArr(rootfile, dcArrR){
}
TAT0CalibDCArrR::~TAT0CalibDCArrR(){}

