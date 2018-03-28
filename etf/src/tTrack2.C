///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/tTrack2.C														     //
//   tTrack2.C -- source file for struct tTrack2								     //
//   Introduction: struct for holding track information. The struct is mainly for	 //
// assigning TTree objects and output of track parameters. Different from tTrack,	 //
// this struct serves for DCs around the target.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/22.															     //
// Last modified: 2018/3/23, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include "tTrack2.h"

using std::cout;
using std::endl;

tTrack2::tTrack2() : tTrack(){} // the default constructor
void tTrack2::show() const{
	cout << "name: " << name << endl;
	cout << "type: " << type << endl;
	cout << "index: " << index << endl;
	cout << "fFiredAnodeLayer: " << nFiredAnodeLayer << endl;
	cout << "nu:" << endl;
	for(int x : nu) cout << x << "\t";
	cout << endl;
	cout << "LAYER:" << endl;
	for(int x : LAYER) cout << x << "\t";
	cout << endl;
	cout << "z:" << endl;
	for(double x : z) cout << x << "\t";
	cout << endl;
	cout << "x:" << endl;
	for(double z : x) cout << z << "\t";
	cout << endl;
	cout << "t:" << endl;
	for(double x : t) cout << x << "\t";
	cout << endl;
	cout << "w:" << endl;
	for(double x : w) cout << x << "\t";
	cout << endl;
	cout << "r:" << endl;
	for(double x : r) cout << x << "\t";
	cout << endl;
	cout << "chi:" << endl;
	for(double x : chi) cout << x << "\t";
	cout << endl;
	cout << "TOF: " << TOF << endl;
	cout << "k: " << k << endl;
	cout << "b: " << b << endl;
	cout << "dsquare: " << dsquare << endl;
	cout << "chi2: " << chi2 << endl;
	cout << "id: " << id << endl;
	cout << "beta: " << beta << endl;
	getchar();
} // end of function show
double tTrack2::dcTOTAvrg() const{
	double sum = 0.; int cnt = 0;
	for(double x : dcTOT){
		if(-9999. != x){
			sum += x;
			cnt++;
		}
	}
	if(0 == cnt) return -9999.;
	return sum / cnt;
}






