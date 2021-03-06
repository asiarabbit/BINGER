///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChPara.C														 //
//   TAChPara.C -- source file for class TAChPara									 //
//   Introduction: class for holding general electronic channel parameter.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2018/1/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAChPara.h"
#include "TAPopMsg.h"
#include "TAParaManager.h"

TAChPara::TAChPara(const string &name, const string &title, unsigned uid)
			: TAParameter(name, title, uid), fDelay(-9999.), fChId(-2){
	fDelay = 0.;
	TAParaManager::Instance()->AddChPara(this);
}

int TAChPara::GetChannelId() const{
//	if(fChId < 0) TAPopMsg::Error(fName.c_str(), "GetChannelId: Minus value: %d", fChId);
	return fChId;
}
double TAChPara::GetDelay() const{
	if(-9999. == fDelay) TAPopMsg::Error(fName.c_str(), "GetDelay: not assigned");
	return fDelay;
}

