///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrR.C												 //
//   TASTRCalibDCArrR.C -- source file for class TASTRCalibDCArrR					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArr, specifically for the right DC array arm.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/11/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TASTRCalibDCArrR.h"
#include "TAMWDCArrayR.h"

TASTRCalibDCArrR::TASTRCalibDCArrR(const string &rootfile, TAMWDCArrayR *dcArrR)
		: TASTRCalibDCArr(rootfile, dcArrR){
}
TASTRCalibDCArrR::~TASTRCalibDCArrR(){}

