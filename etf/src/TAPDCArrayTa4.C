///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPDCArrayTa4.C													 //
//   TAPDCArrayTa4.C -- source file for class TAPDCArrayTa4							 //
//   Introduction: This class is for ameliorated tracking pre- and post-reaction	 //
// target. Note that the tracking is the spline of the two straight tracks, together //
// with consistency of target hit positions and the exit positions from the magField,//
// which has been particularly added to the tracking around the target zone to 		 //
// rectify the tracks and ameliorate the tracking accuracy. And currently this is	 //
// just for the amelioration of Xprojs.	This is a singleton class.					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2019/9/25.															     //
// Last modified: 2019/9/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2019, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>

#include "TAPopMsg.h"
#include "TATrackTa4.h"
#include "TAPDCArrayTa4.h"
#include "TAMWDCArray2.h"
#include "TAUI.h"
#include "TACtrlPara.h"
#include "TAMath.h"
#include "TAParaManager.h"
#include "tTrack.h"
#include "TAMWDC.h"
#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TAAnodeData.h"
#include "TAPlaStrip.h"
#include "TAGPar.h"


//#define DEBUG_MAP // debugging the map function

using std::cout;
using std::endl;

TAPDCArrayTa4 *TAPDCArrayTa4::fInstance = nullptr;
double TAPDCArrayTa4::fChi2ExtraThre = 80. / 3.;
// the object containing all the constructed detector, responsible for fired
// channel distribution, tracking and pid result output
static TACtrlPara *clp = TACtrlPara::Instance();
static TAGPar *gp = TAGPar::Instance();

TAPDCArrayTa4::TAPDCArrayTa4() : fDCArr{0}, fDC{0}{
	SetName("PDCArrayTa4");
	SetTitle("PDCArrTa4: For Splined Tracking around Ta Zone");
	Initialize();
	fTrackTa4List.reserve(1);
}

TAPDCArrayTa4::~TAPDCArrayTa4(){
	for(TATrackTa4 *&tr : fTrackTa4List){
		if(tr){
			delete tr;
			tr = nullptr;
		}
	} // end for over tracks
} // end the destructor

TAPDCArrayTa4 *TAPDCArrayTa4::Instance(){
	if(!fInstance) fInstance = new TAPDCArrayTa4();
	return fInstance;
}

void TAPDCArrayTa4::Initialize(){
	fK2 = -9999.; fB2 = -9999.;
	fIsReady = false; fZeroTrack = false;
	fTOF = -9999.;
}

TAMWDCArray2 *TAPDCArrayTa4::GetMWDCArray(int dcArrId) const{
	if(dcArrId != 0 && dcArrId != 1)
		TAPopMsg::Error(GetName().c_str(), "GetMWDCArray: dcArrId out of range, dcId: %d, only 0-dcArrL and 1-dcArrR are allowed", dcArrId);
	if(!fDCArr[dcArrId])
		TAPopMsg::Error(GetName().c_str(), "GetMWDCArray: requested dcArr pointer is null, dcArrId: %d", dcArrId);
	return fDCArr[dcArrId];
}

void TAPDCArrayTa4::SetPostMagXproj(double k, double b){
	fK2 = k; fB2 = b;
}

double TAPDCArrayTa4::GetK2() const{
	if(-9999. == fK2)
		TAPopMsg::Error(GetName().c_str(), "GetK2: fK2 not set.");
	return fK2;
}
double TAPDCArrayTa4::GetB2() const{
	if(-9999. == fB2)
		TAPopMsg::Error(GetName().c_str(), "GetB2: fB2 not set.");
	return fB2;
}


void TAPDCArrayTa4::AssignTracks(vector<tTrack *> &track_ls){
	if(!fTrackTa4List.size()){ // no tracks to assign
		TAPopMsg::Warn(GetName().c_str(), "AssignTracks: fTrackTa4List is empty");
		return;
	}
	if(fTrackTa4List.size() != 1){
		TAPopMsg::Error(GetName().c_str(), "AssignTracks: fTrackTa4List.size is not 1");
		return;
	}
	TATrackTa4 *trkTa4 = fTrackTa4List[0];
	TATrack2 *trk[2] = {trkTa4->GetTrackPreTa(), trkTa4->GetTrackPostTa()};

	tTrack *ptrack_t = nullptr; // a temporary variable
	for(int i = 0; i < 2; i++){
		ptrack_t = new tTrack;
		trk[i]->AssignTrack(ptrack_t);
		ptrack_t->type = 120 + i*10; // [pre-post]: [120, 130]
		track_ls.push_back(ptrack_t);
	} // end for over i	
} // end member function AssignTracks

