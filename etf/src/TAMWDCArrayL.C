///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayL.C													 //
//   TAMWDCArrayL.C -- source file for class TAMWDCArrayL							 //
//   Introduction: the left arm of the MWDC array. A derived class from TAMWDCArray	 //
// with distinct features of the left arm.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/10/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAMWDCArrayL.h"
#include "TAPopMsg.h"
#include "TAMWDC.h"
#include "TATOFWall.h"
#include "TACtrlPara.h"

TAMWDCArrayL::TAMWDCArrayL(const string &name, const string &title, int uid)
		: TAMWDCArray(name, title, uid){
}
TAMWDCArrayL::~TAMWDCArrayL(){}

// create detector objects
void TAMWDCArrayL::Configure(){
	if(fMWDC[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetDsquareThresholdPerDot(TACtrlPara::DsquareThresholdPerDot(fUID));
	if(!fMWDC[0]) fMWDC[0] = new TAMWDC(fName+"->DC0", fTitle+"->DC0", fUID+(0<<6));
	if(!fMWDC[1]) fMWDC[1] = new TAMWDC(fName+"->DC1", fTitle+"->DC1", fUID+(1<<6));
	if(!fMWDC[2]) fMWDC[2] = new TAMWDC(fName+"->DC2", fTitle+"->DC2", fUID+(2<<6));
	if(!fTOFWall) fTOFWall = new TATOFWall(fName+"->TOFWall", fTitle+"->TOFWall", fUID+(3<<6));

	for(int i = 3; i--;){
		fMWDC[i]->SetMWDCId(i);
		fMWDC[i]->SetMotherDCArr(this);		
		fMWDC[i]->Configure();
	}
	fTOFWall->Configure();
}

