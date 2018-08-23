///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/two.C														 //
//   two.C -- source file for method Dsquare2 and minid2_2							 //
//   Introduction: Definition of function double Dsquare2(...). Linear least square	 //
// method fitting for four points that belongs to only two DCs and calculate their	 //
// fitting chi2s.																	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/3/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

double Dsquare2(const double *x, const double *y, double &k, double &b, const int *LAYER, double DsqThrePerDot){

	double result1 = 0., result2 = 0., result = 0.; // d2 for the two methods implemented hereby.
	double dsquare = 0.;
	double denominator = 0.;
	double increment = 0.; // a temporary variable.
//	DsqThrePerDot = 20.; // for gGOOD == 2 specifically.

	/////////////////////----------------------- least square method ------------//////////////////////////////
	const int n = 6; // six anode layers
	double punishment = 1E10; // individual anodes exceeding DsquThre would recieve a punishment in dsquare. See the code.
	double k1m = 0., b1m = 0.;
	double SUMx = 0., SUMy = 0., SUMxx = 0., SUMxy = 0.;
	for(int i = 0; i < n; i++){
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2] || i == LAYER[3]){ // X1 of each MWDC // JULY THE FIRST //
			SUMx  += x[i];
			SUMy  += y[i];
			SUMxx += x[i] * x[i];
			SUMxy += x[i] * y[i];
		} // JULY THE FIRST //
	}
	// The function of the line to be fitted is y = kx + b
	k1m = (4. * SUMxy - SUMx * SUMy) / (4. * SUMxx - SUMx * SUMx);
	if(fabs(k1m) > 1E4){
		if(k1m > 1E4)
			k1m = 1E4;
		else if(k1m < -1E4)
			k1m = -1E4;
		b1m = -k1m * x[0];
	}
	else
		b1m = (SUMxx * SUMy - SUMxy * SUMx) / (4. * SUMxx - SUMx * SUMx);
	// calculate the deviation
	denominator = k1m * k1m + 1.;
	for(int i = 0; i < 6; i++){
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2] || i == LAYER[3]){ // X1 of each MWDC // JULY THE FIRST //
			increment = (k1m * x[i] - y[i] + b1m) * (k1m * x[i] - y[i] + b1m) / denominator; 
			if(increment > DsqThrePerDot){
				increment *= punishment;
			} // end if
			dsquare += increment;
			
		} // JULY THE FIRST //
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
	// here we use the central point of the two fired anodes in X1 and X2 plane in the two MWDCs selected above respectively.
	double x1 = (x[LAYER[0]] + x[LAYER[1]]) / 2., y1 = (y[LAYER[0]] + y[LAYER[1]]) / 2.;
	double x2 = (x[LAYER[2]] + x[LAYER[3]]) / 2., y2 = (y[LAYER[2]] + y[LAYER[3]]) / 2.;
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
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2] || i == LAYER[3]){ // X1 of each MWDC // JULY THE FIRST //
			increment = (k2m * x[i] - y[i] + b2m) * (k2m * x[i] - y[i] + b2m) / denominator;
			if(increment > DsqThrePerDot){
				increment *= punishment;
			} // end if
			dsquare += increment;
		}
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


	if(result > 100){ // 5*5*4 = 100
//		cout << "Fitting has failed. dsquare = " << result << endl;
	}

	return result;
} // end of function Dsquare

double minid2_2(const double *x, const double *y, const double *r, double k, double b, const int *LAYER){
	double temp = 0;
	double d2 = 0.;
	for(int i = 0; i < 6; i++){
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2] || i == LAYER[3]){ // X1 of each MWDC // JULY THE FIRST //
			temp = fabs(k * x[i] - y[i] + b) / sqrt(1. + k * k) - r[i];
			d2 += temp * temp;
		} // end if
	} // end for over i
	
	return d2;
} // end of function minid2_2


