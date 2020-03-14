///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADetectorPara.C													 //
//   TADetectorPara.C -- source file for class TADetectorPara						 //
//   Introduction: store parameter set of Detector level(MWDC, TOFWall, etc.). 		 //
// Container class, storing position parameter (x, y, z, phi, theta, psi).			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/10/7, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TADetectorPara.h"
#include "TAMath.h"
#include "TAPopMsg.h"

TADetectorPara::TADetectorPara(const string &name, const string &title, unsigned uid)
				: TAStuff(name, title, uid){
	fX = -9999.; fY = -9999.; fZ = -9999.;
	fPhi = -9999.; fTheta = -9999.; fPsi = -9999.;
	
	fXOffset = 0.; fYOffset = 0.; fZOffset = 0.;
	fAlphaOffset = 0.; fBetaOffset = 0.; fGammaOffset = 0.;
	
	fIsOffset = false; fIsAssigned = false;
}

TADetectorPara::~TADetectorPara(){}

double TADetectorPara::GetX() const{
	if(-9999. == fX) TAPopMsg::Warn(GetName().c_str(), "GetX: Not assigned yet~");
	return fX;
}
double TADetectorPara::GetY() const{
	if(-9999. == fY) TAPopMsg::Warn(GetName().c_str(), "GetY: Not assigned yet~");
	return fY;
}
double TADetectorPara::GetZ() const{
	if(-9999. == fZ) TAPopMsg::Warn(GetName().c_str(), "GetZ: Not assigned yet~");
	return fZ;
}
double TADetectorPara::GetPhi() const{
	if(-9999. == fPhi) TAPopMsg::Warn(GetName().c_str(), "GetPhi: Not assigned yet~");
	return fPhi;
}
double TADetectorPara::GetTheta() const{
	if(-9999. == fTheta) TAPopMsg::Warn(GetName().c_str(), "GetTheta: Not assigned yet~");
	return fTheta;
}
double TADetectorPara::GetPsi() const{
	if(-9999. == fPsi) TAPopMsg::Warn(GetName().c_str(), "GetPsi: Not assigned yet~");
	return fPsi;
}

// r_global = dR.(R.r_local) - intrinsic rotation
// tranformation from detector local coordiantes to global coordinates - the rotation part
void TADetectorPara::GetGlobalRotation(const double *p_local, double *p_global) const{
	double angle[3] = {fPhi, fTheta, fPsi};
	double angOff[3] = {fAlphaOffset, fBetaOffset, fGammaOffset};
	double p[3]; // an intermediate temporary array
	TAMath::rotate(p_local, p, angle); // R.a
	if(fIsOffset) TAMath::rotateOffset(p, p_global, angOff); // dR.(R.a)
	else
		for(int i = 3; i--;) p_global[i] = p[i];
}
// r_global = dR.(R.r_local)+r0+dr
// tranformation from detector local coordiantes to global coordinates
// - the rotation and the translation part
void TADetectorPara::GetGlobalPosition(const double *p_local, double *p_global) const{
	GetGlobalRotation(p_local, p_global);
	p_global[0] += fX + fXOffset; // x
	p_global[1] += fY + fYOffset; // y
	p_global[2] += fZ + fZOffset; // z
}

// p[0-5]: [xc, yc, zc, yaw, pitch, roll] (mm, rad)
void TADetectorPara::SetPosition(const double *p){
	fX = p[0];   fY = p[1];     fZ = p[2];
	fPhi = p[3]; fTheta = p[4]; fPsi = p[5];
	fIsAssigned = true;
}
void TADetectorPara::SetPositionOffset(const double *p){
	fXOffset = p[0];     fYOffset = p[1];    fZOffset = p[2];
	fAlphaOffset = p[3]; fBetaOffset = p[4]; fGammaOffset = p[5];
	fIsOffset = true;
}




