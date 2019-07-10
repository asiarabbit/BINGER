/**
	\file TAAssess.h
	\class TAAssess
	\brief Assess the tracking results for code performances and quality of
	the data. This is actually a user-interface class, and output interface to papers
	and presentations.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/12/14 Last revised: 2018/5/24, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef _TAASSESS_H_
#define _TAASSESS_H_

#include <string>
#include <cmath>

#include "TAParaManager.h"

class TAMWDCArray;

using std::string;

class TAAssess{
public:
	typedef TAParaManager::ArrDet_t DetArr_t;
	static TAAssess *Instance();
	virtual ~TAAssess();

	/// assess tracking results of MWDC array L and MWDC array R
	void SetROOTFile(const string &file){ fROOTFile = file; }
	virtual void EvalDCArr(int runId = 0, bool isDCArrR = true);
	static void EvalDCArr(const string &rootfile, DetArr_t *detList, int runid = 0, bool isDCArrR = true);
	/// evaluation done after Eval event by event
	virtual void PostEval(int round = 0, bool isDCArrR = true);
	static bool PostEval(const string &rootfile, int round = 0, bool isDCArrR = true, bool is3D = true);
protected:
	static TAAssess *fInstance;
	TAAssess();
	DetArr_t *fDetList;
	string fROOTFile;
	static bool fNullDCArr; ///< to mark if the DCArr for the class is valid or not
};
	
#endif
