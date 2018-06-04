///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMagnet.C														 //
//   TAMagnet.C -- source file for class TAMagnet									 //
//   Introduction: magnetic rigidity analysis for PID using fourth-order Runge-Kutta //
// method to track particles in magnetic field of the dipole magnet in ETF.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2018/1/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

// ROOT include
#include "TGraph.h"
#include "TGraph2D.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstring>
#include "TAMagnet.h"
#include "TAParaManager.h"
#include "TACtrlPara.h"
#include "TAPopMsg.h"
#include "TAMath.h"

//#define VERBOSE // show TAPopMsg::Info() information

using std::ifstream;
using std::cout;
using std::endl;

//#define DEBUG

TAMagnet::TAMagnet(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid){ // the default constructor
	fRKMethod = 1; fScale = 1.;
	fh0 = 1E1; fStepError = 1E1;
	fIsFileLoaded = false; fTrackLength = -9999.;
	memset(fB, 0, sizeof(fB)); fEnableEnergyLoss = false;
	fQoP = -9999.; fTrackVec.clear(); fAoZ = -9999.;
	fOutOfRangeError = false; // out of the active volume of the magnet
} // end of the default constructor
TAMagnet::~TAMagnet(){}

// i: initial; f: final
void TAMagnet::TransportIon(double *y, double *yp, double zi, double zf, bool isTracking){
	if(-9999. == fQoP){
		cout << "\033[31;1mTAMagnet::TransportIon(...):\n";
		cout << "QoP = -9999. fQoP not set?\033[0m\n";
		getchar();
	} // end if
	fTrackLength = 0.; // initialize track length
	double direction = 1.; // 1.: forward; -1.: backward
	if(zf < zi){ // backward deduction
		direction = -1.;
	} // end if

	double x = zi, h = fh0 * direction; // set the iteration start and iteration step length	
	// propagate the particle step by step
	int i = 0; tra_t tra;
	if(isTracking) fTrackVec.clear();
	double B[3], p[3]; // to extract B array
//	cout << "Trans:while\n"; // DEBUG
//	cout << "fOutOfRangeError: " << fOutOfRangeError << endl; getchar();  // DEBUG
	while((x - zf) * direction <= 0.){
//			cout << "\33[31;1mfOutOfRangeError: " << fOutOfRangeError << "\033[0m"; // DEBUG
		if(fOutOfRangeError) break;
		if(fabs(x - zf) < fabs(h) && fabs(h/fh0) > 0.01){
			h /= 5.;
		}
		double x0 = x, y0[2] = {y[0], y[1]}; // store the previous position
		double ypp[2], rho; // the second order derivative

		RKvl(x, y, yp, h); // one iteration //////&&&&&&&////#####$$$$%%%%%%/////

		f(ypp, x, y, yp); // assign ypp using Lorentz equation
		p[0] = y[0]; p[1] = y[1]; p[2] = x; // x, y, z
		GetMagneticIntensity(B, p); // assign B
		rho = pow(1.+yp[0]*yp[0], 1.5) / fabs(ypp[0]) / 1000.; // unit: m
		tra.x = y[0]; tra.y = y[1]; tra.z = x; tra.rho = rho; tra.brho = B[1]*rho;
		if(isTracking){
			fTrackVec.push_back(tra);
#ifdef DEBUG
//			cout << "x: " << x << "\tzf: " << zf << endl; // DEBUG
//			cout << "y[0]: " << y[0] << "\ty[1]: " << y[1] << endl; // DEBUG
			cout << "dx/dz: " << yp[0] << "\tdy/dz: " << yp[1] << endl; // DEBUG
			cout<< "d2x/dz2: " << ypp[0] << "\td2y/dz2: " << ypp[1] << endl; // DEBUG
			cout << "B[0]: " << B[0] << "\tB[1]: " << B[1]; // DEBUG
			cout << "\tB[2]: " << B[2] << endl; // DEBUG
			tra.show(); getchar(); // DEBUG
#endif
		}
//		tra.show(); // DEBUG
//		cout << "dx/dz: " << yp[0] << "\tdy/dz: " << yp[1] << endl; // DEBUG
//		cout<< "d2x/dz2: " << ypp[0] << "\td2y/dz2: " << ypp[1] << endl; // DEBUG
//		cout << "B[0]: " << B[0] << "\tB[1]: " << B[1]; // DEBUG
//		cout << "\tB[2]: " << B[2] << endl; getchar(); // DEBUG
		fTrackLength += sqrt((x-x0)*(x-x0) + (y[0]-y0[0])*(y[0]-y0[0])
			 + (y[1]-y0[1])*(y[1]-y0[1])); // increment the track length
		if(fEnableEnergyLoss) UpdateQoP(fL0 + fL1 - fTrackLength);
//		cout << "fL0: " << fL0 << "\tfL1: " << fL1 << endl; // DEBUG
//		cout << "fL0 + fL1 - fTrackLength: " << fL0 + fL1 - fTrackLength << endl; // DEBUG
//		cout << "fQoP: " << fQoP << endl; getchar(); // DEBUG
		i++; // DEBUG
//		cout << "x: " << x << "\tx0: " << x0 << "\tx-x0: " << x-x0 << endl; // DEBUG
//		cout << "The first variable: y[0]: " << y[0] << "\typ[0]: " << yp[0] << endl; // DEBUG
//		cout << "The second variable: y[1]: " << y[1] << "\typ[1]: " << yp[1] << endl; // DEBUG
		// to see the curvature of the track // DEBUG
//		cout << "ypp[0]: " << ypp[0] << "\typp[1]: " << ypp[1] << endl; // DEBUG
//		cout << "curvature: " << rho << "m \ti: " << i << "\n"; // DEBUG
//		getchar(); // DEBUG
		// out of range check
		if(fabs(x) < 1050. && fabs(x) > 475.){ // z within the magnetic range
			if(fabs(y[0]) > 150.+fabs(x)*1.3){
//				cout << "Mark 1" << endl; getchar(); // DEBUG
				fOutOfRangeError = true; break;
			} // x well out of range
			if(fabs(y[1]) > 300.+fabs(x)*0.2){
//				cout << "Mark 2" << endl; getchar(); // DEBUG
				fOutOfRangeError = true; break;
			 } // y well out of range
		} // end if
		if(fabs(x) < 475.){ // z within the magnetic range (in the cavity of the magnet)
			if(fabs(y[0]) > 600.){
//				cout << "Mark 3" << endl; getchar(); // DEBUG
				fOutOfRangeError = true; break;
			 } // x well out of range
			if(fabs(y[1]) > 200.){
//				cout << "Mark 4" << endl; getchar(); // DEBUG
				fOutOfRangeError = true; break;
			 } // y well out of range
		} // end if
	} // end while


	if((x - zf) * direction <= 0.){ // abnormal exit -- out of range
//		if(isTracking){
//			cout << "x: " << x << endl; // DEBUG
//			cout << "y[0]: " << y[0] << "\ty[1]: " << y[1] << endl; // DEBUG
//			cout << "yp[0]: " << yp[0] << "\typ[1]: " << yp[1] << endl; // DEBUG
//			getchar(); // DEBUG
//		}
		x = -9999.;
		y[0] = -9999.; y[1] = -9999.;
		yp[0] = -9999.; yp[1] = -9999.;
	} // end if
//	cout << "TAMagnet::TransportIon(...): The iteration has been implemented " << i << " times.\n"; // DEBUG
	fQoP = -9999.; // reset QoP
} // end of function TransportIon

