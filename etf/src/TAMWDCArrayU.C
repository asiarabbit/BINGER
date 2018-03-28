///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayU.h													 //
//   TAMWDCArrayU.h -- header file for class TAMWDCArrayU							 //
//   Introduction: the MWDC array upstream of the target. A derived class from		 //
// TAMWDCArray2. Note that there's only two DCs in the DC array.					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/22.															     //
// Last modified: 2018/3/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAMWDCArrayU.h"
#include "TAPopMsg.h"
#include "TAMWDCTaM.h"
#include "TAPlaStrip.h"
#include "TACtrlPara.h"

TAMWDCArrayU::TAMWDCArrayU(const string &name, const string &title, unsigned uid)
		: TAMWDCArray2(name, title, uid){
}
TAMWDCArrayU::~TAMWDCArrayU(){}

// create detector objects
void TAMWDCArrayU::Configure(){
	if(fMWDC[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetDsquareThresholdPerDot(TACtrlPara::DsquareThresholdPerDot(fUID));
	if(!fMWDC[0]) fMWDC[0] = new TAMWDCTaM(fName+"->DC0", fTitle+"->DC0", fUID+(0<<6));
	if(!fMWDC[1]) fMWDC[1] = new TAMWDCTaM(fName+"->DC1", fTitle+"->DC1", fUID+(1<<6));
	if(!fPlaT0) fPlaT0 = new TAPlaStrip(fName+"->PlaT0", fTitle+"->PlaT0", fUID+(2<<6));

	for(int i = 2; i--;){
		fMWDC[i]->SetMWDCId(i);
		fMWDC[i]->SetMotherDCArr(this);		
		fMWDC[i]->Configure();
	}
	fPlaT0->Configure();
}

