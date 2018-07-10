///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/iterFit.C													 //
//   iterFit.C -- source file for method TAMath::iterFit							 //
//   Introduction: an iterative and asymptotic fitting method. It is precise enough, //
// and currently is used for providing a starting point for further fittings.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/1/4, SUN Yazhou.										  	     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

double TAMath::iterativeFit(const double *x, const double *y, const double *r, double &kL, double &bL, int gGOOD, const int *LAYER, double d2PerDot){
//	cout << "Welcome, this is IterFit." << endl; getchar(); // DEBUG
	double kl, bl, d2;
	d2 = Dsquare(x, y, kl, bl, gGOOD, LAYER, d2PerDot); // set the iteration origin - kl and bl
	kL = kl; bL = bl; // DEBUG
//	return d2;
//	for(int i = 0; i < 6; i++){
//		cout << "x[i]: " << x[i] << "\ty[i]: " << y[i] << endl; // DEBUG
//		cout << "r: " << r[i] << endl; // DEBUG
//	}
//	getchar(); // DEBUG
//	return d2; 
//	cout << "the 0-th iteration, kl: " << kl << "\tbl: " << bl << endl; getchar(); // DEBUG
	// the updated position considering the drift distance
	double xNew[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double yNew[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	int nF_ = gGOOD;
	if(2 == gGOOD) nF_ = 4;
	const int nF = nF_;
	const int n = pow(2, nF);
//	cout << "n: " << n << endl; // DEBUG
	double d2min = 1e200;
	short sign[nF], signMin[nF]{};
	const double deno0 = kl / sqrt(1. + kl * kl), deno1 = 1. / sqrt(1. + kl * kl);
	for(int j = 0; j < n; j++){
		for(int k = 0; k < nF; k++){
			sign[k] = ((j>>k)&0x1)*2 - 1;
			xNew[LAYER[k]] = x[LAYER[k]] + sign[k] * r[LAYER[k]] * deno0;
			yNew[LAYER[k]] = y[LAYER[k]] + sign[k] * r[LAYER[k]] * deno1;
		} // end for over k
		d2 = Dsquare(xNew, yNew, kl, bl, gGOOD, LAYER, d2PerDot); // update kl and bl
		if(d2 < d2min){
			d2min = d2;
			for(int k = 0; k < nF; k++) signMin[k] = sign[k];
			kL = kl; bL = bl;
//			cout << "o.kl: " << kl << "\tbl: " << bl << endl; // DEBUG
//			cout << "d2: " << d2 << endl; // DEBUG
//			getchar(); // DEBUG
		} // end if
	} // end for over j
//	cout << "0.iter: kL: " << kL << "\tbL: " << bL << "\td2min: " << d2min << endl; // DEBUG
//	getchar(); // DEBUG
	// update kL and bL using themselves
	for(int k = 0; k < nF; k++){
		xNew[LAYER[k]] = x[LAYER[k]] + signMin[k] * r[LAYER[k]] * kL / sqrt(1. + kL * kL);
		yNew[LAYER[k]] = y[LAYER[k]] + signMin[k] * r[LAYER[k]] / sqrt(1. + kL * kL);
	} // end for over k
	d2min = Dsquare(xNew, yNew, kL, bL, gGOOD, LAYER, d2PerDot); // update kl and bl
//	cout << "1.iter: kL: " << kL << "\tbL: " << bL << "\td2min: " << d2min << endl; getchar(); // DEBUG
	return d2min;
} // end of function iterativeFit






