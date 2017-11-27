///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAUI.C															 //
//   TAUI.C -- source file for class TAUI											 //
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

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <unistd.h>
#include "TAUI.h"
#include "TAPopMsg.h"

using std::cout;
using std::endl;

TAUI *TAUI::fInstance = nullptr;

TAUI::TAUI() : fDataFile(""), fROOTFile(""), fIndex0(0), fIndex1(INT_MAX),
	fRunId(0), fAnaDepth(0), fEvLenLim(INT_MAX){
}
TAUI::~TAUI(){}
TAUI *TAUI::Instance(){
	if(!fInstance) fInstance = new TAUI();
	return fInstance;
}

void TAUI::GetOpt(int argc, char *argv[]){
	int ch;
//	opterr = 0;
	while((ch = getopt(argc, argv, ":r:i:f:t::v::m::")) != -1){
		switch(ch){
			case 'r': strcpy(fROOTFile, optarg); break;
			case 'i': fIndex0 = atoi(optarg); break;
			case 'f': fIndex1 = atoi(optarg); break;
			case 'l': fAnaDepth = 1; if(optarg) fAnaDepth = atoi(optarg); break;
			case 'v': fRunId = 1; if(optarg) fRunId = atoi(optarg); break;
			case 'm': fEvLenLim = atoi(optarg); break;
			default: PromptHelp(); exit(1);
		}
	}
	if(argv[optind]) strcpy(fDataFile, argv[optind]);
	if(strcmp(fROOTFile, "") && strcmp(fDataFile, ""))
		TAPopMsg::Error("TAUI", "GetOpt: The data file and root file are designated at the same time, which is conflicting.\n\t(The rootfile ought be generated from the datafile.)");
	if(!strcmp(fROOTFile, "") && !strcmp(fDataFile, ""))
		TAPopMsg::Error("TAUI", "GetOpt: Neither a data file or root file is given.");
}
// show manual for input rules
void TAUI::PromptHelp(){
	cout << "\n          ----------------- USER MANUL ----------------\n";
	cout << "Usage:\n";
	cout << "\t./pre [options] [<datafile> | -r <rootfile>]\n\n";
	cout << "\t[datafile]: \n\t\traw binary datafile\n";
	cout << "\t[-r <rootfile>]: \n\t\traw rootfile\n";
	cout << "\t[-i <index0>] [-f <index1>]: \n\t\tindex range of events to be analysed.\n";
	cout << "\t\ti-initial, f-final. All events are chosen by default.\n";
	cout << "\t[-t[<depth>]]: \n\t\tanalyze depth:\n";
	cout << "\t\tnot used or 0: daq and detector statistics;\n\t\t1: tracking (default); 2: PID\n";
	cout << "\t[-m[<EvLenLim>]]: \n\t\tevent length limit (word)\n";
	cout << "\t[-v[<runId>]]: \n\t\trun_number\n";
	cout << "\t\t - would be suffixed to the created rootfile name.\n";
	cout << "\t\tNot used: 0; default: 1.\n";
}

void TAUI::Run(){
	SetDataFile(fDataFile, fRunId);
	if(0 == fAnaDepth){ SetIsTracking(false); SetIsPID(false); }
	if(1 == fAnaDepth){ SetIsTracking(true);  SetIsPID(false); }
	if(2 == fAnaDepth){ SetIsTracking(true);  SetIsPID(true); }
	cout << "fDataFile: " << fDataFile << "\tfROOTFile: " << fROOTFile << "\tfAnaDepth: " << fAnaDepth;
	cout << "\nfRunId: " << fRunId << "\tfEvLenLim: " << fEvLenLim << "\tfIndex0: " << fIndex0;
	cout << "\tfIndex1: " << fIndex1;
	Run(fIndex0, fIndex1, fEvLenLim, fROOTFile);
}










