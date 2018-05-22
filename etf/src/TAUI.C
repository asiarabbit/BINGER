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
// Last modified: 2018/5/5, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
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

TAUI::TAUI() : fDataFile(""), fVMEDataFile(""), fROOTFile(""), fIndex0(0),
	fIndex1(INT_MAX), fRunId(0), fAnaDepth(0), fEvLenLim(INT_MAX){
}
TAUI::~TAUI(){}
TAUI *TAUI::Instance(){
	if(!fInstance) fInstance = new TAUI();
	return fInstance;
}

void TAUI::GetOpt(int argc, char *argv[]){
	int ch;
//	opterr = 0;
	if(1 == argc) PromptHelp();
	while((ch = getopt(argc, argv, ":i:f:d::u::m::hv")) != -1){
		switch(ch){
			case 'i': fIndex0 = atoi(optarg); break;
			case 'f': fIndex1 = atoi(optarg); break;
			case 'd': fAnaDepth = 2; if(optarg) fAnaDepth = atoi(optarg); break;
			case 'u': fRunId = 1; if(optarg) fRunId = atoi(optarg); break;
			case 'm': fEvLenLim = atoi(optarg); break;
			case 'h': PromptHelp(true); break;
			case 'v': Verbose(true); break;
			default: PromptHelp();
		}
	}
	// operand without an option dash would be pushed to the end of the input array
	char suffix[64];
	short cnt[3]{}; // to check if any data type is assigned more than once, which is forbidden
	for(int I = 2; I--;){
		const char *arg = argv[optind+I];
		if(arg){
			if(!strcmp(arg+strlen(arg)-4, ".dat")){ // PXI binary data file
				strcpy(fDataFile, arg);
				cnt[0]++;
			}
			else if(!strcmp(arg+strlen(arg)-5, ".root")){ // PXI ROOT file
				strcpy(fROOTFile, arg);
				cnt[1]++;
			}
			else{
				strcpy(suffix, arg+strlen(arg)-3);
				// to see if the suffix is made of numerals
				bool isNum = true;
				for(unsigned i = 0; i < strlen(suffix); i++)
					if(suffix[i] < 48 || suffix[i] > 57) isNum = false;
				if(isNum){
					strcpy(fVMEDataFile, arg); // VME data file
					cnt[2]++;
				} // end if(isNum)
			} // end the final else
		} // end if argv[optind]
	} // end for over I
	if(cnt[0] > 1) TAPopMsg::Error("TAUI", "GetOpt: more than 1 PXI data file is offered");
	if(cnt[1] > 1) TAPopMsg::Error("TAUI", "GetOpt: more than 1 root file is offered");
	if(cnt[2] > 1) TAPopMsg::Error("TAUI", "GetOpt: more than 1 VME data file is offered");
	
	// input file name validity check
	if(strcmp(fROOTFile, "") && strcmp(fDataFile, ""))
		TAPopMsg::Error("TAUI", "GetOpt: The data file and root file are designated at the same time, which is conflicting.\n\t(The rootfile ought be generated from the datafile.)");
	if(!strcmp(fROOTFile, "") && !strcmp(fDataFile, "") && !strcmp(fVMEDataFile, ""))
		TAPopMsg::Error("TAUI", "GetOpt: Neither a data file or root file is given.");
	// check data type by filename suffixes
	if(strcmp(fDataFile, "") && strcmp(fDataFile+strlen(fDataFile)-4, ".dat"))
		TAPopMsg::Error("TAUI", "GetOpt: datafile name is invalid: %s", fDataFile);
	if(strcmp(fROOTFile, "") && strcmp(fROOTFile+strlen(fROOTFile)-5, ".root"))
		TAPopMsg::Error("TAUI", "GetOpt: rootfile name is invalid: %s", fROOTFile);

	// echo user input
//	return;
	cout << "       --- USER INPUT CHECK LIST ---\n"; // DEBUG
	cout << "fDataFile: " << fDataFile << "   fROOTFile: " << fROOTFile; // DEBUG
	cout << "   fAnaDepth: " << fAnaDepth;
	if(!strcmp(fROOTFile, "")) cout << "   fRunId: " << fRunId << endl; // DEBUG
	else cout << "   fRunId: (null)" << endl; // DEBUG
	cout  << "fEvLenLim: " << fEvLenLim << "   fIndex0: " << fIndex0; // DEBUG
	cout << "   fIndex1: " << fIndex1 << endl; // DEBUG
	cout << "       -------------------------------\n"; // DEBUG
}
// show manual for input rules
void TAUI::PromptHelp(bool isVerbose){
	cout << "\n          ----------------- USER MANUL ----------------\n";
	cout << "Usage:\n";
	cout << "\t./pre <datafile or rootfile>" ;
	cout << "[-\033[1mi\033[0m index0] [-\033[1mf\033[0m index1]\n";
	cout << "\t\t[-\033[1md\033[0manalyze-depth] ";
	cout << "[-\033[1mm\033[0mevent-length-limit] [-\033[1mu\033[0mrunId]\n";
	cout << "\t\t[-\033[1mh\033[0m] [-\033[1mv\033[0m]\n";
	if(!isVerbose) cout << "Use './pre -h' for detailed help.\n";
	if(!isVerbose) exit(1);

	cout << endl;
	cout << "\t[datafile]: \n\t\traw binary datafile\n";
	cout << "\t[-i <index0>] [-f <index1>]: \n\t\tindex range of events to be analysed.\n";
	cout << "\t\ti-initial, f-final. All events are chosen by default.\n";
	cout << "\t[-d[<depth>]]: \n\t\tanalyze depth:\n";
	cout << "\t\tnot used or 0: daq and detector statistics.";
	cout << "\n\t\t1: simple tracking; 2: normal tracking (default); 3: PID";
	cout << "\n\t\t4: 3D simple tracking; 5: 3D normal tracking; 6: 3D PID\n";
	cout << "\t[-m[<EvLenLim>]]: \n\t\tevent length limit (word). INT_MAX by default.\n";
	cout << "\t[-u[<runId>]]: \n\t\trun_number\n";
	cout << "\t\t - would be suffixed to the created rootfile name.\n";
	cout << "\t\tNot used: 0; default: 1.\n";
	cout << "\t[-v]: verbose mode (print detector information)\n";
	cout << "\tAbsolute path/data or ../data/relative-path/data is adopted\n";
	cout << "\tfor input binary data files.\n";
	exit(1);
}

