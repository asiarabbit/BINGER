///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath.C															 //
//   TAMath.C -- source file for class TAMath										 //
//   Introduction: offering a mathematical toolkit for math problems in the data	 //
// analysis.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/25.															     //
// Last modified: 2018/6/20, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


#define DEBUG

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <complex> //complex number operation
#include "TAMath.h"
#include "TAPopMsg.h"
#include "TACtrlPara.h"

using std::cout;
using std::endl;
using std::setprecision;
using std::complex;
typedef complex<double> cdouble;

static TACtrlPara *clp = TACtrlPara::Instance();
const double TAMath::kzMagIn = -475.;
const double TAMath::kzMagOut = 475.;

// length of the vector, len: vector dimension
double TAMath::norm(const double *p, int len){
	double norm = 0.;
	for(int i = 0; i < len; i++){
		norm += p[i]*p[i];
	}
	return sqrt(norm);
}
// |p0-p1|, len=3
double TAMath::L(const double *p0, const double *p1, int len){
	const int n = 10;
	double p[n]{};
	if(len > n)
		TAPopMsg::Error("TAMath", "L: array length out of range - len: %d (should be no more than %d)", len, n);
	
	for(int i = len; i--;) p[i] = p0[i] - p1[i];
	return norm(p, len);
} // end of member function L
// angle between two 3D vectors <a, b>
double TAMath::VecAng3D(const double *a, const double *b){
	// cosA = a*b / (|a||b|)
	return acos( (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]) / (norm(a,3)*norm(b,3)) );
}
// well, this is an anyhow specialised function written specifically to calculate alpha-angle
// as usual, a, b is dire-vec of trk and anode, v.s. angAxis is the dire-vec of X(xuv)-axis
// the ratiocination process is written in misc/tools/math/3DTrkProjVecWhole.m
double TAMath::AlphaAngle3D(const double *b, const double *ag, const double *agAxis){
	const double k1 = b[0]/b[2], k2 = b[1]/b[2]; // {b0,b1,b2} => {k1, k2, 1}
	const double bpr[3] = { // b of track projection => bpr
		(ag[1]*ag[1]+ag[2]*ag[2])*k1 - ag[0]*(ag[2]+ag[1]*k2),
		(ag[0]*ag[0]+ag[2]*ag[2])*k2 - ag[1]*(ag[2]+ag[0]*k1),
		(ag[0]      -ag[2]*k1)*ag[0] + ag[1]*(ag[1]-ag[2]*k2)
	};
//	cout << "ag[0]: " << ag[0] << "\tag[1]: " << ag[1]; // DEBUG
//	cout << "\tag[2]: " << ag[2] << endl; // DEBUG
//	cout << "b[0]: " << b[0] << "\tb[1]: " << b[1]; // DEBUG
//	cout << "\tb[2]: " << b[2] << endl; // DEBUG
//	cout << "agAxis[0]: " << agAxis[0] << "\tagAxis[1]: " << agAxis[1]; // DEBUG
//	cout << "\tagAxis[2]: " << agAxis[2] << endl; // DEBUG
//	getchar(); // DEBUG
//	cout << "bpr[0]: " << bpr[0] << "\tbpr[1]: " << bpr[1]; // DEBUG
//	cout << "\tbpr[2]: " << bpr[2] << endl; // DEBUG
//	cout << "|bpr|: " << norm(bpr) << endl; // DEBUG
	return Pi() / 2. - VecAng3D(bpr, agAxis);
}

// the triangle defined by (p0, p1, O). return the angle O: coordinate origin
double TAMath::acceptance(const double *p0, const double *p1){
	double innerProduct = p0[0]*p1[0] + p0[1]*p1[1];
	double cosTheta = innerProduct/(norm(p0,2)*norm(p1,2));
	return acos(cosTheta);
}
// r_global = R.r_local
// angle0: yaw, angle1: pitch, angle2: roll, intrinsic rotation; (y-x'-z")
// R(yaw, pitch, roll) = Ry(yaw).Rx'(pitch).Rz"(roll)
void TAMath::rotate(const double *pIn, double *pOut, const double *angIn){
	double s1 = sin(angIn[0]), s2 = sin(angIn[1]), s3 = sin(angIn[2]);
	double c1 = cos(angIn[0]), c2 = cos(angIn[1]), c3 = cos(angIn[2]);

	pOut[0] = (c1*c3+s1*s2*s3)*  pIn[0] +(c3*s1*s2-c1*s3)* pIn[1] +c2*s1* pIn[2];
	pOut[1] = c2*s3*             pIn[0] +c2*c3*            pIn[1] -s2*    pIn[2];
	pOut[2] = (c1*s2*s3-c3*s1)*  pIn[0] +(c1*c3*s2+s1*s3)* pIn[1] +c1*c2* pIn[2];
}
void TAMath::rotateOffset(const double *pIn, double *pOut, const double *angOff){
	pOut[0] = pIn[0] -angOff[0]* pIn[1] +angOff[2]* pIn[2]; // x
	pOut[1] = angOff[0]* pIn[0] + pIn[1] -angOff[1]* pIn[2]; // y
	pOut[2] = -angOff[2]* pIn[0] +angOff[1]* pIn[1] + pIn[2]; // z
}

