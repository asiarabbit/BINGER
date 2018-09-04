///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/tTrack.h														     //
//   tTrack.h -- header file for struct tTrack									     //
//   Introduction: struct for holding track information. The struct is mainly for	 //
// assigning TTree objects and output of track parameters.							 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2018/4/9, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _tTRACK_H_
#define _tTRACK_H_

struct tTrack{ // struct of tracks.
	char name[128];
	int index; // sec index
	int gGOOD;
	int nFiredAnodeLayer;
	int nu[8];
	int LAYER[8];
	double z[8];
	double x[8];
	double t[8];
	double dcTOT[8];
	double w[8]; // weight
	double r[8];
	double TOF; // time tag from TOF Wall
	double nStripStray; // count of strips from a fired strips to the fitted track
	int firedStripId; // serial id of fired TOF wall strip for the track.
	double k; // ->thetaOut
	double b;
	double dsquare;
	double chi[8];
	double Chi; // sqrt(chi2 / nFiredAnodeLayer)
	double chi2;
	int type; // 1[LR][XUV] <=> 1[01][012]
	int id; // 3-D track id
	double xMiss3D[3]; // 3D track coincidence test at z coordinates of the three DCs
	double beta; // particle speed, after the magnet
	double A; // atomic masss number
	double Z; // atomic number
	double m; // atomic mass/MeV
	tTrack(); // the default constructor
	virtual ~tTrack(){};
	virtual void initialize();
	virtual void show() const;
	double dcTOTAvrg() const;
};


#endif
