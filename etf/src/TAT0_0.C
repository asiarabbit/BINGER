///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0_0.C														     //
//   TAT0_0.C -- source file for class TAT0_0									     //
//   Introduction: start time detector, located at the middle of RIBLL2 to measure	 //
// particle time of flight. It is actually a plastic scintillator with one-end PMT	 //
// readout.																			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/12/29, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAUIDParser.h"
#include "TAPopMsg.h"
#include "TAT0_0.h"
#include "TAChannel.h"
#include "TAGPar.h"

static const TAGPar *gp = TAGPar::Instance();

TAT0_0::TAT0_0(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid), fUV(nullptr), fUH(nullptr){
	fFiredStatus = -2; fDelay = -9999.;
}
TAT0_0::~TAT0_0(){
	if(fUV){ delete fUV; fUV = nullptr; }
	if(fUH){ delete fUH; fUH = nullptr; }
}

// not simply return fFired, but based on the data members
short TAT0_0::GetFiredStatus(){
	if(-2 == fFiredStatus){ // not assigned
		if(GetUV()->GetFiredStatus()) fFiredStatus = 4;
		else fFiredStatus = -1;
	}
	return fFiredStatus;
}
double TAT0_0::GetDelay() const{
	if(-9999. == fDelay) TAPopMsg::Error(GetName().c_str(), "GetDelay: not defined.");
	return fDelay;
}
// get TOF time
double TAT0_0::GetTime(double t0, double t1, double t2) const{
	return GetUV()->GetLT(t0,t1,t2) - GetDelay();
}
// return up end of the strip, HPTDC very high resolution mode.
TAChannel *TAT0_0::GetUV() const{
	if(!fUV) TAPopMsg::Error(GetName().c_str(), "GetUV: pointer is null.");
	return fUV;
}
// return up end of the strip, HPTDC high resolution mode.
TAChannel *TAT0_0::GetUH() const{
	if(!fUH) TAPopMsg::Error(GetName().c_str(), "GetUH: pointer is null.");
	return fUH;
}

// get the channel that belongs to this with uid.
TAStuff *TAT0_0::GetChannel(unsigned uid) const{
	if(uid > 0x7F) return nullptr; // not a uid belonging to this object
	int type[2]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[2]{}; TAUIDParser::DNS(TYPE, fUID); // parse uid of this
	if(type[0] == TYPE[0]){ // belongs to this
		if(0 == type[1])
			return GetUV();
		else if(1 == type[1])
			return GetUH();
	}
	return nullptr;
}
void TAT0_0::Initialize(){
	if(fUV && fUV->GetFiredStatus()) fUV->Initialize();
	if(fUH && fUH->GetFiredStatus()) fUH->Initialize();
}
void TAT0_0::Configure(){
	if(fUV){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	fUV = new TAChannel(fName+"->UV", fTitle+"->UV", fUID+(0<<6));
	fUH = new TAChannel(fName+"->UH", fTitle+"->UH", fUID+(1<<6));
	fDelay = gp->Val(53); // using TOF start as the delay reference

	// print user-defined configurations
	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure: \nfDelay: %f\n", fDelay);
}

