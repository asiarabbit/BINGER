///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//   																			     //
// BINGER/inc/etf/TADCSuperLayer.h 													 //
//   TADCSuperLayer.h -- header file for class TADCSuperLayer 						 //
//   Introduction: a super layer is the combination of two adjacent layers in a DC,  //
// which of course share the same wire-orientation. This is a container class, which //
// accommodates TADCCable objects.		 											 //
//     																				 //
// Author: SUN Yazhou, asia.rabbit@163.com.    										 //
// Created: 2017/10/3.   														     //
// Last modified: 2018/4/26, SUN Yazhou.     										 //
//     																				 //
//     																				 //
// Copyright (C) 2017-2018, SUN Yazhou.     										 //
// All rights reserved.     														 //
///////////////////////////////////////////////////////////////////////////////////////

#include <cstring>
#include "TADCSuperLayer.h"
#include "TADCCable.h"
#include "TADeployPara.h"
#include "TAUIDParser.h"
#include "TAPopMsg.h"

TADCSuperLayer::TADCSuperLayer(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid), fSLayerId(-1), fNCable(-1){
	fDCCableArr.reserve(5);
	for(double &x : fGlobalDirection) x = -9999.;
}
TADCSuperLayer::~TADCSuperLayer(){
	for(TADCCable *cab : fDCCableArr) if(cab){
		delete cab; cab = nullptr;
	}
}
short TADCSuperLayer::GetSLayerId() const{
	if(fSLayerId < 0) TAPopMsg::Error(GetName().c_str(), "GetSLayerId: Super layer id not assigend");
	return fSLayerId;
}
short TADCSuperLayer::GetNCable() const{
	if(fSLayerId < 0) TAPopMsg::Error(GetName().c_str(), "GetNCable: Not assigend");
	return fNCable;
}
TADCCable *TADCSuperLayer::GetCable(int n) const{
	if(n < 0 || (unsigned)n >= fDCCableArr.size())
		TAPopMsg::Error(GetName().c_str(), "GetCable: The input subscript out of range: %d", n);
	if(!fDCCableArr[n])
		TAPopMsg::Error(GetName().c_str(), "GetCable: requested cable pointer is null");
	return fDCCableArr[n];
}
const double *TADCSuperLayer::GetGlobalDirection() const{
	if(-9999. == fGlobalDirection[0])
		TAPopMsg::Error(GetName().c_str(), "GetGlobalDirection: Not Assigned yet~");
	return fGlobalDirection;
}

void TADCSuperLayer::SetSLayerId(int id){
	if(id < 0) TAPopMsg::Warn(GetName().c_str(), "SetSLayerId: Input id is minus: %d", id);
	fSLayerId = id;
}
void TADCSuperLayer::SetGlobalDirection(const double *ag){
	for(int i = 3; i--;) fGlobalDirection[i] = ag[i];
}

void TADCSuperLayer::Initialize(){
	for(TADCCable *&ca: fDCCableArr) ca->Initialize();
}
void TADCSuperLayer::Configure(){
	if(fDCCableArr.size() > 0){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}

	if(-1 == fNCable) fNCable = TADeployPara::Instance()->GetNDCCable(GetUID());
	const int n = fNCable;
//	TAPopMsg::Debug(GetName().c_str(), "Configure: ncable: %d", n);
	TADCCable *ca[n]{0};
	char name[64];
	for(int i = 0; i < fNCable; i++){
		sprintf(name, "%s->Cable%d", GetName().c_str(), i);
		ca[i] = new TADCCable(name, name, fUID+(i<<11));
		ca[i]->SetCableId(i);
		ca[i]->Configure();
		fDCCableArr.push_back(ca[i]);
	} // end loop over i
} // end of function Configure

void TADCSuperLayer::SetNDCCable(int n){
	if(n < 0 || n > 6) TAPopMsg::Error(GetName().c_str(), "SetNDCCable: abnormal input: %d", n);
	fNCable = n;
}



