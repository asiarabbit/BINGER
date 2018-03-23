///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TATrack.h															 //
//   TATrack.h -- header file for class TATrack										 //
//   Introduction: class for track fitting, which is mainly a series of minimization //
// algorithms. One of the core classes for the data analysis project.				 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/3/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TATRACK_H_
#define _TATRACK_H_

#include "TAStuff.h"

// ROOT include
class TGraph;

struct tTrack;

class TATrack : public TAStuff{
public:
	TATrack(const string &name = "", const string &title = "", unsigned uid = 999999999);
	TATrack(const TATrack &track); // the copy constructor
	TATrack &operator=(const TATrack &track); // the assignment constructor
	virtual ~TATrack();
	
	// the get functions
	double GetDsquare();
	double GetChi2();
	double GetChi(); // sqrt(chi2) / nFiredAnodeLayer
	double GetSlope();
	double GetIntercept();
	virtual void GetNu(int *nu) const; // output array fNu
	virtual void GetLAYER(int *LAYER) const; // output array fLAYER
	virtual void GetZ(double *z) const; // output array fZ
	virtual void GetX(double *x) const; // output array fX
	virtual void GetDriftTime(double *t) const; // output array fT
	virtual void GetDriftDistance(double *r) const; // output array fR
	virtual void GetChi(double *chi); // output array fChi
	double GetTOF() const; // return fTOF;
	double GetNStripStray() const; // return fNStripStray;
	int GetFiredStripId() const; // return fFiredStripId;
	int GetNFiredAnodeLayer() const;
	int GetgGOOD() const;
	int GetFitMethod() const;
	int GetFitPrecision() const;
	double GetDsquareThresholdPerDot() const;
	int Get3DId() const{ return f3DId; }

	bool IsAssigned() const; // to tell if the track is not assigned; return fIsAssigned
	bool IsFitted() const; // to tell if the data has been fitted.

	// the set functions
	// FitPrecision: -1, 0, 1, 2, 3 from shallow to deep in order.
	// Other values would be the least fit precision.
	void SetFitPrecision(int fitPrecision); // set calculation depth.
	// x, z: fired anode position; r: drift distance
	virtual void SetData(const double *x, const double *z, const double *t, const double *r, double kL, double bL, double dsquare, int gGOOD, const int *nu, const int *LAYER, const double *weight);
	virtual void SetDriftTime(const double *t, const double *w = nullptr); // as the name indicates; w: weight
	virtual void SetDriftDistance(const double *r); // as the name indicates
	void SetBeta(double beta){ fBeta = beta; }
	double GetBeta() const{ return fBeta; }
	void SetTOF(double TOF, int firedStripId, double nstripStray); // set fTOF;
	void AssignTrack(tTrack *ptrack); // assign tTrack
	void SetFitMethod(int method); // 0: normal fit; 1: BFGS fit
	// for kNormalFit only. (xc, zc) is the perturbative rotating center of particle track line in an attempt to minimize chi2 and get the optimal combination of linear parameter set
	void SetFitRotationCenter(double xc, double zc); // 0: normal fit; 1: BFGS fit
	void SetDsquareThresholdPerDot(double threshold);
	void Set3DId(int id){ f3DId = id; }
	
	// the fit method
	void Fit();
	// show information about the track
	virtual void Show();
	// a method dedicated for TAVisual::Fill()
	void FillTrack(TGraph *gTrack, TGraph *gTrack_R);

	virtual void Initialize();

	static const int kNormalFit = 0;
	static const int kBFGSFit = 1;
	static const int kIterFit = 2;
	bool marked; // for any possible use
	bool fIsDEBUG; // DEBUG
	double fXMiss3D[3]; // 3D track coincidence test at z coordinates of the three DCs

protected:
	// fit functions
	// general recursive fit
	void NormalFit(); // fit using normal and conventional searching minimization method.
	void BFGSFit(); // fit using BFGS minimization method.
	void IterFit(); // fit using iteartion minimization method, relatively coarse, but fast, so is more preferrable and recommended.

	// there are six anode layers for each MWDC array in the External Target Facility,
	// hence the length of the data array being 6.
	double fZ[6];
	double fX[6];
	double fR[6];
	double fT[6]; // drift time
	double fWeight[6]; // weight for weighted addition of chi to chi2
	double fTOF;
	double fNStripStray; // count of strips from a fired strips to the fitted track.
	int fFiredStripId; // serial id of the fired TOF wall strip for the track.
	int fNu[6]; // the fired anode serial id in each of the six anode layers for the track specifically.
	int fLAYER[6]; // to tell the condition of the 6 fired andoe layers.
	int f3DId; // 3-D track identifier
	int fNFiredAnodeLayer; // count of fired anodes for the track. 
	int fgGOOD; // count of fired anodes for the track, but a little different from fNFiredAnodeLayer. 3, 4, 5 and 6. Less than 3 is not allowed, except 2, which has a special meaning. Three MWDC must all be fired, or value 2. You know that. It is the avatar of gGOOD. It is a very important variable and not to be messed with.

	double fK; // the slope of the linear track
	double fB; // the intercept of the linear track
	double fChi2; // the sum of squares of fitting residuals of all the fired anodes.
	double fDsquare; // the Dsquare of least square fit of the fired andoes, regardless of drift time.
	double fChi[6]; // fit residue
	// the value of fIsFitted can only be assigned true through Fit function.
	bool fIsFitted; // to tell if the data is fitted after either the data or the fit precision has been altered.
	bool fIsAssigned; // to tell if the track is not assigned.
	double fBeta; // particle speed, while it zipps along this track; mainly for drift time correction

	// coordinate of the rotating center for the track fitting.
	// geometirc center of the MWDC array.
	int fFitMethod; // 0: normal fit; 1: BFGS fit
	int fFitPrecision; // the calculation precision.
	double fZc; // MWDC_X(U, V)[1]->GetZc()
	double fXc; // MWDC_X(U, V)[1]->GetXc()
	double fDsquareThresholdPerDot;
};

#endif



