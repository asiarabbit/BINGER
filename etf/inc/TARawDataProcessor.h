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
// Last modified: 2018/5/7, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
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

	int GeEventCnt() const { return fEventCnt; }
	int GetVMEEventCnt() const { return fVMEEventCnt; }
	int GetBunchIdMisAlignCnt() const { return fBunchIdMisAlignCnt; }
	const char *GetROOTFileName() const { return fROOTFile.c_str(); }
	const char *GetPXIDataFileName() const { return fPXIDataFile.c_str(); }
	const char *GetVMEDataFileName() const { return fVMEDataFile.c_str(); }
	int ReadOffline(); // read offline binary data file and store them in a tree and a rootfile
	int ReadOfflinePXI();
	int ReadOfflineVME();
	void SetROOTFileName(const string &name){ fROOTFile = name; }
	// isPXI: false -> VME
	void SetDataFileName(const string &name, int runId, bool isPXI = true);
	void SetBunchIdCheck(bool isCheck){ fIsCheckBunchId = isCheck; }
	void SetPeriod(int index0, int index1);
protected:
	TARawDataProcessor();

	static TARawDataProcessor *fInstance;
	string fPXIDataFile; // raw PXI binary data file
	string fVMEDataFile; // raw VME data file
	string fVMEROOTFile; // VME ROOT file, if no PXI data or ROOT file is provided
	// ROOTFile would contain both PXI and VME treeData
	string fROOTFile; // ROOT file storing treeData and treeTrack for simulation or experiment

	bool fIsCheckBunchId; // check bunch id alignment or not
	int fBunchIdMisAlignCnt; // count of bunchId misalignments
	int fEventCnt; // count of PXI events processed
	int fVMEEventCnt; // count of VME events processed
	int fRunId;
	int fIndex0; // start of index
	int fIndex1; // end of index
};

#endif
