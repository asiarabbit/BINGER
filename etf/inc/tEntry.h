///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/tEntry.h														     //
//   tEntry.h -- header file for struct tEntry									     //
//   Introduction: for holding raw data information of a data section defined in GDAQ//
// system. This class is used in binary data reading and data distribution.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2017/10/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _tENTRY_H_
#define _tENTRY_H_


/////////////////////////////// DATA STRUCT ///////////////////////////
struct tEntry{ // struct of reading one channel from binary files.
	char name[256]; // entry name, showing the identity of the entry.
	int index;
	int channelId;
	int nl; // count of leading edges in one data section.
	int nt; // count of trailing edges in one dat section.
	double leadingTime[10];
	double trailingTime[10];
	bool is_V;
	int bunchId; // trigger time in one time cycle

	tEntry();
	void initialize();
	void show() const;
	double TOT() const;
	double TOT1() const;
};

#endif
