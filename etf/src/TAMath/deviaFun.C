///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/deviaFun.C													 //
//   deviaFun.C -- source file for method TAMath::Dsquare, TAMath::minid2.			 //
//   Introduction: This scriptlet is to implement two functions: Dsquare and minid2	 //
// for different gGOOD values, which marks number of fired anode planes	in X, U or V //
// anode plane set for an event.													 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2017/10/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

static double Dsquare6(const double *x, const double *y, double &k, double &b, double DsqThrePerDot);
static double Dsquare5(const double *x, const double *y, double &k, double &b, const int *LAYER, double DsqThrePerDot);
static double Dsquare4(const double *x, const double *y, double &k, double &b, const int *LAYER, double DsqThrePerDot);
static double Dsquare3(const double *x, const double *y, double &k, double &b, const int *LAYER, double DsqThrePerDot);
static double Dsquare2(const double *x, const double *y, double &k, double &b, const int *LAYER, double DsqThrePerDot);

static double minid2_6(const double *x, const double *y, const double *r, double k, double b);
static double minid2_5(const double *x, const double *y, const double *r, double k, double b, const int *LAYER);
static double minid2_4(const double *x, const double *y, const double *r, double k, double b, const int *LAYER);
static double minid2_3(const double *x, const double *y, const double *r, double k, double b, const int *LAYER);
static double minid2_2(const double *x, const double *y, const double *r, double k, double b, const int *LAYER);

double TAMath::Dsquare(const double *x, const double *y, double &k, double &b, int gGOOD, const int *LAYER, double DsqThrePerDot){
	if(gGOOD == 2) return Dsquare2(x, y, k, b, LAYER, DsqThrePerDot);
	if(gGOOD == 3) return Dsquare3(x, y, k, b, LAYER, DsqThrePerDot);
	if(gGOOD == 4) return Dsquare4(x, y, k, b, LAYER, DsqThrePerDot);
	if(gGOOD == 5) return Dsquare5(x, y, k, b, LAYER, DsqThrePerDot);
	if(gGOOD == 6) return Dsquare6(x, y, k, b, DsqThrePerDot);
	else{
		cout << "Dsquare. ODD..." << endl;
		getchar();
	}
	return 1E200;
} // end of global function Dsquare.

double TAMath::minid2(const double *x, const double *y, const double *r, double k, double b, int gGOOD, const int *LAYER){
	if(gGOOD == 2) return minid2_2(x, y, r, k, b, LAYER);
	if(gGOOD == 3) return minid2_3(x, y, r, k, b, LAYER);
	if(gGOOD == 4) return minid2_4(x, y, r, k, b, LAYER);
	if(gGOOD == 5) return minid2_5(x, y, r, k, b, LAYER);
	if(gGOOD == 6) return minid2_6(x, y, r, k, b);
	else{
		cout << "minid2. ODD..." << endl;
		getchar();
	}
	return 1E200;
} // end of global function minid2.


#include "two.C"
#include "three.C"
#include "four.C"
#include "five.C"
#include "six.C"

