///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADCSFE16.h														 //
//   TADCSFE16.h -- header file for class TADCSFE16									 //
//   Introduction: result of an elaborated classification of DC anodes, a container  //
// class, storing TAAnode objects that belongs to the same SFE16 Ampli-Shape-Discri  //
// chip, which is desigend to contain 16 channels.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/1.															     //
// Last modified: 2017/10/12, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TADCSFE16_H_
#define _TADCSFE16_H_

#include "TAStuff.h"
#include <array>

using std::array;

class TAAnode;

class TADCSFE16 : public TAStuff{
public:
	TADCSFE16(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TADCSFE16();
	TAAnode *GetAnode(int n) const;
	int GetChipId() const;
	void SetAnode(int n, TAAnode *ano);
	void SetChipId(int id);

	virtual void Initialize();
	virtual void Configure();
protected:
	array<TAAnode *, 16> fAnodeArr;
	int fChipId;
};

#endif