// variable step length Runge-Kutta method for one iteration
// h0 is the initial step length of the iteration
void TAMagnet::RKvl(double &x, double *y, double *yp, double h0){
//	cout << "This is TAMagnet::RKvl(...):\n"; getchar(); // DEBUG
	double yy[2]{}, yyp[2]{}; // for the next step
	double yyh[2]{}, yyph[2]{}; // for the next step, halfed step length
	// delta/15. is the approximate error of rk(h/2.)
	double delta = 1E200, h = h0;
	if(fRKMethod == kClassic) RK(yy, yyp, x, y, yp, h);
	else RK_Nys(yy, yyp, x, y, yp, h);
	while(1){
		if(fOutOfRangeError) break;
		h /= 2.;
		if(fRKMethod == kClassic) RK(yyh, yyph, x, y, yp, h);
		else RK_Nys(yyh, yyph, x, y, yp, h);
		double dy[2] = {yy[0] - yyh[0], yy[1] - yyh[1]};
		delta = sqrt(dy[0]*dy[0]+dy[1]*dy[1]);
		memcpy(yy, yyh, sizeof(yyh));
		memcpy(yyp, yyph, sizeof(yyph));
//		cout << "\033[36;1mdelta: " << delta << "\th: " << h << "\033[0m\n"; // DEBUG
		if(delta < fStepError){
			x += h;
			y[0] = yyh[0]; y[1] = yyh[1];
			yp[0] = yyph[0]; yp[1] = yyph[1];

//			cout << "\033[33;1mBINGO.\033[0m\n"; // DEBUG
//			cout << "yyh[0]: " << yyh[0] << "\tyyh[1]: " << yyh[1] << endl; // DEBUG
//			cout << "yyph[0]: " << yyph[0] << "\tyyph[1]: " << yyph[1] << endl; // DEBUG
//			cout << "delta: " << delta << "\th: " << h << endl; // DEBUG
//			cout << "x: " << x << endl; // DEBUG
//			double B[3]{}, p[3] = {y[0], y[1], x}; // DEBUG
//			GetMagneticIntensity(B, p); // DEBUG
//			cout << "B[0]: " << B[0] << "\tB[1]: " << B[1] << "\tB[2]: " << B[2] << endl; // DEBUG
//			cout << "\033[1mB: " << sqrt(B[0]*B[0]+B[1]*B[1]+B[2]*B[2]) << "\033[0m\n";
//			cout << "p[0]: " << p[0] << "\tp[1]: " << p[1] << "\tp[2]: " << p[2] << endl; // DEBUG
//			getchar(); // DEBUG

			break;
		} // end if
	} // end while
} // end of function RKvl

