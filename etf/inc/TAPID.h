/**
	\file TAPID.h
	\class TAPID
	\brief Derived from TAMagnet, particle identification using magnetic
	rigidity analysis. An interface class from TAMagnet to user. Again, this should
	be a singleton class.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/23 Last revised: 2018/9/13, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPID_H_
#define _TAPID_H_

#include "TAMagnet.h"

class TATOFWall;
class TAT0_1;

class TAPID : public TAMagnet{
public:
	/// \enum OPTION
	/// kOpt0: nonuniform magnetic field low precision\n
	/// kOpt1: nonuniform magnetic field high precision\n
	/// kOpt2: uniform magnetic field analytic solution - point+trk\n
	/// kOpt3: result of option 2 used as the start for iteration of option 1\n
	/// kOpt4: uniform magnetic field analytic solution - trk+trk
	enum OPTION{kOpt0, kOpt1, kOpt2, kOpt3, kOpt4};

	static TAPID *Instance();
	virtual ~TAPID();

	virtual void Configure();
	void SetMagneticIntensity(double B){ fExB = B; } ///< unit: Telsa
	double GetExtMag() const{ return fExB; } ///< unit: Telsa
	/// l: x = k1*z+b1; y = k2*z+b2; ki:(k1, k2); bi:(b1, b2);
	/// \param tof2: from target to TOFWall;
	/// \param p[0-3]: k1-k2-b1-b2;
	/// particle transportation in the dipole magnet
	/// \param pIn[0-3]: {k1_Ta, k2_Ta, b1_Ta, b2_Ta}
	/// \param pOut[0-3]: {k1, k2, b1, b2}
	/// \param pIn0_: trk info before target
	virtual void Fly(double tof2, double x0TaHit, const double *pOut, short dcArrId = 1, const OPTION option = kOpt1, const double *pIn = nullptr, const double *pIn0 = nullptr);
	/// Fly function dedicated for pion experiment
	virtual void FlyPion(double tof2, double x0TaHit, const double *pOut, short dcArrId = 1, const OPTION option = kOpt1);
	using TAMagnet::FillGraphTrajectory;
	void FillGraphTrajectory() const;
	void SetCurveGraph(TGraph *gcurve){ fGCurve = gcurve; }
	bool IsFlied() const{ return fIsFlied; }
	double GetAoZ() const;
	double GetBeta() const;
	double GetGamma() const;
	double GetPoZ() const; ///< MeV/c
	double GetBrho() const; ///< Tesla*m
	double GetChi() const;
	void GetTargetExitAngle(double *a) const;
	double GetTotalTrackLength() const;
	void GetX2Arr(double *x2) const;
	virtual void Initialize();
protected:
	TAPID(const string &name = "PID-ETF", const string &title = "Magnet-Rigidity-Analysis");
	double GetExB() const; ///< \return the magnetic instensity value input by user, i.e. fExB
	static TAPID *fInstance;
	double fExB; ///< external B value to scale to, at the geometrical center of the Mag, unit: Tesla
	TGraph *fGCurve; ///< distorted track in the diple magnet
	/// the output PID results
	double fAoZ, fAoZdmin;
	double fBeta, fGamma, fPoZ, fBrho;
	double fAngleTaOut[2]; ///< [0-1]: [k1, k2] -> x=k1z+b1; y=k2z+b2
	double fTotalTrackLength;
	/// X2: abscissa of Mag exit point from real trk and calculated arc,
	/// as a PID estimator; 0-1: real-calcu. Note that this is only for kOpt4 method
	double fX2Arr[2];

	TATOFWall *fTOFWall[2]; ///< the TOF walls used in ETF facility [0-1]: [DCArrL-R]
	TAT0_1 *fT0_1; ///< TOF start detector just upstream of the target
	bool fIsFlied;
};

#endif

