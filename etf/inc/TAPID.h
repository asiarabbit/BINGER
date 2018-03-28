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
// Last modified: 2018/1/25, SUN Yazhou.										     //
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
	static TAPID *Instance();
	virtual ~TAPID();

	virtual void Configure();
	void SetMagneticIntensity(double B){ fExB = B; } // unit: Telsa
	// l: x = k1*z+b1; y = k2*z+b2; ki:(k1, k2); bi(b1, b2);
	// tof2: from target to TOFWall; p[0-3]: k1-k2-b1-b2;
	// particle transportation in the dipole magnet
	// option: 0: nonuniform magnetic field low precision
	// option: 1: nonuniform magnetic field high precision
	// option: 2: uniform magnetic field analytic solution
	virtual void Fly(double tof2, double x0TaHit, const double *pOut, short dcArrId = 1, const int option = 1, const double *pIn = nullptr);
	using TAMagnet::FillGraphTrajectory;
	void FillGraphTrajectory() const;
	void SetCurveGraph(TGraph *gcurve){ fGCurve = gcurve; }
	bool IsFlied() const{ return fIsFlied; }
	double GetAoZ() const;
	double GetBeta() const;
	double GetGamma() const;
	double GetPoZ() const; // MeV/c
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
	double fBeta, fGamma, fPoZ;
	double fAngleTaOut[2]; // [0-1]: [k1, k2] -> x=k1z+b1; y=k2z+b2
	double fTotalTrackLength;

	bool fIsFlied;
	TATOFWall *fTOFWall[2]; // the TOF walls used in ETF facility [0-1]: [DCArrL-R]
	TAT0_1 *fT0_1; // TOF start detector just upstream of the target
};

#endif

