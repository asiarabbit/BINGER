///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TATrack4.C														 //
//   TATrack4.C -- source file for class TATrack4									 //
//   Introduction: A derived class from TATrack, specifically designed for DC array  //
// fabracated by MA. This class is mainly for track fitting of those DCs.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/9/3.															     //
// Last modified: 2018/9/3, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <cstdlib>

// ROOT include
#include "TGraph.h"

#include "TATrack4.h"
#include "TAPopMsg.h"
#include "TAMath.h"
#include "tTrack.h"
#include "TAGPar.h"

using std::cout;
using std::endl;

static const TAGPar *gp = TAGPar::Instance();

TATrack4::TATrack4(const string &name, const string &title, unsigned uid)
		: TATrack(name, title, uid){
}
TATrack4::~TATrack4(){}

double TATrack4::GetDsquare(){
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetDsquare(): Data not assigned.");
		return -9999.;
	} // end of if
	if(fDsquare <= 0){
		int gGOOD = GetgGOOD();
		double d2ThrePp = GetDsquareThresholdPerDot();
		fDsquare = TAMath::Dsquare8(fZ, fX, fK, fB, gGOOD, fLAYER, d2ThrePp);
	} // end if(fDsquare <= 0)
	return fDsquare;
} // end of function GetDsquare
void TATrack4::GetNu(int *nu) const{
	for(int i = 0; i < 8; i++) nu[i] = fNu[i];
}
void TATrack4::GetLAYER(int *LAYER) const{
	for(int i = 0; i < 8; i++) LAYER[i] = fLAYER[i];
} // end of function GetLAYER
void TATrack4::GetZ(double *z) const{ // output array fZ
	for(int i = 0; i < 8; i++) z[i] = fZ[i];
} // end of function GetZ
void TATrack4::GetX(double *x) const{ // output array fX
	for(int i = 0; i < 8; i++) x[i] = fX[i];
} // end of function GetX
void TATrack4::GetDriftTime(double *t) const{
	for(int i = 0; i < 8; i++) t[i] = fT[i];
} // end of function GetDriftTime()
void TATrack4::GetDriftDistance(double *r) const{
	for(int i = 0; i < 8; i++) r[i] = fR[i];
} // end of function GetDriftDistance() 
void TATrack4::GetChi(double *chi){ // output array fChi
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetChi(double *): Data not assigned.");
		return;
	} // end of if
	if(!fIsFitted) Fit();

	for(int i = 0; i < 8; i++) chi[i] = fChi[i];
} // end function GetChi

// x, z: fired anode position{} r: drift distance
void TATrack4::SetData(const double *x, const double *z, const double *t, const double *r, double kL, double bL, double dsquare, int gGOOD, const int *nu, const int *LAYER, const double *weight){
	int temp_good = 0;
	int nFiredAnodeLayer = -1; // count of fired andoe layers
	for(int i = 0; i < 8; i++){
		fX[i] = x[i];
		fZ[i] = z[i];
		fT[i] = t[i];
		fWeight[i] = weight[i]; // weight for weighted addition of chi to chi2
		fR[i] = r[i];
		fNu[i] = nu[i];
		fLAYER[i] = LAYER[i];
		if(fNu[i] >= 0) temp_good++;
	} // end for over i
	// gGOOD == 2 specially marks the kind of tracks where two MWDCs are both-anode-layer fired.
	nFiredAnodeLayer = gGOOD;
	if(2 == gGOOD) nFiredAnodeLayer = 4;
	if(1 == gGOOD) nFiredAnodeLayer = 3;
	if(temp_good != nFiredAnodeLayer){
		TAPopMsg::Warn(GetName().c_str(), "SetData: the input count of fired anode layers disagrees with the one obtained from counting non-zero elements of fired anode serial id array.\nThe counting result of count of fired andoe layers: %d\ngGOOD:%d\nThe input count of fired anode layers: %d", temp_good, gGOOD, nFiredAnodeLayer);
	} // end if(temp_good != gGOOD)
	fgGOOD = gGOOD;
	fNFiredAnodeLayer = nFiredAnodeLayer;
	fK = kL; fB = bL; // kL and bL are values derived from leastest square fit of fired anodes
	fDsquare = dsquare; fChi2 = -9999.;
	fIsFitted = false; fIsAssigned = true;
} // end of function SetData(.....)

