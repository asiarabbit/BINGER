/**
	\file TAAssessPDC.h
	\class TAAssessPDC
	\brief Assess the tracking results for code performances and quality of
	the data. This is actually a user-interface class, and output interface to papers
	and presentations. What is worth mentioning is that compared with TAAssess, this
	class is specifically for MWDCs around the target.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/5/24 Last revised: 2018/5/24, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef _TAASSESSPDC_H_
#define _TAASSESSPDC_H_

#include <string>
#include <cmath>

#include "TAParaManager.h"

class TAMWDCArray2;

using std::string;

class TAAssessPDC{
public:
	typedef TAParaManager::ArrDet_t DetArr_t;
	static TAAssessPDC *Instance();
	virtual ~TAAssessPDC();

	/// assess tracking results of MWDC array U and MWDC array D
	void SetROOTFile(const string &file){ fROOTFile = file; }
	virtual void EvalDCArr(int runId = 0, bool isDCArrD = true);
	static void EvalDCArr(const string &rootfile, DetArr_t *detList, int runid = 0, bool isDCArrD = true);
	/// evaluation done after Eval event by event
	virtual void PostEval(int round = 0, bool isDCArrD = true);
	static bool PostEval(const string &rootfile, int round = 0, bool isDCArrD = true, bool is3D = true);
protected:
	static TAAssessPDC *fInstance;
	TAAssessPDC();
	DetArr_t *fDetList;
	string fROOTFile;
	static bool fNullDCArr; ///< to mark if the DCArr for the class is valid or not
};
	
#endif
