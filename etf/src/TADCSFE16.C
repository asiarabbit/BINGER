///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADCSFE16.C														 //
//   TADCSFE16.C -- source file for class TADCSFE16									 //
//   Introduction: result of an elaborated classification of DC anodes, a container  //
// class, storing TAAnode objects that belongs to the same SFE16 Ampli-Shape-Discri  //
// chip, which is desigend to contain 16 channels.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/1.															     //
// Last modified: 2018/1/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAPopMsg.h"
#include "TADCSFE16.h"
#include "TAAnode.h"

TADCSFE16::TADCSFE16(const string &name, const string &title, unsigned uid)
	: TAStuff(name, title, uid), fAnodeArr{0}, fChipId(-1){
}
TADCSFE16::~TADCSFE16(){
	for(TAAnode *ano : fAnodeArr) if(ano){
		delete ano; ano = nullptr;
	}
}
TAAnode *TADCSFE16::GetAnode(int n) const{
	if(n < 0 || n > 15) TAPopMsg::Warn(GetName().c_str(), "GetAnode: anode id out of range: %d", n);
	if(!fAnodeArr[n]) TAPopMsg::Warn(GetName().c_str(), "GetAnode: anode#%d not assigend", n);
	return fAnodeArr[n];
}
int TADCSFE16::GetChipId() const{
	if(fChipId < 0) TAPopMsg::Warn(GetName().c_str(), "GetChipId: chip id not assigend");
	return fChipId;
}
void TADCSFE16::SetAnode(int n, TAAnode *ano){
	if(n < 0 || (unsigned)n >= fAnodeArr.size())
		TAPopMsg::Error(GetName().c_str(), "SetAnode: The input sugscript out of range: %d", n);
	if(!ano) TAPopMsg::Error(GetName().c_str(), "SetAnode: The input pointer is null.");
	fAnodeArr[n] = ano;
}
void TADCSFE16::SetChipId(int id){
	if(id < 0) TAPopMsg::Warn(GetName().c_str(), "SetChipId: Input id is minus: %d", id);
	fChipId = id;
}

void TADCSFE16::Initialize(){
	for(TAAnode *&ano : fAnodeArr)
		if(ano && ano->GetFiredStatus())
			ano->Initialize();
}
void TADCSFE16::Configure(){
	if(fAnodeArr[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	char name[128];
	TAAnode *ano{nullptr};
	for(int i = 16; i--;){
		sprintf(name, "%s->Anode%d", fName.c_str(), i);
		ano = new TAAnode(name, name, fUID+(i<<15));
		ano->SetAnodeId(i);
		fAnodeArr[i] = ano;
	}
}








