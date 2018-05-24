///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAssessPDC.h													     //
//   TAAssessPDC.h -- header file for class TAAssessPDC							     //
//   Introduction: Assess the tracking results for code performances and quality of  //
// the data. This is actually a user-interface class, and output interface to papers //
// and presentations. What is worth mentioning is that compared with TAAssess, this	 //
// class is specifically for MWDCs around the target.								 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/5/24.															     //
// Last modified: 2018/5/24, SUN Yazhou.									  	     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAASSESSPDC_H_
#define _TAASSESSPDC_H_

#include <string>
#include <cmath>

#include "TAParaManager.h"

class TAMWDCArray2;

using std::string;

class TAAssessPDC{
public:
	typedef TAParaManager::ArrDet_t DetArr_t;
	static TAAssessPDC *Instance();
	virtual ~TAAssessPDC();

	// assess tracking results of MWDC array U and MWDC array D
	void SetROOTFile(const string &file){ fROOTFile = file; }
	virtual void EvalDCArr(int runId = 0, bool isDCArrD = true);
	static void EvalDCArr(const string &rootfile, DetArr_t *detList, int runid = 0, bool isDCArrD = true);
	// evaluation done after Eval event by event
	virtual void PostEval(int round = 0, bool isDCArrD = true);
	static bool PostEval(const string &rootfile, int round = 0, bool isDCArrD = true, bool is3D = true);
protected:
	static TAAssessPDC *fInstance;
	TAAssessPDC();
	DetArr_t *fDetList;
	string fROOTFile;
};
	
#endif
