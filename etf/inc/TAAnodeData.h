///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAnodeData.h														 //
//   TAAnodeData.h -- header file for class TAAnodeData								 //
//   Introduction: derived from TAAnodeData, embodiment of DC anodes.				 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/30.															     //
// Last modified: 2017/10/15, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAANODEDATA_H
#define _TAANODEDATA_H

#include "TAChData.h"

class TAAnodeData : public TAChData{
public:
	TAAnodeData(const string &name = "", const string &title = "", unsigned uid = 999999999)
			: TAChData(name, title, uid){}
	virtual ~TAAnodeData(){}

	virtual void Initialize() override;
	virtual void Show() const override;
	double GetTOF() const;
	void SetTOF(double tof){ fTOF = tof; }
protected:
	double fTOF; // time that a particle crossing the TOF array.
};

#endif