// NOTE that mm is used as the unit of length in the RK method. //
// Runge-Kutta method: fourth order, two dimensional, second order derivative
void TAMagnet::RK(double *yNew, double *ypNew, const double x, const double *y, const double *yp, const double h){
	double ypp[2]{}, k0[4][2]{}, k1[4][2]{}; // k0: yp*h; k1: ypp*h
	double xx, yy[2], yyp[2]; // updated values of x, y and yp

	// calculate derivatives at various positions in the current iteration step
	xx = x;
	yy[0] = y[0]; yyp[0] = yp[0]; // y0
	yy[1] = y[1]; yyp[1] = yp[1]; // y1
	f(ypp, xx, yy, yyp); // calculate array ypp
	k0[0][0] = h*yyp[0]; k1[0][0] = h*ypp[0]; // y0
	k0[0][1] = h*yyp[1]; k1[0][1] = h*ypp[1]; // y1

	xx = x + h/2.;
	yy[0] = y[0] + k0[0][0]/2.; yyp[0] = yp[0] + k1[0][0]/2.;
	yy[1] = y[1] + k0[0][1]/2.; yyp[1] = yp[1] + k1[0][1]/2.;
	f(ypp, xx, yy, yyp);
	k0[1][0] = h*yyp[0]; k1[1][0] = h*ypp[0];
	k0[1][1] = h*yyp[1]; k1[1][1] = h*ypp[1];

	xx = x + h/2.;
	yy[0] = y[0] + k0[1][0]/2.; yyp[0] = yp[0] + k1[1][0]/2.;
	yy[1] = y[1] + k0[1][1]/2.; yyp[1] = yp[1] + k1[1][1]/2.;
	f(ypp, xx, yy, yyp);
	k0[2][0] = h*yyp[0]; k1[2][0] = h*ypp[0];
	k0[2][1] = h*yyp[1]; k1[2][1] = h*ypp[1];

	xx = x + h;
	yy[0] = y[0] + k0[2][0]; yyp[0] = yp[0] + k1[2][0];
	yy[1] = y[1] + k0[2][1]; yyp[1] = yp[1] + k1[2][1];
	f(ypp, xx, yy, yyp);
	k0[3][0] = h*yyp[0]; k1[3][0] = h*ypp[0];
	k0[3][1] = h*yyp[1]; k1[3][1] = h*ypp[1];

	// calculate y and y' for next iteration
	yNew[0]  = y[0] + (k0[0][0]+2.*k0[1][0]+2.*k0[2][0]+k0[3][0])/6.;
	yNew[1]  = y[1] + (k0[0][1]+2.*k0[1][1]+2.*k0[2][1]+k0[3][1])/6.;
	ypNew[0] = yp[0] + (k1[0][0]+2.*k1[1][0]+2.*k1[2][0]+k1[3][0])/6.;
	ypNew[1] = yp[1] + (k1[0][1]+2.*k1[1][1]+2.*k1[2][1]+k1[3][1])/6.;
} // end of function RK

