///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPlaStrip.h														 //
//   TAPlaStrip.h -- header file for class TAPlaStrip								 //
//   Introduction: designed for both-end-readout plastic scintillator strip.		 //
// Container class, representating all the attributes and function of the strip.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/30.															     //
// Last modified: 2017/10/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPLASTRIP_H_
#define _TAPLASTRIP_H_

#include "TAStuff.h"

class TAChannel;
class TAPlaStripPara;
class TAPlaStripData;

class TAPlaStrip : public TAStuff{
public:
	TAPlaStrip(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPlaStrip();

	int GetStripId() const;
	double GetDelay() const;
	double GetHitPosition() const; // distance from hit point to the down end.
	// 0: not fired; 1:only upper end fired; 2: only down end fired;
	// 3: both end fired, but hit position out of range. 4: both end fired and hit point within ration range. -1: H and V not simultaneously fired. -2: Member channels not assigned.
	int GetFiredStatus() const; // not simply return fFired, but based on the data members
	// t0, t1 and t2 are set for choosing ch->GetLT over edges
	// (ch->GetLT-t0) within t1 and t2 is chosen. 
	// t0, t1 and t2 using default values, choose the 1st edge
	double GetTime(double t0 = -9999., double t1 = -9999., double t2 = -9999.); // get TOF time
	TAChannel *GetUV() const; // return up end of the strip, HPTDC very high resolution mode.
	TAChannel *GetUH() const; // return up end of the strip, HPTDC high resolution mode.
	TAChannel *GetDV() const; // return down end of the strip, HPTDC very high resolution mode.
	TAChannel *GetDH() const; // return down end of the strip, HPTDC high resolution mode.
	TAPlaStripPara *GetStripPara() const;
	TAPlaStripData *GetStripData() const;
	// Set functions
	void SetStripLength(double length);
	void SetStripId(int id){ fStripId = id; }

	virtual void Initialize();
	virtual void Configure();
protected:
	TAPlaStripPara *fStripPara; // strip parameters, including size and calibration constants
	TAPlaStripData *fStripData; // strip data, reconstructed data from the four channels
	TAChannel *fUV; // Up end - Very high resolution mode channel
	TAChannel *fUH; // Up end - High resolution mode
	TAChannel *fDV; // Down end - Very high resolution mode channel
	TAChannel *fDH; // Down end - High resolution mode
	int fStripId; // strip serial id
	double ft0, ft1, ft2; // used in GetTime(...) to mark the difference of them between two calls
};
#endif
