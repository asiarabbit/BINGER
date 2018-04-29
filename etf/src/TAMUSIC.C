///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMUSIC.h															 //
//   TAMUSIC.h -- header file for class TAMUSIC										 //
//   Introduction: A class for multi-sampleing ionization chamber. It is for the 	 //
// measurement of particle charge information.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/22.															     //
// Last modified: 2018/4/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAMUSIC.h"
#include "TAChannel.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"

TAMUSIC::TAMUSIC(const string &name, const string &title, unsigned uid)
	: TAStuff(name, title, uid), fNChannel(-1){
}
TAMUSIC::~TAMUSIC(){
	for(TAChannel *&c : fChArr) if(c){
		delete c;
		c = nullptr;
	}
}

void TAMUSIC::SetBeta(const double beta){ fBeta = beta; }
double TAMUSIC::GetBeta() const{
	if(-1. == fBeta) TAPopMsg::Error(GetName().c_str(), "GeBeta: beta not defined yet");
	if(fBeta < 0. || fBeta > 1.){
//		TAPopMsg::Warn(GetName().c_str(), "GeBeta: abnormal beta: %f", fBeta);
		return -1.;
	}
	return fBeta;
}
int TAMUSIC::GetNChannel() const{
	if(-1 == fNChannel) TAPopMsg::Error(GetName().c_str(), "GeNChannel: not defined yet");
	return fNChannel;
}
bool TAMUSIC::GetFiredStatus() const{
	if(GetNFiredChannel() <= 1) return false;
	return true;
}
int TAMUSIC::GetNFiredChannel() const{
	int cnt = 0;
	for(TAChannel *c : fChArr) if(c && c->GetFiredStatus()) cnt++;
	return cnt;
}
double TAMUSIC::GetPileUp() const{
	if(fChArr.size() > 0 && fChArr[0] && fChArr[0]->GetFiredStatus())
		return fChArr[0]->GetLeadingTime();
	TAPopMsg::Error(GetName().c_str(), "GetPileUp: fChArr[0] is invalid or not fired.");
	return -9999.;
}

void TAMUSIC::Initialize(){
	for(TAChannel *&c : fChArr) if(c) c->Initialize();
	fBeta = -1; fDeltaE = -9999.; fZ = -9999.;
	fIsPileUp = false;
}
TAStuff *TAMUSIC::GetChannel(unsigned uid) const{
	int type[6]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[6]{}; TAUIDParser::DNS(TYPE, GetUID()); // parse uid of this

	if(type[0] == TYPE[0]){ // belongs to this object
		TAChannel *c = fChArr[type[1]];
		if(!c) TAPopMsg::Error(GetName().c_str(), "GetChannel: required pointer is null");
		return c;
	}
	return nullptr;
} // end of member function GetChannel
