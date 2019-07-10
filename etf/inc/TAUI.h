/**
	\file TAUI.h
	\class TAUI
	\brief The user interface, inherited from TAEventProcessor. Providing
	user option parsing, among other functions that relating to interactiion with the
	data analysis practitioner. A singleton class.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/11/27 Last revised: 2018/5/5, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAUI_H_
#define _TAUI_H_

#include "TAEventProcessor.h"

class TAUI : public TAEventProcessor{
public:
	static TAUI *Instance();
	void GetOpt(int argc, char *argv[]);
	void Go(); ///< analyze, call the TAEventProcessor::Run(...)
	const char *GetROOTFile() const{ return fROOTFile; }

	virtual ~TAUI();
protected:
	TAUI(); ///< the constructor
	void PromptHelp(bool isVerbose = false); ///< show manual for input rules

	static TAUI *fInstance;
	/// parameters to be passed to the constructor of TAEventProcessor
	char fDataFile[256], fVMEDataFile[256], fROOTFile[256];
	int fIndex0, fIndex1;
	int fRunId, fAnaDepth, fEvLenLim; ///< analyze depth
};

#endif

