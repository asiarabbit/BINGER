///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/tTrack2.h														     //
//   tTrack2.h -- header file for struct tTrack2								     //
//   Introduction: struct for holding track information. The struct is mainly for	 //
// assigning TTree objects and output of track parameters. Different from tTrack,	 //
// this struct serves for DCs around the target.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/22.															     //
// Last modified: 2018/3/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _tTRACK2_H_
#define _tTRACK2_H_

struct tTrack2{ // struct of tracks.
	char name[128];
	int index; // sec index
	int gGOOD;
	int nFiredAnodeLayer;
	int nu[4];
	int LAYER[4];
	double z[4];
	double x[4];
	double t[4];
	double dcTOT[4];
	double w[4]; // weight
	double r[4];
	double TOF; // time tag from TOF Wall
//	double nStripStray; // count of strips from a fired strips to the fitted track.
//	int firedStripId; // serial id of fired TOF wall strip for the track.
	double k; // ->thetaOut
	double b;
	double dsquare;
	double chi[4];
	double Chi; // sqrt(chi2 / nFiredAnodeLayer)
	double chi2;
	int type; // 1[UD][XY] <=> 1[23][01]
//	int id; // 3-D track id
//	double xMiss3D[3]; // 3D track coincidence test at z coordinates of the three DCs
	double beta; // particle speed, after the magnet
	double A; // atomic masss number
	double Z; // atomic number
	double m; // atomic mass/MeV
	tTrack2(); // the default constructor
	void initialize();
	void show() const;
	double dcTOTAvrg() const;
};

#endif
