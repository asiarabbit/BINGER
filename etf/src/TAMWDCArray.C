///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray.C														 //
//   TAMWDCArray.C -- source file for class TAMWDCArray								 //
//   Introduction: MWDC array class, implementing pattern recognition and 3-D		 //
// tracking through track projection matching.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/7.															     //
// Last modified: 2018/1/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include "TH1F.h" // ROOT include
#include "TAMWDCArray.h"
#include "TACtrlPara.h"
#include "TAUIDParser.h"
#include "TATrack.h"
#include "TAPopMsg.h"
#include "tTrack.h"
#include "TAAnodePara.h"
#include "TAAnodeData.h"
#include "TAAnode.h"
#include "TADCSFE16.h"
#include "TADCCable.h"
#include "TADCSuperLayer.h"
#include "TAMWDC.h"
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
const double TAMWDCArray::kPropagationSpeed = 200.; // mm/ns
const double DEGREE = TAMath::DEGREE();
static TAGPar *gp = TAGPar::Instance();

TAMWDCArray::TAMWDCArray(const string &name, const string &title, int uid)
		: TAStuff(name, title, uid), fMWDC{0}, fTOFWall(0){
	fPhiAvrg = -9999.;
	fDsquareThresholdPerDot = 100.;
}
TAMWDCArray::~TAMWDCArray(){
	for(TAMWDC *dc : fMWDC) if(dc) delete dc;
	if(fTOFWall) delete fTOFWall;

	for(vector<TATrack *> &trls : fTrackList){
		for(TATrack *tr : trls){
			if(tr){
				delete tr;
				tr = nullptr;
			}
		} // end for over tracks
		trls.clear();
	} // end for over vectors
}
void TAMWDCArray::SetDsquareThresholdPerDot(double d2Thre){
	if(d2Thre < 0.)
		TAPopMsg::Error(GetName().c_str(), "SetDsquareThresholdPerDot: minus input: %f", d2Thre);
	fDsquareThresholdPerDot = d2Thre;
}
vector<TATrack *> &TAMWDCArray::GetTrackList(int dcType){
	if(dcType < 0 || dcType > 2){
		TAPopMsg::Error(GetName().c_str(), "GetTrackList: dcType out of range: dcType: %d", dcType);
	}
	return fTrackList[dcType];
}
// get MWDC #dcId
TAMWDC *TAMWDCArray::GetMWDC(int dcId) const{
	if(dcId < 0 || dcId > 2){
		TAPopMsg::Error(GetName().c_str(), "GetMWDC: dcId out of range: dcId: %d", dcId);
	}
	if(!fMWDC[dcId])
		TAPopMsg::Error(GetName().c_str(), "GetMWDC: requested dc pointer is null");
	return fMWDC[dcId];
}
TATOFWall *TAMWDCArray::GetTOFWall() const{
	if(!fTOFWall){
		TAPopMsg::Error(GetName().c_str(), "GetTOFWall: fTOFWall is null.");
	}
	return fTOFWall;
}
double TAMWDCArray::GetPhiAvrg(){
	if(-9999. == fPhiAvrg){
		fPhiAvrg = 0.;
		for(int i = 3; i--;) fPhiAvrg += GetMWDC(i)->GetDetPara()->GetPhi();
		fPhiAvrg /= 3.;
	}
	return fPhiAvrg;
}

