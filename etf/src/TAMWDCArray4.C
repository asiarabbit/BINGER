///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray4.C													 //
//   TAMWDCArray4.C -- source file for class TAMWDCArray4							 //
//   Introduction: an MWDC array composed of 4 medium sized DCs. A derived class 	 //
// from TAMWDCArray with distinct features of the medium DCs.						 //
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
#include <cstdlib>
#include "TH1F.h" // ROOT include
#include "TAMWDCArray4.h"
#include "TACtrlPara.h"
#include "TAUIDParser.h"
#include "TATrack4.h"
#include "TAPopMsg.h"
#include "tTrack.h"
#include "TAAnodePara.h"
#include "TAAnodeData.h"
#include "TAAnode.h"
#include "TADCSFE16.h"
#include "TADCCable.h"
#include "TADCSuperLayer.h"
#include "TAMWDCTaL.h"
#include "TADetectorPara.h"
#include "TAPlaStripPara.h"
#include "TAPlaStrip.h"
#include "TATOFWall.h"
#include "TAChannel.h"
#include "TAMath.h"
#include "TAParaManager.h"
#include "TAGPar.h"

//#define DEBUG // DEBUG MODE

using std::cout;
using std::endl;

static TACtrlPara *ctrlPara = TACtrlPara::Instance();
const double DEGREE = TAMath::DEGREE();
static TAGPar *gp = TAGPar::Instance();

TAMWDCArray4::TAMWDCArray4(const string &name, const string &title, unsigned uid)
		: TAMWDCArray(name, title, uid){
}
TAMWDCArray4::~TAMWDCArray4(){}

// get MWDC #dcId
TAMWDC *TAMWDCArray4::GetMWDC(int dcId) const{
	if(dcId < 0 || dcId > 33){
		TAPopMsg::Error(GetName().c_str(), "GetMWDC: dcId out of range: dcId: %d", dcId);
	}
	if(!fMWDC[dcId])
		TAPopMsg::Error(GetName().c_str(), "GetMWDC: requested dc pointer is null");
	return fMWDC[dcId];
}
void TAMWDCArray4::AssignTracks(vector<tTrack *> &track_ls){ // assign tracks
	if(!fTrackList[0].size()) return; // no tracks to assign
	int type[6]{}; TAUIDParser::DNS(type, GetUID());
	tTrack *ptrack_t = nullptr; // a temporary variable
	for(int l = 0; l < 3; l++){ // loop over X-U-V
		for(TATrack *x : fTrackList[l]){
//			x.SetFitRotationCenter(fMWDC[1]->GetZc(), fMWDC[1]->GetXc()); // DEBUG
//			x.SetFitMethod(TATrack::kNormalFit); // DEBUG
			ptrack_t = new tTrack;
			x->AssignTrack(ptrack_t);
			// track type: 1[LR][XUV] <=> 1[01][012]
			ptrack_t->type = 100 + (type[0] - 3) * 10 + l;
			track_ls.push_back(ptrack_t);
		}
	} // end for over l
} // end of funtion AssignTracks

#include "TAMWDCArray4/map4.C" // definition of bool Map(TAMWDC **MWDC, vector<TATrack> &track)
void TAMWDCArray4::Map(){ // map the fired channels in one data section once and for all
	if(4 != GetPlaT0()->GetFiredStatus()
	&& 3 != GetPlaT0()->GetFiredStatus()
	) return;

	Map4(fMWDC, fTrackList[0], 0); // X
} // end of function void Map()

void TAMWDCArray4::Initialize(){
	for(int i = 0; i < 4; i++){
		if(fMWDC[i]) fMWDC[i]->Initialize();
	}
	fN3DTrk = 0;
	if(fPlaT0) fPlaT0->Initialize();

	for(vector<TATrack *> &trls : fTrackList){
		for(TATrack *&tr : trls){
			if(tr){
				delete tr;
				tr = nullptr;
			}
		} // end for over tracks
		trls.clear();
	} // end for over vectors
} // end of function Initialize
// get the channel that belongs to this and has the specified uid

void TAMWDCArray4::Configure(){
	if(fMWDC[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetDsquareThresholdPerDot(TACtrlPara::DsquareThresholdPerDot(fUID));
	if(!fMWDC[0]) fMWDC[0] = new TAMWDCTaL(fName+"->DC0", fTitle+"->DC0", fUID+(0<<6));
	if(!fMWDC[1]) fMWDC[1] = new TAMWDCTaL(fName+"->DC1", fTitle+"->DC1", fUID+(1<<6));
	if(!fMWDC[2]) fMWDC[2] = new TAMWDCTaL(fName+"->DC2", fTitle+"->DC2", fUID+(2<<6));
	if(!fMWDC[3]) fMWDC[3] = new TAMWDCTaL(fName+"->DC3", fTitle+"->DC3", fUID+(3<<6));

	for(int i = 4; i--;){
		fMWDC[i]->SetMWDCId(i);
		fMWDC[i]->SetMotherDCArr(this);
		fMWDC[i]->Configure();
	}
} // end of function Configure

TAStuff *TAMWDCArray4::GetChannel(unsigned uid) const{
	if(uid > 0x7FFFF) return nullptr; // not a uid belonging to this class (only 19 bits)
	int type[6]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[6]{}; TAUIDParser::DNS(TYPE, GetUID()); // parse uid of this

	if(type[0] == TYPE[0]){ // belongs to this object
		if(type[1] < 4){ // MWDCs
			return
				GetMWDC(type[1])->GetSLayer(type[2])->GetCable(type[3])->GetSFE16(type[4])->GetAnode(type[5]);
		}
		else if(4 == type[1]){ // TOFWall
			TAPlaStrip *str = GetTOFWall()->GetStrip(type[2]);
			switch(type[3]){
				case 0: return str->GetUV();
				case 1: return str->GetUH();
				case 2: return str->GetDV();
				case 3: return str->GetDH();
				default: return nullptr;
			} // end switch
		}
	} // end if uid belongs to this object
	return nullptr;
} // end of function GetChannel
// display the detector information
void TAMWDCArray4::Info() const{
	if(!TAPopMsg::IsVerbose()) return;
	for(int i = 0; i < 4; i++){
		GetMWDC(i)->Info();
		cout << "----------------------------------------------------------\n";
	}
	GetPlaT0()->Info();
	cout << "\n\033[1m______________________________________________________________\033[0m\n\n";
}
