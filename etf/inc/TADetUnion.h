///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADetUnion.h														 //
//   TADetUnion.h -- header file for class TADetUnion								 //
//   Introduction: Abstract base class, providing a common interface to detector	 //
// classes that has UID resolution function and is at the first level of the		 //
// detector hierarchy, which also has the right to design UID resolution scheme of	 //
// their own.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/13.															     //
// Last modified: 2018/4/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TADETUNION_H_
#define _TADETUNION_H_

#include "TAStuff.h"

class TADetUnion{
public:
	TADetUnion();
	virtual ~TADetUnion();

	virtual void Initialize() = 0;
	// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const = 0;
	virtual void Configure() = 0; // create detector objects
	virtual bool IsDCArr() const{ return false; } // whether this is an MWDC array or not
};

#endif
