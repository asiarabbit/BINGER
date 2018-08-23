///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0CalibDCArrTa.h												 //
//   TAT0CalibDCArrTa.h -- header file for class TAT0CalibDCArrTa					 //
//   Introduction: a tool class to calibrate DCT0(delay)s anode by anode by fitting	 //
// drift time distribution. An abstract base class.	Used for DCs around the target.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/3.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAT0CALIBDCARRTA_H_
#define _TAT0CALIBDCARRTA_H_

#include <string>

class TAMWDCArray2;

using std::string;

class TAT0CalibDCArrTa{
public:
	TAT0CalibDCArrTa(const string &rootfile = "", TAMWDCArray2 *dcArr = nullptr);
	virtual ~TAT0CalibDCArrTa() = 0;
	
	void SetROOTFile(const string &rootfile){ fROOTFile = rootfile; }
	void SetMWDCArray(TAMWDCArray2 *dcArr){ fDCArr = dcArr; }
	// generate T0 calibration text files
	virtual void Refine_DTHisto();
	virtual void GenerateCalibFile(bool isShowFit = false);
	static void Refine_DTHisto(const string &rootfile, TAMWDCArray2 *dcArr);
	// isShowFit: whether to show the fitting graphs
	static void GenerateCalibFile(const string &rootfile, TAMWDCArray2 *dcArr, bool isShowFit = false);
protected:
	static double Fermi_Dirac_Function(double *x, double *p); // used to fit the drfit time spectrum.
	string fROOTFile;
	TAMWDCArray2 *fDCArr;
};

#endif
