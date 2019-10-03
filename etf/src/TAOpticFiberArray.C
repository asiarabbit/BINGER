///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAOpticFiberArray.C											     //
//   TAOpticFiberArray.C -- source file for class TAOpticFiberArray				     //
//   Introduction: A optical fiber array to measure position and time of beams with	 //
// high intensity. SiPM is used for readout.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/19.															     //
// Last modified: 2018/6/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAOpticFiberArray.h"
#include "TAUIDParser.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAPopMsg.h"
#include "TADetectorPara.h"
#include "TAChannel.h"

TAOpticFiberArray::TAOpticFiberArray(const string &name, const string &title, unsigned uid)
	: TATOFWall(name, title, uid), TADetUnion(uid){
}
TAOpticFiberArray::~TAOpticFiberArray(){}

void TAOpticFiberArray::Initialize(){
	TATOFWall::Initialize();
}
// get the channel that belongs to this with uid
TAStuff *TAOpticFiberArray::GetChannel(unsigned uid) const{
	int type[4]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[4]{}; TAUIDParser::DNS(TYPE, GetUID()); // parse uid of this

	if(type[0] == TYPE[0]){ // belongs to this object
		TAPlaStrip *str = GetStrip(type[2]);
		switch(type[3]){
			case 0: return str->GetUV();
			case 1: return str->GetUH();
			case 2: return str->GetDV();
			case 3: return str->GetDH();
			default: return nullptr;
		}
	}
	return nullptr;
}
// create detector objects
void TAOpticFiberArray::Configure(){
	if(fStripArr.size() > 0){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}

	TADetector::Configure();
	SetDelayAvrg(0.);
	const int n = 40;
	if(-9999 == fNStrip) fNStrip = n;

	char name[64];
	TAPlaStrip *str;
	const double delay_str[n] = {0.};
	for(int i = 0; i < fNStrip; i++){
		sprintf(name, "%s->Strip%d", GetName().c_str(), i);
		str = new TAPlaStrip(name, name, GetUID()+(i<<9));
		fStripArr.push_back(str);
		str->SetStripId(i); str->Configure();
		TAPlaStripPara *para = str->GetStripPara();
		para->SetWidth(1.5);
		para->AppendDelay(delay_str[i]);
		para->SetLength(60.);
		para->SetVeff(154.); // mm/ns
	}
	
	double pos[6]{};
	GetDetPara()->SetPosition(pos);
	AssignStripPosition();
} // end of function Configure


