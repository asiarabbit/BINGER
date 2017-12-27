///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TACtrlPara.h													     //
//   TACtrlPara.h -- header file for class TACtrlPara							     //
//   Introduction: mainly for holding global parameters used for program control in  //
// tracking precision level and tracking procedures.								 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/7.															     //
// Last modified: 2017/10/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TACTRLPARA_H_
#define _TACTRLPARA_H_

#include <string>
using std::string;

class TAAnodePara;

class TACtrlPara{
public:
	static TACtrlPara *Instance();
	virtual ~TACtrlPara();
	//-------------- read offline data --------------//
	static bool IsCheckBunchIdAlignment();
	static bool IsDriftTimeQtCorrection();
	bool IsCoarseFit();
	bool Is3DTracking();
	// tolerance window for 3D coincidence test of X U and V track projections
	// 5: half a DC cell, given all kinds of errors
	static double Get3DCoincideWindow();
	static double D2Thre(); // for eliminating falsely fired andoes. unit: mm^2
	static double DsquareThresholdPerDot(unsigned uid);
	static bool TimeThre(double t); // if time is within set range
	// threshold for chi per dot, to eliminate false combinations. 4.0
	static double ChiThrePD();
	static int Vicinity(); // used in discerning multiple tracks, unit: cell
	static int StripTolerance(); // used in discerning multiple tracks, unit: strip
	// TATrack::kBFGSFit; // kNormalFit: 0; kBFGSFit: 1 kIterFit: 2
	static int FitMethod();
	// only effective if input fit method is kNormalFit
	// allowed value: -2, -1, 0, 1, 2, 3, with calculation depth increasing. unit: mm^2
	static int Precision();
	// MWDCArrayR_DC1_U: installation error
	static double DCArrR_DC1UHorizontalDeviation();
	double ChiThre() const{ return kChiThre; }
	// as its name indicates, used in TAAnode::GetDriftTime
	double DriftTimeQtCorrectionWeight() const{ return kDriftTimeQtCorrectionWeight; }
	const char *DataFileName() const;
	const char *RootFileName() const;
	const char *ConfigExpDir() const;
	const char *STRROOTFile() const;
	// calculate the minmum deviation of a track off the fired strips in a TOF wall
	void GetNStripStrayRangeR(double &minR, double &maxR) const;
	void GetNStripStrayRangeL(double &minL, double &maxL) const;
	bool TOFWallStripStrayTest(double strayMin, unsigned uid) const;
	void AssignSTR(TAAnodePara *para) const;

	// rough time of flight from DC to TOF wall unit: ns
	// for pattern recognition purposes only
	// PION: {12.5, 7.5, 2.5}; BEAM TEST: {29.06, 20.34, 18.23} - 17.73 - 1.5
	static double T_tofDCtoTOFW(unsigned uid);
	static double T_wireMean(unsigned uid);

	void SetConfigExpDir(const string &dir);
	void SetSTRROOTFile(const string &file);
	void CoarseFit(bool opt = true){ kIsCoarseFit = opt; }
	void SetIs3DTracking(bool opt = true){ kIs3DTracking = opt; }
private:
	TACtrlPara();
	static TACtrlPara *kInstance;

	bool kIsCoarseFit; // true then only iterative fit would be used
	bool kIs3DTracking; // whether or not to implement 3D tracking

	//---------  for pattern recognition -------------------//
	// for map function
	double kDriftTimeQtCorrectionWeight; // as its name indicates, used in TAAnode.C
	double kChiThre; // threshold for chi. (sqrt(chi2 / nFiredAnodeLayer)) unit: mm map.C 1.0 1.5

	string kDataFile; // for extra use
	// the directory name in config directory holding the selected experiment config files
	string kConfigExpDir;
	// STRs for MWDC calculated from simulation
	string kSTRROOTFile; // root file containing TF1 objects for MWDCs: aaa%d.root 1500 1000 900 [V]
	string kRootFile; // used in refined fit and calibration phase //  20161126_0522.dat.root
};

#endif
