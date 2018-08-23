///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADetector.C														 //
//   TADetector.C -- source file for class TADetector								 //
//   Introduction: Detector base class for detectors that could move freely in space,//
// a container class, including parameters for a detector.							 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/1.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TADetector.h"
#include "TAPopMsg.h"
#include "TADetectorPara.h"

TADetector::TADetector(const string &name, const string &title, int uid)
		: TAStuff(name, title, uid), fDetPara(nullptr){
}
TADetector::~TADetector(){
	if(!fDetPara){
		delete fDetPara;
		fDetPara = nullptr;
	}
}
TADetectorPara *TADetector::GetDetPara() const{
	if(!fDetPara) TAPopMsg::Error(GetName().c_str(), "GetDetPara: fDetPara is not assigned.");
	return fDetPara;
}
void TADetector::Configure(){
	if(!fDetPara) fDetPara = new TADetectorPara(GetName()+"->Para", GetTitle()+"->Para", GetUID());
}