// the Map function, the core track improving function
bool TAPDCArrayTa4::Map(){
	// check if this event is eligible for PID
	// if IsReady() returns true, the current event is appropriate for splined
	// target-track track amelioration
	if(!IsReady()){
		TAPopMsg::Warn(GetName().c_str(), "Map: The current event is not appropriate\
 (pre-Tracking not done yet) for splined target-zone track amelioration.");
		return false;
	}

	// prepare for the track amelioration,
	// needed info includes post-Mag track, and the pointers of the four PDCs
	// assign the four DCs around the target //
	static TAUI *usr = TAUI::Instance();
	static TAParaManager::ArrDet_t &det_vec
		= usr->GetParaManager()->GetDetList();
	if(!fDCArr[0]) fDCArr[0] = (TAMWDCArray2*)det_vec[8]; // pdc array U
	if(!fDCArr[1]) fDCArr[1] = (TAMWDCArray2*)det_vec[9]; // pdc array D
	for(int i = 2; i--;) for(int j = 2; j--;){
		if(!fDC[i][j] && fDCArr[i]){
			fDC[i][j] = fDCArr[i]->GetMWDC(j);
			if(!fDC[i][j]){
				TAPopMsg::Error(GetName().c_str(), "Map(): fDC[%d][%d] is nullptr", i, j);
				return false;
			} // end if DC is nullptr
		} // end if
	} // end doubly nested for loop
	// the track list
	static vector<tTrack *> &tl = usr->GetTrackList();
	
	
	///////////// THE CORE MAP ALGORITHM ///////////////
#ifdef DEBUG_MAP
	cout << endl << "\033[32;1m" << GetName() << "\33[0m" << endl; // DEBUG
	cout << "TAPDCArrayTa4::Map()" << endl; // DEBUG
	getchar(); // DEBUG
#endif
	vector<TATrackTa4 *> &track = fTrackTa4List; // just for convenience
	if(track.size()) track.clear();
	bool cmpShow = false; // debug for function int compare(...)
	const int UID = GetMWDCArray(0)->GetUID();
	const double d2Thre = clp->D2Thre(UID);
	const double d2ThrePD[2] = {
		GetMWDCArray(0)->GetDsquareThresholdPerDot(),
		GetMWDCArray(1)->GetDsquareThresholdPerDot()
	};
	char tail[64] = ""; // for naming newTrack
//	const double chiThre = clp->ChiThre(UID);
//	const double chiThrePD = clp->ChiThrePD(UID); // chi threshold per dot
	const double chiThrePD = 3.; // unit: mm
	const double chiThre = chiThrePD * 0.8; // XXX: exacting chi control
	short nAnodePerLayer[2][2]{};
	for(int i = 2; i--;) for(int j = 2; j--;){
		nAnodePerLayer[i][j] = fDC[i][j]->GetNAnodePerLayer();
	}
	double z[2][6], x[2][6], t[2][6], r[2][6], chi[2][6];
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 6; j++){
			z[i][j] = -9999.; x[i][j] = -9999.; t[i][j] = -9999.;
			r[i][j] = -9999.; chi[i][j] = -9999.;
		} // end for over j
	} // end for over i
	double kl[2], bl[2], d2[2], TOF[2] = {-9999., -9999.};
	double k2 = GetK2(), b2 = GetB2(), chi2ExtraThre = GetChi2ExtraThre();
	// the live track for the current hit combination
	TATrackTa4 newTrack("newTrackTa4", "Particle TrackTa4-ASIA.SUN");
	newTrack.SetK2(GetK2()); newTrack.SetB2(GetB2());
	TATrack2 *newTrackPreTa = newTrack.GetTrackPreTa();
	TATrack2 *newTrackPostTa = newTrack.GetTrackPostTa();
	TATrack2 *newtrk[2] = {newTrackPreTa, newTrackPostTa};
	int nu[2][4]; // [0-1]: [trkPreTa-trkPostTa], anode id in one layer
	// gGOOD: indicator for different fired sense wire layer scenarios
	int gGOOD[2], LAYER[2][4];
	int nFiredAnodeLayer[2]{}; // number of fired sense wire layers
	int overlapTrackCnt = 0; // for special use (checking of the tracking process)
	int overlap = -2; // track incompatibility type
	// normalEvent: all the 4 sense wire layers are fired
	// specialEvent: of all the 4 sense wire layers, only one of them is not fired
	// gGOOD: 1 -> specialEvent; 2 -> normalEvent
	bool normalEvent[2]{}, specialEvent[2]{};
	// valid drift time range
	double drfTA = gp->Val(42), drfTB = gp->Val(43);
	const short detId = GetMWDCArray(0)->GetDetId(); // 8-9: PDCArr; 6-7: DCTaArr
	if(8 == detId || 9 == detId){ // PDCArr
		drfTA = gp->Val(103); drfTB = gp->Val(104); // corresponds to larger drfit time
	}

	/////////////////////////////////////////////////////////////////////////
	///// ->->-> the LOOP begins ->->-> /////
	for(int i = 0; i <= nAnodePerLayer[0][0]; i++){ nu[0][0] = -1; // DC0-X1
		if(i < nAnodePerLayer[0][0] && fDC[0][0]->GetAnodeL1(0, i)->GetFiredStatus()) nu[0][0] = i; // DC0-X1 --------------------------------------------------------------
		if(-1 == nu[0][0] && i < nAnodePerLayer[0][0]) continue; // inert anodes within the anode layers would  be ignored

	for(int j = 0; j <= nAnodePerLayer[0][0]; j++){ nu[0][1] = -1; // DC0-X2
		if(j < nAnodePerLayer[0][0] && fDC[0][0]->GetAnodeL2(0, j)->GetFiredStatus()) nu[0][1] = j; // DC0-X1 --------------------------------------------------------------
		if(-1 == nu[0][1] && j < nAnodePerLayer[0][0]) continue;

	for(int k = 0; k <= nAnodePerLayer[0][1]; k++){ nu[0][2] = -1; // DC1-X1
		if(k < nAnodePerLayer[0][1] && fDC[0][1]->GetAnodeL1(0, k)->GetFiredStatus()) nu[0][2] = k; // DC1-X1 --------------------------------------------------------------
		if(-1 == nu[0][2] && k < nAnodePerLayer[0][1]) continue;

	for(int l = 0; l <= nAnodePerLayer[0][1]; l++){ nu[0][3] = -1; // DC1-X2
		if(l < nAnodePerLayer[0][1] && fDC[0][1]->GetAnodeL2(0, l)->GetFiredStatus()) nu[0][3] = l; // DC1-X2 --------------------------------------------------------------
		if(-1 == nu[0][3] && l < nAnodePerLayer[0][1]) continue;

	for(int ii = 0; ii <= nAnodePerLayer[1][0]; ii++){ nu[1][0] = -1; // DC2-X1
		if(ii < nAnodePerLayer[1][0] && fDC[1][0]->GetAnodeL1(0, ii)->GetFiredStatus()) nu[1][0] = ii; // DC2-X1 --------------------------------------------------------------
		if(-1 == nu[1][0] && ii < nAnodePerLayer[1][0]) continue;

	for(int jj = 0; jj <= nAnodePerLayer[1][0]; jj++){ nu[1][1] = -1; // DC2-X2
		if(jj < nAnodePerLayer[1][0] && fDC[1][0]->GetAnodeL2(0, jj)->GetFiredStatus()) nu[1][1] = jj; // DC2-X2 --------------------------------------------------------------
		if(-1 == nu[1][1] && jj < nAnodePerLayer[1][0]) continue;

	for(int kk = 0; kk <= nAnodePerLayer[1][1]; kk++){ nu[1][2] = -1; // DC3-X1
		if(kk < nAnodePerLayer[1][1] && fDC[1][1]->GetAnodeL1(0, kk)->GetFiredStatus()) nu[1][2] = kk; // DC3-X1 --------------------------------------------------------------
		if(-1 == nu[1][2] && kk < nAnodePerLayer[1][1]) continue;

	for(int ll = 0; ll <= nAnodePerLayer[1][1]; ll++){ nu[1][3] = -1; // DC3-X2
		if(ll < nAnodePerLayer[1][1] && fDC[1][1]->GetAnodeL2(0, ll)->GetFiredStatus()) nu[1][3] = ll; // DC3-X2 --------------------------------------------------------------
		if(-1 == nu[1][3] && ll < nAnodePerLayer[1][1]) continue;

		for(int I = 0; I < 2; I++){ // loop over DCsLayer[
			// all the 4 sense wires layers are effectively fired
			normalEvent[I] = nu[I][0] >= 0 && nu[I][1] >=0 && nu[I][2] >= 0 && nu[I][3] >= 0;
			specialEvent[I] = false;
			bool inert[4]{};
			if(!normalEvent[I]){
				inert[0] = nu[I][0] <  0 && nu[I][1] >= 0 && nu[I][2] >= 0 && nu[I][3] >= 0;
				inert[1] = nu[I][0] >= 0 && nu[I][1] <  0 && nu[I][2] >= 0 && nu[I][3] >= 0;
				inert[2] = nu[I][0] >= 0 && nu[I][1] >= 0 && nu[I][2] <  0 && nu[I][3] >= 0;
				inert[3] = nu[I][0] >= 0 && nu[I][1] >= 0 && nu[I][2] >= 0 && nu[I][3] <  0;
			} // end if
			// only one sense wire layer is inert
			specialEvent[I] = inert[0] || inert[1] || inert[2] || inert[3];
		} // end for over I: DCs
		// each sense wire layer has to be fired or at least three sense wire layers have to be fired
#ifdef DEBUG_MAP
		cout << "\033[33;1m\nThis is DC[0](PreTa)______________________\n";
		cout << endl << "i: " << i << "\tj: " << j; // DEUBG
		cout << endl << "k: " << k << "\tl: " << l; // DEUBG
		cout << endl; // DEBUG
		for(int i = 0; i < 4; i++){ // DEBUG
			cout << "  nu[0][" << i << "]: " << nu[0][i] << "\t"; // DEBUG
		}
		cout << endl; // DEBUG
		cout << "normalEvent[0]: " << normalEvent[0] << "\tspecialEvent[0]: " << specialEvent[0] << endl;
		cout << "\033[32;1m\nThis is DC[1](PostTa)______________________\n";
		cout << endl << "ii: " << ii << "\tjj: " << jj; // DEUBG
		cout << endl << "kk: " << kk << "\tll: " << ll; // DEUBG
		cout << endl; // DEBUG
		for(int i = 0; i < 4; i++){ // DEBUG
			cout << "  nu[1][" << i << "]: " << nu[1][i] << "\t"; // DEBUG
		}
		cout << endl; // DEBUG
		cout << "normalEvent[1]: " << normalEvent[1] << "\tspecialEvent[1]: " << specialEvent[1] << endl;
		cout << "END\033[0m" << endl;
		getchar(); // DEBUG
#endif
		if(!normalEvent[0] && !specialEvent[0]) continue;
		if(!normalEvent[1] && !specialEvent[1]) continue;

		for(int i = 0; i < 2; i++){
			gGOOD[i] = -1;
			if(normalEvent[i]) gGOOD[i] = 2;
			if(specialEvent[i]) gGOOD[i] = 1;
		}
	//	if(1 == gGOOD[0] || 1 == gGOOD[1]) continue;
		// initialization
		for(int i = 0; i < 2; i++){
			kl[i] = -9999.; bl[i] = -9999.;
			d2[i] = -9999.; 
			nFiredAnodeLayer[i] = 0;
			for(int j = 0; j < 4; j++){
				z[i][j] = -9999.; x[i][j] = -9999.;
				t[i][j] = -9999.; r[i][j] = -9999.;
				chi[i][j] = -9999.; LAYER[i][j] = -1;
				if(nu[i][j] >= 0){
					LAYER[i][nFiredAnodeLayer[i]++] = j;
					TAAnodePara *pa = fDC[i][j/2]->GetAnode(0, j%2+1, nu[i][j])->GetAnodePara();
					z[i][j] = pa->GetProjectionZ();
					x[i][j] = pa->GetProjectionX();
#ifdef DEBUG_MAP
					cout << std::fixed << std::setprecision(9);
					cout << "\tz[" << i << "][" << j << "] = " << z[i][j] << ";" << endl;
					cout << "\tx[" << i << "][" << j << "] = " << x[i][j] << ";" << endl;
#endif
				} // end if
			} // end for over j
			// assign initial values to kl and bl
			d2[i] = TAMath::Dsquare(z[i], x[i], kl[i], bl[i], gGOOD[i], LAYER[i], d2ThrePD[i]);
#ifdef DEBUG_MAP
			cout << endl << "d2[" << i << "]: " << d2[i]; // DEBUG
			cout << "\tkl[" << i << "]: " << kl[i]; // DEBUG
			cout << "\tbl[" << i << "]: " << bl[i] << endl;
			cout << "d2Thre[" << i << "] * nFiredAnodeLayer[" << i << "]: ";
			cout << d2Thre * nFiredAnodeLayer[i] << endl;
#endif
		} // end for over i
		double d2Extra = TAMath::Dx2DxTa_2(kl[0], kl[1], k2, bl[0], bl[1], b2);
#ifdef DEBUG_MAP
		cout << "d2Extra: " << d2Extra << "\tchi2ExtraThre * 1.5: " << chi2ExtraThre * 1.5 << endl; // DEBUUG
		getchar(); // DEBUG
#endif
		if(d2[0] < d2Thre * nFiredAnodeLayer[0] && d2[1] < d2Thre * nFiredAnodeLayer[1] && d2Extra < chi2ExtraThre * 1.5){
#ifdef DEBUG_MAP
			cout << "\033[31;1mBINGO!\033[0m" << endl; // DEBUG
			getchar(); // DEBUG
#endif
			// whether the current track is bad
			bool isBadTrack = false;
			overlapTrackCnt = 0;

			// choose the TOF_stop edge that gives a rational drift time //
			int lid = LAYER[0][0]; // using the layer closest to fPlaT0
			TAAnode *ano = fDC[0][lid/2]->GetAnode(0, lid%2+1, nu[0][lid]);
			const double t0 = ano->GetTime();
			const unsigned uid = ano->GetUID();
			const double delta = -clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid);
			// -20 ~ 250: speculated drift time range
			// 0+t_wire+t_drift=t_DC; 0-t_tof=t_TOF;
			// t_TOF-t_DC=(-t_tof-t_wire) - t_drift; => delta-t_drift
			// (as small and correct as possible while inclusive)
			// drift time bound for drift time start selection
			const double t1 = delta - drfTB, t2 = delta - drfTA; // the range borders
			// the two PDCs are separately synchronized to their own plugins
			// so have two versions of T0_1
			for(int i = 0; i < 2; i++){
				TOF[i] = GetMWDCArray(i)->GetPlaT0()->GetTime(t0, t1, t2);
				if(-9999. == TOF[i]){ // drift time start is not available
					isBadTrack = true;
					continue;
				}
			} // end for over i: two PDCs
			// assign the drift time array and drift distance array
			for(int i = 0; i < 2; i++){ // loop over DCTas
				for(int j = 0; j < 4; j++) if(nu[i][j] >= 0){ // loop over anode layers
					TAAnode *ano = fDC[i][j/2]->GetAnode(0, j%2+1, nu[i][j]);
					ano->GetAnodeData()->SetTOF(TOF[i]);
					t[i][j] = ano->GetDriftTime();
					unsigned uid = ano->GetUID();
					t[i][j] +=
						-clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid);
					// test the validity of drift time
					if(!TAMath::Within(t[i][j], drfTA, drfTB)) isBadTrack = true;
					if(-9999. != TOF[i]) r[i][j] = ano->GetDriftDistance(t[i][j], kl[i]);
				} // end for over j
#ifdef DEBUG_MAP
				for(double tt : t[i]) cout << "t: " << tt << endl; // DEBUG
				getchar(); // DEBUG
				for(double rr : r[i]) cout << "r: " << rr << endl; // DEBUG
				cout << "------------------------------------------\n"; // DEBUG
				getchar(); // DEBUG
#endif
			} // end for over i
			if(isBadTrack) continue;


			/// assign newTrack ///
			newTrack.Initialize(); // initialize newTrack
			for(int i = 0; i < 2; i++){ // loop over the two DCTas
				newtrk[i]->SetData(x[i], z[i], t[i], r[i], kl[i], bl[i], d2[i], gGOOD[i], nu[i], LAYER[i]);
				newtrk[i]->SetTOF(TOF[i], -1, -9999.);
			} // end for over i


			//////// XXX XXX XXX XXX XXX XXX XXX XXX //////////
			//// // the magnificant majestic FIT method // ////
			newTrack.Fit();
			//////// XXX XXX XXX XXX XXX XXX XXX XXX //////////


			/// check the fitting ///
#ifdef DEBUG_MAP
			newTrack.Show(); // DEBUG
#endif
			for(int i = 0; i < 2; i++){ // loop over the two DCTas
				newtrk[i]->GetChi(chi[i]);
#ifdef DEBUG_MAP
				cout << "newtrk[" << i << "].GetChi2(): " << newtrk[i]->GetChi() << endl;
				for(double cc : chi[i]) cout << "cc: " << cc << endl; // DEBUG
				getchar(); // DEBUG
#endif
				if(fabs(newtrk[i]->GetChi()) > chiThre){
					isBadTrack = true;
					continue;
				}
//				cout << "mark 1" << endl; // DEBUG
				for(double cc : chi[i]){
					if(-9999. != cc && fabs(cc) > chiThrePD){
						isBadTrack = true;
						continue;
					} // end inner if
				} // end for over chi array elements
			} // end for over i
//			cout << "mark 2" << endl; // DEBUG
			if(newTrack.GetChi2Extra() > GetChi2ExtraThre()) isBadTrack = true;
			if(fabs(newtrk[0]->GetSlope() - newtrk[1]->GetSlope()) > 0.011){
				isBadTrack = true; // 0.011: 3 sigma of dk. Heng!
			}
//			cout << "mark 3" << endl; // DEBUG
			if(isBadTrack) continue;

			//////////////// INCOMPATIBILITY CHECK ////////////////////
			// check if two tracks derives from the same particle //
			for(unsigned k = 0; k < track.size(); k++){
				// 0: the two tracks are different; 1: newTrack is defeated by oldTrack
				// 2: newTrack defeats oldTrack
				// compare the two tracks, and mark the obsolete ones
				overlap = compare(&newTrack, track.at(k), cmpShow);
				if(cmpShow){ // DEBUG
					TAPopMsg::Debug(GetName().c_str(), "map: overlap: %d", overlap); // DEBUG
				} // DEBUG
				if(2 == overlap){ // an overlap happened
					overlapTrackCnt++; // overlapTrackCnt'd never be larger than 1
				} // end if
				if(1 == overlap) continue; // newTrack is part of oldTrack, and would be dropped
			} // end for over k
			// eliminate the obsolete tracks
			for(unsigned k = 0; k < track.size(); k++){
				if(track[k]->GetName() == "OBSOLETE"){
					delete track[k]; track[k] = nullptr;
					track.erase(track.begin()+k);
					k--;
				} // end if
			} // end for over k (the cached tracks)
#ifdef DEBUG_MAP
			if(1 != overlap){
				TAPopMsg::Debug(GetName().c_str(), "map: new track confirmed, track.size(): %d, overlap: %d_______________", track.size(), overlap);
			}
			if(cmpShow){
				TAPopMsg::Debug(GetName().c_str(), "map: Before pushback, track.size(): %d", track.size());
			} // end if // DEBUG
#endif
			if(1 != overlap){ // new track accepted
				newTrack.SetName(GetName());
				sprintf(tail, "->TrackTa4X_%lu", track.size());
				newTrack.AppendName(tail);
				track.push_back(new TATrackTa4(newTrack));
			}
#ifdef DEBUG_MAP
			if(cmpShow){ // DEBUG
				TAPopMsg::Debug(GetName().c_str(), "map: After pushback, track.size(): %d", track.size());
			} // end if
			cout << "track.size(): " << track.size(); // DEBUG
			getchar(); // DEBUG
			for(TATrackTa4 *t : track) t->Show();
			getchar(); // DEBUG
#endif
		} // end BINGO (dsquare and )

	} // end of DC3-X2 loop
	} // end of DC3-X1 loop
	} // end of DC2-X2 loop
	} // end of DC2-X1 loop
	} // end of DC1-X2 loop
	} // end of DC1-X1 loop
	} // end of DC0-X2 loop
	} // end of DC0-X1 loop
	/////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_MAP
