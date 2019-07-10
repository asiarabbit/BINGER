/**
	\file TASimulation.h
	\class TASimulation
	\brief Simulation data generator, following the format of struct tEntry.
	A toy Monta Carlo simulator. Only geometrical specifications of detectors are considered.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2017/12/22, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


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

	/// generate simulation data based on the current experiment setup
	/// \param run: event count; nTrkPerEvEx: number of tracks per event; effEx: detector efficiency
	virtual void GenerateSim(int run, int nTrkPerEvEx, double effEx, const string &simFile = "");
	virtual void Evaluate();
	void SetFixDCArr(short opt){ kFixDCArr = opt; }
	void SetIsDebug(bool opt){ kIsDebug = opt; }
	/// \param simfile: name of the rootfile containing the simulation data
	/// \param simrootfile: rootfile name designated by user input
	static void GenerateSim(int run, int nTrkPerEvEx, double effEx, char *simFile, DetArr_t *detList, const double *betaEx, const string &simrootfilename = "");
	/// evaluate the performance of pattern recognition code
	static void Evaluate(const string &rootfile);
	/// time to trig of T0_1 signal. unit: clock cycle (25 ns)
	static const int kT0_1TimeToTrigNCycle = 46; ///< 16*25 = 400ns
	void SetBeta(double beta0, double beta1){ fBeta[0] = beta0; fBeta[1] = beta1; }
protected:
	DetArr_t *fDetList;
	string fROOTFile;
	static short kFixDCArr; ///< 0: DCArrL only;\n 1: DCArrR only;\n others: randomly chosen DCArrL or DCArrR
	static bool kIsDebug;
	double fBeta[2]; ///< [DCArrL-R], beta used in simulation
};

#endif