// NOTE that mm is used as the unit of length in the RK method. //
// the so-called Nystrom fourth order Runge-Kutta method
// quotated from Data Analysis Techniques for High-Energy Physics.
void TAMagnet::RK_Nys(double *yNew, double *ypNew, const double x, const double *y, const double *yp, const double h){
	double ypp[2]{}, k[4][2]{}; // k: ypp*h
	double xx, yy[2], yyp[2]; // updated values of x, y and yp

	// calculate derivatives at various positions in the current iteration step
	xx = x;
	yy[0] = y[0]; yyp[0] = yp[0]; // y0
	yy[1] = y[1]; yyp[1] = yp[1]; // y1
	f(ypp, xx, yy, yyp); // calculate array ypp
	k[0][0] = h*ypp[0]; // y0
	k[0][1] = h*ypp[1]; // y1

	xx = x + h/2.;
	yy[0] = y[0] + h*yp[0]/2. + h*k[0][0]/8.; yyp[0] = yp[0] + k[0][0]/2.;
	yy[1] = y[1] + h*yp[1]/2. + h*k[0][1]/8.; yyp[1] = yp[1] + k[0][1]/2.;
	f(ypp, xx, yy, yyp);
	k[1][0] = h*ypp[0];
	k[1][1] = h*ypp[1];

	xx = x + h/2.;
	yy[0] = y[0] + h*yp[0]/2. + h*k[0][0]/8.; yyp[0] = yp[0] + k[1][0]/2.;
	yy[1] = y[1] + h*yp[1]/2. + h*k[0][1]/8.; yyp[1] = yp[1] + k[1][1]/2.;
	f(ypp, xx, yy, yyp);
	k[2][0] = h*ypp[0];
	k[2][1] = h*ypp[1];

	xx = x + h;
	yy[0] = y[0] + h*yp[0] + h*k[2][0]/2.; yyp[0] = yp[0] + k[2][0];
	yy[1] = y[1] + h*yp[1] + h*k[2][1]/2.; yyp[1] = yp[1] + k[2][1];
	f(ypp, xx, yy, yyp);
	k[3][0] = h*ypp[0];
	k[3][1] = h*ypp[1];

	// calculate y and y' for next iteration
	yNew[0]  = y[0] + h*(yp[0] + (k[0][0]+k[1][0]+k[2][0])/6.);
	yNew[1]  = y[1] + h*(yp[1] + (k[0][1]+k[1][1]+k[2][1])/6.);
	ypNew[0] = yp[0] + (k[0][0]+2.*k[1][0]+2.*k[2][0]+k[3][0])/6.;
	ypNew[1] = yp[1] + (k[0][1]+2.*k[1][1]+2.*k[2][1]+k[3][1])/6.;
} // end of function RK

