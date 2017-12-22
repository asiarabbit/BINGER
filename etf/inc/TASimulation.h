///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASimulation.h													 //
//   TASimulation.h -- header file for class TASimulation							 //
//   Introduction: Simulation data generator, following the format of struct tEntry. //
// A toy Monta Carlo simulator. Only geometrical specifications of detectors are	 //
// considered.																		 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/12/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TASIMGENERAOTR_H_
#define _TASIMGENERAOTR_H_

#include <string>

#include "TAParaManager.h"

using std::string;

class TASimulation{
public:
	typedef TAParaManager::ArrDet_t DetArr_t;
	TASimulation(DetArr_t *detList = nullptr);
	virtual ~TASimulation();

	// generate simulation data based on the current experiment setup
	// run: event count; nTrkPerEvEx: number of tracks per event; effEx: detector efficiency
	virtual void GenerateSim(int run, int nTrkPerEvEx, double effEx, const string &simFile = "");
	virtual void Evaluate();
	void SetFixDCArr(short opt){ kFixDCArr = opt; }
	void SetIsDebug(bool opt){ kIsDebug = opt; }
	// simfile: name of the rootfile containing the simulation data
	static void GenerateSim(int run, int nTrkPerEvEx, double effEx, char *simFile, DetArr_t *detList, const string &simrootfilename = "");
	// evaluate the performance of pattern recognition code
	static void Evaluate(const string &rootfile);
	// time to trig of T0_1 signal. unit: clock cycle (25 ns)
	static const int kT0_1TimeToTrigNCycle = 46; // 16*25 = 400ns
protected:
	DetArr_t *fDetList;
	string fROOTFile;
	static short kFixDCArr; // 0: DCArrL only; 1: DCArrR only; others: randomly chosen DCArrL or DCArrR
	static bool kIsDebug;
};

#endif
