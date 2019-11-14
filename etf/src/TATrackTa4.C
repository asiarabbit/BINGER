///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TATrackTa4.C														 //
//   TATrackTa4.C -- source file for class TATrackTa4								 //
//   Introduction: This class is to store the splined track pre- and post-target, so //
// as to make use of the target hit position and the exit magFiled position, to		 //
// achieve tracking amelioration around the target zone.							 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2019/9/27.															     //
// Last modified: 2019/9/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2019, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "TATrackTa4.h"
#include "TAPopMsg.h"
#include "TAMath.h"

TATrackTa4::TATrackTa4(const string &name, const string &title)
	: TAStuff(name, title), fPreTaTrk(0), fPostTaTrk(0){

	fPreTaTrk = new TATrack2(name + "-PreTa", title + "-PreTa");
	fPostTaTrk = new TATrack2(name + "-PostTa", title + "-PostTa");

	fK2 = -9999.; fB2 = -9999.;

	Initialize();
}

TATrackTa4::~TATrackTa4(){
	if(fPreTaTrk){
		delete fPreTaTrk;
		fPreTaTrk = nullptr;
	}
	if(fPostTaTrk){
		delete fPostTaTrk;
		fPostTaTrk = nullptr;
	}
} // end of the destructor

// the copy constructor
TATrackTa4::TATrackTa4(const TATrackTa4 &track){
	*this = track;
}
// the move constructor
TATrackTa4::TATrackTa4(TATrackTa4 &&track){
	*this = track;
}
// the assignment constructor
TATrackTa4 &TATrackTa4::operator=(const TATrackTa4 &track){
	if(this == &track) return *this;

	fPreTaTrk = new TATrack2(*track.fPreTaTrk);
	fPostTaTrk = new TATrack2(*track.fPostTaTrk);

	SetName(track.GetName());
	SetTitle(track.GetTitle());
	fK2 = track.fK2; fB2 = track.fB2;
	fDX2 = track.fDX2; fDxTa = track.fDxTa;
	fChi2 = track.fChi2; fChi2Extra = track.fChi2Extra;
	fIsFitted = track.fIsFitted;

	return *this;
}
TATrackTa4 &TATrackTa4::operator=(TATrackTa4 &&track){
	if(this == &track) return *this;

	fPreTaTrk = track.fPreTaTrk; track.fPreTaTrk = nullptr;
	fPostTaTrk = track.fPostTaTrk; track.fPostTaTrk = nullptr;

	SetName(track.GetName());
	SetTitle(track.GetTitle());
	fK2 = track.fK2; fB2 = track.fB2;
	fDX2 = track.fDX2; fDxTa = track.fDxTa;
	fChi2 = track.fChi2; fChi2Extra = track.fChi2Extra;
	fIsFitted = track.fIsFitted;

	return *this;
}


// set name, together with the sub-trk's name
void TATrackTa4::SetName(const string &name){
	TAStuff::SetName(name);
	GetTrackPreTa()->SetName(name + "-PreTa");
	GetTrackPostTa()->SetName(name + "-PostTa");
}
// set title, together with the sub-trk's title
void TATrackTa4::SetTitle(const string &title){
	TAStuff::SetTitle(title);
	GetTrackPreTa()->SetTitle(title + "-PreTa");
	GetTrackPostTa()->SetTitle(title + "-PostTa");
}

void TATrackTa4::Initialize(){
	fDX2 = -9999.; fDxTa = -9999.;
	fChi2 = -9999.; fChi2Extra = -9999.;

	GetTrackPreTa()->Initialize();
	GetTrackPostTa()->Initialize();

	fIsFitted = false;
}

TATrack2 *TATrackTa4::GetTrackPreTa() const{
	if(!fPreTaTrk) TAPopMsg::Warn(GetName().c_str(), "GetTrackPreTa::Requested pointer is null");
	return fPreTaTrk;
}

TATrack2 *TATrackTa4::GetTrackPostTa() const{
	if(!fPreTaTrk) TAPopMsg::Warn(GetName().c_str(), "GetTrackPostTa::Requested pointer is null");
	return fPostTaTrk;
}

// fit residue = sqrt( chi2/ndf ). ndf = N - L or N - L + K
double TATrackTa4::GetChi(){
	int ndf = GetNFiredAnodeLayer();
	return sqrt(GetChi2() / ndf);
}


