///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAAnodeData.cxx																										 //
//   TAAnodeData.cxx -- source file for class TAAnodeData														 //
//   Introduction: derived from TAAnodeData, embodiment of DC anodes.								 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/9/30.																													     //
// Last modified: 2017/10/15, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "TAAnodeData.h"
#include "TAPopMsg.h"

void TAAnodeData::Initialize(){
	TAChData::Initialize();
	fTOF = -9999.;
}
void TAAnodeData::Show() const{
	TAChData::Show();
	std::cout << "TOF: " << fTOF << std::endl;
}
double TAAnodeData::GetTOF() const{
//	if(-9999. == fTOF) TAPopMsg::Warn(GetName().c_str(), "GetTOF: TOF not assigned.");
	return fTOF;
}
