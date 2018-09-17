///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath.h															 //
//   TAMath.h -- header file for class TAMath										 //
//   Introduction: offering a mathematical toolkit for math problems in the data	 //
// analysis.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/25.															     //
// Last modified: 2018/9/13, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMATH_H_
#define _TAMATH_H_

class TAMath{
public:
	TAMath(){};
	~TAMath(){};

	// mathematic constants
	static constexpr double Pi(){ return 3.14159265358979323846; }
	static constexpr double Alpha(){ return 0.61803398874989484820458683436565; } // golden cut ratio
	static constexpr double DEGREE(){ return 0.01745329251994329547; } // rad per degree
	static constexpr double Sqrt3(){ return 1.73205080756887719318; }

	// r_global = dR.(R.r_local)+r0+dr
	static void rotate(const double *pIn, double *pOut, const double *angle);
	static void rotateOffset(const double *pIn, double *pOut, const double *angOff);
	static double norm(const double *p, int len = 3); // length of the vector, len: vector dimension
	static double L(const double *p0, const double *p1, int len = 3); // |p0-p1|
	static double VecAng3D(const double *a, const double *b); // angle between two 3D vectors <a, b>
	// well, this is an anyhow specialised function written specifically to calculate alpha-angle
	// as usual, a, b is dire-vec of trk and anode, v.s. angAxis is the dire-vec of X(xuv)-axis
	// the ratiocination process is written in math/3DTrkProjVecWhole.m
	static double AlphaAngle3D(const double *b, const double *ag, const double *agAxis);

	static double acceptance(const double *p0, const double *p1);
	/////////////////////// TAMWDCARRAY 3D TRACK PROJRECTION TRANSFORMATION ///////////////////////
	// 3D track projections transformation, functions serving TAMWDCArray tracking.
	// U+V->X tranformation: l: x=kz+b: slope
	static double kUV_X(double phi, double ku, double kv);
	// U+V->X tranformation: l: x=kz+b: intercept
	static double bUV_X(double phi, double ku, double kv, double bu, double bv);
	// U+V->Y tranformation: l: y=kz+b: slope
	static double kUV_Y(double phi, double ku, double kv);
	// U+V->Y tranformation: l: y=kz+b: intercept
	static double bUV_Y(double phi, double ku, double kv, double bu, double bv);
	// X+Y->U tranformation: l: xu=kzu+bu: slope
	static double kXY_U(double phi, double k1, double k2);
	// X+Y->U tranformation: l: xu=kzu+bu: intercept
	static double bXY_U(double phi, double k1, double k2, double b1, double b2);
	// X+Y->V tranformation: l: yv=kzv+bv: slope
	static double kXY_V(double phi, double k1, double k2);
	// X+Y->V tranformation: l: yv=kzv+bv: intercept
	static double bXY_V(double phi, double k1, double k2, double b1, double b2);
	// the closest point of two skew lines -> hitpA: hit point on lA: (a, A)
	// B, b: track point and track vector; A, a: anode point and track vector;
	static void GetHitPoint(const double *b, const double *B, const double *a, const double *A, double *hitpA);
	// solve particle trajectory in uniform magnetic field, with only Mag boundry, exit track
	// and target position known; returning the track radius of curvature in the magnetic field
	// input unit: mm; output unit: mm
	// x=ki z+bi, track after Target, before Mag; (zo, xo): rotating center of the arc
	// result: [0-5]: [thetaDeflect, rho, ki, bi, zo, xo]
	static void UniformMagneticSolution(double k1, double b1, double zMagOut, double zMagIn, double zTa, double xTa, double *result);
	// analytic PID method using DCTaArr
	static double rho(double kin, double bin, double kout, double bout, double *zo = nullptr, double *xo = nullptr, double *x2Arr = nullptr);
	static double Gamma(double beta);
	static double BetaGamma(double beta){ return Gamma(beta) * beta; }
	static double GammaBeta(double beta){ return BetaGamma(beta); }

	///////////////////// TATRACK MINIMIZATION & LSM FUNCTIONS /////////////////////////////
	// minimization and Least Squares Method functions, serving TATrack track fitting
	// the least square fit of fired andoes
	static double Dsquare(const double *x, const double *y, double &kL, double &bL, int gGOOD, const int *LAYER, double DsqThrePerDot); // a global function
	// calculate chi of line(k, b) to drift distance set (x, y, r).
	static double minid2(const double *x, const double *y, const double *r, double k, double b, int gGOOD, const int *LAYER); // a global function
	// distance between two skew lines
	// Ag, B: a point on the line; ag, b: line vector; p[4] = {k1, k2, b1, b2};
	// r: drift distance; nF: number of fired sense wire layers
	static double dSkew(const double *ag, const double *Ag, const double *b, const double *B);
	static double dSkew(const double *ag, const double *Ag, const double *p);
	static double BFGS4(const double Ag[][3], const double ag[][3], double *p, const double *r, const int nF); // for 3D linear tracking
	// direct search method
	static double refinedFit(const double *z, const double *x, const double *r, double &k, double &b, int gGOOD, const int *LAYER, int isTest, double x0, double y0, double d2ThrePerDot, bool isDEBUG = false);
	// fit using BFGS minimization algorithm
	static double refinedFitBFGS(const double *z, const double *x, const double *r, double &k, double &b, int gGOOD, const int *LAYER, double d2ThrePerDot);
	static double iterativeFit(const double *z, const double *x, const double *r, double &k, double &b, int gGOOD, const int *LAYER, double d2ThrePerDot);
	static const double kzMagIn, kzMagOut;
};

#endif
