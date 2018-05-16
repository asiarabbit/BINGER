///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArrayM.h													 //
//   TAMWDCArrayM.h -- header file for class TAMWDCArrayM							 //
//   Introduction: an MWDC array composed of three medium sized DCs. A derived class //
// from TAMWDCArray with distinct features of the medium DCs.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/13.															     //
// Last modified: 2018/4/13, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCARRAYM_H_
#define _TAMWDCARRAYM_H_

#include "TAMWDCArrayR.h"

class TAMWDCArrayM : public TAMWDCArrayR{
public:
	TAMWDCArrayM(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayM();

	using TAMWDCArray::Map;
	virtual void Map() override; // Map for XY-DCs
	virtual void Configure() override; // create detectors of the MWDC array
protected:
};

#endif

