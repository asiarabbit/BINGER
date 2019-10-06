/**
	\file TAMagnet.h
	\class TAMagnet
	\brief Magnetic rigidity analysis for PID using fourth-order Runge-Kutta
	method to track particles in magnetic field of the dipole magnet in ETF.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/10 Last revised: 2019/10/5, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMAGNET_H_
#define _TAMAGNET_H_

#include <vector>
#include "TAStuff.h"

using std::vector;

class TGraph2D;
class TGraph;

class TAMagnet : public TAStuff{
public:
	/// \struct tra_t
	/// \brief to trace a particle
	/// \note unit: mm for (x, y, z), m for rho and Tesla for B
	struct tra_t{
		double x; double y; double z; double rho; double brho;
		void show() const{
			cout << "x: " << x << "\ty: " << y << "\tz: " << z;
			cout << "\trho: " << rho << "\tbrho: " << brho << endl;
		}
	};


	TAMagnet(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMagnet();

	/// read the magnetic field measurement file, generated from measurements using Hall sensor.
	void LoadMagneticFieldFile(const string &file);
	/// transport ions in magnetic field. zf: final z coordinate\n
	/// note that z is the independent variable.\n
	/// fourth-order Runge-Kutta method is utilized to solve the differential equation. \n
	/// \param (y, yp): position coorinates and their first derivatives.\n
	/// \param isTracking: whether to store particle trajectories for showcase purposes
	void TransportIon(double *y, double *yp, double zi, double zf, bool isTracking = false);
	/// assign B with the magnetic intensity vector at position p
	void GetMagneticIntensity(double *B, const double *p);
	double GetTrackLength() const; // return fTrackLength

	void SetRKMethod(int selection = 1);
	void SetIterationStep(double h); // unit: mm
	void SetStepErrorTolerance(double e); // unit: mm
	double GetIterationStep() const{ return fh0; }
	double GetStepError() const{ return fStepError; }
	/// \param beta: particle velocity; aoz: mass/(atomic number)
	/// mass unit: atomic mass unit
	/// \note QoP = Z/A * e0/(mu*beta*gamma*c0)
	void SetQoP(double aoz, double beta, double V0 = -9999., double tof2 = -9999., double L = -9999., double L0 = -9999., double L2 = -9999.);
	double GetQoP() const{ return fQoP; }
	void SetScale(double scale){ fScale = scale; }
	const vector<tra_t>& GetTrackVec() const{ return fTrackVec; }
	void FillGraphTrajectory(TGraph2D *gcurve) const;
	void FillGraphRho(TGraph *grho) const;
	void FillGraphBrho(TGraph *gbrho) const;
	void EnableEnergyLoss(bool option = true){ fEnableEnergyLoss = option; }
	bool IsOutOfRange(){ return fOutOfRangeError; }
	void SetOutOfRangeError(bool opt){ fOutOfRangeError = opt; }
	/// real time QoP update along track length
	/// V0: particle velocity in RIBLL2, tof2: TOF after RIBLL2, L: track length after RIBLL2
	/// x: the current track length. unit: mm, ns
	void UpdateQoP(double x);
	virtual void Initialize(); // clear and initialize for next event
	/// two kinds of Runge-Kutta method
	static const int kClassic = 0;
	static const int kNystrom = 1;

protected:
	vector<tra_t> fTrackVec; ///< to trace the particle in the magnetic field: (x, y, z, rho)

private:
	/// 2nd order derivative: f0(x;y0,y1;yp0,yp1), f0(x;y0,y1;yp0,yp1) // y'=z; z'=g(x,y,z);
	/// substantially this is the relativistic dynamic equation itself, under Lorentz force.
	void f(double *ypp, const double x, const double *y, const double *yp);
	/// variable step length Runge-Kutta method for one iteration
	/// h0 is the initial step length of the iteration
	void RKvl(double &x, double *y, double *yp, const double h0);
	/// Runge-Kutta method: fourth order, two dimensional, second order derivative
	void RK(double *yNew, double *ypNew, const double x, const double *y, const double *yp, const double h);
	/// the so-called Nystrom fourth order Runge-Kutta method
	/// quotated from Data Analysis Techniques for High-Energy Physics.
	void RK_Nys(double *yNew, double *ypNew, const double x, const double *y, const double *yp, const double h);
	static double trilinear(const double *f, const double *dp); // trilinear interpolation

	/// the magnetic field
	/// this is the one eighth of the whole magnetic field for the dipole magnet.
	/// x: -1000, 0; y: 0, 90; z: 0, 1050         10mm spacing
	double fB[3][101][10][106]; ///< [BxByBz][x][y][z]
	bool fOutOfRangeError; ///< out of the active volume of the magnet
	double fScale; ///< scale coefficient to deduce B at different currents.
	double fh0; ///< the initial iteration step length for RK method  unit: mm. Yes, it is mm, not m.
	double fStepError; ///< error tolerance per step, unit: mm
	double fQoP; ///< qop: q/p, electric charge over momentum, in SI unit.

	/// member varialbe for dE considerations
	double fAoZ; ///< A/Z. A: in amu.
	double fV0; ///< V0: particle velocity in RIBLL2
	double fTOF2; ///< TOF corresponding to L
	double fL; ///< total track length
	double fL0; ///< from TOF stop to MWPC2
	double fL1; ///< from MWPC2 to the exit of the magnetic field
	double fL2; ///< from the exit of the magnetic field to the TOF Wall

	bool fIsFileLoaded; ///< to mark if the magnetic field file is loaded.
	int fRKMethod; ///< classic or Nystrom
	double fTrackLength; ///< as the name indicates
	bool fEnableEnergyLoss; ///< if calculating energy loss along the particle tracjectory.
}; // end of class TAMagnet

#endif


