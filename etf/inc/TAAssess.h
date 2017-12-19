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
// Last modified: 2017/12/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
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
	void SetRunId(int run){ fRunId = abs(run); }
	virtual void EvalDCArr(bool isDCArrR = true, int runId = 0);
	static void EvalDCArr(const string &rootfile, DetArr_t *detList, bool isDCArrR = true, unsigned short runid = 0);
protected:
	static TAAssess *fInstance;
	TAAssess();
	DetArr_t *fDetList;
	string fROOTFile;
	unsigned short fRunId; // run Id, to avoid overwriting
};
	
#endif
