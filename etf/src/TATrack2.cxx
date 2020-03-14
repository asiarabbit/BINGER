///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TATrack2.cxx																											 //
//   TATrack2.cxx -- source file for class TATrack2																	 //
//   Introduction: A derived class from TATrack, specifically designed for DCs			 //
// around the target. This class is mainly for track fitting of those DCs.					 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2018/3/21.																													     //
// Last modified: 2018/4/4, SUN Yazhou.																							 //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <cstdlib>

#include "TATrack2.h"
#include "TAPopMsg.h"
#include "tTrack.h"

TATrack2::TATrack2(const string &name, const string &title, unsigned uid)
		: TATrack(name, title, uid){
}

// the copy constructor
TATrack2::TATrack2(const TATrack2 &track) : TATrack(track){
}
// the assignment constructor
TATrack2 &TATrack2::operator=(const TATrack2 &track){
	TATrack::operator=(track);
	return *this;
}
TATrack2::~TATrack2(){}

// since that TATrack2 class processes 4-d arrays, some of the methods //
// involving in TATrack would be re-written //

// output array fNu
void TATrack2::GetNu(int *nu) const{
	for(int i = 0; i < 4; i++) nu[i] = fNu[i];
}
void TATrack2::GetLAYER(int *LAYER) const{
	for(int i = 0; i < 4; i++) LAYER[i] = fLAYER[i];
} // end of function GetLAYER
void TATrack2::GetZ(double *z) const{ // output array fZ
	for(int i = 0; i < 4; i++) z[i] = fZ[i];
} // end of function GetZ
void TATrack2::GetX(double *x) const{ // output array fX
	for(int i = 0; i < 4; i++) x[i] = fX[i];
} // end of function GetX
void TATrack2::GetDriftTime(double *t) const{
	for(int i = 0; i < 4; i++) t[i] = fT[i];
} // end of function GetDriftTime()
void TATrack2::GetDriftDistance(double *r) const{
	for(int i = 0; i < 4; i++) r[i] = fR[i];
} // end of function GetDriftDistance()
void TATrack2::GetChi(double *chi){ // output array fChi
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetChi(double *): Data not assigned.");
		return;
	} // end of if
	if(!fIsFitted) Fit();

	for(int i = 0; i < 4; i++) chi[i] = fChi[i];
} // end function GetChi
void TATrack2::SetData(const double *x, const double *z, const double *t, const double *r, double kL, double bL, double dsquare, int gGOOD, const int *nu, const int *LAYER, const double *weight){
	if(1 != gGOOD && 2 != gGOOD) TAPopMsg::Error("TATrack2", "SetData: gGOOD is abnormal (only 1 and 2 are allowed: %d): ", gGOOD);
	double x2[6] = {x[0], x[1], x[2], x[3], -9999., -9999.};
	double z2[6] = {z[0], z[1], z[2], z[3], -9999., -9999.};
	double t2[6] = {t[0], t[1], t[2], t[3], -9999., -9999.};
	double r2[6] = {r[0], r[1], r[2], r[3], -9999., -9999.};
	int nu2[6] = {nu[0], nu[1], nu[2], nu[3], -1, -1};
	int LAYER2[6] = {LAYER[0], LAYER[1], LAYER[2], LAYER[3], -1, -1};
	if(!weight)
		return TATrack::SetData(x2, z2, t2, r2, kL, bL, dsquare, gGOOD, nu2, LAYER2);
	double weight2[6] = {weight[0], weight[1], weight[2], weight[3], 1., 1.};
	TATrack::SetData(x2, z2, t2, r2, kL, bL, dsquare, gGOOD, nu2, LAYER2, weight2);
}
void TATrack2::SetDriftTime(const double *t, const double *w){
	double t2[6] = {t[0], t[1], t[2], t[3], -9999., -9999.};
	if(!w) return TATrack::SetDriftTime(t2, w);
	double w2[6] = {w[0], w[1], w[2], w[3], 1., 1.};
	return TATrack::SetDriftTime(t2, w2);
}
void TATrack2::SetDriftDistance(const double *r){
	double r2[6] = {r[0], r[1], r[2], r[3], -9999., -9999.};
	return SetDriftDistance(r2);
}
// assign tTrack2
void TATrack2::AssignTrack(tTrack *ptrack){
	strcpy(ptrack->name, GetName().c_str());
	GetNu(ptrack->nu);
	GetLAYER(ptrack->LAYER);
	GetZ(ptrack->z);
	GetX(ptrack->x);
	GetDriftTime(ptrack->t);
	for(int i = 0; i < 4; i++) ptrack->w[i] = fWeight[i];
	GetDriftDistance(ptrack->r);
	ptrack->TOF = GetTOF();
//	ptrack->nStripStray = GetNStripStray();
//	ptrack->firedStripId = GetFiredStripId();
	ptrack->gGOOD = GetgGOOD();
	ptrack->nFiredAnodeLayer = GetNFiredAnodeLayer();
	ptrack->k = GetSlope();
	ptrack->b = GetIntercept();
	ptrack->dsquare = GetDsquare();
	GetChi(ptrack->chi);
	ptrack->Chi = GetChi();
	ptrack->chi2 = GetChi2();
//	ptrack->id = Get3DId();
//	memcpy(ptrack->xMiss3D, fXMiss3D, sizeof(fXMiss3D));
}

// show information about the track
void TATrack2::Show(){
	Fit(); // Then all the member variables deserving assignemnts would be assigned accordingly.
	cout << "\033[32;1m" << GetName() << "\033[0m" << endl;
	cout << "TATrack2::Show(): Show information about the track." << endl;
	cout << "Dsquare: " << GetDsquare() << endl;
	cout << "gGOOD: " << GetgGOOD() << endl;
	cout << "NFiredAnodeLayer: " << GetNFiredAnodeLayer() << endl;
	cout << "TOF: " << GetTOF() << endl;
	cout << "Nu:" << endl;
	for(int x: fNu) cout << x << "\t";
	cout << endl;
	cout << "Drift Time:" << endl;
	for(double tt : fT) cout << tt << "\t";
	cout << endl;
	cout << "Weight for each measurement:" << endl;
	for(double w : fWeight) cout << w << "\t";
	cout << endl;
	cout << "Drift Distance:" << endl;
	for(double rr : fR) cout << rr << "\t";
	cout << endl;
	cout << "fChi:" << endl;
	for(double cc : fChi) cout << cc << "\t";
	cout << endl;
	cout << "DsquareThresholdPerDot: " << GetDsquareThresholdPerDot() << endl;
	cout << "fXc: " << fXc << endl;
	cout << "fZc: " << fZc << endl;
	cout << "Slope: " << GetSlope() << endl;
	cout << "Intercept: " << GetIntercept() << endl;
	cout << "Chi2: " << GetChi2() << endl;
	cout << "Chi: " << GetChi() << endl;
	cout << "IsAssigned: " << IsAssigned() << endl;
	cout << "IsFitted: " << IsFitted() << endl;
	if(fFitMethod == kIterFit) cout << "FitMethod: kIterFit" << endl;
	else if(fFitMethod == kBFGSFit) cout << "FitMethod: kBFGSFit" << endl;
	else if(fFitMethod == kNormalFit) cout << "FitMethod: kNormalFit" << endl;
	else cout << "FitMethod: " << fFitMethod << endl;
	getchar();
} // end function Show().
