///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPDCR.h															 //
//   TAPDCR.h -- header file for class TAPDCR										 //
//   Introduction: A class derived from TAMWDCTa. This class is for MWDCs from BUAA	 //
// which was R&D-ed by Japan researchers, and would be used around the target. R here//
// stands for "Right", out of historical reasons.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/20.															     //
// Last modified: 2018/4/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPDCR_H_
#define _TAPDCR_H_

#include "TAMWDCTa.h"

class TAPDCR : public TAMWDCTa{
public:
	TAPDCR(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPDCR();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const override;
	virtual void Configure() override;
protected:
};

#endif
