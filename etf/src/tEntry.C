///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/tEntry.C														     //
//   tEntry.C -- source file for struct tEntry									     //
//   Introduction: for holding raw data information of a data section defined in GDAQ//
// system. This class is used in binary data reading and data distribution.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/10/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include "tEntry.h"

using std::cout;
using std::endl;

tEntry::tEntry(){
	initialize();
} // end of the constructor

void tEntry::initialize(){
	index = -1; channelId = -1;
	nl = -1; nt = -1;
	for(int i = 0; i < 10; i++){
		leadingTime[i] = -9999.;
		trailingTime[i] = -9999.;
	} // end for over i
	is_V = false;
	strcpy(name, "");
} // end of function initialize
void tEntry::show() const{
	cout << std::boolalpha << std::fixed << std::setprecision(6) << endl;
	cout << "This is \033[32;1m" << name << "\033[0m." << endl;
	cout << "entry.index:\t\t" << index << endl;
	cout << "entry.channelId:\t" << channelId << endl;
	cout << "entry.nl:\t\t" << nl << endl;
	cout << "entry.nt:\t\t" << nt << endl;
	for(int i = 0; i < nl; i++){
		cout << "leadingTime[" << i << "]:\t" << leadingTime[i] << endl;
	}
	for(int i = 0; i < nt; i++){
		cout << "trailingTime[" << i << "]:\t" << trailingTime[i] << endl;
	}
	cout << "is_V:\t\t" << is_V << endl;
	getchar();
} // end of inline function
double tEntry::TOT() const{
	return trailingTime[0] - leadingTime[0];
} // end function TOT0.
double tEntry::TOT1() const{
	return trailingTime[1] - leadingTime[1];
} // end function TOT1.

