///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAPlaStripPara.cxx																								 //
//   TAPlaStripPara.cxx -- source file for class TAPlaStripPara											 //
//   Introduction: for caching Parameter of a TOF Wall plastic strip with						 //
// both-end-readout uisng PMT coupling.																							 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/10/9.																													     //
// Last modified: 2017/10/9, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAPlaStripPara.h"
#include "TAPopMsg.h"

TAPlaStripPara::TAPlaStripPara(const string &name, const string &title, unsigned uid)
		:TAStuff(name, title, uid){
	fWidth = -9999.; fLength = -9999.;
	fVeff = -9999.; fDelay = 0.;
	for(double &x : fGlobalProjection) x = -9999.;
}
TAPlaStripPara::~TAPlaStripPara(){}

double TAPlaStripPara::GetWidth() const{
	if(-9999. == fWidth) TAPopMsg::Error(GetName().c_str(), "GetWidth: not defined.");
	return fWidth;
}
double TAPlaStripPara::GetLength() const{
	if(-9999. == fLength) TAPopMsg::Error(GetName().c_str(), "GetLength: not defined.");
	return fLength;
}
double TAPlaStripPara::GetVeff() const{
	if(-9999. == fVeff) TAPopMsg::Error(GetName().c_str(), "GetVeff: not defined.");
	return fVeff;
}
double TAPlaStripPara::GetDelay() const{
	if(0. == fDelay) TAPopMsg::Warn(GetName().c_str(), "GetDelay: may not be defined yet.");
	return fDelay;
}
void TAPlaStripPara::GetGlobalProjection(double *Ag){
	if(-9999. == fGlobalProjection[0])
		TAPopMsg::Error(GetName().c_str(), "GetGlobalProjection: Not Assigned yet~");
	for(int i = 3; i--;) Ag[i] = fGlobalProjection[i];
}
const double *TAPlaStripPara::GetGlobalProjection() const{
	if(-9999. == fGlobalProjection[0])
		TAPopMsg::Error(GetName().c_str(), "GetGlobalProjection: Not Assigned yet~");
	return fGlobalProjection;
}

void TAPlaStripPara::SetGlobalProjection(const double *Ag){
	for(int i = 3; i--;) fGlobalProjection[i] = Ag[i];
}
