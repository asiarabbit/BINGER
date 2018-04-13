///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayM.C													 //
//   TAMWDCArrayM.C -- source file for class TAMWDCArrayM							 //
//   Introduction: an MWDC array composed of three medium sized DCs. A derived class //
// from TAMWDCArray with distinct features of the medium DCs.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/13.															     //
// Last modified: 2018/4/13, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAMWDCArrayM.h"
#include "TAPopMsg.h"
#include "TAMWDCTaM.h"
#include "TATOFWall.h"
#include "TACtrlPara.h"

TAMWDCArrayM::TAMWDCArrayM(const string &name, const string &title, unsigned uid)
		: TAMWDCArrayR(name, title, uid){
}
TAMWDCArrayM::~TAMWDCArrayM(){}

// create detector objects
void TAMWDCArrayM::Configure(){
	if(fMWDC[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetDsquareThresholdPerDot(TACtrlPara::DsquareThresholdPerDot(fUID));
	if(!fMWDC[0]) fMWDC[0] = new TAMWDCTaM(fName+"->DC0", fTitle+"->DC0", fUID+(0<<6));
	if(!fMWDC[1]) fMWDC[1] = new TAMWDCTaM(fName+"->DC1", fTitle+"->DC1", fUID+(1<<6));
	if(!fMWDC[2]) fMWDC[2] = new TAMWDCTaM(fName+"->DC2", fTitle+"->DC2", fUID+(2<<6));
	if(!fTOFWall) fTOFWall = new TATOFWall(fName+"->TOFWall", fTitle+"->TOFWall", fUID+(3<<6));

	for(int i = 3; i--;){
		fMWDC[i]->SetMWDCId(i);
		fMWDC[i]->SetMotherDCArr(this);
		fMWDC[i]->Configure();
	}
	fTOFWall->SetNStrip(1);
	fTOFWall->Configure();
}

void TAMWDCArrayM::Map(){
	if(GetTOFWall()->GetNFiredStrip() <= 0) return; // event filter

	Map(fMWDC, fTrackList[0], 0); // X
	Map(fMWDC, fTrackList[1], 0); // Y
}


