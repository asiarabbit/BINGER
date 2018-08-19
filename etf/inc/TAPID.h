///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPID.h															 //
//   TAPID.h -- header file for class TAPID											 //
//   Introduction: derived from TAMagnet, particle identification using magnetic	 //
// rigidity analysis. An interface class from TAMagnet to user.	Again, this should	 //
// be a singleton class.															 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/23.															     //
// Last modified: 2018/6/11, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPID_H_
#define _TAPID_H_

#include "TAMagnet.h"

class TATOFWall;
class TAT0_1;

class TAPID : public TAMagnet{
public:
	enum OPTION{kOpt0, kOpt1, kOpt2, kOpt3, kOpt4};

	static TAPID *Instance();
	virtual ~TAPID();

	virtual void Configure();
	void SetMagneticIntensity(double B){ fExB = B; } // unit: Telsa
	// l: x = k1*z+b1; y = k2*z+b2; ki:(k1, k2); bi(b1, b2);
	// tof2: from target to TOFWall; p[0-3]: k1-k2-b1-b2;
	// particle transportation in the dipole magnet
	// kOpt0: nonuniform magnetic field low precision
	// kOpt1: nonuniform magnetic field high precision
	// kOpt2: uniform magnetic field analytic solution - point+trk
	// kOpt3: result of option 2 used as the start for iteration of option 1
	// kOpt4: uniform magnetic field analytic solution - trk+trk
	// pIn[0-3]: {k1_Ta, k2_Ta, b1_Ta, b2_Ta}
	// pOut[0-3]: {k1, k2, b1, b2}
	// pIn0_: trk info before target
	virtual void Fly(double tof2, double x0TaHit, const double *pOut, short dcArrId = 1, const OPTION option = kOpt1, const double *pIn = nullptr, const double *pIn0 = nullptr);
	using TAMagnet::FillGraphTrajectory;
	void FillGraphTrajectory() const;
	void SetCurveGraph(TGraph *gcurve){ fGCurve = gcurve; }
	bool IsFlied() const{ return fIsFlied; }
	double GetAoZ() const;
	double GetBeta() const;
	double GetGamma() const;
	double GetPoZ() const; // MeV/c
	double GetBrho() const; // Tesla*m
	double GetChi() const;
	void GetTargetExitAngle(double *a) const;
	double GetTotalTrackLength() const;
	virtual void Initialize();
protected:
	TAPID(const string &name = "PID-ETF", const string &title = "Magnet-Rigidity-Analysis");
	double GetExB() const; // return the magnetic instensity value input by user, i.e. fExB
	static TAPID *fInstance;
	double fExB; // external B value to scale to, at the geometrical center of the Mag, unit: Tesla
	TGraph *fGCurve; // distorted track in the diple magnet
	// the output PID results
	double fAoZ, fAoZdmin;
	double fBeta, fGamma, fPoZ, fBrho;
	double fAngleTaOut[2]; // [0-1]: [k1, k2] -> x=k1z+b1; y=k2z+b2
	double fTotalTrackLength;

	TATOFWall *fTOFWall[2]; // the TOF walls used in ETF facility [0-1]: [DCArrL-R]
	TAT0_1 *fT0_1; // TOF start detector just upstream of the target
	bool fIsFlied;
};

#endif

