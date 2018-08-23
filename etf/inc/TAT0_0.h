///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0_0.h														     //
//   TAT0_0.h -- header file for class TAT0_0									     //
//   Introduction: start time detector, located at the middle of RIBLL2 to measure	 //
// particle time of flight. It is actually a plastic scintillator with one-end PMT	 //
// readout.																			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/12/29, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAT0_0_H_
#define _TAT0_0_H_

#include "TAStuff.h"
#include "TADetUnion.h"

class TAChannel;

class TAT0_0 : public TAStuff, public TADetUnion{
public:
	TAT0_0(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAT0_0();

	short GetFiredStatus(); // 0: not fired; 4: fired; -2: not assigned
	double GetDelay() const;
	// t0, t1 and t2 are set for choosing ch->GetLT over edges
	// (ch->GetLT-t0) within t1 and t2 is chosen
	// t0, t1 and t2 using default values, choose the 1st edge
	double GetTime(double t0 = -9999., double t1 = -9999., double t2 = -9999.) const; // get TOF time
	TAChannel *GetUV() const; // return up end of the strip, HPTDC very high resolution mode
	TAChannel *GetUH() const; // return up end of the strip, HPTDC high resolution mode
	void SetDelay(double delay){ fDelay = delay; }

	virtual void Initialize() override;
	// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Configure() override; // create detector objects
protected:
	short fFiredStatus;
	double fDelay;
	TAChannel *fUV; // Up end - Very high resolution mode channel
	TAChannel *fUH; // Up end - High resolution mode
};

#endif
