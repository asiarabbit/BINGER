///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray4.h													 //
//   TAMWDCArray4.h -- header file for class TAMWDCArray4							 //
//   Introduction: an MWDC array composed of 4 medium sized DCs. A derived class 	 //
// from TAMWDCArray with distinct features of the medium DCs.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/9/3.															     //
// Last modified: 2018/9/3, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCARRAY4_H_
#define _TAMWDCARRAY4_H_

#include "TAMWDCArray.h"

class TATrack4;

class TAMWDCArray4 : public TAMWDCArray{
public:
	TAMWDCArray4(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArray4(); // abstract base class

	virtual TAMWDC *GetMWDC(int dcId) const;
	virtual void AssignTracks(vector<tTrack *> &track_ls);
	int compare4(TATrack *newTrack, TATrack *oldTrack, char dcType = 'X', bool show = false);
	bool Map4(TAMWDC **MWDC, vector<TATrack *> &track, int dcType);
	virtual void Map();
	virtual void Configure();
	virtual void Initialize();
	virtual TAStuff *GetChannel(unsigned uid) const;
	virtual void Info() const;
};

#endif