// sleep(3);
	TAPopMsg::Debug(GetName().c_str(), "map: track.size(): %d", track.size());
	for(auto t : track) t->Show(); // DEBUG
	cout << "Map returning true..." << endl; // DEBUG
	getchar(); // DEBUG
#endif

	// updated fTrackList with the ameliorated tracks
	if(0 == track.size()){
		fZeroTrack = true;
		return true;
	}
	// only keep the minimal chi2 track
	if(track.size() > 1){
		double d2min = 1e200;
		for(TATrackTa4 *t : track){
			if(t->GetChi2() < d2min){
				d2min = t->GetChi2();
			}
		} // end for
		vector<TATrackTa4 *>::iterator it = 
			remove_if(track.begin(), track.end(), [=](TATrackTa4 *t){ return !(t->GetChi2() == d2min); });
		track.erase(it, track.end());
	} // end if
	/////// remove all the UX and DX tracks to be replaced by new ones	
	vector<tTrack *>::iterator last = remove_if(tl.begin(), tl.end(), [](tTrack *t){ return t->type == 120 || t->type == 130; });
	tl.erase(last, tl.end());
	AssignTracks(tl);

	return true;

} // end of Map function

int TAPDCArrayTa4::compare(TATrackTa4 *newTrack, TATrackTa4 *oldTrack, bool show){
	int vicinity = clp->Vicinity();
	if(GetMWDCArray(0)->GetDetId() == 8) vicinity = 2; // PDC
	// PDC requires specific vicinity condition (drift distance too large)
	// fired anode layers
	int nValid_nu = newTrack->GetNFiredAnodeLayer();
	int nValid_nu_temp = oldTrack->GetNFiredAnodeLayer();
	int nu[12], nu_temp[12];
	newTrack->GetNu(nu);
	oldTrack->GetNu(nu_temp);

	if(show){ // DEBUG
		cout << "nValid_nu: " << nValid_nu << "\tnValid_nu_temp: " << nValid_nu_temp << endl; // DEBUG
//		newTrack->Show(); oldTrack->Show(); // DEBUG
	} // end if(show) // DEBUG
	
	
	int vicinityVioCnt = 0; // vicinity violation count
	if(nValid_nu < nValid_nu_temp){
		// to tell if the two are completely seperate
		for(int i = 0; i < 12; i++){
			if(nu[i] >= 0 && nu_temp[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
				} // end if two anodes are seperate
			} // end if two anode are fired at the same time
			if(vicinityVioCnt == nValid_nu) return 0; // different tracks
		} // end for over anode layers
		if(show){
			cout << "mark2" << endl; // DEBUG
			getchar();
		}
		return 1; // newTrack is defeated
	} // end if newTrack is shorter than oldTrack

	vicinityVioCnt = 0;
	if(nValid_nu > nValid_nu_temp){
		for(int i = 0; i < 12; i++){
			if(nu_temp[i] >= 0 && nu[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
				}
			} // end if two anodes are fired at the same time
		} // end for over i
		if(vicinityVioCnt == nValid_nu_temp) return 0;
		oldTrack->SetName("OBSOLETE");
		return 2;
	} // end if newTrack is longer than oldTrack
	
	vicinityVioCnt = 0;
	if(nValid_nu == nValid_nu_temp){
		for(int i = 0; i < 12; i++){
			if(nu_temp[i] >= 0 && nu[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
				}
			} // end if two anodes are fired at the same time
		} // end for over i
		if(nValid_nu == vicinityVioCnt) return 0;
		if(newTrack->GetChi2() < oldTrack->GetChi2()){
			oldTrack->SetName("OBSOLETE");
			return 2;
		}
		else{
			if(show){ // DEBUG
				cout << "mark 3" << endl;
				getchar(); // DEBUG
			} // DEBUG
			return 1; // newTrack is defeated by oldTrack
		} // end else
	} // end if the two tracks are of the same length
	
	return 0;
} // end member function compare

// a method dedicated for TAVisual::Fill()
void TAPDCArrayTa4::FillTrack(TGraph *gTrack, TGraph *gTrack_R) const{
	if(!gTrack || !gTrack_R)
		TAPopMsg::Error(GetName().c_str(), "FillTrack: input TGraph pointer is null");
	for(TATrackTa4 *t : fTrackTa4List){
		t->GetTrackPreTa()->FillTrack(gTrack, gTrack_R, 5);
		t->GetTrackPostTa()->FillTrack(gTrack, gTrack_R, 5);
	}
} // end member method FillTrack








