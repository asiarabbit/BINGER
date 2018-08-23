///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAOpticFiberArray.h											     //
//   TAOpticFiberArray.h -- header file for class TAOpticFiberArray				     //
//   Introduction: A optical fiber array to measure position and time of beams with	 //
// high intensity. SiPM is used for readout.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/19.															     //
// Last modified: 2018/6/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAOPTICFIBERARRAY_H_
#define _TAOPTICFIBERARRAY_H_

#include "TATOFWall.h"
#include "TADetUnion.h"

class TAOpticFiberArray : public TATOFWall, public TADetUnion{
public:
	TAOpticFiberArray(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAOpticFiberArray();

	virtual void Initialize();
	// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const;
	virtual void Configure(); // create detector objects
};
#endif
