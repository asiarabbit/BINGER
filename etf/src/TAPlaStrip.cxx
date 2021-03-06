///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPlaStrip.C														 //
//   TAPlaStrip.C -- source file for class TAPlaStrip								 //
//   Introduction: designed for both-end-readout plastic scintillator strip.		 //
// Container class, representating all the attributes and function of the strip.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/6/11, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAPlaStripData.h"
#include "TAChannel.h"
#include "TAPopMsg.h"
#include "TAChData.h"
#include "TAUIDParser.h"
#include "TADeployPara.h"
#include "TAVisual.h"

TAPlaStrip::TAPlaStrip(const string &name, const string &title, unsigned uid)
	: TAStuff(name, title, uid), fIsSingleEnd(false){
	fStripPara = nullptr; fStripData = nullptr;
	fUV = nullptr; fUH = nullptr;
	fDV = nullptr; fDH = nullptr;
	fStripId = -1;
	ft0 = 0.; ft1 = 0.; ft2 = 0.;

	TAVisual::Instance()->AddPlaStrip(this);
}
TAPlaStrip::~TAPlaStrip(){
	if(fStripPara){ delete fStripPara; fStripPara = nullptr; }
	if(fStripData){ delete fStripData; fStripData = nullptr; }
	if(fUV){ delete fUV; fUV = nullptr; }
	if(fUH){ delete fUH; fUH = nullptr; }
	if(fDV){ delete fDV; fDV = nullptr; }
	if(fDH){ delete fDH; fDH = nullptr; }
}

// Range: 1-30(TATOFWall::kNStrip)
int TAPlaStrip::GetStripId() const{
	if(fStripId < 0)
		TAPopMsg::Error(GetName().c_str(), "GetStripId: fStripId is minus: %d", fStripId);
	return fStripId;
}
double TAPlaStrip::GetHitPosition() const{ // distance from hit point to the down end.
	double pos = GetStripData()->GetHitPosition();
	if(-9999. == pos){ // not assigned
		if(GetDV()->GetData()->GetFiredStatus() && GetUV()->GetData()->GetFiredStatus()){
			// distance from hit point to the down end.
			pos = GetStripPara()->GetLength() / 2. + 
				( GetDV()->GetTime() - GetUV()->GetTime() ) * 
				GetStripPara()->GetVeff() / 2.;
			GetStripData()->SetHitPosition(pos); // so that the hit position would only be calculated once
		} // end if(f..)
		else{
			return -9999.;
//			TAPopMsg::Error(GetName().c_str(), "GetHitPosition: not both ends are fired.");
		}
	} // end outer if
	return pos;
}
// see the header file for fired status index interpretation
int TAPlaStrip::GetFiredStatus() const{
	int sta = GetStripData()->GetFiredStatus();
	bool df[2] = {GetDV()->GetFiredStatus(), GetDH()->GetFiredStatus()}; // [V-H]
	bool uf[2] = {GetUV()->GetFiredStatus(), GetUH()->GetFiredStatus()}; // [V-H]
	if(-2 == sta){ // not assigned
		for(int i = 0; i < 2; i++){ // loop over U and V channels
			if(df[i] && uf[i]){
				sta = 3; // off strip hit
				if(0 == i){ // Very high resolution
					double x = GetHitPosition();
					double l = GetStripPara()->GetLength();
					if(x >= -3. * l && x < 3. * l) sta = 4; // l +- 1*l: good hit
				}
			} // end if(... && ...)
			else if(!df[i] && uf[i]) sta = 2;
			else if(df[i] && !uf[i]) sta = 1;
			else sta = 0;
			if(0 == i && sta >= 3) break; // XXX: V sta is good enough for the strip
			if(1 == i && sta > 0) sta += 10; // U
		} // end for over H and V resolution mode
		GetStripData()->SetFiredStatus(sta); // assign the fStripData object
	} // end the outer if
	return sta;
} // end function GetFiredStatus
// t0, t1 and t2 are set for choosing ch->GetLT over edges
// (ch->GetLT-t0) within t1 and t2 is chosen. t0, t1 and t2 using default values, choose the 1st edge
double TAPlaStrip::GetTime(double t0x, double t1x, double t2x){ // get hit time
	// to make t1 and t2 meaningful with the presence of delay
	TAPlaStripPara *pa = GetStripPara();
	const double delay = pa->GetDelay(); t0x += delay;

	double time = GetStripData()->GetTime();
	// the last calculated time should be updated if t0, t1, and t2 are not the same as before
	if(!(ft0 == t0x && ft1 == t1x && ft2 == t2x)) time = -9999.;
	if(-9999. == time){ // not assigned
		ft0 = t0x; ft1 = t1x; ft2 = t2x;
		double tt = GetUV()->GetLT(ft0,ft1,ft2) + GetDV()->GetLT(ft0,ft1,ft2);
//		cout << GetName() << endl; // DEBUG
//		GetUV()->GetData()->Show(); // DEBUG
//		cout << "GetUV()->GetLT(t0,t1,t2): " << GetUV()->GetLT(t0,t1,t2) << endl; // DEBUG
//		cout << "GetDV()->GetLT(t0,t1,t2): " << GetDV()->GetLT(t0,t1,t2) << endl; // DEBUG
//		getchar(); // DEBUG
		short sta = GetFiredStatus();
		if(4 == sta || 
		3 == sta
		   ){
			time = -pa->GetLength()/(2.*pa->GetVeff()) + tt / 2. - delay;
			GetStripData()->SetTime(time); // assigne the fStripData object
		}
		else{ // strip not properly fired
			time = -9999.;
		}
	}
	return time;
} // end of function GetTime
// return up end of the strip, HPTDC very high resolution mode.
TAChannel *TAPlaStrip::GetUV() const{
	if(!fUV) TAPopMsg::Error(GetName().c_str(), "GetUV: pointer is null.");
	return fUV;
}
// return up end of the strip, HPTDC high resolution mode.
TAChannel *TAPlaStrip::GetUH() const{
	if(!fUH) TAPopMsg::Error(GetName().c_str(), "GetUH: pointer is null.");
	return fUH;
}
// return down end of the strip, HPTDC very high resolution mode.
TAChannel *TAPlaStrip::GetDV() const{
	if(!fDV) TAPopMsg::Error(GetName().c_str(), "GetDV: pointer is null.");
	return fDV;
}
// return down end of the strip, HPTDC high resolution mode.
TAChannel *TAPlaStrip::GetDH() const{
	if(!fDH) TAPopMsg::Error(GetName().c_str(), "GetDH: pointer is null.");
	return fDH;
}
TAPlaStripPara *TAPlaStrip::GetStripPara() const{
	if(!fStripPara){
		cout << GetName() << endl; // DEBUG
		cout << GetUID() << endl; // DEBUG
		TAPopMsg::Error(GetName().c_str(), "GetStripPara: pointer is null.");
	}
	return fStripPara;
}
TAPlaStripData *TAPlaStrip::GetStripData() const{
	if(!fStripData) TAPopMsg::Error(GetName().c_str(), "GetStripData: pointer is null.");
	return fStripData;
}
double TAPlaStrip::GetDelay() const{
	return GetStripPara()->GetDelay();
}
void TAPlaStrip::SetStripLength(double length){
	GetStripPara()->SetLength(length);
}