void TATrack4::SetDriftTime(const double *t, const double *w){ // as the name indicates
	for(int i = 0; i < 8; i++){
		if((fNu[i] > 0 && -9999. == t[i]) || (fNu[i] < 0 && -9999. != t[i]))
			TAPopMsg::Error(GetName().c_str(),
				"SetDriftTime: t[%d] here contradicates with former assignment of array nu. fNu[%d]: %d - t[%d]: %f", i, i, fNu[i], i, t[i]);
		fT[i] = t[i];
		if(w) fWeight[i] = w[i];
	} // end for over i
	fIsFitted = false;
} // end of function SetDriftTime()
void TATrack4::SetTOF(double TOF, int firedStripId, double nstripsStray){
	fTOF = TOF;
	fFiredStripId = firedStripId;
	fNStripStray = nstripsStray;
} // end of function SetTOF
void TATrack4::SetDriftDistance(const double *r){ // as the name indicates
	for(int i = 0; i < 8; i++){
		if(fNu[i] * r[i] < 0){
			TAPopMsg::Error(GetName().c_str(),
				"SetDriftDistance: r[%d] here contradicates with former assignment of array nu. fNu[%d]: %d - r[%d]: %f", i, i, fNu[i], i, r[i]);
		} // end if
		fR[i] = r[i];
	} // end for over i
	fIsFitted = false;
} // end of function SetDriftDistance()
void TATrack4::AssignTrack(tTrack *ptrack){ // assign tTrack
	strcpy(ptrack->name, GetName().c_str());
	GetNu(ptrack->nu);
	GetLAYER(ptrack->LAYER);
	GetZ(ptrack->z);
	GetX(ptrack->x);
	GetDriftTime(ptrack->t);
	for(int i = 0; i < 8; i++) ptrack->w[i] = fWeight[i];
	GetDriftDistance(ptrack->r);
	ptrack->TOF = GetTOF();
	ptrack->nStripStray = GetNStripStray();
	ptrack->firedStripId = GetFiredStripId();
	ptrack->gGOOD = GetgGOOD();
	ptrack->nFiredAnodeLayer = GetNFiredAnodeLayer();
	ptrack->k = GetSlope();
	ptrack->b = GetIntercept();
	ptrack->dsquare = GetDsquare();
	GetChi(ptrack->chi);
	ptrack->Chi = GetChi();
	ptrack->chi2 = GetChi2();
	ptrack->id = Get3DId();
	memcpy(ptrack->xMiss3D, fXMiss3D, sizeof(fXMiss3D));
} // end of function AssignTrack.


void TATrack4::Fit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "Fit: the track has not been assigned.");
	if(IsFitted()) return;
	if(fFitMethod != kNormalFit && fFitMethod != kBFGSFit && fFitMethod != kIterFit){
		TAPopMsg::Error(GetName().c_str(), "Fit: The Fit method is invalid.\n Accepted value:\n");
		cout << "TATrack4::kNormalFit: " << kNormalFit;
		cout << "\tTATrack4::kBFGSFit: " << kBFGSFit;
		cout << "\tTATrack4::kIterFit: " << kIterFit;
		getchar();
	}
	// to tell whether drift time has been assigned //
	int cnt = 0; // count of void r[i]
	for(double x : fR) if(-9999 == x) cnt++;
	if(8 == cnt){
		fChi2 = 0.; GetDsquare();
		for(int i = 0; i < 8; i++){
			if(fLAYER[i] != -1){
				fChi[fLAYER[i]] = fabs(fK * fZ[fLAYER[i]] - fX[fLAYER[i]] + fB) / sqrt(1. + fK * fK);
				fChi2 += fChi[fLAYER[i]] * fChi[fLAYER[i]];
			}
		}
		fIsFitted = true; return;
	} // end if(6 == cnt)
	// // FIT // //
	if(kNormalFit == fFitMethod) NormalFit();
	else if(kBFGSFit == fFitMethod) BFGSFit();
	else IterFit();
	// assign array fChi
	for(double &xx : fChi) xx = -9999.;
	fChi2 = 0.;
