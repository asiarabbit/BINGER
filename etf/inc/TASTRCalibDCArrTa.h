///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASTRCalibDCArrTa.h												 //
//   TASTRCalibDCArrTa.h -- header file for class TASTRCalibDCArrTa					 //
//   Introduction: a tool class to calibrate DC STRs using auto-calibration. An		 //
// abstract base class.	Note that this is for DCs around the target, made by P.Ma.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/8.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TASTRCALIBDCARRTA_H_
#define _TASTRCALIBDCARRTA_H_

#include <string>

class TAMWDCArray2;

using std::string;

class TASTRCalibDCArrTa{
public:
	TASTRCalibDCArrTa(const string &rootfile = "", TAMWDCArray2 *dcArr = nullptr);
	virtual ~TASTRCalibDCArrTa() = 0;

	void SetROOTFile(const string &rootfile){ fROOTFile = rootfile; }
	void SetMWDCArray(TAMWDCArray2 *dcArr){ fDCArr = dcArr; }
	void SetIsBigStatistics(bool opt){ fIsBigStatistics = opt; }
	bool IsBigStatistics() const{ return fIsBigStatistics; }
	virtual void ChiHistogramming(bool is3D); // is3D: whether to use 3D chi to fill the histograms
	virtual void GenerateSTRCorFile(int round = 0);
	// isBigData: whether or not the data statistics is enough
	static void ChiHistogramming(const string &rootfile, TAMWDCArray2 *dcArr, bool is3D = false, bool isBigSta = true);
	// round: STR auto-calibration round
	static void GenerateCalibFile(const string &rootfile, TAMWDCArray2 *dcArr, int round = 0);
protected:
	// parameters for STR correction binning, from class TAAnodePara
	static const int nr_; // number of dirft distance bins
	static const double rmx_; // maximum drift distance
	static const int nAng; // number of bins for track-drift_cell angle
	static const double rstep; // bin width of drfit distance

	TAMWDCArray2 *fDCArr;
	string fROOTFile;
	// mark if the statistics is enough
	// not enough, then a whole anode layer would be used as the data
	// for any of the sense wire in the layer
	bool fIsBigStatistics;
	static short fNAnodePerLayer; // as the name indicates
};

#endif