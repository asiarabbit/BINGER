///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPlaStripPara.h													 //
//   TAPlaStripPara.h -- header file for class TAPlaStripPara						 //
//   Introduction: for caching Parameter of a TOF Wall plastic strip with			 //
// both-end-readout uisng PMT coupling.												 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/1/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPLASTRIPPARA_H_
#define _TAPLASTRIPPARA_H_

#include "TAStuff.h"

class TAPlaStrip;

class TAPlaStripPara : public TAStuff{
public:
	TAPlaStripPara(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPlaStripPara();

	double GetWidth() const;
	double GetLength() const;
	double GetVeff() const;
	double GetDelay() const;
	const double *GetGlobalProjection() const;
	void GetGlobalProjection(double *Ag);
	void SetWidth(double width){ fWidth = width; }
	void SetLength(double length){ fLength = length; }
	void SetVeff(double veff){ fVeff = veff; }
	void AppendDelay(double delay){ fDelay += delay; }
	void SetGlobalProjection(const double *Ag);
	friend class TAPlaStrip;
protected:
	void SetDelay(double delay){ fDelay = delay; } // should be called with caution
	double fWidth; // width of a stip
	double fLength; // length of a strip
	double fVeff; // effective light speed in the plastic scintiilator strip.
	double fDelay; // delay time specific for the strip; /ns
	double fGlobalProjection[3];
};
#endif
