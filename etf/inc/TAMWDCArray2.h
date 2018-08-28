///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray2.h													 //
//   TAMWDCArray2.h -- header file for class TAMWDCArray2							 //
//   Introduction: MWDC array class, implementing pattern recognition and 3-D		 //
// tracking through track projection matching. Note that this type of MWDC array is  //
// composed of two MWDCs, which usually is placed upstream the dipole magnet.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/18.															     //
// Last modified: 2018/4/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCARRAY2_H_
#define _TAMWDCARRAY2_H_

#include <vector>
#include "TAStuff.h"
#include "TADetUnion.h"

using std::vector;

// ROOT include
class TGraph;

class TAMWDC;
class TATrack2;
class tTrack; // for transmission of track information
class TAPlaStrip;

class TAMWDCArray2 : public TAStuff, public TADetUnion{
public:
	TAMWDCArray2(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArray2() = 0;

	vector<TATrack2 *> &GetTrackList(int dcType);
	TAMWDC *GetMWDC(int dcId) const; // get MWDC #dcId
	TAPlaStrip *GetPlaT0() const;
	void SetPlaT0(TAPlaStrip *t0);
	double GetDsquareThresholdPerDot() const{ return fDsquareThresholdPerDot; }
	void SetDsquareThresholdPerDot(double d2Thre);
	int GetNXTrack() const{ return fTrackList[0].size(); }
	int GetNYTrack() const{ return fTrackList[1].size(); }
	// Map function
	virtual void Map();
	// subordinate function of void Map()
	virtual bool Map(TAMWDC **MWDC, vector<TATrack2 *> &track, int dcType);
	virtual void AssignTracks(vector<tTrack *> &track_ls); // assign tracks to track list
	void FillTrack(TGraph *gTrack, TGraph *gTrack_R) const;
	
	virtual void Initialize() override;
	// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Info() const; // print MWDC array information
	virtual bool IsDCArr() const override { return true; }
protected:
	// copmare two tracks
	// 0: the two tracks are different; 1: newTrack is defeated by  oldTrack
	// 2: newTrack defeats oldTrack
	// dcType: [0-1]: [X-Y]
	int compare(TATrack2 *newTrack, TATrack2 *oldTATrack, int dcType = 0, bool show = false);

	TAMWDC *fMWDC[2]; // two MWDCs in the MWDC array
	// projection of 3-D track to the normals of [X-Y] sense wire layers
	vector<TATrack2 *> fTrackList[2];
	double fDsquareThresholdPerDot;
	TAPlaStrip *fPlaT0; // the plastic scintillator placed around the target for beam timing
};

#endif
