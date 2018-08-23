///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCTaL.h														 //
//   TAMWDCTaL.h -- header file for class TAMWDCTaL									 //
//   Introduction: A class derived from TAMWDCTa. This class is for large size DCs	 //
// downstream of the target.														 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/28.															     //
// Last modified: 2018/4/3, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCTAL_H_
#define _TAMWDCTAL_H_

#include "TAMWDCTa.h"

class TAMWDCTaL : public TAMWDCTa{
public:
	TAMWDCTaL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCTaL();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const override;
protected:
};

#endif