// 2nd order derivative: f0(x;y0,y1;yp0,yp1), f0(x;y0,y1;yp0,yp1) // y'=z; z'=g(x,y,z);
void TAMagnet::f(double *ypp, const double x, const double *y, const double *yp){
	double B[3]{}, p[3] = {y[0], y[1], x};
	GetMagneticIntensity(B, p);
//	B[0] = 0.; B[1] = 0.; B[2] = 0.; // DEBUG

	// XXX: this differential equation set uses International System of Units,
	// XXX: well usually mm is used for length. So please note the unit transformation
	// XXX: if length is involved in the equation set. VITAL VITAL VITAL VITAL VITAL VITAL
	double sp = sqrt(1.+yp[0]*yp[0]+yp[1]*yp[1]); // ds/dz
	// the following two equations is the dynamics equation set.
	// QoP is in SI unit
	ypp[0] = fQoP*sp*(yp[0]*yp[1]*B[0] - (1.+yp[0]*yp[0])*B[1] + yp[1]*B[2]);
	ypp[1] = fQoP*sp*((1.+yp[1]*yp[1])*B[0] - yp[0]*yp[1]*B[1] - yp[0]*B[2]);
//	cout << "fQoP: " << fQoP << endl; // DEBUG
//	cout << "sp: " << sp << endl; // DEBUG
//	cout << "yp[0]: " << yp[0] << "\typ[1]: " << yp[1] << endl; // DEBUG
//	cout << "B[0]: " << B[0] << "\tB[1]: " << B[1] << "\tB[2]: " << B[2] << endl; // DEBUG
//	cout << "p[0]: " << p[0] << "\tp[1]: " << p[1] << "\tp[2]: " << p[2] << endl; // DEBUG
//	cout << "ypp[0]: " << ypp[0] << "\typp[1]: " << ypp[1] << endl; // DEBUG
//	getchar(); // DEBUG

	// ypp is in m^-1, but mm^-1 is what is needed. 17:14, Jun. 28, 2017.
	ypp[0] /= 1000.; // 1/m -> 1/mm
	ypp[1] /= 1000.; // 1/m -> 1/mm
} // end of function f(...)

void TAMagnet::SetRKMethod(int selection){
	if(selection != 0 && selection != 1){
		cout << "TAMagnet::SetRKMethod(): invalid input R-K Method.\n";
		getchar();
	} // end if
	fRKMethod = selection;
} // end of function SetRKMethod

// the following two methods are for control of RK numerical method
void TAMagnet::SetIterationStep(double h){
	if(h > 100.)
		TAPopMsg::Warn("TAMagnet", "SetIterationStep(): Input iteration might be too big: h: %f", h);
	fh0 = h;
} // end of function SetIterationStep
void TAMagnet::SetStepErrorTolerance(double e){
	if(e > 100.)
		TAPopMsg::Warn("TAMagnet", "SetStepErrorTolerance(): Input step error tolerance might be too big: e: %f", e);
	fStepError = e;
} // end of function SetStepErrorTolerance(double e)


// read the magnetic field measurement file, generated from measurements using Hall sensor.
void TAMagnet::LoadMagneticFieldFile(const string &file){
	const char *filename = file.c_str();
	ifstream inFile(filename);
	if(!inFile.is_open()){
		cout << "\033[31;1mERROR: TAMagnet::LoadMagneticFieldFile():\033[0m\n";
		cout << "File " << filename << " doesn't exist. Exiting...\n";
		exit(EXIT_FAILURE);
	} // end if
	string s_t;
	for(int i = 5; i--;){
		getline(inFile, s_t); // skip the commentary lines.
//		cout << s_t << endl; // DEBUG
	} // end for over i

	double current, x, y, z, Bx, By, Bz, Ihall, Tpt; // y(x)
	while(inFile >> current >> x >> y >> z >> Bx >> By >> Bz >> Ihall >> Tpt){
		if(fabs(Bx) > 1E50 || fabs(By) > 1E50 || fabs(By) > 1E50){
			TAPopMsg::Error("TAMagnet",
				"LoadMagneticFieldFile: Abnormal B value: (%f, %f, %f)", Bx, By, Bz);
		}
		int i = int(-x/10.); // x: -1000, 0
		int j = int(y/10.); // y: 0, 90
		int k = int(z/10.); // z: -20, 1050
		if(i >= 0 && i <= 100
		&& j >= 0 && j <= 9
		&& k >= 0 && k <= 105){
			fB[0][i][j][k] = Bx;
			fB[1][i][j][k] = By;
			fB[2][i][j][k] = Bz;
		} // end if
		while(inFile.get() != '\n') continue; // skip the reset of the line
//		cout << "current: " << current << "\tx: " << x << "\ty: " << y << "\tz: " << z << "\tBx: " << Bx << "\tBy: " << By << "\tBz: " << Bz << endl; getchar(); // DEBUG
	} // end while
	if(inFile.eof()){
#ifdef VERBOSE
		TAPopMsg::Info("TAMagnet", "LoadMagneticFieldFile: End of the file reached.\nFile %s loaded successfully.\n", filename);
#endif
		fIsFileLoaded = true;
	} // end if
	else if(inFile.fail()){
		TAPopMsg::Error("TAMagnet",
				"LoadMagneticFieldFile: Input terminated by data mismatch.");
		fIsFileLoaded = false;
	} // end else if
	else{
		TAPopMsg::Error("TAMagnet",
				"LoadMagneticFieldFile: WTF?");
		while(1) getchar();
	} // end else
	inFile.close();
} // end of function LoadMagneticFieldFile


