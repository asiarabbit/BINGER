///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TARawDataProcessor.h												 //
//   TARawDataProcessor.h -- header file for class TARawDataProcessor				 //
//   Introduction: transform raw binary datafile from PXI crates to rootfile for	 //
// further analysis. The result is a ROOT tree containing channel id and edge timing //
// results, including edge type and number of edges. This is a singleton class.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/10/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TARAWDATAPROCESSOR_H_
#define _TARAWDATAPROCESSOR_H_

#include <string>

using std::string;

class TARawDataProcessor{
public:
	static TARawDataProcessor *Instance();
	virtual ~TARawDataProcessor();

	int GetEventCnt() const { return fEventCnt; }
	int GetBunchIdMisAlignCnt() const { return fBunchIdMisAlignCnt; }
	const char *GetROOTFileName() const { return fROOTFile.c_str(); }
	const char *GetDataFileName() const { return fDataFile.c_str(); }
	int ReadOffline(); // read offline binary data file and store them in a tree and a rootfile.
	void SetROOTFileName(const string &name){ fROOTFile = name; }
	void SetDataFileName(const string &name, int runId);
	void SetBunchIdCheck(bool isCheck){ fIsCheckBunchId = isCheck; }
	void SetPeriod(int index0, int index1);
protected:
	TARawDataProcessor();

	static TARawDataProcessor *fInstance;
	string fDataFile; // raw binary data file
	string fROOTFile; // ROOT file storing treeData and treeTrack for simulation or experiment

	bool fIsCheckBunchId; // check bunch id alignment or not.
	int fBunchIdMisAlignCnt; // count of bunchId misalignments.
	int fEventCnt; // count of events processed.
	int fRunId;
	int fIndex0; // start of index
	int fIndex1; // end of index
};

#endif
