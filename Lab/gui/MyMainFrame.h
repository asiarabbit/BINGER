///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/MyMainFrame.h															 //
//   MyMainFrame.h -- header file for class MyMainFrame								 //
//   Introduction: This class is for BINGER project graphical usr interface (GUI)	 //
// system. The GUI system is mainly for user-friendly demonstration purposes.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/24.															     //
// Last modified: 2018/6/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _MYMAINFRAME_H_
#define _MYMAINFRAME_H_

#include "TGFrame.h"

class TRootEmbeddedCanvas;

class MyMainFrame : public TGMainFrame{
public:
	MyMainFrame(const TGWindow *p, int w, int h);
	virtual ~MyMainFrame();
	void DoDraw();
	void DoClose();
	ClassDef(MyMainFrame, 0);

private:
	TRootEmbeddedCanvas *fEcanvas;
};

#endif
