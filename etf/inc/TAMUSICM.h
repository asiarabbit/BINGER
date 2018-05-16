///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMUSICM.h														 //
//   TAMUSICM.h -- header file for class TAMUSICM									 //
//   Introduction: A derived class from TAMUSIC. "M" stands for Medium. This MUSIC	 //
// has 4 channels, i.e., 4 sampling unit.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/22.															     //
// Last modified: 2018/4/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMUSICM_H_
#define _TAMUSICM_H_

#include "TAMUSIC.h"

class TAMUSICM : public TAMUSIC{
public:
	TAMUSICM(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMUSICM();
	
	virtual void Configure();
	virtual double GetDeltaE();
	virtual double GetZ(); // particle charge
protected:
};

#endif

