///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAMath/six.cxx																										 //
//   six.cxx -- source file for method Dsquare6 and minid2_6												 //
//   Introduction: Definition of function double Dsquare6(...). Linear least square	 //
// method fitting for six points that belongs to three DCs and calculate their			 //
// fitting chi2s.																																		 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/10/9.																													     //
// Last modified: 2017/10/9, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

double Dsquare6(const double *x, const double *y, double &k, double &b, double DsqThrePerDot){

	double result1 = 0., result2 = 0., result = 0.; // d2 for the two methods implemented hereby.
	double dsquare = 0.;
	double eta[6]; // square of distance of every point to the fitted line.
	double denominator = 0.;

	/////////////////////----------------------- least square method ------------//////////////////////////////
	int n = 6; // six anode layers
	double punishment = 1E10; // individual anodes exceeding DsquThre would recieve a punishment in dsquare. See the code.
	double k1m = 0., b1m = 0.;
	double SUMx = 0., SUMy = 0., SUMxx = 0., SUMxy = 0.;
	for(int i = 0; i < n; i++){
		SUMx  += x[i];
		SUMy  += y[i];
		SUMxx += x[i] * x[i];
		SUMxy += x[i] * y[i];
	}
	// The function of the line to be fitted is y = kx + b
	k1m = (n * SUMxy - SUMx * SUMy) / (n * SUMxx - SUMx * SUMx);
	if(fabs(k1m) > 1E4){
		if(k1m > 1E4)
			k1m = 1E4;
		else if(k1m < -1E4)
			k1m = -1E4;
		b1m = -k1m * x[0];
	}
	else
		b1m = (SUMxx * SUMy - SUMxy * SUMx) / (n * SUMxx - SUMx * SUMx);
	// calculate the deviation
	denominator = k1m * k1m + 1.;
	dsquare = 0.;
	for(int i = 0; i < 6; i++){
		eta[i] = (k1m * x[i] - y[i] + b1m) * (k1m * x[i] - y[i] + b1m) / denominator;
		if(eta[i] > DsqThrePerDot){
			eta[i] *= punishment;
		} // end if
		dsquare += eta[i];
	}
	result1 = dsquare;

	////////////  track slope would be very small ////////////////
	k = k1m;
	b = b1m;
	return result1;
	/////////// 2018/8/17 ////////////////////////////////////////
//	cout << "For the almost vertical line case, the least square method yield result as below: " << endl;
//	cout << "kc = " << k1m << " bc = " << b1m << endl;

	//////////////////////////-------------- the classcical method ---------------//////////////////////////////
	double x1 = (x[0] + x[1]) / 2., y1 = (y[0] + y[1]) / 2.;
	double x2 = (x[4] + x[5]) / 2., y2 = (y[4] + y[5]) / 2.;
	double k2m = 0., b2m = 0.;
	k2m = (y2 - y1) / (x2 - x1);
	if(k2m > 1E4)
		k2m = 1E4;
	else if(k2m < -1E4)
		k2m = -1E4;
	b2m = y2 - k2m * x2;
	// calculate the deviation
	denominator = k2m * k2m + 1.;
	dsquare = 0.;
	for(int i = 0; i < 6; i++){
		eta[i] = (k2m * x[i] - y[i] + b2m) * (k2m * x[i] - y[i] + b2m) / denominator;
		if(eta[i] > DsqThrePerDot){
			eta[i] *= punishment;
		} // end if
		dsquare += eta[i];
	}
	result2 = dsquare;

	if(result1 <= result2){
		k = k1m;
		b = b1m;
		result = result1;
//		cout << "Least square method wins!." << endl; // DEBUG
	}
	else{
		k = k2m;
		b = b2m;
		result = result2;
//		cout << "The classical method wins!." << endl; // DEBUG
	}

//	for(int i = 0; i < 6; i++){ // DEBUG
//		cout << "x[" << i << "] = " << x[i] << endl; // DEBUG
//		cout << "y[" << i << "] = " << y[i] << endl; // DEBUG
//	} // DEBUG
//	cout << "kc = " << k << " bc = " << b << endl; // DEBUG
//	cout << "dsquare = " << result << endl; // DEBUG
//	getchar(); // DEBUG
/////////////////////-------------------------------------------------------------////////////////////////////////////
/////////////////////-------------------------------------------------------------////////////////////////////////////


	if(result > 150){ // 5*5*6 = 150
//		cout << "Fitting has failed. dsquare = " << result << endl;
	}

	return result;
} // end of function Dsquare

double minid2_6(const double *x, const double *y, const double *r, double k, double b){
	double d2 = 0.;
	double d[6];
	for(int i = 0; i < 6; i++){
		d[i] = fabs(k * x[i] - y[i] + b) / sqrt(1. + k * k) - r[i];
		d2 += d[i] * d[i];
	}

	return d2;
}
