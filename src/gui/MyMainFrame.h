///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// src/gui/MyMainFrame.h															 //
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

#include <array>
#include "TGFrame.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TGStatusBar;
class TGRadioButton;
class TGComboBox;
class TGButtonGroup;
class TGLabel;
class TGTextButton;

class TTree;
class TFile;

using std::array;

class MyMainFrame : public TGMainFrame{
public:
	MyMainFrame(const TGWindow *p, int w, int h);
	virtual ~MyMainFrame();
	void DoDraw(const int opt);
	void DoClose();
	void SetStatusText(const char *txt, Int_t pi); // set status bar field information
	void EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected); // capture cusor info
	void SetGroupEnabled(int id);
	void HandleButtonOption(int widgetid, int id);
	void PreviousOption(); // toggle to the previous option in a function module
	void NextOption(); // toogle to the next option in a function module
	// Initialize: extract root objs from input root file; (ass)rootfile: origin rootfile and assess root file
	virtual void Initialize(const char *rootfile, const char *assrootfile = "");

	ClassDef(MyMainFrame, 0);

protected:
	TRootEmbeddedCanvas *fECanvas;
	TGStatusBar *fStatusBar;
	TCanvas *fMyCanvas; // Canvas of fECanvas
	TGComboBox *fComboBox[3];
	TGRadioButton *fRadioButton[3];
	TGLabel *fLabel[3];
	TGButtonGroup *fButtonGroup;
	TGTextButton *fPrevious, *fNext, *fExit;
	int fCurrentOption; // the current status of option
	short fNComboBoxEntry[3];
private:
	TFile *fFile[5]; // ROOT file containing data analysis results
	TTree *treeTrack; // main data analysis result tree
	TTree *vme; // raw data tree of VME Daq
	TTree *treeshoot; // derived tree storing further-processed results from treeTrack
	array<TObject *, 50> fObjArr; // to store objs other than trees (histos, graphs, etc.)
	int fNObj;
};

#endif
