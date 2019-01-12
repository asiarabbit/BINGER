///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TATrack.C															 //
//   TATrack.C -- source file for class TATrack										 //
//   Introduction: class for track fitting, which is mainly a series of minimization //
// algorithms. One of the core classes for the data analysis project.				 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2018/9/30, SUN Yazhou.										     //
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

#include "TATrack.h"
#include "TAPopMsg.h"
#include "TAMath.h"
#include "tTrack.h"
#include "TAGPar.h"

using std::cout;
using std::endl;

static const TAGPar *gp = TAGPar::Instance();

TATrack::TATrack(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid){
	Initialize();
	fXc = -9999.; fZc = -9999.;
	fFitMethod = TATrack::kBFGSFit; fFitPrecision = 0;
	fDsquareThresholdPerDot = 65.;

	fIsDEBUG = false; marked = false;
} // end of the constructor.
// copy constructor
TATrack::TATrack(const TATrack &track){
	*this = track;
} // end of the copy constructor
// assignment constructor
TATrack &TATrack::operator=(const TATrack &track){
	if(this == &track) return *this;
	SetName(track.GetName()); SetTitle(track.GetTitle());
	for(int i = 0; i < 6; i++){
		fZ[i] = track.fZ[i]; fX[i] = track.fX[i];
		fR[i] = track.fR[i]; fT[i] = track.fT[i];
		fWeight[i] = track.fWeight[i];
		fNu[i] = track.fNu[i]; fLAYER[i] = track.fLAYER[i];
		fChi[i] = track.fChi[i];
	} // end for over i
	fTOF = track.fTOF;
	fNStripStray = track.fNStripStray;
	fFiredStripId = track.fFiredStripId;
	fNFiredAnodeLayer = track.fNFiredAnodeLayer;
	fgGOOD = track.fgGOOD;
	fK = track.fK; fB = track.fB;
	fChi2 = track.fChi2; fDsquare = track.fDsquare;
	fIsFitted = track.fIsFitted; fIsAssigned = track.fIsAssigned;
	fFitMethod = track.fFitMethod; fFitPrecision = track.fFitPrecision;
	fZc = track.fZc; fXc = track.fXc; f3DId = track.f3DId;
	fDsquareThresholdPerDot = track.fDsquareThresholdPerDot;
	memcpy(fXMiss3D, track.fXMiss3D, sizeof(fXMiss3D));
	return *this;
} // end of assignment constructor
TATrack::~TATrack(){} // the destructor

double TATrack::GetDsquare(){
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetDsquare(): Data not assigned.");
		return -9999.;
	} // end of if
	if(fDsquare <= 0){
		int gGOOD = GetgGOOD();
		double d2ThrePp = GetDsquareThresholdPerDot();
		fDsquare = TAMath::Dsquare(fZ, fX, fK, fB, gGOOD, fLAYER, d2ThrePp);
	} // end if(fDsquare <= 0)
	return fDsquare;
} // end of function GetDsquare
double TATrack::GetChi2(){
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetChi2(): Data not assigned.");
		return -9999.;
	} // end of if
	if(!fIsFitted) Fit();

	if(fChi2 < 0.) TAPopMsg::Error(GetName().c_str(), "GetChi2: minus value!");
	return fChi2;
} // end of function GetChi2
// fit residue = sqrt( chi2/ndf ). ndf = N - L or N - L + K
double TATrack::GetChi(){
	int ndf = GetNFiredAnodeLayer();
	return sqrt(GetChi2() / ndf);
} //
double TATrack::GetSlope(){
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetSlope(): Data not assigned.");
		return -9999.;
	} // end of if
	if(!fIsFitted) Fit();

	return fK;
} // end of function GetSlope.
double TATrack::GetIntercept(){
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetIntercept(): Data not assigned.");
		return -9999.;
	} // end of if

	if(!fIsFitted) Fit();

	return fB;
} // end of function GetIntercept
void TATrack::GetNu(int *nu) const{
	for(int i = 0; i < 6; i++) nu[i] = fNu[i];
}
void TATrack::GetLAYER(int *LAYER) const{
	for(int i = 0; i < 6; i++) LAYER[i] = fLAYER[i];
} // end of function GetLAYER
void TATrack::GetZ(double *z) const{ // output array fZ
	for(int i = 0; i < 6; i++) z[i] = fZ[i];
} // end of function GetZ
void TATrack::GetX(double *x) const{ // output array fX
	for(int i = 0; i < 6; i++) x[i] = fX[i];
} // end of function GetX
void TATrack::GetDriftTime(double *t) const{
	for(int i = 0; i < 6; i++) t[i] = fT[i];
} // end of function GetDriftTime()
void TATrack::GetDriftDistance(double *r) const{
	for(int i = 0; i < 6; i++) r[i] = fR[i];
} // end of function GetDriftDistance() 
void TATrack::GetChi(double *chi){ // output array fChi
	if(!fIsAssigned){
		TAPopMsg::Error(GetName().c_str(), "GetChi(double *): Data not assigned.");
		return;
	} // end of if
	if(!fIsFitted) Fit();

	for(int i = 0; i < 6; i++) chi[i] = fChi[i];
} // end function GetChi
double TATrack::GetTOF() const{ return fTOF; }
double TATrack::GetNStripStray() const{ return fNStripStray; }
int TATrack::GetFiredStripId() const{ return fFiredStripId; }

