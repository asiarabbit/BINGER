///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChPara.h														 //
//   TAChPara.h -- header file for class TAChPara									 //
//   Introduction: class for holding general electronic channel parameter.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/11/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TACHPARA_H_
#define _TACHPARA_H_

#include "TAParameter.h"

class TAChPara : public TAParameter{
public:
	TAChPara(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAChPara() {};

	int GetChannelId() const;
	virtual double GetDelay() const;
	void SetChannelId(int chid){ fChId = chid; }
	void AppendDelay(double delay) { fDelay += delay; }
protected:
	void SetDelay(double delay){ fDelay = delay; } // should be called with caution

	double fDelay; // time offset
	int fChId; // channel id
};

#endif
