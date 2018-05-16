///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayD.C													 //
//   TAMWDCArrayD.C -- source file for class TAMWDCArrayD							 //
//   Introduction: the MWDC array downstream of the target. A derived class from	 //
// TAMWDCArray2. Note that there's only two DCs in the DC array.					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/22.															     //
// Last modified: 2018/4/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAMWDCArrayD.h"
#include "TAPopMsg.h"
#include "TAMWDCTaM.h"
#include "TACtrlPara.h"

TAMWDCArrayD::TAMWDCArrayD(const string &name, const string &title, unsigned uid)
		: TAMWDCArray2(name, title, uid){
}
TAMWDCArrayD::~TAMWDCArrayD(){}

// create detector objects
void TAMWDCArrayD::Configure(){
	if(fMWDC[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetDsquareThresholdPerDot(TACtrlPara::DsquareThresholdPerDot(fUID));
	if(!fMWDC[0]) fMWDC[0] = new TAMWDCTaM(fName+"->DC0", fTitle+"->DC0", fUID+(0<<6));
	if(!fMWDC[1]) fMWDC[1] = new TAMWDCTaM(fName+"->DC1", fTitle+"->DC1", fUID+(1<<6));

	for(int i = 2; i--;){
		fMWDC[i]->SetMWDCId(i);
		fMWDC[i]->Configure();
	}
} // end of Configure

