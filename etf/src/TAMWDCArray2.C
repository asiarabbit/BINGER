///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray2.C													 //
//   TAMWDCArray2.C -- source file for class TAMWDCArray2							 //
//   Introduction: MWDC array class, implementing pattern recognition and 3-D		 //
// tracking through track projection matching. Note that this type of MWDC array is  //
// composed of two MWDCs, which usually is placed upstream the dipole magnet.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/18.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <cstdlib>

#include "TAMWDCArray2.h"
#include "TACtrlPara.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TATrack2.h"
#include "tTrack.h"
#include "TAAnodePara.h"
#include "TAAnodeData.h"
#include "TAAnode.h"
#include "TADCSFE16.h"
#include "TADCCable.h"
#include "TADCSuperLayer.h"
#include "TAMWDC.h"
#include "TADetectorPara.h"
#include "TAChannel.h"
#include "TAParaManager.h"
#include "TAMath.h"
#include "TAPlaStrip.h"
#include "TAGPar.h"
#include "TAEventProcessor.h"
#include "tEntry.h"


static TAGPar *gp = TAGPar::Instance();

TAMWDCArray2::TAMWDCArray2(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid), TADetUnion(uid), fMWDC{0}, fPlaT0(0){
	// would be overwritten later in the derived classes
	// using values from TACtrlPara::DsquareThresholdPerDot()
	fDsquareThresholdPerDot = 100.;
}

TAMWDCArray2::~TAMWDCArray2(){
	for(TAMWDC *&dc : fMWDC) if(dc){
		delete dc;
		dc = nullptr;
	}
	for(vector<TATrack2 *> &trls : fTrackList){
		for(TATrack2 *&tr : trls){
			if(tr){
				delete tr;
				tr = nullptr;
			}
		} // end for over tracks
		trls.clear();
	} // end for over vectors
}

void TAMWDCArray2::SetDsquareThresholdPerDot(double d2Thre){
	if(d2Thre < 0.)
		TAPopMsg::Error(GetName().c_str(), "SetDsquareThresholdPerDot: minus input: %f", d2Thre);
	fDsquareThresholdPerDot = d2Thre;
}
vector<TATrack2 *> &TAMWDCArray2::GetTrackList(int dcType){
	if(dcType < 0 || dcType > 1){
		TAPopMsg::Error(GetName().c_str(), "GetTrackList: dcType out of range: dcType: %d", dcType);
	}
	return fTrackList[dcType];
}
// get MWDC #dcId
TAMWDC *TAMWDCArray2::GetMWDC(int dcId) const{
	if(dcId < 0 || dcId > 1){
		TAPopMsg::Error(GetName().c_str(), "GetMWDC: dcId out of range: dcId: %d", dcId);
	}
	if(!fMWDC[dcId])
		TAPopMsg::Error(GetName().c_str(), "GetMWDC: requested dc pointer is null");
	return fMWDC[dcId];
}
TAPlaStrip *TAMWDCArray2::GetPlaT0() const{
	if(!fPlaT0) TAPopMsg::Error(GetName().c_str(), "GetPlaT0: requested PlaT0 pointer is null");
	return fPlaT0;
}
void TAMWDCArray2::SetPlaT0(TAPlaStrip *t0){
	if(!t0) TAPopMsg::Error(GetName().c_str(), "SetPlaT0: input t0 is a null pointer");
	if(fPlaT0){
		TAPopMsg::Warn(GetName().c_str(), "SetPlaT0: fPlaT0 has already been assigned: %s", fPlaT0->GetName().c_str());
	}
	fPlaT0 = t0;
}

// assign the recognized TATrack2 objects to tTrack objects for assignments
void TAMWDCArray2::AssignTracks(vector<tTrack *> &track_ls){ // assign tracks
	if(!fTrackList[0].size() && !fTrackList[1].size()) return; // no tracks to assign

	static bool usingPDC = gp->Val(83);
	int index = TAEventProcessor::Instance()->GetEntryList()[0]->index;
	int type[6]{}; TAUIDParser::DNS(type, GetUID());
	tTrack *ptrack_t = nullptr; // a temporary variable
	for(int l = 0; l < 2; l++){ // loop over X-Y
		for(TATrack2 *x : fTrackList[l]){
//			x.SetFitRotationCenter(fMWDC[1]->GetZc(), fMWDC[1]->GetXc()); // DEBUG
//			x.SetFitMethod(TATrack::kNormalFit); // DEBUG
			ptrack_t = new tTrack;
			x->AssignTrack(ptrack_t);

			// track type: 1[UD, pdcUD][XY] <=> 1[23, 45][01]
			int detId = type[0];
			if(usingPDC){ // swithc 67 with 89
				switch(type[0]){
					case 6: detId = 8; break;
					case 7: detId = 9; break;
					case 8: detId = 6; break;
					case 9: detId = 7; break;
					default: break;
				}
			} // end if
			ptrack_t->type = 100 + (detId - 4) * 10 + l; // [67, 89]-4 -> [23, 45]
			ptrack_t->index = index;

			track_ls.push_back(ptrack_t);
		}
	} // end for over l
} // end of funtion AssignTracks

#include "TAMWDCArray2/map.C" // definition of bool Map(TAMWDC **MWDC, vector<TATrack2> &track)
void TAMWDCArray2::Map(){ // map the fired channels in one data section once and for all
	if(4 != GetPlaT0()->GetFiredStatus()
	&& 3 != GetPlaT0()->GetFiredStatus()
	) return;

	Map(fMWDC, fTrackList[0], 0); // X
	Map(fMWDC, fTrackList[1], 1); // Y
} // end of function void Map()

void TAMWDCArray2::Initialize(){
	if(fMWDC[0]) fMWDC[0]->Initialize();
	if(fMWDC[1]) fMWDC[1]->Initialize();
//	if(fPlaT0) fPlaT0->Initialize(); // fPlaT0 is adventitious

	for(vector<TATrack2 *> &trls : fTrackList){
		for(TATrack2 *&tr : trls){
			if(tr){
				delete tr;
				tr = nullptr;
			}
		} // end for over tracks
		trls.clear();
	} // end for over vectors
} // end of function Initialize
// get the channel that belongs to this and has the specified uid
TAStuff *TAMWDCArray2::GetChannel(unsigned uid) const{
	if(uid > 0x7FFFF) return nullptr; // not a uid belonging to this class (only 19 bits)
	int type[6]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[6]{}; TAUIDParser::DNS(TYPE, GetUID()); // parse uid of this

	if(type[0] == TYPE[0]){ // belongs to this object
		if(type[1] < 2){ // MWDCs
			return
				GetMWDC(type[1])->GetSLayer(type[2])->GetCable(type[3])->GetSFE16(type[4])->GetAnode(type[5]);
		}
		else if(2 == type[1]){ // the plastic scintillator timing detector
			TAPlaStrip *str = GetPlaT0();
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
} // end of member function GetChannel
// display the detector information
void TAMWDCArray2::Info() const{
	if(!TAPopMsg::IsVerbose()) return;
	for(int i = 0; i < 2; i++){
		GetMWDC(i)->Info();
		cout << "----------------------------------------------------------\n";
	}
	cout << "\n\033[1m______________________________________________________________\033[0m\n\n";
}
// a method dedicated for TAVisual::Fill()
void TAMWDCArray2::FillTrack(TGraph *gTrack, TGraph *gTrack_R) const{
	if(!gTrack || !gTrack_R)
		TAPopMsg::Error(GetName().c_str(), "FillTrack: input TGraph pointer is null");
	for(TATrack2 *t : fTrackList[0]) t->FillTrack(gTrack, gTrack_R);
}