// assign B with the magnetic intensity vector at position p through trilinear interpolation
void TAMagnet::GetMagneticIntensity(double *B, const double *p){
	if(!fIsFileLoaded){
		TAPopMsg::Error("TAMagnet",
				"GetMagneticIntensity: Magnetic Field File NOT loaded.");
	} // end if

	// retrieve the magnetic intensity vector at the requested position
	// the position coordinates are deprived of their signs,
	// because of the symmetry of the magnetic field, the magnetic filed intensity is
	// irrelevant to the signs of the position coordinates.
	double pp[3] = {fabs(p[0]), fabs(p[1]), fabs(p[2])};

	// Expand the magnet field with measured B using extrapolation // ***********************
	// extrapolation for B at Y greater than 90mm and X greater than 390mm in the magnet
	if(pp[2] < 475.){ // inside the cavity
		if(pp[1] >= 90. && pp[1] <= 200.) pp[1] = 90. - 1E-2; // 170+30, including the V error
		if(pp[0] >= 390. && pp[0] <= 550.) pp[0] = 390. - 1E-2; // 500+50, including the H error
	}
	if(pp[2] >= 475. && pp[2] <= 690.){ // between the iron wall (z=475) and the z=690 plane
		if(pp[0] >= 390. && pp[0] < 700.){
			if(pp[2] - 690. < pp[0] - 390.) pp[2] = 690. + 1E-2; // z-ward extrapolation
			else pp[0] = 390. - 1E-2; // x-ward extrapolation
		}
		if(pp[1] >= 90. && pp[1] < 400.) pp[1] = 90. - 1E-2; // FEEL MY ECSTASY ~~~~~~~
	}
	if(pp[2] > 690.){ // out of the inner area
		if(pp[1] >= 90. && pp[1] < 500.) pp[1] = 90. - 1E-2; // AND YOU TOGETHER - ESCTASY ~~~
	}
	// Expand the magnet field with measured B using extrapolation // ***********************

	int ii[3] = {int(pp[0]), int(pp[1]), int(pp[2])}; for(int &x : ii) x /= 10;
	double dp[3] = {pp[0]-ii[0]*10., pp[1]-ii[1]*10., pp[2]-ii[2]*10.}; // for interpolation
	double B_t[3][8]{}; // [xyz][8 vetices]

	if(ii[0] >= 0 && ii[0] < 100
	&& ii[1] >= 0 && ii[1] < 9
	&& ii[2] >= 0 && ii[2] < 105){
		for(int i = 0; i < 3; i++){ // loop over x, y and z
			// plane in the back
			B_t[i][0] = fB[i][ii[0]][ii[1]][ii[2]];
			B_t[i][1] = fB[i][ii[0]+1][ii[1]][ii[2]];
			B_t[i][2] = fB[i][ii[0]][ii[1]+1][ii[2]];
			B_t[i][3] = fB[i][ii[0]+1][ii[1]+1][ii[2]];
			// plane in the front
			B_t[i][4] = fB[i][ii[0]][ii[1]][ii[2]+1];
			B_t[i][5] = fB[i][ii[0]+1][ii[1]][ii[2]+1];
			B_t[i][6] = fB[i][ii[0]][ii[1]+1][ii[2]+1];
			B_t[i][7] = fB[i][ii[0]+1][ii[1]+1][ii[2]+1];
		} // end for over i
		B[0] = trilinear(B_t[0], dp) * fScale;
		B[1] = trilinear(B_t[1], dp) * fScale;
		B[2] = trilinear(B_t[2], dp) * fScale;
	} // end if(...&&...&&...)
	else{
		B[0] = 0.; B[1] = 0.; B[2] = 0.;
	} // end else

	if(0. == B[0] && 0. == B[1] && 0. == B[2]){
//		cout << "p[0]: " << p[0] << "\tp[1]: " << p[1] << "\tp[2]: " << p[2] << endl; // DEBUG
//		cout << "ii[0]: " << ii[0] << "\tii[1]: " << ii[1] << "\tii[2]: " << ii[2] << endl; // DEBUG
//		getchar(); // DEBUG
		fOutOfRangeError = true;
	}
	// Gs -> Tesla     VITAL VITAL VITAL
	B[0] *= 1E-4; B[1] *= 1E-4; B[2] *= 1E-4;

#ifdef DEBUG
	cout << "fOutOfRangeError: " << fOutOfRangeError << endl; // DEBUG
	cout << "fScale: " << fScale << endl; // DEBUG
	cout << "p[0]: " << p[0] << "\tp[1]: " << p[1] << "\tp[2]: " << p[2] << endl; // DEBUG
	cout << "ii[0]: " << ii[0] << "\tii[1]: " << ii[1] << "\tii[2]: " << ii[2] << endl; // DEBUG
	cout << "Bx: " << B[0] << endl; // DEBUG
	cout << "By: " << B[1] << endl; // DEBUG
	cout << "Bz: " << B[2] << endl; getchar(); // DEBUG
	for(int i = 0; i < 3; i++) cout << "dp[i]: " << dp[i] << endl; // DEBUG
	for(int i = 0; i < 3; i++){ // DEBUG
		cout << "i: " << i << endl; // DEBUG
		for(int j = 0; j < 8; j++){ // DEBUG
			cout << "B_t[i][j]: " << B_t[i][j] << endl; // DEBUG
		} // end for over j // DEBUG
		getchar(); // DEBUG
	} // end for over i // DEBUG
#endif
} // end of function GetMagneticIntensity
double TAMagnet::GetTrackLength() const{
	if(fTrackLength == -9999.){
		cout << "\033[31;1mTAMagnet::GetTrackLength():\n";
		cout << "fTrackLength is not assigned yet.\033[0m\n";
		getchar();
	} // end if
	else if(fTrackLength < 0.){
		cout << "\033[31;1mTAMagnet::GetTrackLength():\n";
		cout << "WTF?\033[0m\n";
		while(1) getchar();
	} // end if
	
	return fTrackLength;
} // end of member function GetTrackLength()