//	fK = 0.225525; fB = -37.8764; // XXX XXX XXX XXX DEBUG DEBUG DEBUG XXX XXX XXX XXX
	for(int i = 0; i < 8; i++){
		if(fLAYER[i] != -1){
			fChi[fLAYER[i]] = fabs(fK * fZ[fLAYER[i]] - fX[fLAYER[i]] + fB) / sqrt(1. + fK * fK) - fR[fLAYER[i]];
			if(fWeight[fLAYER[i]] < 0.)
				TAPopMsg::Error(GetName().c_str(), "Fit: weight value is minus: fWeight[%d]: %f", fLAYER[i], fWeight[fLAYER[i]]);
			fChi2 += fChi[fLAYER[i]] * fChi[fLAYER[i]] * fWeight[fLAYER[i]];
		}
	}

	fIsFitted = true;
} // end of member function void fit().

void TATrack4::BFGSFit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "BFGSFit: the data has not been assigned.");
	if(IsFitted()) return;
	int gGOOD = GetgGOOD();

	// Here it's refinedFitBFGS(fZ, fX, ...), NOT refinedFitBFGS(fX, fZ, ...), because the coordinate system is different from those conventional ones now.
//	TAMath::refinedFitBFGS(fZ, fX, fR, fK, fB, gGOOD, fLAYER, fDsquareThresholdPerDot);
	TAMath::iterativeFit8(fZ, fX, fR, fK, fB, gGOOD, fLAYER, fDsquareThresholdPerDot);

	fIsFitted = true;
} // end of function BFGSFit()
void TATrack4::IterFit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "IterFit: the data has not been assigned.");
	if(IsFitted()) return;
	int gGOOD = GetgGOOD();

	// Here it's refinedFitBFGS(fZ, fX, ...), NOT refinedFitBFGS(fX, fZ, ...), because the coordinate system is different from those conventional ones now.
	TAMath::iterativeFit8(fZ, fX, fR, fK, fB, gGOOD, fLAYER, fDsquareThresholdPerDot);

	fIsFitted = true;
}

// initialization functions
void TATrack4::Initialize(){
	for(int i = 0; i < 8; i++){
		fX[i] = -9999.;
		fZ[i] = -9999.;
		fT[i] = -9999.;
		fWeight[i] = 1.;
		fR[i] = -9999.;
		fNu[i] = -1;
		fLAYER[i] = -1;
		fChi[i] = -9999.;
	} // end for over i
	fTOF = -9999.; fNStripStray = -1.; fFiredStripId = -1; fBeta = -1.;
	fgGOOD = -1; fNFiredAnodeLayer = -1; fK = -9999.; fB = -9999.; fChi2 = -9999.;
	fDsquare = -9999.; fIsFitted = false; fIsAssigned = false; f3DId = -1;
	for(double &x : fXMiss3D) x = -9999.;
} // end of function Initialize().

// a method dedicated for TAVisual::Fill()
void TATrack4::FillTrack(TGraph *gTrack, TGraph *gTrack_R){
	if(!gTrack || !gTrack_R)
		TAPopMsg::Error(GetName().c_str(), "FillTrack: input TGraph pointer is null");
	double z, x, theta; // temporary variables
	// first fill the track line
	// calculate the interpolation pattern for drawing gTrack
	static const double L = gp->Val(31), W = gp->Val(32), M = (L - W) / 2., S = (L + W) / 2.;
	static const int nTr = int(L), nCir = 300; // number of points to be filled
	const double k = GetSlope(), b = GetIntercept();
	for(int i = nTr; i--;){
		z = M + (2.*i/nTr - 1.) * S;
		x = k*z+b;
		gTrack->SetPoint(gTrack->GetN(), z, x);
	}
//	return;
	// next fill the drift distance circle
	for(int i = 8; i--;) if(fR[i] > 0.){
		for(int j = nCir; j--;){
			theta = (2.*j/nCir - 1.) * TAMath::Pi(); // +-Pi
			z = fZ[i] + fR[i] * cos(theta);
			x = fX[i] + fR[i] * sin(theta);
			gTrack_R->SetPoint(gTrack_R->GetN(), z, x);
		}
	}
} // end member method FillTrack






