///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChData.h														 //
//   TAChData.h -- header file for class TAChData									 //
//   Introduction: base class for holding experiment data per channel, also			 //
// responsible for data initialization, caching data, data access during event		 //
// reconstruction, and self-marking according to requirements of the code.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/30.															     //
// Last modified: 2017/10/12, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TACHDATA_H_
#define _TACHDATA_H_

#include "TAStuff.h"
struct tEntry;

class TAChData : public TAStuff{
public:
	TAChData(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAChData();

	int GetEventIndex() const; // return fEventIndex.
	bool GetFiredStatus() const{ return fFired; }
	double GetLeadingTime(int n = 0) const; // get the n-th pulse
	double GetTrailingTime(int n = 0) const; // get the n-th pulse
	int GetNLeadingEdge() const { return fNLeadingEdge; }
	int GetNTrailingEdge() const { return fNTrailingEdge; }
	bool IsV() const { return fIs_V; }
	double GetTOT(int n = 0) const; // get the time over threshold information of the n-th pulse.
	void SetFiredStatus(bool sta){ fFired = sta; }

	virtual void Initialize(); // initialize the data member
	virtual bool Assign(tEntry *entry);
	virtual void Show() const; // for debugging purposes.
protected:
	bool fFired; // fired status. false: not fired. true: fired.
	int fEventIndex; // event index;	
	int fNLeadingEdge; // count of leading edges
	int fNTrailingEdge; // counf of trailing edges.
	static const int NLMAX = 5, NTMAX = 5; // maximum number of leading and trailing edges per event.
	double fLeadingTime[NLMAX]; // signal leading edge time tag.
	double fTrailingTime[NTMAX]; // signal trailing edge time tag.
	bool fIs_V; // mark if the channel is HPTDC very high resolution mode data.
};

#endif
