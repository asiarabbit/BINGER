///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCTa.h														 //
//   TAMWDCTa.h -- header file for class TAMWDCTa									 //
//   Introduction: A class derived from TAMWDC. TA stands for target. This class is	 //
// for MWDCs around the target.	Note that this class is designed to be an abstract 	 //
// base class, to be derived before instantiation.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/27.															     //
// Last modified: 2018/3/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCTA_H_
#define _TAMWDCTA_H_

#include "TAMWDC.h"

class TAMWDCTa : public TAMWDC{
public:
	TAMWDCTa(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCTa();
	
	// to be defined in the derived classes
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const = 0;

	virtual void GetAnodeGlobalDirection(int dcType, double *ag) const;
	virtual void GetAnodeGlobalProjection(int dcType, const double *globalCenter, double *globalProjection) const;

	virtual void Configure() override;
protected:
};

#endif
