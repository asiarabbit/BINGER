///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADCCable.h														 //
//   TADCCable.h -- header file for class TADCCable									 //
//   Introduction: result of an elaborated classification of DC anodes, a container  //
// class, storing two TASFE16 objects that are intalled int the same MWDC FEE box.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/1.															     //
// Last modified: 2017/10/12, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TADCCABLE_H_
#define _TADCCABLE_H_

#include "TAStuff.h"
#include <array>

using std::array;

class TADCSFE16;

class TADCCable : public TAStuff{
public:
	TADCCable(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TADCCable();
	TAStuff *GetChannel(int uid) const; // get the channel that belongs to this with uid.
	int GetCableId() const;
	TADCSFE16 *GetSFE16(int n) const;
	void SetSFE16(int n, TADCSFE16 *sfe);
	void SetCableId(int id);	

	virtual void Initialize();
	virtual void Configure();
protected:
	array<TADCSFE16 *, 2> fSFE16Arr;
	int fCableId;
};

#endif
