///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrL.C													 //
//   TAT0CalibDCArrL.C -- source file for class TAT0CalibDCArrL						 //
//   Introduction: a tool class to calibrate DC anode T0(delay)s anode by anode by	 //
// fitting drift time distribution. A derived class of TAT0CalibDCArr, specifically	 //
// for the left DC array arm.														 //
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

#include "TAT0CalibDCArrL.h"
#include "TAMWDCArrayL.h"

TAT0CalibDCArrL::TAT0CalibDCArrL(const string &rootfile, TAMWDCArrayL *dcArrL)
		: TAT0CalibDCArr(rootfile, dcArrL){
}
TAT0CalibDCArrL::~TAT0CalibDCArrL(){}

