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
// Last modified: 2017/11/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAUI_H_
#define _TAUI_H_

#include "TAEventProcessor.h"

class TAUI : public TAEventProcessor{
public:
	static TAUI *Instance();
	void GetOpt(int argc, char *argv[]);
	using TAEventProcessor::Run;
	void Run(); // analyze, call the TAEventProcessor::Run(...)

	virtual ~TAUI();
protected:
	TAUI(); // the constructor
	void PromptHelp(); // show manual for input rules

	static TAUI *fInstance;
	// parameters to be passed to the constructor of TAEventProcessor
	char fDataFile[128], fROOTFile[128];
	int fIndex0, fIndex1;
	int fRunId, fAnaDepth, fEvLenLim; // analyze depth
};

#endif

