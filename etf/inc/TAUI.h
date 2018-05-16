///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAUI.h															 //
//   TAUI.h -- header file for class TAUI											 //
//   Introduction: the user interface, inherited from TAEventProcessor. Providing 	 //
// user option parsing, among other functions that relating to interactiion with the //
// data analysis practitioner. A singleton class.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/11/27.															     //
// Last modified: 2018/5/5, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAUI_H_
#define _TAUI_H_

#include "TAEventProcessor.h"

class TAUI : public TAEventProcessor{
public:
	static TAUI *Instance();
	void GetOpt(int argc, char *argv[]);
	void Go(); // analyze, call the TAEventProcessor::Run(...)
	const char *GetROOTFile() const{ return fROOTFile; }

	virtual ~TAUI();
protected:
	TAUI(); // the constructor
	void PromptHelp(bool isVerbose = false); // show manual for input rules

	static TAUI *fInstance;
	// parameters to be passed to the constructor of TAEventProcessor
	char fDataFile[256], fVMEDataFile[256], fROOTFile[256];
	int fIndex0, fIndex1;
	int fRunId, fAnaDepth, fEvLenLim; // analyze depth
};

#endif