int TATrack::GetNFiredAnodeLayer() const{
	if(fNFiredAnodeLayer < 0){
		TAPopMsg::Error(GetName().c_str(), "GetNFiredAnodeLayer: Minus value. %d", fNFiredAnodeLayer);
	} // end if(NFiredAnodeLayer < 0)
	return fNFiredAnodeLayer;
} // end of function GetNFiredAnodeLayer().
int TATrack::GetgGOOD() const{
	if(fgGOOD < 0)
		TAPopMsg::Error(GetName().c_str(), "GetgGOOD: Minus value. %d", fgGOOD);
	return fgGOOD;
} // end of function GetgGOOD().
int TATrack::GetFitMethod() const{
	if(fFitMethod == -1){
		TAPopMsg::Error(GetName().c_str(), "GetFitMethod: Not Assigned.");
		return -1;
	}
	return fFitMethod;
} // end of function GetFitMethod().
int TATrack::GetFitPrecision() const{ return fFitPrecision; }
double TATrack::GetDsquareThresholdPerDot() const{ return fDsquareThresholdPerDot; }
bool TATrack::IsAssigned() const{ return fIsAssigned; } // to tell if the data has been fitted.
bool TATrack::IsFitted() const{ return fIsFitted; } // to tell if the data has been fitted.

