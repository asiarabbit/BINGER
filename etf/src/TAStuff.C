///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAStuff.C														     //
//   TAStuff.C -- source file for class TAStuff									     //
//   Introduction: This is a top base class supposed to be inherited by all classes  //
// in the code project, providing UID and interpretation of UID, togehter with name  //
// and title for object identification in the code.								     //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2018/8/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAStuff.h"
#include "TAPopMsg.h"

TAStuff::~TAStuff(){} // the destructor

unsigned TAStuff::GetUID() const{
	if(999999999 == fUID) TAPopMsg::Warn(GetName().c_str(), "GetUID: Not Assigned.");
	return fUID;
}

TAChPara *TAStuff::GetPara() const{
	TAPopMsg::Warn(GetName().c_str(), "TAStuff::GetPara: Method not defined.");
	return nullptr;
}
TAChData *TAStuff::GetData() const{
	TAPopMsg::Warn(GetName().c_str(), "TAStuff::GetData: Method not defined.");
	return nullptr;
}
void TAStuff::Info() const{
	TAPopMsg::Info(GetName().c_str(), "TAStuff::Info: object title: %s", GetTitle().c_str());
}

