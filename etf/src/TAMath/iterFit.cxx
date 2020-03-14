///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/iterFit.C													 //
//   iterFit.C -- source file for method TAMath::iterFit							 //
//   Introduction: an iterative and asymptotic fitting method. It is precise enough, //
// and currently is used for providing a starting point for further fittings, or to	 //
// give a fast estimate of the tracks.												 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/8/17, SUN Yazhou.											 //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

double TAMath::iterativeFit(const double *x, const double *y, const double *r,
		double &kL, double &bL, int gGOOD, const int *LAYER, double d2PerDot){
	int nF = gGOOD; // number of fired sense wire layers
	if(2 == gGOOD) nF = 4; // 2 layers for each of the two DCs
	if(1 == gGOOD) nF = 3; // 1 layer for 1 DC and 2 for another
	const short n = pow(2, nF); // number of drift distance sign combinations
	double d2, kl, bl;
	// initialize kL and bL using LSM
	double d2min = Dsquare(x, y, kL, bL, gGOOD, LAYER, d2PerDot);
	double xt[6], yt[6]; short sign[6]{};
	for(int i = 6; i--;){ xt[i] = yt[i] = -9999.; }
	
	const double cosTheta = 1. / sqrt(1. + kL*kL), sinTheta = kL * cosTheta;
	for(int i = n; i--;){
		for(int j = 0; j < nF; j++){
			const short s = LAYER[j];
			sign[s] = 2 * ((i>>j) & 1) - 1;
			xt[s] = x[s] + sign[s] * r[s] * sinTheta;
			yt[s] = y[s] - sign[s] * r[s] * cosTheta;
		}
		d2 = Dsquare(xt, yt, kl, bl, gGOOD, LAYER, d2PerDot);
		if(d2 < d2min){
			kL = kl; bL = bl; d2min = d2;
		}
	} // end for over i

//	cout << "kL: " << kL << "\tbL: " << bL << "\td2min: " << d2min << endl;
	return d2min;
}

