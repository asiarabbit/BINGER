///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChannel.C														 //
//   TAChannel.C -- source file for class TAChannel									 //
//   Introduction: Representating an electronic channel. A container class.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/30.															     //
// Last modified: 2018/5/3, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAChannel.h"
#include "TAPopMsg.h"
#include "tEntry.h"

TAChannel::TAChannel(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid), fPara(nullptr), fData(nullptr){
	fPara = new TAChPara(name+"->Para", title+"->Para", uid);
	fData = new TAChData(name+"->Data", title+"->Data", uid);
}

TAChannel::~TAChannel(){
	if(!fPara){
		delete fPara;
		fPara = nullptr;
	}
	if(!fData){
		delete fData;
		fData = nullptr;
	}
}

TAChPara *TAChannel::GetPara() const{
	if(!fPara) TAPopMsg::Error(GetName().c_str(), "GetPara: Para not assgined.");
	return fPara;
}
TAChData *TAChannel::GetData() const{
	if(!fData) TAPopMsg::Error(GetName().c_str(), "GetData: Data not assgined.");
	return fData;
}
bool TAChannel::GetFiredStatus() const{
	return GetData()->GetFiredStatus();
}
double TAChannel::GetLeadingTime(int n) const{
	return GetData()->GetLeadingTime(n);
}
// t0, t1 and t2 are set for choosing ch->GetLT over edges
// (ch->GetLT-t0) within t1 and t2 is chosen. t0, t1 and t2 using default values, choose the 1st edge
double TAChannel::GetLT(double t0, double t1, double t2) const{
	if(!GetFiredStatus()) return -9999.; // not fired
//	cout << "t0: " << t0 << endl; getchar(); // DEBUG
	if(-9999. == t0 && -9999. == t1 && -9999. == t2) return GetTime();
	else return GetData()->GetLT(t0 + GetPara()->GetDelay(), t1, t2) - GetPara()->GetDelay();
}
double TAChannel::GetTOT(int n) const{
	const double tl = GetData()->GetLeadingTime(n);
	const double tt = GetData()->GetTrailingTime(n);
	if(-9999. == tt || -9999. == tl) return -9999.; // one of them is not assigned
	return tt - tl;
}
void TAChannel::SetChId(int id){
	GetPara()->SetChannelId(id);
}

double TAChannel::GetTime(int i) const{
	double t = GetData()->GetLeadingTime(i);
	if(-9999 == t) return -9999.;
	return t - GetPara()->GetDelay();
}
int TAChannel::GetSerialId() const{
	if(-1 == fSerialId) TAPopMsg::Error(GetName().c_str(), "GetSerialId: not defined.");
	return fSerialId;
}
bool TAChannel::Assign(tEntry *entry){
	if(GetPara()->GetChannelId() != entry->channelId) return false;
	return GetData()->Assign(entry);
}
void TAChannel::Initialize(){
	GetData()->Initialize();
}






