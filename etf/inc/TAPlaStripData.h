///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPlaStripData.h													 //
//   TAPlaStripData.h -- header file for class TAPlaStripData						 //
//   Introduction: for caching data of an event relating to a TOF Wall plastic strip //
// with both-end-readout uisng PMT coupling.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2017/10/29, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPLASTRIPDATA_H_
#define _TAPLASTRIPDATA_H_

#include "TAStuff.h"

class TAPlaStripData : public TAStuff{
public:
	TAPlaStripData(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPlaStripData();

	// 0: not fired; 1:only upper end fired; 2: only down end fired;
	// 3: both end fired, but hit position out of range. 4: both end fired and hit point within ration range. -1: H and V not simultaneously fired. -2: Member channels not assigned. -10: altered manually
	int GetFiredStatus() const; // not simply return fFired, but based on the data members
	double GetTime() const; // get TOF time
	double GetHitPosition() const; // distance from hit point to the down end.
	void Initialize(); // initialized to unfired status
	void SetFiredStatus(int sta){ fFiredStatus = sta; }
	void SetTime(double time){ fTime = time; }
	void SetHitPosition(double pos){ pos = fHitPosition; }
	void SetFireStatus(bool sta){ fFiredStatus = sta; }

protected:
	int fFiredStatus;
	double fTime; // time when the particle hits the strip, certain for an event
	double fHitPosition; // position where the particle hits the strip, certain for an event
};
#endif
