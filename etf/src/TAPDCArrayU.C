///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPDCArrayU.C														 //
//   TAPDCArrayU.C -- source file for class TAPDCArrayU								 //
//   Introduction: The PDC array upstream of the target. A derived class from		 //
// TAMWDCArray2. Note that there's only two DCs in the DC array.					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/21.															     //
// Last modified: 2018/4/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAPDCArrayU.h"
#include "TAPopMsg.h"
#include "TAPDCL.h"
#include "TACtrlPara.h"

TAPDCArrayU::TAPDCArrayU(const string &name, const string &title, unsigned uid)
		: TAMWDCArray2(name, title, uid){
}
TAPDCArrayU::~TAPDCArrayU(){}

// create detector objects
void TAPDCArrayU::Configure(){
	if(fMWDC[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetDsquareThresholdPerDot(TACtrlPara::DsquareThresholdPerDot(fUID));
	if(!fMWDC[0]) fMWDC[0] = new TAPDCL(fName+"->DC0", fTitle+"->DC0", fUID+(0<<6));
	if(!fMWDC[1]) fMWDC[1] = new TAPDCL(fName+"->DC1", fTitle+"->DC1", fUID+(1<<6));

	for(int i = 2; i--;){
		fMWDC[i]->SetMWDCId(i);
		fMWDC[i]->Configure();
	}
} // end of Configure

