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
	double kl, bl, d2, d2_tmp;
	d2 = Dsquare(x, y, kl, bl, gGOOD, LAYER, d2PerDot); // set the iteration origin - kl and bl
//	kL = kl; bL = bl; return d2; // DEBUG
//	cout << "the 0-th iteration, kl: " << kl << "\tbl: " << bl << endl; getchar(); // DEBUG
	double xd; // abscissa of the foot point
	// the updated position considering the drift distance
	double xNew[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double yNew[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	short sign = 0;
	for(int i = 0; i < 10; i++){ // i: the iteration times
		// update x, y coordinates using drift distance information
		for(int j = 0; j < 6; j++){
			if(x[j] != -9999.){
				xd = (kl * y[j] + x[j] - kl * bl) / (1. + kl * kl);
				if(x[j] < xd) sign = 1;
				else sign = -1;
//				cout << "sign: " << sign << endl; getchar(); // DEBUG

				xNew[j] = x[j] + sign * r[j] / sqrt(1. + kl * kl);
				yNew[j] = y[j] - sign * r[j] * kl / sqrt(1. + kl * kl);
			} // end if
		} // end for over j
		d2_tmp = d2;
		d2 = Dsquare(xNew, yNew, kl, bl, gGOOD, LAYER, d2PerDot); // update kl and bl
//		cout << "fabs((d2_tmp - d2) / d2): " << fabs((d2_tmp - d2) / d2) << endl;
		if(fabs((d2_tmp - d2) / d2) < 0.001 && i > 0) break;

//		cout << "the " << i << "-th iteration, kl: " << kl << "\tbl: " << bl << endl; getchar(); // DEBUG
	} // end for over i
//	cout << "the final k and b:\n"; // DEBUG
	kL = kl; bL = bl;
//	cout << "kL: " << kL << "\tbL: " << bL << endl; getchar(); // DEBUG
	return d2;
} // end of function iterativeFit.
