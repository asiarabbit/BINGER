///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0_1.h														     //
//   TAT0_1.h -- header file for class TAT0_1									     //
//   Introduction: start time detector, located at the end of RIBLL2 to measure		 //
// particle time of flight. It is actually a plastic scintillator with both-end PMT	 //
// readouts.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/10/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAT0_1_H_
#define _TAT0_1_H_

#include "TAPlaStrip.h"
#include "TADetUnion.h"

class TAT0_1 : public TAPlaStrip, public TADetUnion{
public:
	TAT0_1(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAT0_1();
	// position along the beam line in the global coordinate system
	double GetZ0() const;
	double GetDistanceOfFlight() const; // from T0_0 to T0_1
	void SetZ0(double z0){ fZ0 = z0; } // from this to 32Q02
	void SetDistanceOfFlight(double l){ fDistanceOfFlight = l; }

	virtual void Initialize() override{ TAPlaStrip::Initialize(); }
	// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Configure() override; // create detector objects
protected:
	double fDistanceOfFlight; // from T0_0 to T0_1
	double fZ0; // from this to 32Q02
};

#endif