void TAUI::Go(){
	if(fIndex0 >= fIndex1)
		TAPopMsg::Error("TAUI", "Run: index0 %d is not smaller than index1 %d", fIndex0, fIndex1);
	switch(fAnaDepth){
		case 0: SetIsTracking(false); SetIsPID(false); break;
		case 1: SetIsTracking(true); SetIsPID(false);  CoarseFit(true); break;
		case 2: SetIsTracking(true); SetIsPID(false); break;
		case 3: SetIsTracking(true); SetIsPID(true); break;
		case 4: SetIs3DTracking(true); SetIsPID(false); CoarseFit(true); break;
		case 5: SetIs3DTracking(true); SetIsPID(false); break;
		case 6: SetIs3DTracking(true); SetIsPID(true); break;
		default: SetIsTracking(false); SetIsPID(false);
			TAPopMsg::Error("TAUI", "Go: Allowed value for anaDepth is 0 - 6");
			PromptHelp(true);
	}
	if(!strcmp(fROOTFile, "")){
		if(strcmp(fDataFile, ""))
			SetDataFile(fDataFile, fRunId);
		if(strcmp(fVMEDataFile, ""))
			SetDataFile(fVMEDataFile, fRunId, false);
	} // end if(!strcmp(fROOTFile, ""))

	Run(fIndex0, fIndex1, fEvLenLim, fROOTFile);
}






