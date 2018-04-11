///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADCCable.C														 //
//   TADCCable.C -- source file for class TADCCable									 //
//   Introduction: result of an elaborated classification of DC anodes, a container  //
// class, storing two TASFE16 objects that are intalled int the same MWDC FEE box.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/1.															     //
// Last modified: 2018/1/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TADCCable.h"
#include "TADCSFE16.h"
#include "TAPopMsg.h"

TADCCable::TADCCable(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid), fSFE16Arr{0}, fCableId(-1){
}
TADCCable::~TADCCable(){
	for(TADCSFE16 *sfe : fSFE16Arr) if(sfe){
		delete sfe; sfe = nullptr;
	}
}
int TADCCable::GetCableId() const{
	if(fCableId < 0) TAPopMsg::Error(GetName().c_str(), "GetCableId: Cable id not assigend");
	return fCableId;
}
TADCSFE16 *TADCCable::GetSFE16(int n) const{
	if(n < 0 || (unsigned)n >= fSFE16Arr.size())
		TAPopMsg::Error(GetName().c_str(), "GetSFE16: The input subscript out of range: %d", n);
	if(!fSFE16Arr[n])
		TAPopMsg::Error(GetName().c_str(), "GetSFE16Id: SFE16 Chip#%d not assigend", n);
	return fSFE16Arr[n];
}
void TADCCable::SetSFE16(int n, TADCSFE16 *sfe){
	if(n < 0 || (unsigned)n >= fSFE16Arr.size())
		TAPopMsg::Error(GetName().c_str(), "SetSFE16: The input subscript out of range: %d", n);
	if(!sfe) TAPopMsg::Error(GetName().c_str(), "SetSFE16: The input pointer is null.");
	fSFE16Arr[n] = sfe;
}
void TADCCable::SetCableId(int id){
	if(id < 0) TAPopMsg::Warn(GetName().c_str(), "SetCableId: Input id is minus: %d", id);
	fCableId = id;
}

void TADCCable::Initialize(){
	for(TADCSFE16 *&sfe : fSFE16Arr) if(sfe) sfe->Initialize();
}
void TADCCable::Configure(){
	if(fSFE16Arr[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	TADCSFE16 *sfe[2];

	char name[64];
	for(int i = 2; i--;){
		sprintf(name, "->SFE16_%d", i);
		sfe[i] = new TADCSFE16(fName+name, fTitle+name, fUID+(i<<14));
		sfe[i]->SetChipId(i);
		sfe[i]->Configure();
		fSFE16Arr[i] = sfe[i];
	}
}









