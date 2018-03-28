///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCTaM.h														 //
//   TAMWDCTaM.h -- header file for class TAMWDCTaM									 //
//   Introduction: A class derived from TAMWDCTa. This class is for medium size DC	 //
// downstream of the target.														 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/28.															     //
// Last modified: 2018/3/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCTAM_H_
#define _TAMWDCTAM_H_

#include "TAMWDCTa.h"

class TAMWDCTaM : public TAMWDCTa{
public:
	TAMWDCTaM(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCTaM();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag);
protected:
};

#endif
