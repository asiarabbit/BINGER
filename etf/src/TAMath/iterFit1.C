///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/iterFit1.C													 //
//   iterFit1.C -- source file for method TAMath::iterFit (for backup use)			 //
//   Introduction: an iterative and asymptotic fitting method. It is precise enough, //
// and currently is used for providing a starting point for further fittings.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2017/10/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

double TAMath::iterativeFit(const double *x, const double *y, const double *r, double &kL, double &bL, int gGOOD, const int *LAYER, double d2PerDot){
//	cout << "Welcome, this is IterFit." << endl; getchar(); // DEBUG
	double kl, bl, km, bm;
	double d2, d2min = 1E200;
	d2 = Dsquare(x, y, kl, bl, gGOOD, LAYER, d2PerDot); // set the iteration origin - kl and bl
//	cout << "the 0-th iteration, kl: " << kl << "\tbl: " << bl << endl; getchar(); // DEBUG
	// the updated position considering the drift distance
	double xNew[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double yNew[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	short sign[6];
	for(int i = 0; i < 50; i++){ // i: the iteration times
		// chose a sign combination to solve the left-right ambiguity
		for(sign[0] = -1; sign[0] <= 1; sign[0]+=2)
		for(sign[1] = -1; sign[1] <= 1; sign[1]+=2)
		for(sign[2] = -1; sign[2] <= 1; sign[2]+=2)
		for(sign[3] = -1; sign[3] <= 1; sign[3]+=2)
		for(sign[4] = -1; sign[4] <= 1; sign[4]+=2)
		for(sign[5] = -1; sign[5] <= 1; sign[5]+=2){
			// update x, y coordinates using drift distance information
			for(int j = 0; j < 6; j++){
				if(x[j] != -9999.){				
					xNew[j] = x[j] + sign[j] * r[j] / sqrt(1. + kl * kl);
					yNew[j] = y[j] - sign[j] * r[j] * kl / sqrt(1. + kl * kl);
				} // end if
			} // end for over j
			// update kl and bl
			d2 = Dsquare(xNew, yNew, kl, bl, gGOOD, LAYER, d2PerDot);
			// pick the best one
			if(d2 < d2min){
				d2min = d2;
				km = kl;
				bm = bl;
//				cout << "sign: \n"; // DEBUG
//				for(short s: sign) cout << s << "\t"; // DEBUG
//				cout << "\ni: " << i << "\td2min: " << d2min << endl; // DEBUG
//				cout << "km: " << km << "\tbm: " << bm << endl; getchar(); // DEBUG
			} // end if
		} // end the multi-nested loops
		// reset the start point for next iteration
		kl = km;
		bl = bm;
//		cout << "the " << i << "-th iteration, km: " << km << "\tbm: " << bm << endl; getchar(); // DEBUG
	} // end for over i
//	cout << "the final k and b:\n"; // DEBUG
	kL = km; bL = bm;
//	cout << "kL: " << kL << "\tbL: " << bL << endl; getchar(); // DEBUG
	return d2;
} // end of function iterativeFit.
