///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMUSICL.h														 //
//   TAMUSICL.h -- header file for class TAMUSICL									 //
//   Introduction: A derived class from TAMUSIC. "L" stands for large. This MUSIC has//
// 8 channels, i.e., 8 sampling unit.												 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/22.															     //
// Last modified: 2018/4/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMUSICL_H_
#define _TAMUSICL_H_

#include "TAMUSIC.h"

class TAMUSICL : public TAMUSIC{
public:
	TAMUSICL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMUSICL();

	virtual void Configure();
	virtual double GetDeltaE();
	virtual double GetZ(); // particle charge
protected:
};

#endif

