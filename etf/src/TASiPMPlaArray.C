///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASiPMPlaArray.C												     //
//   TASiPMPlaArray.C -- source file for class TASiPMPlaArray					     //
//   Introduction: fast particle tracking detector dedicated especially for high	 //
// counting rates circumstances, consisting of an array of micro-strips of plastic	 //
// scintillators coupled with SiPM readouts, which is analogous to PMT but much		 //
// smaller.																			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/11.															     //
// Last modified: 2017/10/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "TASiPMPlaArray.h"
#include "TAChannel.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TAVisual.h"
#include "TADeployPara.h"

TASiPMPlaArray::TASiPMPlaArray(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid){
	fWidth = -9999.; fLength = -9999.;
	fX0 = -9999.; fZ0 = -9999.;
	fNStripV = -1; fNStripH = -1; fDelay = -9999.;
	fUVArr.reserve(10); fUHArr.reserve(10);
}
TASiPMPlaArray::~TASiPMPlaArray(){
	for(TAChannel *&ch : fUVArr) { delete ch; ch = nullptr; }
	for(TAChannel *&ch : fUHArr) { delete ch; ch = nullptr; }
}

short TASiPMPlaArray::GetNStrip(bool isV) const{ // total number of strips
	if(isV){
		if(-1 == fNStripV){ // not assigned
			TAPopMsg::Warn(GetName().c_str(), "GetNStrip(V): Not Assigned");
		}
		return fNStripV;
	}
	else{
		if(-1 == fNStripH){ // not assigned
			TAPopMsg::Warn(GetName().c_str(), "GetNStrip(H): Not Assigned");
		}
		return fNStripH;
	}
}
vector<TAChannel *> &TASiPMPlaArray::GetChArr(bool isV){
	if(isV) return fUVArr;
	else return fUHArr;
}
short TASiPMPlaArray::GetNFiredStrip(bool isV) const{ // number of fired strips
	short cnt = 0;
	if(isV){
		for(const TAChannel *ch : fUVArr) if(ch->GetFiredStatus()) cnt++;
		return cnt;
	}
	else{
		for(const TAChannel *ch : fUHArr) if(ch->GetFiredStatus()) cnt++;
		return cnt;
	}
}
void TASiPMPlaArray::GetFiredStripArr(int &multi, int *idLs, double *lTLs, bool isV) const{
	multi = 0;
	auto arr = &fUVArr; if(!isV) arr = &fUHArr;
	for(const TAChannel *ch : (*arr)) if(ch->GetFiredStatus()){
		idLs[multi] = ch->GetSerialId();
		if(lTLs) lTLs[multi] = ch->GetLeadingTime();
		multi++;
	}
}
double TASiPMPlaArray::GetDelay() const{
	if(-9999. == fDelay) TAPopMsg::Error(GetName().c_str(), "GetDelay: not assigned");
	return fDelay;
}
double TASiPMPlaArray::GetX0() const{
	if(-9999. == fX0){
		TAPopMsg::Error(GetName().c_str(), "GetX0: Not assigned");
	}
	return fX0;
}
double TASiPMPlaArray::GetZ0() const{
	if(-9999. == fZ0){
		TAPopMsg::Error(GetName().c_str(), "GetZ0: Not assigned");
	}
	return fZ0;
}
double TASiPMPlaArray::GetWidth() const{
	if(-9999. == fWidth){
		TAPopMsg::Error(GetName().c_str(), "GetWidth: Not assigned");
	}
	return fWidth;
}

