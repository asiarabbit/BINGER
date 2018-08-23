///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPDCL.h															 //
//   TAPDCL.h -- header file for class TAPDCL										 //
//   Introduction: A class derived from TAMWDCTa. This class is for MWDCs from BUAA	 //
// which was R&D-ed by Japan researchers, and would be used around the target. L here//
// stands for "Left", out of historical reasons.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/20.															     //
// Last modified: 2018/4/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPDCL_H_
#define _TAPDCL_H_

#include "TAMWDCTa.h"

class TAPDCL : public TAMWDCTa{
public:
	TAPDCL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPDCL();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const override;
	virtual void Configure() override;
protected:
};

#endif