/////// functions serving TAMWDCArray tracking //////
// Oxuyuzu = Rz[-theta].Ry[-phi].Oxyz: detector is along the zu axis and U wire is parallel to y axis
// Similar rotation has been implemented for V tracking
// U+V->X tranformation: lx: x=kz+b: slope
double TAMath::kUV_X(double phi, double ku, double kv){
	return ((ku+kv)*cos(phi)+Sqrt3()*sin(phi))/(Sqrt3()*cos(phi)-(ku+kv)*sin(phi));
}
// U+V->X tranformation: lx: x=kz+b: intercept
double TAMath::bUV_X(double phi, double ku, double kv, double bu, double bv){
	return (bu+bv)/(Sqrt3()*cos(phi)-(ku+kv)*sin(phi));
}
// U+V->Y tranformation: ly: y=kz+b: slope
double TAMath::kUV_Y(double phi, double ku, double kv){
	return Sqrt3()*(-ku+kv)/(Sqrt3()*cos(phi)-(ku+kv)*sin(phi));
}
// U+V->Y tranformation: ly: y=kz+b: intercept
double TAMath::bUV_Y(double phi, double ku, double kv, double bu, double bv){
	return (Sqrt3()*(-bu+bv)*cos(phi)+2.*(-bv*ku+bu*kv)*sin(phi))/(Sqrt3()*cos(phi)-(ku+kv)*sin(phi));
}
// X+Y->U tranformation: lu: xu=k zu+bu: slope
double TAMath::kXY_U(double phi, double k1, double k2){
	return (-k2+Sqrt3()*(k1*cos(phi)-sin(phi)))/(2.*(cos(phi)+k1*sin(phi)));
}
// X+Y->U tranformation: lu: xu=k zu+bu: intercept
double TAMath::bXY_U(double phi, double k1, double k2, double b1, double b2){
	return 1./2.*(-b2+b1*(Sqrt3()+k2*sin(phi))/(cos(phi)+k1*sin(phi)));
}
// X+Y->V tranformation: lv: yv=k zv+bv: slope
double TAMath::kXY_V(double phi, double k1, double k2){
	return (k2+Sqrt3()*(k1*cos(phi)-sin(phi)))/(2.*(cos(phi)+k1*sin(phi)));
}
// X+Y->V tranformation: lv: yv=k zv+bv: intercept
double TAMath::bXY_V(double phi, double k1, double k2, double b1, double b2){
	return 1./2.*(b2+b1*(Sqrt3()-k2*sin(phi))/(cos(phi)+k1*sin(phi)));
}

// the closest point of two skew lines -> hitp, note that hip is in the (a, A) line
// B, b: track point and track vector; A, a: anode point and track vector
void TAMath::GetHitPoint(const double *b, const double *B, const double *a, const double *A, double *hitpA){
	double aa = a[0]*a[0]+a[1]*a[1]+a[2]*a[2], bb = b[0]*b[0]+b[1]*b[1]+b[2]*b[2],
	       ab = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	double AB[3] = {A[0]-B[0], A[1]-B[1], A[2]-B[2]};
	double tt1 = bb*(a[0]*AB[0] + a[1]*AB[1] + a[2]*AB[2])
		     -ab*(b[0]*AB[0] + b[1]*AB[1] + b[2]*AB[2]);
	tt1 /= ab*ab - aa*bb;
	hitpA[0] = a[0]*tt1+A[0];
	hitpA[1] = a[1]*tt1+A[1];
	hitpA[2] = a[2]*tt1+A[2];
} // end of function GetHitPoint

