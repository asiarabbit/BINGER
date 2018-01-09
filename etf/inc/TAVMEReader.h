///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAVMEReader.h														 //
//   TAVMEReader.h -- header file for class TAVMEReader								 //
//   Introduction: read binary data file from VME Daq and transform it into root tree//
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/1/9.															     //
// Last modified: 2018/1/9, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAVMEREADER_H_
#define _TAVMEREADER_H_

#include <string>

using std::string;

class TAVMEReader{
public:
	TAVMEReader(const string &datafile = "");
	virtual ~TAVMEReader();
	virtual void ReadVME();
	void SetDatafile(const string &datafile);
	// match vme and PXI data tree event by event
	virtual void Match(const string &PXIROOTFile);
	static void ReadVME(const string &vmeDatafile);
	static void Match(const string &PXIROOTFile, const string &VMEROOTFile);
private:
	string fDatafile;
	string fROOTFile;
};

#endif
