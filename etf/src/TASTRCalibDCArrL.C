///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrL.C												 //
//   TASTRCalibDCArrL.C -- source file for class TASTRCalibDCArrL					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. A		 //
// derived class of TASTRCalibDCArr, specifically for the left DC array arm.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/11/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TASTRCalibDCArrL.h"
#include "TAMWDCArrayL.h"

TASTRCalibDCArrL::TASTRCalibDCArrL(const string &rootfile, TAMWDCArrayL *dcArrL)
		: TASTRCalibDCArr(rootfile, dcArrL){
}
TASTRCalibDCArrL::~TASTRCalibDCArrL(){}

