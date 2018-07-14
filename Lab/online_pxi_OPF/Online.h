/////////////////////////////////////////////////////////
// File name: Online.h                                 //
// Brief introduction:                                 //
//       Online program of RIBLL2-DAQ                  //
//                                                     //
// Version: V1.0                                       //
// Author: Han Jianlong                                //
// Date: Mar. 2012                                     //
// For: RIBLL1                                         //
/////////////////////////////////////////////////////////

#ifndef Online_ROOT_H
#define Online_ROOT_H

#ifndef OnlineFrame_H
#include "OnlineFrame.h"
#endif //#ifndef OnlineFrame_H

#include "./gdaq_include.h"
#include "./gdaq_include1.h"

class TGWindow;
class TGLVEntry;
class TFile;
class TCanvas;

class Online :public OnlineFrame
{
protected:
	bool setonline;
	unsigned int *onevent;


public:
	Online(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~Online();
	virtual bool ProcessMessage(Long_t msg, Long_t param1, Long_t);
	void SetOffline();
	void SetOnline();

protected:
	Online(const Online &onl);
	Online &operator=(const Online &onl);

};


#endif //#ifndef Online_ROOT_H