void TAPlaStrip::Initialize(){
	if(fUV && fUV->GetFiredStatus()) fUV->Initialize();
	if(fUH && fUH->GetFiredStatus()) fUH->Initialize();
	if(fDV && fDV->GetFiredStatus()) fDV->Initialize();
	if(fDV && fDH->GetFiredStatus()) fDH->Initialize();
	GetStripData()->Initialize();
}
void TAPlaStrip::Configure(){
	if(fUV){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	fUV = new TAChannel(fName+"->UV", fName+"->UV", fUID+(0<<15)); fUV->SetSerialId(0);
	fUH = new TAChannel(fName+"->UH", fName+"->UH", fUID+(1<<15)); fUV->SetSerialId(1);
	fDV = new TAChannel(fName+"->DV", fName+"->DV", fUID+(2<<15)); fUV->SetSerialId(2);
	fDH = new TAChannel(fName+"->DH", fName+"->DH", fUID+(3<<15)); fUV->SetSerialId(3);

	fStripPara = new TAPlaStripPara(fName+"->Para", fName+"->Para", fUID);
	fStripData = new TAPlaStripData(fName+"->Data", fName+"->Data", fUID);

	unsigned uid = GetUID();
	int type[6]{}; TAUIDParser::DNS(type, uid);
	TADeployPara *deploy = TADeployPara::Instance();
	if((3 == type[0] || 4 == type[0]) && 3 == type[1]){ // DCArr->TOFWall
		fStripPara->SetWidth(deploy->GetTOFWallStripWidth(uid));
		fStripPara->SetLength(deploy->GetTOFWallStripLength(uid));
		fStripPara->SetDelay(deploy->GetTOFWallStripDelay(uid));
		fStripPara->SetVeff(deploy->GetTOFWallStripVeff(uid));
	}
}








