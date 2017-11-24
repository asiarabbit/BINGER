///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChannel.h														 //
//   TAChannel.h -- header file for class TAChannel									 //
//   Introduction: Representating an electronic channel. A container class.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/30.															     //
// Last modified: 2017/10/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TACHANNEL_H_
#define _TACHANNEL_H_

#include "TAStuff.h"
#include "TAChPara.h"
#include "TAChData.h"

struct tEntry;

class TAChannel : public TAStuff{
public:
	TAChannel(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAChannel();

	virtual TAChPara *GetPara() const override;
	virtual TAChData *GetData() const override;
	bool GetFiredStatus() const;
	double GetLeadingTime(int n = 0) const; // get the n-th pulse
	double GetTOT(int n = 0) const;
	double GetTime(int i = 0); // Get meaningful timing information for users
	int GetSerialId() const;
	void SetPara(TAChPara *para){ fPara = para; }
	void SetData(TAChData *data){ fData = data; }
	void SetSerialId(int id){ fSerialId = id; }
	virtual void SetChId(int id);
	virtual bool Assign(tEntry *entry);

	virtual void Initialize();
protected:
	TAChPara *fPara;
	TAChData *fData;
	int fSerialId;
};

#endif
