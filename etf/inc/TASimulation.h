///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASimulation.h													 //
//   TASimulation.h -- header file for class TASimulation						 //
//   Introduction: Simulation data generator, following the format of struct tEntry. //
// A toy Monta Carlo simulator. Only geometrical specifications of detectors are	 //
// considered.																		 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2017/11/20, SUN Yazhou.										     //
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
	virtual void GenerateSim(int run, int nTrkPerEvEx, double effEx);
	virtual void Evaluate();
	// simfile: name of the rootfile containing the simulation data
	static void GenerateSim(int run, int nTrkPerEvEx, double effEx, char *simFile, DetArr_t *detList);
	static void Evaluate(const string &rootfile);
protected:
	DetArr_t *fDetList;
	string fROOTFile;
};

#endif
