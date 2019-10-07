/**
	\file TAMath.h
	\class TAMath
	\brief Offering a mathematical toolkit for math problems in the data analysis.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/25 Last revised: 2018/9/13, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMATH_H_
#define _TAMATH_H_

#include <cstring>
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

#include "TADeployPara.h"
#include "TAPopMsg.h"

class TAMath{
public:
	TAMath(){};
	~TAMath(){};

	static constexpr double Pi(){ return 3.14159265358979323846; }
	static constexpr double Alpha(){ return 0.61803398874989484820458683436565; } ///< golden cut ratio
	static constexpr double DEGREE(){ return 0.01745329251994329547; } ///< rad per degree
	static constexpr double Sqrt3(){ return 1.73205080756887719318; }

	/// \note r_global = dR.(R.r_local)+r0+dr
	static void rotate(const double *pIn, double *pOut, const double *angle);
	static void rotateOffset(const double *pIn, double *pOut, const double *angOff);
	static double norm(const double *p, int len = 3); ///< length of the vector, len: vector dimension
	static double L(const double *p0, const double *p1, int len = 3); ///< |p0-p1|
	static double VecAng3D(const double *a, const double *b); ///< angle between two 3D vectors <a, b>
	/// well, this is an anyhow specialised function written specifically to calculate alpha-angle
	/// as usual, a, b is dire-vec of trk and anode, v.s. angAxis is the dire-vec of X(xuv)-axis
	/// the ratiocination process is written in math/3DTrkProjVecWhole.m
	static double AlphaAngle3D(const double *b, const double *ag, const double *agAxis);

	static double acceptance(const double *p0, const double *p1);
	// ///////////////////// TAMWDCARRAY 3D TRACK PROJRECTION TRANSFORMATION ///////////////////////
	/// 3D track projections transformation, functions serving TAMWDCArray tracking.
	/// U+V->X tranformation: l: x=kz+b: slope
	static double kUV_X(double phi, double ku, double kv);
	/// U+V->X tranformation: l: x=kz+b: intercept
	static double bUV_X(double phi, double ku, double kv, double bu, double bv);
	/// U+V->Y tranformation: l: y=kz+b: slope
	static double kUV_Y(double phi, double ku, double kv);
	/// U+V->Y tranformation: l: y=kz+b: intercept
	static double bUV_Y(double phi, double ku, double kv, double bu, double bv);
	/// X+Y->U tranformation: l: xu=kzu+bu: slope
	static double kXY_U(double phi, double k1, double k2);
	/// X+Y->U tranformation: l: xu=kzu+bu: intercept
	static double bXY_U(double phi, double k1, double k2, double b1, double b2);
	/// X+Y->V tranformation: l: yv=kzv+bv: slope
	static double kXY_V(double phi, double k1, double k2);
	/// X+Y->V tranformation: l: yv=kzv+bv: intercept
	static double bXY_V(double phi, double k1, double k2, double b1, double b2);
	/// the closest point of two skew lines -> hitpA: hit point on lA: (a, A)
	/// B, b: track point and track vector; A, a: anode point and track vector;
	static void GetHitPoint(const double *b, const double *B, const double *a, const double *A, double *hitpA);
	/// solve particle trajectory in uniform magnetic field, with only Mag boundry, exit track
	/// and target position known; returning the track radius of curvature in the magnetic field\n
	/// input unit: mm; output unit: mm\n
	/// x=ki z+bi, track after Target, before Mag; (zo, xo): rotating center of the arc\n
	/// \retval [0-5]: [thetaDeflect, rho, ki, bi, zo, xo]
	static void UniformMagneticSolution(double k1, double b1, double zMagOut, double zMagIn, double zTa, double xTa, double *result);
	/// analytic PID method using DCTaArr
	static double rho(double kin, double bin, double kout, double bout, double *zo = nullptr, double *xo = nullptr, double *x2Arr = nullptr);
	static double Gamma(double beta);
	static double BetaGamma(double beta){ return Gamma(beta) * beta; }
	static double GammaBeta(double beta){ return BetaGamma(beta); }
	// generic programming :)
	/// \retval: sum of square of each one in the parameter list
	static double Sum2(){ return 0.; }
	template <typename T, typename ...Args> // typename preferred, class deprecated
	static T Sum2(const T &v, const Args &...args){
		return v*v + Sum2(args...);
	}
	template <typename T>
	static bool Within(const T &v, const T &A, const T &B){
		if(v >= A && v <= B) return true;
		if(v >= B && v <= A) return true;
		return false;
	}

	// /////////////////// TATRACK MINIMIZATION & LSM FUNCTIONS /////////////////////////////

	/// generic programming ///
	/// \todo BFGS minimization algorithm
	/// \param fun: the objective function
	template <typename FUNCTOR>
	static double BFGS4(FUNCTOR &fun);
	/// \todo derivarives of fun at xk[4], stored in gk[4]
	/// \param fun: the objective function
	template <typename FUNCTOR>
	static void funxy4(FUNCTOR &fun, const double *xk, double *gk);
	static double DxTa(const double k0, const double k1, const double b0, const double b1){
		static const double taZ = TADeployPara::Instance()->GetTargetZ0();
		return (k0 * taZ + b0) - (k1 * taZ + b1);
	}
	static double DxTa(const double *k, const double *b){
		return DxTa(k[0], k[1], b[0], b[1]);
	}
	static double DX2(double k1, double k2, double b1, double b2){
		double x2[2];
		TAMath::rho(k1, b1, k2, b2, nullptr, nullptr, x2); // assign x2 array
		double dx2 = x2[0] - x2[1];
		return dx2;
	}

	// return dxTa^2 + dx2^2
	static double Dx2DxTa_2(double *k, double *b){
		return Dx2DxTa_2(k[0], k[1], k[2], b[0], b[1], b[2]);
	}
	static double Dx2DxTa_2(double k0, double k1, double k2, double b0, double b1, double b2){
		double dxTa = DxTa(k0, k1, b0, b1);
		double dx2 = DX2(k1, k2, b1, b2);
		return
			dxTa*dxTa * 1. / kVdxTa + // 1.72 the relative variance V/sigma_DC
			dx2*dx2 * 1. / kVdx2; // 7.5
	}


	/// the objective function for BFGS minimization algorithm
	class Fun{
	public:
		Fun(){}
		virtual ~Fun(){}
		virtual double operator()(const double *xk) const = 0;
		/// update fp with xkm
		virtual void UpdatePar(const double *xkm) = 0;
	};
	/// nested class for definition of BFGS-es with various argument lists
	/// calculate chi2 of a track, evenly weighted
	/// Ag[nF][3], ag[nF][3], p[4], r[nF]
	/// xk[4]: the real variables of the functino fun, correction to p[4]
	class Chi3D : public Fun{
	public:
		Chi3D(const double Ag[][3], const double ag[][3], double *p, const double *r, const int nF) : fp(p), fAg(Ag), fag(ag), fr(r), fnF(nF){
			if(!p){
				TAPopMsg::Error("TAMath::Chi3D", "constructor: the input par arr is nullptr!");
			}
		} // end of the constructor
		~Chi3D(){}
		double operator()(const double *xk) const override{
			double dr[fnF]{}, pp[4]{}, d2 = 0.;
			pp[0] = fp[0] * (1. + xk[0]); pp[1] = fp[1] * (1. + xk[1]); // k1, k2
			pp[2] = fp[2] * (1. + xk[2]); pp[3] = fp[3] * (1. + xk[3]); // b1, b2
			for(int i = 0; i < fnF; i++){
				dr[i] = TAMath::dSkew(fAg[i], fag[i], pp) - fr[i];
				d2 += dr[i] * dr[i];
			} // end for over i
			TAMath::kCallCnt++;
			return d2;
		} // end of function operator()(doubel *xk)
		virtual void UpdatePar(const double *xkm) override{
			for(int i = 0; i < 4; i++) fp[i] *= 1. + xkm[i];
		}
	private:
		double *fp; ///< the track parameter set
		const double (*fAg)[3]; ///< one point in the anode
		const double (*fag)[3]; ///< the anode orientation array
		const double *fr; ///< the drift distance array
		const int fnF; ///< number of fired andoes all over the 3-D track
	}; // end of declaration of nested class Chi3D
	// \param Ag: a point in anode; \param ag: the orientation of an anode;
	/// for 3D linear tracking
	static double BFGS4(const double Ag[][3], const double ag[][3], double *p, const double *r, const int nF){
		Chi3D fun(Ag, ag, p, r, nF);
		return BFGS4(fun); // template BFGS function
	} // end of member function BFGS for 3-D tracking
	
	/// \todo for the amelioration of the splined pre- and post-Ta trk
	class ChiTa4 : public Fun{
	public:
		/// \param z,x,r[4+4]: 2-D pos of fired anodes and their drift distances
		/// \param k,b[3]: pars of [0-1-2]->[pre-postTa-postMag] Xproj
		/// \param LAYER[2]: fired andoe layer id of [0-1]->[pre-postTa] trk
		/// \param gGOOD[2]: fired andoe layer distribution type
		/// \param d2ThrePD: LSM chi square threshold per dot, irrespective of r
		ChiTa4(const double *z, const double *x, const double *r, double *k, double *b, int *gGOOD, const int (*LAYER)[6], double d2ThrePD)
			: fZ(z), fX(x), fR(r), fK(k), fB(b),
			fgGOOD(gGOOD), fLAYER(LAYER), fd2ThrePD(d2ThrePD){}
		~ChiTa4(){}
		/// \retval: return the fitting chi2 sum
		double operator()(const double *xk) const override{
			// update parameters
			double k[3] = {-9999., -9999., fK[2]};
			double b[3] = {-9999., -9999., fB[2]};
			k[0] = fK[0] * (1. + xk[0]); k[1] = fK[1] * (1. + xk[1]); // k0, k1
			b[0] = fB[0] * (1. + xk[2]); b[1] = fB[1] * (1. + xk[3]); // b0, b1

			kCallCnt++;
			return
		TAMath::minid2(fZ, fX, fR, k[0], b[0], fgGOOD[0], fLAYER[0]) + // preTaTrk
		TAMath::minid2(fZ+6, fX+6, fR+6, k[1], b[1], fgGOOD[1], fLAYER[1]) + // post Ta trk
		TAMath::Dx2DxTa_2(k, b); // XXX the core of the new tracking algorithm
		} // end of the member function operator()(const double *xk)

		// xkm[0-1-2-3]: [k0, k1, b0, b1]
		virtual void UpdatePar(const double *xkm) override{
//			cout << "Before update, k, b:\n"; // DEBUG
//			cout << fK[0] << "\t" << fK[1] << endl; // DEBUG
//			cout << fB[0] << "\t" << fB[1] << endl; // DEBUG
			fK[0] *= 1. + xkm[0]; fB[0] *= 1. + xkm[2]; // preTaTrk: x=k0z+b0
			fK[1] *= 1. + xkm[1]; fB[1] *= 1. + xkm[3]; // postTaTrk: x=k1z+b1
//			cout << "After update, k, b:\n"; // DEBUG
//			cout << fK[0] << "\t" << fK[1] << endl; // DEBUG
//			cout << fB[0] << "\t" << fB[1] << endl; // DEBUG
//			getchar(); // DEBUG
		}
	private:
		const double *fZ, *fX, *fR;
		double *fK, *fB;
		const int *fgGOOD, (*fLAYER)[6];
		double fd2ThrePD;
	}; // end of declaration of nested class ChiTa4
	static double BFGS4(const double *z, const double *x, const double *r, double *k, double *b, int *gGOOD, const int (*LAYER)[6], double d2ThrePD){
		ChiTa4 fun(z, x, r, k, b, gGOOD, LAYER, d2ThrePD);
		return BFGS4(fun);
	} // end of member function for splined tracking around TaZone
	/// minimization using iterative fit method, 
	// exhaust possible drift distance signs to minimize fitting chi2
	/// \param k2: slope of postMagTrk; \param b2: intercept of postMagTrk
	/// \param z2: +z border of the equivalent magField
	static void IterFit4(const double *z, const double *x, const double *r, double *k, double *b, int *gGOOD, const int (*LAYER)[6], double d2ThrePD);


	/// minimization and Least Squares Method functions, serving TATrack track fitting
	/// the least square fit of fired andoes
	static double Dsquare(const double *x, const double *y, double &kL, double &bL, int gGOOD, const int *LAYER, double DsqThrePerDot); ///< a global function
	/// calculate chi of line(k, b) to drift distance set (x, y, r).
	static double minid2(const double *x, const double *y, const double *r, double k, double b, int gGOOD, const int *LAYER); ///< a global function
	/// distance between two skew lines
	/// \param Ag, B: a point on the line;
	/// \param ag, b: line vector;
	/// \param p[4] = {k1, k2, b1, b2};
	/// \param r: drift distance;
	/// \param nF: number of fired sense wire layers
	static double dSkew(const double *ag, const double *Ag, const double *b, const double *B);
	static double dSkew(const double *ag, const double *Ag, const double *p);

	/// direct search method
	static double refinedFit(const double *z, const double *x, const double *r, double &k, double &b, int gGOOD, const int *LAYER, int isTest, double x0, double y0, double d2ThrePerDot, bool isDEBUG = false);
	/// fit using BFGS minimization algorithm
	static double refinedFitBFGS(const double *z, const double *x, const double *r, double &k, double &b, int gGOOD, const int *LAYER, double d2ThrePerDot);
	static double iterativeFit(const double *z, const double *x, const double *r, double &k, double &b, int gGOOD, const int *LAYER, double d2ThrePerDot);

	static const double kzMagIn, kzMagOut;
	static unsigned long long kCallCnt; ///< count of calls of operator()(double *xk)
	static const double kVdxTa, kVdx2; ///< the relative variance: V / sigma_DC
};

#include "TAMath.icc" // definitions of member template function

#endif
