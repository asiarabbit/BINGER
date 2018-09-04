///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TATrack4.h														 //
//   TATrack4.h -- header file for class TATrack4									 //
//   Introduction: A derived class from TATrack, specifically designed for DC array  //
// fabracated by MA. This class is mainly for track fitting of those DCs.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/9/3.															     //
// Last modified: 2018/9/3, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TATRACK4_H_
#define _TATRACK4_H_

#include "TATrack.h"

class TATrack4 : public TATrack{
public:
	TATrack4(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TATrack4();

	virtual double GetDsquare();
	virtual void GetNu(int *nu) const;
	virtual void GetLAYER(int *LAYER) const;
	virtual void GetZ(double *z) const;
	virtual void GetX(double *x) const;
	virtual void GetDriftTime(double *t) const;
	virtual void GetDriftDistance(double *r) const;
	virtual void GetChi(double *chi);
	using TATrack::GetChi;
	virtual void SetData(const double *x, const double *z, const double *t, const double *r, double kL,
		 double bL, double dsquare, int gGOOD, const int *nu, const int *LAYER, const double *weight);
	virtual void SetDriftTime(const double *t, const double *w);
	virtual void SetTOF(double TOF, int firedStripId, double nstripsStray);
	virtual void SetDriftDistance(const double *r);
	virtual void AssignTrack(tTrack *ptrack);
	virtual void Fit();
	virtual void BFGSFit();
	virtual void IterFit();
	virtual void FillTrack(TGraph *gTrack, TGraph *gTrack_R);
	virtual void Initialize();
protected:
};

#endif