int TATrackTa4::GetNFiredAnodeLayer() const{
	return GetTrackPreTa()->GetNFiredAnodeLayer() +
		   GetTrackPostTa()->GetNFiredAnodeLayer();
}

double TATrackTa4::GetChi2(){
	if(!IsFitted()) Fit();
	
	if(fChi2 < 0.) TAPopMsg::Error(GetName().c_str(), "GetChi2: minus value!");
	return fChi2;
}
double TATrackTa4::GetK2(){
	if(-9999. == fK2)
		TAPopMsg::Warn(GetName().c_str(), "GetK2: fK2 not assigned");
	return fK2;
}
double TATrackTa4::GetB2(){
	if(-9999. == fB2)
		TAPopMsg::Warn(GetName().c_str(), "GetB2: fB2 not assigned");
	return fB2;
}

void TATrackTa4::SetK2(double k2){
	if(-9999. == k2)
		TAPopMsg::Warn(GetName().c_str(), "SetK2: -9999. is default value, not allowed");
	fK2 = k2;
}
void TATrackTa4::SetB2(double b2){
	if(-9999. == b2)
		TAPopMsg::Warn(GetName().c_str(), "SetB2: -9999. is default value, not allowed");
	fB2 = b2;
}

double TATrackTa4::GetDX2(){
	if(!IsFitted()) Fit();
	return fDX2;
}
double TATrackTa4::GetDxTa(){
	if(!IsFitted()) Fit();
	return fDxTa;
}
double TATrackTa4::GetChi2Extra(){
	if(-9999. == fChi2Extra)
		TAPopMsg::Warn(GetName().c_str(), "GetChi2Extra: fChi2Extra is not assigned yet.");
	
	return fChi2Extra;
//	double dxTa = GetDxTa(), dx2 = GetDX2();
//			dxTa*dxTa * 1. / 1.72 +
//			dx2*dx2 * 1. / 6.;
//	; // fChi2Extra;
}

// the core fitting method, fChi2, fDX2, and fDxTa would be assigned here
void TATrackTa4::Fit(){
	if(IsFitted()) return;

	// prepare for the calling of BFGSFit() //
	const double k2 = GetK2(), b2 = GetB2();
	TATrack2 *trkPreTa = GetTrackPreTa();
	TATrack2 *trkPostTa = GetTrackPostTa();
	if(!trkPreTa->IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "Fit: the track %s has not been assigned", trkPreTa->GetName().c_str());
	if(!trkPostTa->IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "Fit: the track %s has not been assigned", trkPostTa->GetName().c_str());
	// initialize chi
	fChi2 = 0.;

	// // FIT // //
	// solve the left-right ambiguity (IterFit, embedded in BFGSFit)
	// and optimize the chi2 minmization (BFGSFit)
	// the argument validity check is assumed to have been completed already
	BFGSFit(trkPreTa, trkPostTa, k2, b2);


	// OUTPUT the RESULT: assign fDX2, fDxTa and fChi2 //
	double chi2PreTa = trkPreTa->GetChi2();
	double chi2PostTa = trkPostTa->GetChi2();
	double k0 = trkPreTa->GetSlope();
	double b0 = trkPreTa->GetIntercept();
	double k1 = trkPostTa->GetSlope();
	double b1 = trkPostTa->GetIntercept();
	fDxTa = TAMath::DxTa(k0, k1, b0, b1);
	double x2[2];
	TAMath::rho(k1, b1, k2, b2, nullptr, nullptr, x2);
	fDX2 = x2[0] - x2[1];
	fChi2Extra = TAMath::Dx2DxTa_2(k0, k1, k2, b0, b1, b2);
	fChi2 = chi2PreTa + chi2PostTa + fChi2Extra;
} // end of member function Fit