// the set functions
// set calculation depth.
// FitPrecision: -1, 0, 1, 2, 3. Other values would be the least fit precision.
void TATrack::SetFitPrecision(int fitPrecision){ // only useful to normal fit method
	if(fFitMethod != kNormalFit){
		TAPopMsg::Warn(GetName().c_str(), "Fit precision would only take effect if the current fit method is normal fit.");
		return;
	}
	if(fitPrecision < -2 || fitPrecision > 3){
		TAPopMsg::Warn(GetName().c_str(), "SetFitPrecision: Input fit precision out of range. Must be -2, 0, 1, 2, 3 from shallow to deep in order.");
		return;
	}
	if(fitPrecision == fFitPrecision && IsFitted()) return;
	fFitPrecision = fitPrecision;
	fIsFitted = false;

} // end of function SetFitPrecision().
// x, z: fired anode position{} r: drift distance
void TATrack::SetData(const double *x, const double *z, const double *t, const double *r, double kL, double bL, double dsquare, int gGOOD, const int *nu, const int *LAYER, const double *weight){
	int temp_good = 0;
	int nFiredAnodeLayer = -1; // count of fired andoe layers.
	for(int i = 0; i < 6; i++){
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

void TATrack::SetDriftTime(const double *t, const double *w){ // as the name indicates
	for(int i = 0; i < 6; i++){
		if((fNu[i] > 0 && -9999. == t[i]) || (fNu[i] < 0 && -9999. != t[i]))
			TAPopMsg::Error(GetName().c_str(),
				"SetDriftTime: t[%d] here contradicates with former assignment of array nu. fNu[%d]: %d - t[%d]: %f", i, i, fNu[i], i, t[i]);
		fT[i] = t[i];
		if(w) fWeight[i] = w[i];
	} // end for over i
	fIsFitted = false;
} // end of function SetDriftTime()
void TATrack::SetTOF(double TOF, int firedStripId, double nstripsStray){
	fTOF = TOF;
	fFiredStripId = firedStripId;
	fNStripStray = nstripsStray;
} // end of function SetTOF
void TATrack::SetDriftDistance(const double *r){ // as the name indicates
	for(int i = 0; i < 6; i++){
		if(fNu[i] * r[i] < 0){
			TAPopMsg::Error(GetName().c_str(),
				"SetDriftDistance: r[%d] here contradicates with former assignment of array nu. fNu[%d]: %d - r[%d]: %f", i, i, fNu[i], i, r[i]);
		} // end if
		fR[i] = r[i];
	} // end for over i
	fIsFitted = false;
} // end of function SetDriftDistance()
void TATrack::AssignTrack(tTrack *ptrack){ // assign tTrack
	strcpy(ptrack->name, GetName().c_str());
	GetNu(ptrack->nu);
	GetLAYER(ptrack->LAYER);
	GetZ(ptrack->z);
	GetX(ptrack->x);
	GetDriftTime(ptrack->t);
	for(int i = 0; i < 6; i++) ptrack->w[i] = fWeight[i];
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


void TATrack::SetFitMethod(int method){ // 0: normal fit; 1: BFGS fit
	if(method != kNormalFit && method != kBFGSFit && method != kIterFit){
		TAPopMsg::Error(GetName().c_str(), "SetFitMethod: The Fit method is invalid.\n Accepted value:\n");
		cout << "TATrack::kNormalFit: " << kNormalFit;
		cout << "\tTATrack::kBFGSFit: " << kBFGSFit;
		cout << "\tTATrack::kIterFit: " << kIterFit;
		cout << "Press ENTER to exit...";
		getchar();
		return;
	} // end if(method !=...)
	if(method == fFitMethod && IsFitted()) return;
	fFitMethod = method;
	fIsFitted = false;
} // end of function SetFitMethod().
void TATrack::SetDsquareThresholdPerDot(double threshold){
	fDsquareThresholdPerDot = threshold;
	fIsFitted = false;
} // end of function SetDsquareThresholdPerDot(double threshold).

// for kNormalFit only. (xc, zc) is the perturbative rotating center of particle track line in an attempt to minimize chi2 and get the optimal combination of linear parameter set.
void TATrack::SetFitRotationCenter(double xc, double zc){
	fXc = xc;
	fZc = zc;
	fIsFitted = false;
} // end of function SetFitRotationCenter().

void TATrack::Fit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "Fit: the track has not been assigned.");
	if(IsFitted()) return;
	if(fFitMethod != kNormalFit && fFitMethod != kBFGSFit && fFitMethod != kIterFit){
		TAPopMsg::Error(GetName().c_str(), "Fit: The Fit method is invalid.\n Accepted value:\n");
		cout << "TATrack::kNormalFit: " << kNormalFit;
		cout << "\tTATrack::kBFGSFit: " << kBFGSFit;
		cout << "\tTATrack::kIterFit: " << kIterFit;
		getchar();
	}

	// initialize chi
	for(double &xx : fChi) xx = -9999.;
	fChi2 = 0.;

	// to tell whether drift time has been assigned //
	int cnt = 0; // count of void r[i]
	for(double x : fR) if(-9999. == x) cnt++;
	if(6 == cnt){
		fChi2 = 0.; GetDsquare();
		for(int i = 0; i < 6; i++){
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
//	fK = 0.225525; fB = -37.8764; // XXX XXX XXX XXX DEBUG DEBUG DEBUG XXX XXX XXX XXX
	for(int i = 0; i < 6; i++){
		if(fLAYER[i] != -1){
			fChi[fLAYER[i]] = fabs(fK * fZ[fLAYER[i]] - fX[fLAYER[i]] + fB) / sqrt(1. + fK * fK) - fR[fLAYER[i]];
			if(fWeight[fLAYER[i]] < 0.)
				TAPopMsg::Error(GetName().c_str(), "Fit: weight value is minus: fWeight[%d]: %f", fLAYER[i], fWeight[fLAYER[i]]);
			fChi2 += fChi[fLAYER[i]] * fChi[fLAYER[i]] * fWeight[fLAYER[i]];
		}
	}

	fIsFitted = true;
} // end of member function void fit().

void TATrack::NormalFit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "NormalFit: the data has not been assigned.");
	if(IsFitted()) return;
	if(fXc == -9999. || fZc == -9999.){
		TAPopMsg::Error(GetName().c_str(), "NormalFit: The fit rotation center has not been set.");
		return;
	} // end of if
	if(fFitPrecision < -2 || fFitPrecision > 3){
		TAPopMsg::Error(GetName().c_str(), "NormalFit: Current fit precision out of range. Must be -2, 0, 1, 2, 3 from shallow to deep in order.");
		return;
	} // end of if
	int gGOOD = GetgGOOD();

	// Here it's refinedFit(fZ, fX, ...), NOT refinedFit(fX, fZ, ...), because the coordinate system is different from those conventional ones now.
	TAMath::refinedFit(fZ, fX, fR, fK, fB, gGOOD, fLAYER, fFitPrecision, fZc, fXc, fDsquareThresholdPerDot, fIsDEBUG);

	fIsFitted = true;
} // end of function NormalFit().
void TATrack::BFGSFit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "BFGSFit: the data has not been assigned.");
	if(IsFitted()) return;
	int gGOOD = GetgGOOD();

	// Here it's refinedFitBFGS(fZ, fX, ...), NOT refinedFitBFGS(fX, fZ, ...), because the coordinate system is different from those conventional ones now.
	TAMath::refinedFitBFGS(fZ, fX, fR, fK, fB, gGOOD, fLAYER, fDsquareThresholdPerDot);

	fIsFitted = true;
} // end of function BFGSFit().
void TATrack::IterFit(){
	if(!IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "IterFit: the data has not been assigned.");
	if(IsFitted()) return;
	int gGOOD = GetgGOOD();

	// Here it's refinedFitBFGS(fZ, fX, ...), NOT refinedFitBFGS(fX, fZ, ...), because the coordinate system is different from those conventional ones now.
	TAMath::iterativeFit(fZ, fX, fR, fK, fB, gGOOD, fLAYER, fDsquareThresholdPerDot);

	fIsFitted = true;
}

// show information about the track
void TATrack::Show(){
	Fit(); // Then all the member variables deserving assignemnts would be assigned accordingly.
	cout << "\033[32;1m" << GetName() << "\033[0m" << endl;
	cout << "TATrack::Show(): Show information about the track." << endl;
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
	cout << "x3DMiss:\n";
	for(double x : fXMiss3D) cout << x << "\t";
	cout << endl;
	cout << "3D track Id: " << f3DId << endl;
	cout << "firedStripId: " << GetFiredStripId() << endl;
	cout << "nstripStray: " << GetNStripStray() << endl;
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


// initialization functions.
void TATrack::Initialize(){
	for(int i = 0; i < 6; i++){
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
void TATrack::FillTrack(TGraph *gTrack, TGraph *gTrack_R){
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
	for(int i = 6; i--;) if(fR[i] > 0.){
		for(int j = nCir; j--;){
			theta = (2.*j/nCir - 1.) * TAMath::Pi(); // +-Pi
			z = fZ[i] + fR[i] * cos(theta);
			x = fX[i] + fR[i] * sin(theta);
			gTrack_R->SetPoint(gTrack_R->GetN(), z, x);
		}
	}
} // end member method FillTrack