void TAMagnet::FillGraphTrajectory(TGraph2D *gcurve) const{
	int i = 0;
	for(const tra_t &t : fTrackVec){
		gcurve->SetPoint(i++, t.z, t.x, t.y);
	} // end for
} // end member function FillTrajectory(...)
void TAMagnet::FillGraphRho(TGraph *grho) const{
	int i = 0;
	for(const tra_t &t : fTrackVec){
		double rho = t.rho < 1000. ? t.rho : 1000.; // truncate t.rho
		grho->SetPoint(i++, t.z, rho);
	} // end for
} // end of member function FillGraphRho(...)
void TAMagnet::FillGraphBrho(TGraph *gbrho) const{
	int i = 0;
	for(const tra_t &t : fTrackVec){
		double brho = t.brho < 10. ? t.brho : 10.; // truncate t.brho
		gbrho->SetPoint(i++, t.z, brho);
	} // end for
} // end of member function FillGraphRho(...)

// QoP = Z/A * e0/(mu*beta*gamma*c0)    P: momentum
// beta: particle velocity; aoz: mass/(atomic number)     mass unit: atomic mass unit
// aoz(Ar) = 39.9525/18 = 2.21958333
// beta = 0.66, gamma = 1.33, aoz*beta*gamma = 1.95, qop = 0.1650200     SI unit
void TAMagnet::SetQoP(double aoz, double beta, double V0, double tof2, double L, double L0, double L2){
	if(beta < 0. || beta >= 1.){
		cout << "\033[31;1mERROR: TAMagnet::SetQoP():\033[0m\n";
		cout << "Input particle beta out of range. beta: " << beta << endl;
		cout << "Exiting...\n"; getchar();
		exit(EXIT_FAILURE);
	} // end if
//	double k = 0.321840605; // SI unit      = e0/(u0*c0*1E6)
	fQoP = 0.321840605 / (aoz*TAMath::BetaGamma(beta));
	if(0) if(fabs(fQoP) > 1E3)
		TAPopMsg::Warn("TAMagnet", "SetQoP: abnormal QoP: fQoP: %f, beta: %f", fQoP, beta);

	// for real time update of QoP because of dE
	fAoZ = aoz; fV0 = V0; fTOF2 = tof2;
	fL = L; fL0 = L0; fL2 = L2; fL1 = fL - fL0 - fL2;
//	cout << "fAoZ: " << fAoZ << "\tfV0: " << fV0 << "\tfTOF2: " << fTOF2 << endl;
//	cout << "fL: " << fL << "\tfL0: " << L0 << "\tfL2: " << fL2 << "\tfL1: " << fL1 << endl; // DEBUG
} // end of member function SetQoP
// real time QoP update along track length
// V0: particle velocity in RIBLL2, tof2: TOF after RIBLL2, L: track length after RIBLL2
// x: the current track length.
static double c0 = TAParaManager::Instance()->GetPhysConst("c0");
void TAMagnet::UpdateQoP(double x){
	if(fAoZ == -9999. || fQoP == -9999){
		cout << "\033[31;1mTAMagnet::UpdateQoP(): Error\033[0m\n";
		cout << "fAoZ: " << fAoZ << "\tfQoP: " << fQoP << endl;
		cout << "Exiting...\n";
		getchar(); exit(1);
	} // end if
	if(fV0 < 0. || fV0 >= c0 || fTOF2 < 0. || fL < 0. || fL0 < 0.){
		cout << "\033[31;1mTAMagnet::UpdateQoP(): Error\033[0m\n";
		cout << "V0: " << fV0 << "\ttof2: " << fTOF2;
		cout << "L0: " << fL0 << endl;
		cout << "L: " << fL << "\tx: " << x << "" << endl;
		cout << "Exiting...\n";
		getchar(); exit(1);
	} // end if
	// particle velocity at track length x
	double Vx = fV0 * pow((1. - 8. * (fTOF2 * fV0 / fL - 1.) / fL * x), 0.25); // mm/ns
	double beta = Vx / c0;
	fQoP = 0.321840605 / (fAoZ*TAMath::BetaGamma(beta)); // 0.321840605 = e0/(u0*c0*1E6)
	if(0) if(fabs(fQoP) > 1E3)
		TAPopMsg::Warn("TAMagnet", "UpdateQoP: abnormal QoP: fQoP: %f, beta: %f", fQoP, beta);
} // end of member function UpdateQop(...)

// trilinear interpolation
double TAMagnet::trilinear(const double *f, const double *dp){
	double fx[4], fy[2], fz; // temporary variables
	// interpolation along x-axis
	for(int i = 0; i < 4; i++)
		fx[i] = (10.-dp[0])/10.*f[2*i] + dp[0]/10.*f[2*i+1];
	// interpolation along y-axis
	for(int i = 0; i < 2; i++)
		fy[i] = (10.-dp[1])/10.*fx[2*i] + dp[1]/10.*fx[2*i+1];
	// interpolation along z-axis
	fz = (10.-dp[2])/10.*fy[0] + dp[2]/10.*fy[1];
	
	return fz;
} // end of function trilinear interpolation