void TAMWDCArray::AssignTracks(vector<tTrack *> &track_ls){ // assign tracks
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

#include "TAMWDCArray/map.C" // definition of bool Map(TAMWDC **MWDC, vector<TATrack> &track)
void TAMWDCArray::Map(){ // map the fired channels in one data section once and for all.
//	cout << GetName() << endl; // DEBUG
//	cout << "GetTOFWall()->GetNFiredStrip(): " << GetTOFWall()->GetNFiredStrip() << endl; getchar(); // DEBUG
	if(GetTOFWall()->GetNFiredStrip() <= 0) return; // event filter

	Map(fMWDC, fTrackList[0], 0); // X
	if(ctrlPara->Is3DTracking()){
		Map(fMWDC, fTrackList[1], 1); // U
		Map(fMWDC, fTrackList[2], 2); // V
		// merge track projections into 3-D tracks by labelling them with 3-D track ids
		TrackMerger();
	}
} // end of function void Map()

// to see if particle track resolved from UV data is compatible with that from X data.
bool TAMWDCArray::Compatible(double k, double b, double ku, double bu, double kv, double bv, double &k2, double &b2, double *xMiss3D){
	const double phi = GetPhiAvrg();
	double k1t = TAMath::kUV_X(phi, ku, kv);
	double b1t = TAMath::bUV_X(phi, ku, kv, bu, bv);
	double z, x, xt;
#ifdef DEBUG
	cout << "TAMath::kUV_X(" << phi / DEGREE << ", " << ku << ", " << kv << "): ";
	cout << TAMath::kUV_X(phi, ku, kv) << endl; // DEBUG
	cout << "ku: " << ku << "\tkv: " << kv << endl; // DEBUG
	cout << "bu: " << bu << "\tbv: " << bv << endl; // DEBUG
	cout << "phi: " << phi / DEGREE << endl; // DEBUG
	cout << "k1: " << k << "\tb1: " << b << endl; // DEBUG
	cout << "k1t: " << k1t << "\tb1t: " << b1t << endl; // DEBUG
	k2 = TAMath::kUV_Y(phi, ku, kv); // DEBUG
	b2 = TAMath::bUV_Y(phi, ku, kv, bu, bv); // DEBUG
	cout << "k2: " << k2 << "\tb2: " << b2 << endl; // DEBUG
	cout << "Coincidence test begin: \n"; getchar(); // DEBUG
#endif

	bool coincide = true;
	for(int i = 0; i < 3; i++){
		z = fMWDC[i]->GetDetPara()->GetZ();
		xt = k1t*z+b1t; x = k*z+b;
		xMiss3D[i] = x-xt;
#ifdef DEBUG
		cout << "z: " << z << "\txt: " << xt << "\tx: " << x << endl; // DEBUG
		cout << "fabs(x-xt): " << fabs(x-xt) << endl; getchar(); // DEBUG
#endif
		// test coincidence in every MWDC.
		if(fabs(x-xt) > TACtrlPara::Get3DCoincideWindow()){
			coincide = false; // 105: 10.5 DC cells, given all kinds of errors
		}
	}
	if(!coincide) return false;

	k2 = TAMath::kUV_Y(phi, ku, kv);
	b2 = TAMath::bUV_Y(phi, ku, kv, bu, bv);
	return true;
}

void TAMWDCArray::TrackMerger(){ // assembly projections to 3-D tracks
#ifdef DEBUG
	cout << "\033[32;1m" << GetName() << "\033[0m" << endl; // DEBUG
	cout << "This is TAMWDCArray::TrackMerger():" << endl; // DEBUG
	cout << "____________________________________________" << endl; getchar(); // DEBUG
//	for(TATrack *&x : fTrackList[0]) x->Show(); // DEBUG
//	for(TATrack *&u : fTrackList[1]) u->Show(); // DEBUG
//	for(TATrack *&v : fTrackList[2]) v->Show(); // DEBUG
	cout << "____________________________________________" << endl; getchar(); // DEBUG
	cout << "\n\n\033[33;1m____________________________________________\n\033[0m"; // DEBUG
	cout << "x.size(): " << fTrackList[0].size(); cout << "\tu.size(): " << fTrackList[1].size(); // DEBUG
	cout << "\tv.size(): " << fTrackList[2].size(); // DEBUG
	getchar(); // DEBUG
#endif
	if(fTrackList[0].size() > 0){
		((TH1F*)gp->Agent(0))->Fill(fTrackList[1].size()); // DEBUG
		((TH1F*)gp->Agent(1))->Fill(fTrackList[2].size()); // DEBUG
	}

	if(fTrackList[0].size() <= 0 || // X tracks are indispensable.
		// no U or V tracks are found, no 3D tracks would be matched
		0 == fTrackList[1].size() || 0 == fTrackList[2].size()){
		fTrackList[1].clear();
		fTrackList[2].clear();
		fN3DTrk = 0;
		return;
	} // end if

	bool BINGO = false; // X, U, V are projections of the same one 3-D straight track.
	int id = -1; // 3-D track identifier
	double k2, b2;
	// initialize TATrack member variable -- marked.
	// marked == true indicates that the track projection has found its mother 3-D track already.
	for(TATrack *&x : fTrackList[0]) x->marked = false;
	for(TATrack *&u : fTrackList[1]) u->marked = false;
	for(TATrack *&v : fTrackList[2]) v->marked = false;
	for(unsigned i = 0; i < fTrackList[0].size(); i++){ // loop over track X
		TATrack *&x = fTrackList[0][i];
		bool isMatched = false; // whether the current X track finds its companies.
//		cout << "x.marked: " << x.marked << endl; getchar(); // DEBUG
		for(unsigned j = 0; j < fTrackList[1].size(); j++){ // loop over track U
			TATrack *&u = fTrackList[1][j];
//			u.Show(); // DEBUG
//			cout << "u.marked: " << u.marked << endl; getchar(); // DEBUG
			int id0 = id + 1; if(isMatched) id0 = id; // the current 3D track id
			if(u->Get3DId() != -1 && u->Get3DId() < id0)
				continue; // owned by previous Xes.
			for(unsigned k = 0; k < fTrackList[2].size(); k++){ // loop over track V
				TATrack *&v = fTrackList[2][k];
//				v->Show(); // DEBUG
//				cout << "v->marked: " << v->marked << endl; getchar(); // DEBUG
				// One projection cannot be matched with multiple 3-D tracks.
				if(v->marked) continue;
				double xMiss3D[3]{};
				BINGO = Compatible(x->GetSlope(), x->GetIntercept(), u->GetSlope(), u->GetIntercept(), v->GetSlope(), v->GetIntercept(), k2, b2, xMiss3D);
				if(BINGO){
					// make use of the TOF information of X track projection //
					double TOF = x->GetTOF();
					double firedStripId = x->GetFiredStripId();
					double nStripStray = x->GetNStripStray();
					double tu[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
					double ru[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
					double tv[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
					double rv[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
					// weight for weighted addition of chi to chi2
					double weightU[6] = {1., 1., 1., 1., 1., 1.};
					double weightV[6] = {1., 1., 1., 1., 1., 1.};
					int nuU[6] = {-1, -1, -1, -1, -1, -1};
					int nuV[6] = {-1, -1, -1, -1, -1, -1};
					u->GetNu(nuU); v->GetNu(nuV);
					for(int l = 0; l < 6; l++){
						if(-1 != nuU[l]){
							TAAnode *ano = fMWDC[l/2]->GetAnode(1, l%2+1, nuU[l]);
							((TAAnodeData*)ano->GetData())->SetTOF(TOF);
							tu[l] = ano->GetDriftTime(weightU[l]);
							// rough correction for time of flight from DC to TOF wall
							tu[l] += ctrlPara->T_tofDCtoTOFW(ano->GetUID()) - ctrlPara->T_wireMean(ano->GetUID());
							ru[l] = ano->GetDriftDistance(tu[l], u->GetSlope());
						} // end if
						if(-1 != nuV[l]){
							TAAnode *ano = fMWDC[l/2]->GetAnode(2, l%2+1, nuV[l]);
							((TAAnodeData*)ano->GetData())->SetTOF(TOF);
							tv[l] = ano->GetDriftTime(weightV[l]);
							// rough correction for time of flight from DC to TOF wall
							tv[l] += ctrlPara->T_tofDCtoTOFW(ano->GetUID()) - ctrlPara->T_wireMean(ano->GetUID());
							rv[l] = ano->GetDriftDistance(tv[l], v->GetSlope());
						} // end if
					} // end for over i
					u->SetTOF(TOF, firedStripId, nStripStray);
					v->SetTOF(TOF, firedStripId, nStripStray);
					u->SetDriftTime(tu, weightU); u->SetDriftDistance(ru);
					v->SetDriftTime(tv, weightV); v->SetDriftDistance(rv);
					// check the validity of U and V tracks
					short badTrk = 0;
					for(double tt : tu) if(-9999. != tt && !ctrlPara->TimeThre(tt)) { badTrk = 1; break; }
					for(double tt : tv) if(-9999. != tt && !ctrlPara->TimeThre(tt)) { badTrk = 2; break; }
#ifdef DEBUG
					u->Show(); // DEBUG
					v->Show(); // DEBUG
					cout << "u->GetChi(): " << u->GetChi() << endl; // DEBUG
					cout << "v->GetChi(): " << v->GetChi() << endl; // DEBUG
					cout << "ctrlPara->ChiThre(): " << ctrlPara->ChiThre() << endl; // DEBUG
					cout << "badTrk: " << badTrk << endl; getchar(); // DEBUG
					if(badTrk) continue; // nasty combination
#endif
					if(fabs(u->GetChi()) > ctrlPara->ChiThre()) continue; // nasty combination
					if(fabs(v->GetChi()) > ctrlPara->ChiThre()) continue; // nasty combination
					double chi[6]{};
					u->GetChi(chi);
					for(double cc : chi)
						if(-9999. != cc && fabs(cc) > ctrlPara->ChiThrePD()){ badTrk = 3; break; }
					v->GetChi(chi);
					for(double cc : chi)
						if(-9999. != cc && fabs(cc) > ctrlPara->ChiThrePD()){ badTrk = 4; break; }
#ifdef DEBUG
					cout << "badTrk: " << badTrk << endl; getchar(); // DEBUG
#endif
					if(badTrk) continue; // nasty combination

					if(!isMatched){ id++; isMatched = true; }
					x->Set3DId(id); x->marked = true;
					u->Set3DId(id); u->marked = true;
					v->Set3DId(id); v->marked = true;
					memcpy(x->fXMiss3D, xMiss3D, sizeof(xMiss3D));
					memcpy(u->fXMiss3D, xMiss3D, sizeof(xMiss3D));
					memcpy(v->fXMiss3D, xMiss3D, sizeof(xMiss3D));
#ifdef DEBUG
					// // // // // // // // // // // // // // // // // // //
					cout << "x3DMiss: " << x->fXMiss3D[0] << " ";
					cout << x->fXMiss3D[1] << " " << x->fXMiss3D[2] << endl;
					cout << "k1: " << x->GetSlope(); // DEBUG
					cout << "\tb1: " << x->GetIntercept() << endl; // DEBUG
					cout << "k2: " << k2 << "\tb2: " << b2; // DEBUG
					cout << "\tid: " << id << endl; // DEBUG
//					x->Show(); u->Show(); v->Show(); getchar(); // DEBUG
#endif
				} // end if
			} // end of loop over track v
		} // end of loop over track u
	} // end of loop over track x

	// clean eliminated tracks
	cleanUp(fTrackList[1], id + 1);
	cleanUp(fTrackList[2], id + 1);
#ifdef DEBUG
	cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl; // DEBUG
	cout << "x.size(): " << fTrackList[0].size(); cout << "\tu.size(): " << fTrackList[1].size(); // DEBUG
	cout << "\tv.size(): " << fTrackList[2].size(); // DEBUG
	cout << "\n\033[35;3;1;4m3DId of X tracks\033[0m\n";
	for(TATrack *x : fTrackList[0]) cout << x->Get3DId() << " "; // DEBUG
	for(TATrack *x : fTrackList[0]) x->Show(); // DEBUG
	for(TATrack *x : fTrackList[1]) x->Show(); // DEBUG
	for(TATrack *x : fTrackList[2]) x->Show(); // DEBUG
	cout << "____________________________________________" << endl; getchar(); // DEBUG
#endif
//	for(TATrack &x : fTrackList[0]) x.Show(); // DEBUG
	// number of 3D tracks in the data section; used for global 3DTrk identification
	fN3DTrk = id + 1;
} // end of function TrackFilter().
/// select the best U and V track according to compare(), and clean up unmatched tracks. ///
// n: number of 3D tracks
// because multiple U and V tracks could match one X track, the redundant ones are eliminated in this function
void TAMWDCArray::cleanUp(vector<TATrack *> &tr, const int n){
	// eliminate inferior tracks for every 3D track
//	cout << "n: " << n << endl; // DEBUG
//	for(TATrack &x : tr) cout << x.Get3DId() << " "; // DEBUG
	for(int i = 0; i < n; i++){
		TATrack *tm = nullptr; // the optimal track
//		cout << "\ni: " << i << endl; getchar(); // DEBUG
		const int ntrk = tr.size();
		for(int j = 0; j < ntrk; j++){
			if(tr[j]->Get3DId() != i) continue; // tracks with 3Did == -1 would be eliminated later
			if(!tm) tm = tr[j];
			TATrack *tt = tr[j];
//			cout << "tm: " << tm << "\ttt: " << tt << endl; // DEBUG
			if(tt != tm){
				const int overlap = compare(tt, tm, 0);
//				cout << "overlap: " << overlap << endl; getchar(); // DEBUG
				if(1 == overlap){ // tt is defeated
					tt->Set3DId(-1);
				}
				if(2 == overlap){
					tm->Set3DId(-1); // tm is defeated
					tm = tt;
				}
				if(0 == overlap){ // shit happens
					if(tt->GetChi() > tm->GetChi()){
						tt->Set3DId(-1);
					}
					else{
						tm->Set3DId(-1);
						tm = tt;
					}
				} // end if
			} // end if(tt != tm)
		} // end for over j
	} // end for over i

//	cout << "HAHAHAHAHn: " << n << endl; // DEBUG
//	for(TATrack &x : tr) cout << x.Get3DId() << ""; // DEBUG
	// erase the unmatched and defeated tracks
	for(unsigned i = 0; i < tr.size(); i++){
		if(-1 == tr[i]->Get3DId()){
			delete tr[i]; tr.erase(tr.begin()+i); // erase tr[k]
			i--;
		} // end if
	} // end for over k
//	cout << "tr.size(): " << tr.size() << endl; getchar(); // DEBUG
} // end function cleanUp


// drift time correction: time of flight and signal propagation time
// p[4]: [0-3]: [k1, k2, b1, b2]; a[0-1]: [layer0~layer18, nu]
// t = T_DC - T_TOF
void TAMWDCArray::DriftTimeCorrection(double &t, double &r, const double *a, const double *p, int firedStripId, double beta){
	if(beta < 0. || beta > 1.) return; // invalid beta
	int LAYER = int(a[0]); // 0~17: nuX, nuU+6, nuV+6
	int j = LAYER / 6; // j: 0-1-2: X-U-V
	int k = LAYER % 6; // k: DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
	int DCid = k / 2; // 0-1-2: DC0-DC1-DC2
	int layerType = LAYER % 2 + 1; // 1-2: anodeL1-anode L2
	int nu = int(a[1]); // 0~79; anode id in the layer

	double Bz = GetMWDC(1)->GetDetPara()->GetZ(); // DC[1]-X1-central z
	double B[3] = {0., 0., Bz}, b[3] = {0., 0., 1.}; // B[2] = 0. and b[2] = 1.; preset values.
	B[0] = p[2]+B[2]*p[0]; // B[0] = b1+B[2]*k1;
	B[1] = p[3]+B[2]*p[1]; // B[1] = b2+B[2]*k2;
	b[0] = b[2]*p[0]; // b[0] = b[2]*k1;
	b[1] = b[2]*p[1]; // b[1] = b[2]*k2;
	double T_wire = GetWirePropagationTime(b, B, nu, j, k);
	double T_tof = GetTimeOfFlight(b, B, nu, j, k, firedStripId, beta);
	// t = T_DC - T_TOF; T_DC - (T_TOF - T_tof) = T_wire + T_drift
	t += T_tof - T_wire; // t + T_tof = T_wire + T_drift => T_drift = t + T_tof - T_wire
	double slope = 0.;
	if(0 == j) slope = p[0]; // X
	else if(1 == j) slope = TAMath::kXY_U(GetPhiAvrg(), p[0], p[1]); // U
	else slope = TAMath::kXY_V(GetPhiAvrg(), p[0], p[1]); // V
	r = GetMWDC(DCid)->GetAnode(j, layerType, nu)->GetDriftDistance(t, slope);

#ifdef DEBUG
	cout << "Bz: " << Bz << endl; // DEBUG
	cout << "b[0]: " << b[0] << "\tb[1]: " << b[1] << "\tb[2]: " << b[2] << endl; // DEBUG
	cout << "B[0]: " << B[0] << "\tB[1]: " << B[1] << "\tB[2]: " << B[2] << endl; // DEBUG
	cout << "p[0]: " << p[0] << "\tp[1]: " << p[1] << "\tp[2]: " << p[2] << "\tp[3]: " << p[3] << endl; // DEBUG
	cout << "nu: " << nu << "\tj: " << j << "\tk: " << k << "\tDCid: " << DCid << "\tlayerType: " << layerType << "\tLAYER: " << LAYER << endl; // DEBUG
	cout << "firedStripId: " << firedStripId << endl; // DEBUG
	cout << "j: " << j << "\tk: " << k << endl; // DEBUG
	cout << "T_wire: " << T_wire << "\tT_tof: " << T_tof << endl; // DEBUG
	cout << "t: " << t << "\tr: " << r << endl; getchar(); // DEBUG
#endif
} // end of function DriftTimeCorrection
// tool functions for drift time correction j: X-U-V; k: DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
static const double c0 = TAParaManager::Instance()->GetPhysConst("c0");
double TAMWDCArray::GetWirePropagationTime(const double *b, const double *B, int nu, int j, int k){
	return GetWirePropagationLength(b, B, nu, j, k) / kPropagationSpeed; // mm/ns
}
double TAMWDCArray::GetTimeOfFlight(const double *b, const double *B, int nu, int j, int k, int firedStripId, double beta){
#ifdef DEBUG
	TAPopMsg::Debug(GetName().c_str(), "GetDistanceOfFlight(b, B, nu, j, k, firedStripId): %f\t(beta*c0): %f", GetDistanceOfFlight(b, B, nu, j, k, firedStripId), (beta*c0)); // DEBUG
#endif
	return GetDistanceOfFlight(b, B, nu, j, k, firedStripId) / (beta*c0);
}
double TAMWDCArray::GetWirePropagationLength(const double *b, const double *B, int nu, int j, int k){
	if(j < 0 && j > 2){
		TAPopMsg::Error(GetName().c_str(), "GetWirePropagationLength: Input j (X-U-V) is abnormal, j: %d", j);
	}
	if(k < 0 && k > 5){
		TAPopMsg::Error(GetName().c_str(), "GetWirePropagationLength: Input k (0-5, anode layer) is abnormal, k: %d", k);
	}

	const TAMWDC *dc = GetMWDC(k/2);
	const TAAnode *ano = dc->GetAnode(j, k%2+1, nu);
	double p0[3]{}, p1[3]{}; // p0: the hit point on the anode; p1: anode center position
	double ag[3]{}; ((TAAnodePara*)ano->GetPara())->GetGlobalDirection(ag); // global direction
	((TAAnodePara*)ano->GetPara())->GetGlobalCenter(p1);
	TAMath::GetHitPoint(b, B, ag, p1, p0);
	p0[1] -= dc->GetDetPara()->GetY(); // p0[1] now is roughly in the detector-local coordinate system
#ifdef DEBUG
	cout << "The calculated hit point in the anode: " << endl; // DEBUG
	cout << "p0[0]: " << p0[0] << "\tp0[1]: " << p0[1] << "\tp0[2]: " << p0[2] << endl; // DEBUG
	cout << "p1[0]: " << p1[0] << "\tp1[1]: " << p1[1] << "\tp1[2]: " << p1[2] << endl; // DEBUG
	cout << "ag[0]: " << ag[0] << "\tag[1]: " << ag[1] << "\tag[2]: " << ag[2] << endl; // DEBUG
	getchar(); // DEBUG
#endif
	// type[2]: XUV; type[3]: cable id
	int type[6]{}; TAUIDParser::DNS(type, ano->GetUID());
	double theta = 0.; // wire-vertical angle
	if(0 != j) theta = 30. * DEGREE; // U V
	double FEE_Y = 303.; // Y position of FEE, in detector-local coordinate system
	if(3 == type[0] && 2 == k/2) FEE_Y = 253.; // LDC2
	if(3 == type[0] && 2 == k/2){
		if(1 == j && (type[3] <= 5 && type[3] >= 4)) FEE_Y *= -1.; // U, the last two cable
		if(2 == j && (type[3] <= 1 && type[3] >= 0)) FEE_Y *= -1.; // V, the first two cable
	}
	else{
		if(1 == j && (type[3] <= 4 && type[3] >= 3)) FEE_Y *= -1.; // U, the last two cable
		if(2 == j && (type[3] <= 1 && type[3] >= 0)) FEE_Y *= -1.; // V, the first two cable
	}

	double dd = fabs(p0[1] - FEE_Y) / cos(theta);
#ifdef DEBUG
	cout << "TAMWDCArray::GetWirePropagationLength(): " << dd << endl; getchar(); // DEBUG
#endif
	return dd;
} // end of function GetWirePropagationLength
double TAMWDCArray::GetDistanceOfFlight(const double *b, const double *B, int nu, int j, int k, int firedStripId){
	if(j < 0 && j > 2){
		TAPopMsg::Error(GetName().c_str(), "GetWirePropagationLength: Input j (X-U-V) is abnormal, j: %d", j);
	}
	if(k < 0 && k > 5){
		TAPopMsg::Error(GetName().c_str(), "GetWirePropagationLength: Input k (0-5, anode layer) is abnormal, k: %d", k);
	}

	const TAMWDC *dc = GetMWDC(k/2);
	const TAAnode *ano = dc->GetAnode(j, k%2+1, nu);
	// p0: the hit point on the anode; p1: anode center position
	double p0[3], p1[3], p2[3]; // p2: fired strip projection
	double ag[3]; ((TAAnodePara*)ano->GetPara())->GetGlobalDirection(ag);
	((TAAnodePara*)ano->GetPara())->GetGlobalCenter(p1);
	TAMath::GetHitPoint(b, B, ag, p1, p0);
	// Get the z and x coordinate of the fired strip
	GetTOFWall()->GetStrip(firedStripId)->GetStripPara()->GetGlobalProjection(p2);
	p2[1] = B[1] + b[1] * (p2[2]-B[2])/b[2];
	// more accurate x, calculated from the track function
	p2[0] = B[0] + b[0] * (p2[2]-B[2])/b[2];

	double dd = TAMath::L(p0, p2, 3);
#ifdef DEBUG
	cout << "TAMWDCArray::GetDistanceOfFlight(): " << dd << endl; getchar(); // DEBUG
#endif
	return dd;
} // end of function GetDistanceOfFlight

void TAMWDCArray::Initialize(){
	for(int i = 0; i < 3; i++){
		if(fMWDC[i]) fMWDC[i]->Initialize();
	}
	fN3DTrk = 0;
	if(fTOFWall) fTOFWall->Initialize();

	for(vector<TATrack *> &trls : fTrackList){
		for(TATrack *&tr : trls){
			if(tr){
				delete tr;
				tr = nullptr;
			}
		} // end for over tracks
		trls.clear();		
	} // end for over vectors
} // end of function Initialize.
// get the channel that belongs to this with uid.
TAStuff *TAMWDCArray::GetChannel(unsigned uid) const{
	if(uid > 0xFFFFF) return nullptr; // not a uid belonging to this class
	int type[6]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[6]{}; TAUIDParser::DNS(TYPE, fUID); // parse uid of this
	
	if(type[0] == TYPE[0]){ // belongs to this object
		if(type[1] < 3){ // MWDCs
			return
				GetMWDC(type[1])->GetSLayer(type[2])->GetCable(type[3])->GetSFE16(type[4])->GetAnode(type[5]);
		}
		else if(3 == type[1]){ // TOFWall
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
}
// display the detector information
void TAMWDCArray::Info() const{
	if(!TAPopMsg::IsVerbose()) return;
	for(int i = 0; i < 3; i++){
		GetMWDC(i)->Info();
		cout << "----------------------------------------------------------\n";
	}
	GetTOFWall()->Info();
	cout << "\n\033[1m______________________________________________________________\033[0m\n\n";
}
// a method dedicated for TAVisual::Fill()
void TAMWDCArray::FillTrack(TGraph *gTrack, TGraph *gTrack_R) const{
	if(!gTrack || !gTrack_R)
		TAPopMsg::Error(GetName().c_str(), "FillTrack: input TGraph pointer is null");
	for(TATrack *t : fTrackList[0]) t->FillTrack(gTrack, gTrack_R);
}










