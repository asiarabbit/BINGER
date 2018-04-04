///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray.h														 //
//   TAMWDCArray.h -- header file for class TAMWDCArray								 //
//   Introduction: MWDC array class, implementing pattern recognition and 3-D		 //
// tracking through track projection matching.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/6.															     //
// Last modified: 2018/3/21, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDCARRAY_H_
#define _TAMWDCARRAY_H_

#include <vector>
#include "TAStuff.h"
#include "TADetUnion.h"

using std::vector;

// ROOT include
class TGraph;

class TAMWDC;
class TATOFWall;
class TATrack;
class TASimulation;
struct tTrack;

class TAMWDCArray : public TAStuff, public TADetUnion{
public:
	TAMWDCArray(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArray() = 0; // abstract base class

	vector<TATrack *> &GetTrackList(int dcType);
	TAMWDC *GetMWDC(int dcId) const; // get MWDC #dcId
	TATOFWall *GetTOFWall() const;
	double GetPhiAvrg();
	double GetDsquareThresholdPerDot() const{ return fDsquareThresholdPerDot; }
	void SetDsquareThresholdPerDot(double d2Thre);
	// Map function: have the fired wires filtered, sorted, and stored in TATrack objects
	// for further fittings
	virtual void Map();
	// subordinate function of void Map()
	virtual bool Map(TAMWDC **MWDC, vector<TATrack *> &track, int dcType);
	virtual void AssignTracks(vector<tTrack *> &track_ls); // assign tracks to track list
	void TrackMerger(); // assembly projections to 3-D tracks
	// drift time correction: time of flight and signal propagation time
	// a[2]: [0-1]: [layer0~layer18, nu] p[4]: [0-3]: [k1, k2, b1, b2];
	void DriftTimeCorrection(double &t, double &r, const double *a, const double *p, int firedStripId, double beta); // beta: particle speed; kl: track slope
	// a method dedicated for TAVisual::Fill()
	void FillTrack(TGraph *gTrack, TGraph *gTrack_R) const;
	int GetN3DTrack() const{ return fN3DTrk; }

	virtual void Initialize() override;
	// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Info() const; // print MWDC array information

	friend class TASimulation;
protected:
	// copmare two tracks
	// 0: the two tracks are different; 1: newTrack is defeated by oldTrack
	// 2: newTrack defeats oldTrack
	// dcType: [0-1]: [X-Y]
	static int compare(TATrack *newTrack, TATrack *oldTATrack, int dcType = 0, bool show = false);
	static void cleanUp(vector<TATrack *> &tr, const int n);
	// to see if track projections resolved from UV data is compatible with that from X data.
	bool Compatible(double k, double b, double ku, double bu, double kv, double bv, double &k2, double &b2, double *xMiss3D);
	// tool functions for drift time correction j: X-U-V; k: DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
	double GetWirePropagationTime(const double *b, const double *B, int nu, int j, int k);
	// time of flight for particles from the MWDC to TOF wall
	double GetTimeOfFlight(const double *b, const double *B, int nu, int j, int k, int firedStripId, double beta);
	double GetWirePropagationLength(const double *b, const double *B, int nu, int j, int k);
	double GetDistanceOfFlight(const double *b, const double *B, int nu, int j, int k, int firedStripId);

	TAMWDC *fMWDC[3]; // three MWDCs in the MWDC array
	TATOFWall *fTOFWall; // the TOF wall
	// projection of 3-D track to normal planes of [X-U-V] sense wire layers
	vector<TATrack *> fTrackList[3];
	int fN3DTrk; // number of 3D tracks
	double fPhiAvrg; // average of phis of the three MWDCs
	double fDsquareThresholdPerDot; // as the name indicates
	static const double kPropagationSpeed; // mm/ns electric signal travel speed along wires
};

#endif