// get hit time of the fired strip, multi-hit is ignored
double TASiPMPlaArray::GetTime(){
	if(-9999. == fTime) GetHitX();
	return fTime;
}
short TASiPMPlaArray::GetFiredStripId(){
	if(-1 == fFiredStripId) GetHitX();
	return fFiredStripId;
}
// get the position of the fired strip, multi-hit is ignored
double TASiPMPlaArray::GetHitX(){
	if(-9999. == fHitX){
		if(GetNFiredStrip() >= 1){
			int i = 0;
			int cnt = 0;
			for(TAChannel *&ch : fUVArr){
				if(ch->GetFiredStatus()){
					fFiredStripId = i;
					fTime = ch->GetTime() - GetDelay();
					fHitX = GetStripX(i);
				}
				i++;
			} // end for over channels
		}
	} // end if(-9999 == fTime)
	return fHitX;
}
double TASiPMPlaArray::GetStripX(int serialId) const{
	return GetX0() + serialId * GetWidth();
}
// return up end of the strip, HPTDC very high resolution mode
TAChannel *TASiPMPlaArray::GetUV(int id) const{
	if(id >= GetNStrip())
		TAPopMsg::Error(GetName().c_str(),
			"GetUV: required id out of range. fNStripV: %d, id: %d", GetNStrip(), id);
	return fUVArr[id];
}
// return up end of the strip, HPTDC high resolution mode
TAChannel *TASiPMPlaArray::GetUH(int id) const{
	if(id >= GetNStrip(false))
		TAPopMsg::Error(GetName().c_str(),
			"GetUH: required id out of range. fNStripH: %d, id: %d", GetNStrip(false), id);
	return fUHArr[id];
}

void TASiPMPlaArray::Initialize(){
	fTime = -9999.; fHitX = -9999.; fFiredStripId = -1;
	for(TAChannel *&ch : fUVArr) ch->Initialize();
	for(TAChannel *&ch : fUHArr) ch->Initialize();
}
// get the channel that belongs to this with uid
TAStuff *TASiPMPlaArray::GetChannel(unsigned uid) const{
	int type[2]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[2]{}; TAUIDParser::DNS(TYPE, fUID); // parse uid of this
	if(type[0] == TYPE[0]){ // belongs to this object
		short id = type[1] / 2;
		if(type[1] % 2 == 0 && id < GetNStrip()) return GetUV(id); // V channel
		else if(type[1] % 2 == 1 && id < GetNStrip(false)) return GetUH(id); // H channel
	}
	return nullptr;
}
// create detector objects
void TASiPMPlaArray::Configure(){
	if(fUVArr.size() > 0 || fUHArr.size() > 0){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	const int nStripSiPM = 10;
	if(nStripSiPM % 2 != 0) TAPopMsg::Warn(GetName().c_str(), "Configure: nStripSiPM not even");
	fZ0 = TADeployPara::Instance()->GetSiPMArrayZ0(); // Z position of the SiPM array
	fDelay = -498.107;
	fWidth = 3.; // unit: mm
	fLength = 30.; // unit: mm
	fX0 = -fWidth * (nStripSiPM / 2. - 0.5); // central x of the leftest strip(strip#0)
	fX0 += 3.; // 0 strip is broken, so 0 strip now is strip 1, 20171027_1445

	// print user-defined configurations
//	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure: \nfZ0: %f\nfDelay: %f\nfWidth: %f\nfLength: %f\nfX0: %f\n", fZ0, fDelay, fWidth, fLength, fX0);

	TAChannel *fuv, *fuh;
	char name[128];
	for(int i = 0; i < nStripSiPM; i++){
		sprintf(name, "%s->Strip%d->UV", fName.c_str(), i);
		fuv = new TAChannel(name, name, fUID+((i*2)<<6));
		fuv->SetSerialId(i);
		sprintf(name, "%s->Strip%d->UH", fName.c_str(), i);
		fuh = new TAChannel(name, name, fUID+((i*2+1)<<6));
		fuh->SetSerialId(i);
		fUVArr.push_back(fuv); fuv->GetPara()->SetValue(GetStripX(i));
		fUHArr.push_back(fuh); fuh->GetPara()->SetValue(GetStripX(i));
		TAVisual::Instance()->AddChannel(fuv);
//		TAVisual::Instance()->AddChannel(fuh);
	}
	fNStripV = fUVArr.size();
	fNStripH = fUHArr.size();
}








