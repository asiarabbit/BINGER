///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/one.C														 //
//   one.C -- source file for method Dsquare1 and minid2_1							 //
//   Introduction: Definition of function double Dsquare1(...) and double 			 //
// minid2_1(...). These two methods are specifically written for DCArrs (2 DCs)		 //
// around the target, for the scenario that only three sense wire layers are fired,	 //
// i.e. two	hits in one DC and one hit in another.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/22.															     //
// Last modified: 2018/4/9, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

double Dsquare1(const double *x, const double *y, double &k, double &b, const int *LAYER, double DsqThrePerDot){

	double result1 = 0., result2 = 0., result = 0.; // d2 for the two methods implemented hereby
	double dsquare = 0.;
	double denominator = 0.;
	double increment = 0.; // a temporary variable

	/////////////////////----------------------- least square method ------------//////////////////////////////
	const int n = 6; // six anode layers
	double punishment = 1E10; // individual anodes exceeding DsquThre would recieve a punishment in dsquare. See the code.
	double k1m = 0., b1m = 0.;
	double SUMx = 0., SUMy = 0., SUMxx = 0., SUMxy = 0.;
	for(int i = 0; i < n; i++){
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2]){
			SUMx  += x[i];
			SUMy  += y[i];
			SUMxx += x[i] * x[i];
			SUMxy += x[i] * y[i];
		}
	}
	// The function of the line to be fitted is y = kx + b
	k1m = (3. * SUMxy - SUMx * SUMy) / (3. * SUMxx - SUMx * SUMx);
	if(fabs(k1m) > 1E4){
		if(k1m > 1E4)
			k1m = 1E4;
		else if(k1m < -1E4)
			k1m = -1E4;
		b1m = -k1m * x[0];
	}
	else
		b1m = (SUMxx * SUMy - SUMxy * SUMx) / (3. * SUMxx - SUMx * SUMx);
	// calculate the deviation
	denominator = k1m * k1m + 1.;
	for(int i = 0; i < 6; i++){
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2]){
			increment = (k1m * x[i] - y[i] + b1m) * (k1m * x[i] - y[i] + b1m) / denominator;
			if(increment > DsqThrePerDot){
				increment *= punishment;
			} // end if
			dsquare += increment;
			
		}
	}
	result1 = dsquare;
//	cout << "For the almost vertical line case, the least square method yield result as below: " << endl;
//	cout << "kc = " << k1m << " bc = " << b1m << endl;

	//////////////////////////-------------- the classcical method ---------------//////////////////////////////	
	// extract the two MWDCs with both of their anode planes fired
	int N1[2] = {-1, -1}; // serial number of the MWDC with both of its anode planes fired
	int temp = 0;
	if(x[0] != -9999. && x[1] != -9999.) N1[temp++] = 0; // DC0
	if(x[2] != -9999. && x[3] != -9999.){
		if(temp < 1){
			N1[temp++] = 1; // DC1
		}
		else{
			cout << endl << "temp: " << temp << endl;
			cout << "Dsquare1, more than one MWDC are fired in both planes." << endl; // DEBUG
			getchar(); // DEBUG
		} // end else
	} // end if
	if(x[4] != -9999. && x[5] != -9999.){
		if(temp < 1){
			N1[temp++] = 2; // DC2
		}
		else{
			cout << endl << "temp: " << temp << endl;
			cout << "Dsquare1, more than one MWDC are fired in both planes." << endl; // DEBUG
			getchar(); // DEBUG
		} // end else
	} // end if
	// here we use the central point of the two fired anodes in X1 and X2 plane in the MWDC selected above
	double x1 = (x[2 * N1[0]] + x[2 * N1[0] + 1]) / 2., y1 = (y[2 * N1[0]] + y[2 * N1[0] + 1]) / 2.;
	// pick out the single one
	int singleLAYER = -1;
	for(int i = 3; i--;){
		if(LAYER[i] != 2 * N1[0] && LAYER[i] != 2 * N1[0] + 1) singleLAYER = LAYER[i];
	} // end for over i
	if(singleLAYER < 0){
		cout << "Dsquare1, after assignment, singleLAYER is still minus: " << singleLAYER << endl;
	}
	double x2 = x[singleLAYER], y2 = y[singleLAYER];
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
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2]){
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

double minid2_1(const double *x, const double *y, const double *r, double k, double b, const int *LAYER){
	double temp = 0;
	double d2 = 0.;
	for(int i = 0; i < 6; i++){
		if(i == LAYER[0] || i == LAYER[1] || i == LAYER[2]){
			temp = fabs(k * x[i] - y[i] + b) / sqrt(1. + k * k) - r[i];
			d2 += temp * temp;
		} // end if
	} // end for over i

	return d2;
} // end of function minid2_1


