///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/iterFit.C													 //
//   iterFit.C -- source file for method TAMath::iterFit							 //
//   Introduction: an iterative and asymptotic fitting method. It is precise enough, //
// and currently is used for providing a starting point for further fittings, or to	 //
// give a fast estimate of the tracks. Note that this is a version for 4 DCs with 8	 //
// sense wire layers.
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/9/3.															     //
// Last modified: 2018/9/3, SUN Yazhou.												 //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

double TAMath::iterativeFit8(const double *x, const double *y, const double *r,
		double &kL, double &bL, int gGOOD, const int *LAYER, double d2PerDot){
	int nF = gGOOD; // number of fired sense wire layers
	if(2 == gGOOD) nF = 4; // 2 layers for each of the two DCs
	if(1 == gGOOD) nF = 3; // 1 layer for 1 DC and 2 for another
	const short n = pow(2, nF); // number of drift distance sign combinations
	double d2, kl, bl;
	// initialize kL and bL using LSM
	double d2min = Dsquare8(x, y, kL, bL, gGOOD, LAYER, d2PerDot);
	double xt[8], yt[8]; short sign[8]{};
	for(int i = 8; i--;){ xt[i] = yt[i] = -9999.; }
	
	const double cosTheta = 1. / sqrt(1. + kL*kL), sinTheta = kL * cosTheta;
	for(int i = n; i--;){
		for(int j = 0; j < nF; j++){
			const short s = LAYER[j];
			sign[s] = 2 * ((i>>j) & 0x1) - 1;
			xt[s] = x[s] + sign[s] * r[s] * sinTheta;
			yt[s] = y[s] - sign[s] * r[s] * cosTheta;
		}
		d2 = Dsquare8(xt, yt, kl, bl, gGOOD, LAYER, d2PerDot);
		if(d2 < d2min){
			kL = kl; bL = bl; d2min = d2;
		}
	} // end for over i

//	cout << "kL: " << kL << "\tbL: " << bL << "\td2min: " << d2min << endl;
	return d2min;
}
double TAMath::Dsquare8(const double *x, const double *y, double &k, double &b,
 int gGOOD, const int *LAYER, double DsqThrePerDot){

	double result = 0.;
	double dsquare = 0.;
	double denominator = 0.;
	double increment = 0.; // a temporary variable

	// LSM //
	double punishment = 1E10; // individual anodes exceeding DsquThre would recieve a punishment in dsquare. See the code
	double k1m = 0., b1m = 0.;
	double SUMx = 0., SUMy = 0., SUMxx = 0., SUMxy = 0.;
	int nF = 0; // number of fired sense wire layers
	for(int i = 0; i < 8; i++) if(-9999. != x[i]) nF++;
	for(int i = 0; i < nF; i++){ // valid fired sense wire
		const int s = LAYER[i];
		SUMx  += x[s]; SUMy  += y[s];
		SUMxx += x[s] * x[s]; SUMxy += x[s] * y[s];
	} // end for over i
	// The function of the line to be fitted is y = kx + b
	k1m = (nF * SUMxy - SUMx * SUMy) / (nF * SUMxx - SUMx * SUMx);
	if(fabs(k1m) > 1E4){
		if(k1m > 1E4)
			k1m = 1E4;
		else if(k1m < -1E4)
			k1m = -1E4;
		b1m = -k1m * x[0];
	}
	else
		b1m = (SUMxx * SUMy - SUMxy * SUMx) / (nF * SUMxx - SUMx * SUMx);
	// calculate the deviation
	denominator = k1m * k1m + 1.;
	for(int i = 0; i < nF; i++){ // valid fired sense wire
		const int s = LAYER[i];
		increment = (k1m * x[s] - y[s] + b1m) * (k1m * x[s] - y[s] + b1m) / denominator;
		if(increment > DsqThrePerDot){
			increment *= punishment;
		} // end if
		dsquare += increment;
	} // end for over i
	result = dsquare;

	k = k1m;
	b = b1m;
	return result;
}

