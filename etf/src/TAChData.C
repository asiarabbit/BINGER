///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAChData.C														 //
//   TAChData.C -- source file for class TAChData									 //
//   Introduction: base class for holding experiment data per channel, also			 //
// responsible for data initialization, caching data, data access during event		 //
// reconstruction, and self-marking according to requirements of the code.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/30.															     //
// Last modified: 2017/9/30, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <cmath>
#include "TAChData.h"
#include "TAPopMsg.h"
#include "tEntry.h"

using std::cout;
using std::endl;

TAChData::TAChData(const string &name, const string &title, unsigned uid)
			: TAStuff(name, title, uid){
	Initialize();
}
TAChData::~TAChData(){
}

// get the n-th pulse
double TAChData::GetLeadingTime(int n) const{
	if(n > fNLeadingEdge - 1){
//		TAPopMsg::Error(GetName().c_str(), "GetLeadingTime: %d-th pulse not assigned", n);
		return -9999.;
	}
	if(-9999. == fLeadingTime[0])
		TAPopMsg::Error(GetName().c_str(), "GetLeadingTime: Leading time array is null");
	return fLeadingTime[n];
}
// t0, t1 and t2 are set for choosing ch->GetLT over edges
// (ch->GetLT-t0) within t1 and t2 is chosen. t1 and t2 using default values, choose the 1st edge
double TAChData::GetLT(double t0, double t1, double t2) const{
	const int nl = GetNLeadingEdge();
	double tmp, dt, lt = GetLeadingTime(); // default is the 0-th edge leading edge time
	if(-9999. != t0 && -9999. == t1 && -9999. == t2){ // only the t-start is given
		// select the closest edge to t0 //
//		cout << "BINGO1" << endl; getchar(); // DEBUG
		double dmin = 1E200;
		for(int i = 0; i < nl; i++){
			tmp = GetLeadingTime(i);
			dt = fabs(tmp - t0);
			if(dt < dmin){
				dmin  = dt;
				lt = tmp;
			}
		}
	}
	if(-9999. != t0 && -9999. != t1 && -9999. != t2){ // three pars are all given
		// choose the one that is just within the range and exit the loop
		// t1 is supposed to be no greater than t2
//		cout << "BINGO2" << endl; getchar(); // DEBUG
		if(t1 > t2){ // swap t1 and t2
			tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		for(int i = 0; i < nl; i++){
			const double t = GetLeadingTime(i);
			tmp = t - t0;
			if(tmp > t1 && tmp < t2){
				lt = t;
				break;
			}
		}
	} // end outer else
//	Show(); // DEBUG
//	cout << "t0: " << t0 << "\tt1: " << t1 << "\tt2: " << t2 << endl; // DEBUG
//	cout << "lt: " << lt << endl; getchar(); // DEBUG
	return lt;
}
// get the n-th pulse
double TAChData::GetTrailingTime(int n) const{
	if(n > fNTrailingEdge - 1){
//		TAPopMsg::Error(GetName().c_str(), "GetTrailingTime: %d-th pulse not assigned", n);
		return -9999.;
	}
	if(-9999. == fTrailingTime[0])
		TAPopMsg::Error(GetName().c_str(), "GetTrailingTime: Trailing time array is null.");
	return fTrailingTime[n];
}
// return fEventIndex.
int TAChData::GetEventIndex() const{
	if(fEventIndex < 0) TAPopMsg::Error(GetName().c_str(), "event index not assigned");
	return fEventIndex;
}

// initialize the data member
void TAChData::Initialize(){
	for(double &x : fLeadingTime) x = -9999.;
	for(double &x : fTrailingTime) x = -9999.;
	fFired = false;
	fNLeadingEdge = -1; fNTrailingEdge = -1;
	fIs_V = false; fEventIndex = -1;
}
bool TAChData::Assign(tEntry *entry){
	if(entry->nl >= 1 && entry->nt >= 0){
		fNLeadingEdge = entry->nl < NLMAX ? entry->nl : NLMAX;
		memcpy(fLeadingTime, entry->leadingTime, fNLeadingEdge * sizeof(double));
		fNTrailingEdge = entry->nt < NTMAX ? entry->nt : NTMAX;
		memcpy(fTrailingTime, entry->trailingTime, fNTrailingEdge * sizeof(double));

		fFired = true;
		fIs_V = entry->is_V;
		fEventIndex = entry->index;
		strcpy(entry->name, GetName().c_str());

		return true;
	}

	return false;
}

// for debugging purposes
void TAChData::Show() const{
	cout << std::ios_base::boolalpha;
	TAPopMsg::Info(GetName().c_str(), "Show(): The assigned data for the channel is listed as follow:");
	cout << "fFired: " << fFired << "\tfNLeadingEdge: " << fNLeadingEdge;
	cout << "\tfNTrailingEdge: " << fNTrailingEdge << endl;
	for(int i = 0; i < fNLeadingEdge; i++)
		cout << "fLeadingTime[" << i << "]: " << fLeadingTime[i] << endl;
	for(int i = 0; i < fNTrailingEdge; i++)
		cout << "fTrailingTime[" << i << "]: " << fTrailingTime[i] << endl;
	cout << "fIs_V: " << fIs_V << endl;
	cout << "fEventIndex: " << fEventIndex << endl;
} // end of member function Show.