// k2, b2: postMagTrk parameter
void TATrackTa4::BFGSFit(TATrack2 *trkPreTa, TATrack2 *trkPostTa, double k2, double b2){
	if(IsFitted()) return;

	// output trks info //
	const int n = 12;
	double z[n], x[n], r[n];
	double k[3] = {-9999., -9999., -9999.}, b[3] = {-9999., -9999., -9999.};
	int gGOOD[2], LAYER[2][6];
	for(int i = 0; i < 2; i++){
		gGOOD[i] = -1;
		for(int j = 0; j < 6; j++){
			LAYER[i][j] = -1;
			short k = i*6+j;
			z[k] = -9999.; x[k] = -9999.; r[k] = -9999.;
		} // end for over j
	} // end for over i

	trkPreTa->GetZ(z); trkPreTa->GetX(x); trkPreTa->GetDriftDistance(r);
	trkPreTa->GetLAYER(LAYER[0]); gGOOD[0] = trkPreTa->GetgGOOD();
//	k[0] = trkPreTa->GetSlope(); b[0] = trkPreTa->GetIntercept(); // will trigger fit
	trkPostTa->GetZ(z+6); trkPostTa->GetX(x+6); trkPostTa->GetDriftDistance(r+6);
	trkPostTa->GetLAYER(LAYER[1]); gGOOD[1] = trkPostTa->GetgGOOD();
//	k[1] = trkPostTa->GetSlope(); b[1] = trkPostTa->GetIntercept(); // will trigger fit
	k[2] = k2; b[2] = b2;
	double d2ThrePD = trkPostTa->GetDsquareThresholdPerDot();

	// the core fiting algorithm //
	// guide the BFGS fit with a good start
	TAMath::IterFit4(z, x, r, k, b, gGOOD, LAYER, d2ThrePD);
	// refined fit for perfection
//	TAMath::BFGS4(z, x, r, k, b, gGOOD, LAYER, d2ThrePD);

	// accept the new fitting result
	double chi[2][6], chi2[2]{}; // must be of the length 6
	for(int i = 2; i--;) for(int j = 6; j--;) chi[i][j] = -9999.;
	for(int i = 0; i < 2; i++){ // loop over DCTas
		for(int j = 0; j < 6; j++){ // loop over anode layers
			if(LAYER[i][j] != -1){
				int l = LAYER[i][j];
				int m = i*6+l;
				chi[i][l] = fabs(k[i] * z[m] - x[m] + b[i]) / sqrt(1. + k[i] * k[i]) - r[m];
				chi2[i] += chi[i][l] * chi[i][l];
			} // end if
		} // end for over j
	} // end for over i
//	cout << "This is TATrackTa4, mark 1," << endl; // DEBUG
//	cout << "k[0]: " << k[0] << "\tk[1]: " << k[1] << endl; // DEBUG
//	cout << "b[0]: " << b[0] << "\tb[1]: " << b[1] << endl; // DEBUG
//	getchar(); // DEBUG
	trkPreTa->UpdateFit(k[0], b[0], chi[0], chi2[0]);
	trkPostTa->UpdateFit(k[1], b[1], chi[1], chi2[1]);
//	cout << "This is TATrackTa4, mark 2. The assignment is done." << endl; // DEBUG
//	cout << "k[0]: " << trkPreTa->GetSlope() << "\tk[1]: " << trkPostTa->GetSlope() << endl; // DEBUG
//	cout << "b[0]: " << trkPreTa->GetIntercept() << "\tb[1]: " << trkPostTa->GetIntercept() << endl; // DEBUG
//	getchar(); // DEBUG

//	double dxTa_Math = TAMath::DxTa(k[0], k[1], b[0], b[1]); // DEBUG
//	double dx2_Math = TAMath::DX2(k[1], k[2], b[1], b[2]); // DEBUG
//	cout << "dxTa_Math: " << dxTa_Math << endl; // DEBUG
//	cout << "dx2_Math: " << dx2_Math << endl; // DEBUG
//	getchar(); // DEBUG

	fIsFitted = true;

} // end of member function BFGSFit

void TATrackTa4::Show(){
	cout << "\033[33;1m_____________This is TATrackTa4 show method____________\033[0m\n";

	cout << "Name: " << GetName() << "\tTitle: " << GetTitle() << endl;
	cout << "fDX2: " << GetDX2() << "\tfDxTa: " << GetDxTa();
	cout << "\tfChi2Extra: " << GetChi2Extra() << endl;
	cout << "fChi2: " << GetChi2() << endl;

	GetTrackPreTa()->Show();
	GetTrackPostTa()->Show();
}