// solve particle trajectory in uniform magnetic field, with only Mag boundry, exit track
// and target position known; returning the track radius of curvature in the magnetic field
// input unit: mm; output unit: mm
// x=kiz+bi, track before Target
// result: [0-5]: [thetaDeflect, rho, ki, bi, zo, xo]
// zMagOut->z2; zMagIn->z1; zTa->z0; xTa->x0
struct tSolve{
	double x1, ki, bi, zh, xh, dtheta, dd2, rho, zo, xo; // solution struct
};
void TAMath::UniformMagneticSolution(double k1, double b1, double z2, double z1, double z0, double x0, double *result){
	double x2 = k1*z2+b1;
	tSolve s[3]; // the equation is of 4-th order, having 4 solutions, one is dropped
	memset(s, 0, sizeof(s));
	#include "TAMath/uniformMagneticSolution.C" // solve x1, i.e. xMagIn
	// solution validity check
	int validCnt = 0;
	for(const tSolve &t : s) if(-9999. != t.x1) validCnt++;
	if(1 != validCnt){
		for(int i = 0; i < 6; i++) result[i] = validCnt;
		return;
	} // end if(1 != validCnt)
	for(const tSolve &t : s) if(-9999. != t.x1){
		// output the result
		result[0] = t.dtheta; result[1] = t.rho;
		result[2] = t.ki; result[3] = t.bi;
		result[4] = t.zo; result[5] = t.xo;
		return;
	}
} // end of function UniformMagneticSolution

double TAMath::Gamma(double beta){
	if(beta < 0. || beta >= 1.){
		TAPopMsg::Error("TAMath", "Gamma: input beta invalid: %f", beta);
		return -1.;
	}
	return 1./sqrt(1.-beta*beta);
}
// (zo, xo): rotating center of the arc
double TAMath::rho(double kin, double bin, double kout, double bout, double *zo, double *xo){
	if(fabs(kin - kout) < 1e-8){
		TAPopMsg::Warn("TAMath", "rho: |k1-k2| too small: k1: %f, k2: %f", kin, kout);
		kout = kin + 1e-7;
	}
	const double &k1 = kin, &k2 = kout;
	const double &b1 = bin, &b2 = bout;

	const double z1 = kzMagIn,  x1 = k1 * z1 + b1; // incident point
	const double z2 = kzMagOut, x2 = k2 * z2 + b2; // exit point

	if(zo && xo){
		zo[0] = (k1 * z2 - k2 * z1 + k1 * k2 * (x2 - x1)) / (k1 - k2);
		xo[0] = (k1 * x1 - k2 * x2 + z1 - z2) / (k1 - k2);
	}

	// x2 solved from equatioin L1 == L2
	const double x2p = x1 + 
		(z2 - z1) / (k1 + k2) * (k1 * k2 + sqrt(1. + k1 * k1) * sqrt(1. + k2 * k2) - 1.);
	const double L2p = fabs(((z1 - z2) + k1 * (x1 - x2p)) / (k1 - k2)) * sqrt(1. + k2 * k2);


#ifdef DEBUG
	const double L1 = fabs(((z1 - z2) + k2 * (x1 - x2)) / (k1 - k2)) * sqrt(1. + k1 * k1);
	// XXX: note that L2 is preferred, which agrees with L1p and L2p the better
	const double L2 = fabs(((z1 - z2) + k1 * (x1 - x2)) / (k1 - k2)) * sqrt(1. + k2 * k2);
	const double L1p = fabs(((z1 - z2) + k2 * (x1 - x2p)) / (k1 - k2)) * sqrt(1. + k1 * k1);

	cout << "\n\nL1: " << L1 << "\tL2: " << L2; // DEBUG
	cout << "\nL1p: " << L1p << "\tL2p: " << L2p << "\tx2: " << x2 << "\tx2p: " << x2p << endl; // DEBUG
	getchar(); // DEBUG
#endif

	return L2p;
} // end of member function rho

// definitions for fit functions serving class TATrack.
#include "TAMath/deviaFun.C" // Dsquare, minid2 - global functions.
#include "TAMath/iterFit.C" // iterativeFit - TAMath member function
#include "TAMath/refinedFit.C" // refinedFit - TAMath member function.
#include "TAMath/bfgs2.C" // refinedFitBFGS - TAMath member function.
#include "TAMath/bfgs4.C" // BFGS4 - TAMath member function for 3D linear tracking




