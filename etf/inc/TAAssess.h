///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAssess.h													     //
//   TAAssess.h -- header file for class TAAssess								     //
//   Introduction: Assess the tracking results for code performances and quality of  //
// the data. This is actually a user-interface class, and output interface to papers //
// and presentations.																 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/12/14.															     //
// Last modified: 2018/1/5, SUN Yazhou.										  	     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAASSESS_H_
#define _TAAssess_H_

#include <string>
#include <cmath>

#include "TAParaManager.h"

class TAMWDCArray;

using std::string;

class TAAssess{
public:
	typedef TAParaManager::ArrDet_t DetArr_t;
	static TAAssess *Instance();
	virtual ~TAAssess();

	// assess tracking results of MWDC array L and MWDC array R
	void SetROOTFile(const string &file){ fROOTFile = file; }
	virtual void EvalDCArr(int runId = 0, bool isDCArrR = true);
	static void EvalDCArr(const string &rootfile, DetArr_t *detList, int runid = 0, bool isDCArrR = true);
	// evaluation done after Eval event by event
	virtual void PostEval(int round = 0);
	static bool PostEval(const string &rootfile, int round = 0, bool isDCArrR = true, bool is3D = true);
protected:
	static TAAssess *fInstance;
	TAAssess();
	DetArr_t *fDetList;
	string fROOTFile;
};
	
#endif
