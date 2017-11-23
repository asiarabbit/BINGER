///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPlaStripData.C													 //
//   TAPlaStripData.C -- source file for class TAPlaStripData						 //
//   Introduction: for caching data of an event relating to a TOF Wall plastic strip //
// with both-end-readout uisng PMT coupling.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2017/10/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


#include "TAPlaStripData.h"
#include "TAPopMsg.h"

TAPlaStripData::TAPlaStripData(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid){
//	TAPopMsg::Debug(GetName().c_str(), "Just to pop a bubble :)");
}
TAPlaStripData::~TAPlaStripData(){}

// 0: not fired; 1:only upper end fired; 2: only down end fired;
// 3: both end fired, but hit position out of range. 4: both end fired and hit point within ration range. -1: H and V not simultaneously fired. -2: Member channels not assigned.
int TAPlaStripData::GetFiredStatus() const{
	return fFiredStatus;
}
double TAPlaStripData::GetTime() const{ // get TOF time
	return fTime;
}
double TAPlaStripData::GetHitPosition() const{ // distance from hit point to the down end.
	return fHitPosition;
}
void TAPlaStripData::Initialize(){ // initialized to unfired status
	fFiredStatus = -2; fTime = -9999.;
	fHitPosition = -9999.;
}

