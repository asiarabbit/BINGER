///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAnode.h														     //
//   TAAnode.h -- header file for class TAAnode									     //
//   Introduction: Drift chamber anode class, representative of the physical	     //
// entity. This is a container class, providing the attributes and data analysis     //
// function of the anode.														     //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/12/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAANODE_H_
#define _TAANODE_H_

#include "TAStuff.h"

class TAAnodePara;
class TAAnodeData;

class TAAnode : public TAStuff{
public:
	TAAnode(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAAnode();

	virtual TAChPara *GetPara() const override;
	virtual TAChData *GetData() const override;
	TAAnodeData *GetAnodeData() const{ return (TAAnodeData*)GetData(); }
	TAAnodePara *GetAnodePara() const{ return (TAAnodePara*)GetPara(); }
	short GetAnodeId() const;
	double GetTOT(int n = 0) const;
	void SetPara(TAAnodePara *para){ fPara = para; }
	void SetData(TAAnodeData *data){ fData = data; }
	void SetAnodeId(int id);
	virtual void SetChId(int id);
	// weight: for weighted addition of chi to chi2
	double GetDriftTime() const;
	double GetDriftTime(double &weight) const;
	// for generate simulation data and t suggested by track fitting
	double GetDriftTime(double r, double k);
	// unit: mm, ns. k: track slope, dt: drift time
	double GetDriftDistance(double dt, int STR_id);
	double GetDriftDistance(double dt, double k);
	double GetDriftDistanceCorrection(double dt, int id) const;
	void EnableDriftTimeQtCorrection(bool opt = true){ fIsDriftTimeQtCorrection = opt; }
	bool GetFiredStatus() const;

	virtual void Initialize();
protected:
	short fAnodeId;
	bool fIsDriftTimeQtCorrection; // if implement drift time Q-t correction.
	TAAnodePara *fPara;
	TAAnodeData *fData;
};

#endif
