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
#include <string>
#include "TGFrame.h"

class TRootEmbeddedCanvas;
class TCanvas;
class TGStatusBar;
class TGRadioButton;
class TGComboBox;
class TGButtonGroup;
class TGLabel;
class TGTextButton;
class TGCheckButton;

class TTree;
class TFile;
class TCutG;

using std::array;
using std::string;

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
	void UpdateCut(int opt);
	void PreviousOption(); // toggle to the previous option in a function module
	void NextOption(); // toogle to the next option in a function module
	void ToggleVeto(bool on); // for veto checkbutton action
	// Initialize: extract root objs from input root file; (ass)rootfile: origin rootfile and assess root file
	virtual void Initialize(const char *rootfile, const char *assrootfile = "");

	ClassDef(MyMainFrame, 0);

protected:
	// draw using tree->Draw(s) with cut
	// id: current option; head: variable list; binning: histogram binning;
	// head: X or Y:X or Z:X:Y; binning: (NbinsX, xmin, xmax, NbinsY, ymin, ymax...)
	void cutDraw(int id, const char *head, const char *binning, const char *cut, const char *drawopt, const char *title = "");

	TRootEmbeddedCanvas *fECanvas;
	TGStatusBar *fStatusBar;
	TCanvas *fMyCanvas; // Canvas of fECanvas
	TGComboBox *fComboBox[3];
	TGRadioButton *fRadioButton[3], *fRBCut[5];
	TGLabel *fLabel[3];
	TGButtonGroup *fButtonGroup, *fBGCut;
	TGTextButton *fPrevious, *fNext, *fExit;
	TGCheckButton *fCBVeto;
	int fCurrentOption, fCUTOption; // the current status of option and cut
	short fNComboBoxEntry[3];
	string fCUT; // cut for tree draw
	TCutG *fCutG[4]; // graphical cut
private:
	TFile *fFile[5]; // ROOT file containing data analysis results
	TTree *treeTrack; // main data analysis result tree
	TTree *vme; // raw data tree of VME Daq
	TTree *treeshoot; // derived tree storing further-processed results from treeTrack
	array<TObject *, 50> fObjArr; // to store objs other than trees (histos, graphs, etc.)
	int fNObj;
};

#endif
