///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TATOFWall.C														 //
//   TATOFWall.C -- source file for class TATOFWall									 //
//   Introduction: TOF wall class. A container class, accommodating plastic scinti-  //
// llator strips and choosing fired strip for a given track.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2017/10/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cstring>
#include <cmath>
#include "TATOFWall.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAPopMsg.h"
#include "TADeployPara.h"
#include "TACtrlPara.h"
#include "TAMath.h"
#include "TAChannel.h"
#include "TAChData.h"

TATOFWall::TATOFWall(const string &name, const string &title, unsigned uid)
		: TADetector(name, title, uid){
	fStripArr.reserve(30);
	fDelayAvrg = -9999.;
}
TATOFWall::~TATOFWall(){
	for(TAPlaStrip *str : fStripArr){
		delete str; str = nullptr;
	}
}

TAPlaStrip *TATOFWall::GetStrip(int stripId) const{
	if(stripId < 0 || stripId >= fStripArr.size())
		TAPopMsg::Error(GetName().c_str(), "GetStrip: stripId out of range: %d", stripId);
	return fStripArr[stripId];
}
// return count of strips with fired status being 4
unsigned short TATOFWall::GetNFiredStrip() const{
	unsigned short cnt = 0;
	for(TAPlaStrip *str : fStripArr){
		if(4 == str->GetFiredStatus()){
			cnt++;
		}
	}
	return cnt;
}
// vlT: very high resolution mode time
void TATOFWall::GetFiredStripArr(int &multi, int *idLs, short *staLs, double *uvlTLs, double *dvlTLs) const{
	multi = 0;
	for(TAPlaStrip *str : fStripArr){
		const int sta = str->GetFiredStatus();
		if(
//		  11 == sta || 12 == sta || // either H end is fired
//		  13 == sta || // both U ends are fired
//		   1 == sta || 2 == sta || // either V end is fired
		   3 == sta || // both V ends are fired, not within valid scope
		   4 == sta // both V ends are fired, within valid scope
		){
			idLs[multi] = str->GetStripId();
			if(staLs) staLs[multi] = sta;
			if(uvlTLs) uvlTLs[multi] = str->GetUV()->GetLeadingTime();
			if(dvlTLs) dvlTLs[multi] = str->GetDV()->GetLeadingTime();
			multi++;
		}
	}
}
// t0, t1, t2: see explnanation below
double TATOFWall::GetStripTime(int i, double t0, double t1, double t2) const{
	return GetStrip(i)->GetTime(t0,t1,t2) - GetDelayAvrg();
}
// Get TOF time from strip specified by a track with linear parameter kl and bl.
// firedStripId: serial id of the fired strip for the track specifically.
// if the supposed fired strip is not fired, return -9999.
// t0, t1 and t2 are set for choosing time over edges
// (time-t0) within t1 and t2 is chosen
// t0, t1 and t2 using default values, choose the 1st edge
double TATOFWall::GetTime(double kl, double bl, double &nstripStray, int &firedStripId, double t0, double t1, double t2) const{
	double nStripStray = -9999., nStripStrayMin = 9999.; // see the assignment below.
	int minID = -1; // serial Id of the strip with minimum nStripStray.

	const int n = GetNStrip();
	for(int i = 0; i < n; i++){
		TAPlaStrip *strip = GetStrip(i);
		if(4 == strip->GetFiredStatus()){
			double p[3]{}; // strip projection position
			strip->GetStripPara()->GetGlobalProjection(p); // assign p
			double width = strip->GetStripPara()->GetWidth();
			nStripStray = (kl * p[2] - p[0] + bl) / sqrt(1. + kl * kl) / width;

			if(fabs(nStripStray) < fabs(nStripStrayMin)){
				nStripStrayMin = nStripStray; minID = i;
			}
		}
	} // end for over i
	if(TACtrlPara::Instance()->TOFWallStripStrayTest(nStripStrayMin, GetUID())){
		if(minID >= 0){
			firedStripId = minID;
			nstripStray = nStripStrayMin;
			return GetStrip(minID)->GetTime(t0,t1,t2) - GetDelayAvrg();
		} // end if
	} // end if

	nstripStray = -9999.;
	firedStripId = -1;
	return -9999.;
}
void TATOFWall::GetStripProjection(int serialId, double *p) const{
	double width = GetStrip(0)->GetStripPara()->GetWidth();
	double phi = GetDetPara()->GetPhi();
	double p_local_0 = width * ((1. - GetNStrip()) / 2. + serialId);
	p[2] = -sin(phi) * p_local_0 + GetDetPara()->GetZ();
	p[0] =  cos(phi) * p_local_0 + GetDetPara()->GetX();
	p[1] = 0.;
}
void TATOFWall::AssignStripPosition(){
	double p[3];
	const int n = GetNStrip();
	for(int i = 0; i < n; i++){
		GetStripProjection(i, p);
		GetStrip(i)->GetStripPara()->SetGlobalProjection(p);
	}
}
// acceptance angle of the TOFWall
// (from the coordinate system origin to the both ends of the TOFWall)
double TATOFWall::Acceptance() const{
	if(!GetDetPara()->IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "Acceptance: detector position not assigned.");
	double p0[3]{}, p1[3]{};
	GetStrip(0)->GetStripPara()->GetGlobalProjection(p0);
	GetStrip(GetNStrip()-1)->GetStripPara()->GetGlobalProjection(p1);
	double pp0[2] = {p0[2], p0[0]}, pp1[2] = {p1[2], p1[0]}; // z, x, to fit the format of the method
	return TAMath::acceptance(pp0, pp1);
}

double TATOFWall::GetDelayAvrg() const{
	if(-9999. == fDelayAvrg) TAPopMsg::Error(GetName().c_str(), "GetDelayAvrg: Not assgined.");
	return fDelayAvrg;
}
double TATOFWall::GetDelay(int stripId) const{
	return GetDelayAvrg() + GetStrip(stripId)->GetDelay();
}

void TATOFWall::Initialize(){
	for(TAPlaStrip *str : fStripArr) str->Initialize();
}
void TATOFWall::Configure(){
	TADetector::Configure();
	TADeployPara *deploy = TADeployPara::Instance();
	SetDelayAvrg(deploy->GetTOFWallDelayAvrg(GetUID()));
	if(fStripArr.size() > 0){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	const int n = deploy->GetNTOFWallStrip(GetUID());
	char name[64];
	TAPlaStrip *str;
	for(int i = 0; i < n; i++){
		sprintf(name, "%s->Strip%d", GetName().c_str(), i);
		str = new TAPlaStrip(name, name, fUID+(i<<9));
		str->SetStripId(i); str->Configure();
		fStripArr.push_back(str);
	}
}
// print user-defined configurations
void TATOFWall::Info() const{
	TAPlaStripPara *pra = GetStrip(0)->GetStripPara();
	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure:\nfDelayAvrg: %f\nfWidth: %f\nfLength: %f\nfVeff: %f\nphi: %f\nacceptance: %f\n", GetDelayAvrg(), pra->GetWidth(), pra->GetLength(), pra->GetVeff(), GetDetPara()->GetPhi() / TAMath::DEGREE(), Acceptance() / TAMath::DEGREE());
}









