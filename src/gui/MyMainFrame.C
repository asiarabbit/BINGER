///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// src/gui/MyMainFrame.C															 //
//   MyMainFrame.C -- source file for class MyMainFrame								 //
//   Introduction: This class is for BINGER project graphical usr interface (GUI)	 //
// system. The GUI system is mainly for user-friendly demonstration purposes.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/24.															     //
// Last modified: 2018/6/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <unistd.h>

#include "MyMainFrame.h"
#include "TGButton.h"
#include "TCanvas.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGStatusBar.h"
#include "TGButtonGroup.h"
#include "TGLayout.h"
#include "TGComboBox.h"
#include "TGLabel.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCut.h"
#include "TCutG.h"

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

MyMainFrame::MyMainFrame(const TGWindow *p, int w, int h)
	 : TGMainFrame(p, w, h), fECanvas(0), fMyCanvas(0), fFile{0},
		treeTrack(0), vme(0), treeshoot(0){
	fCurrentOption = -1;
	Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");

	// Create canvas widget
	fECanvas  = new TRootEmbeddedCanvas("ECanvas", this, 800, 600);
	fMyCanvas = fECanvas->GetCanvas();
	fMyCanvas->SetGrid();
	// add a status bar
	int parts[] = {20, 20, 10, 50}; // length proportions of the bar parts
	fStatusBar = new TGStatusBar(this, 800, 10);
	fStatusBar->Draw3DCorner(false);
	fStatusBar->SetParts(parts, 4);
	fMyCanvas->Connect("ProcessedEvent(int, int, int, TObject *)", "MyMainFrame", this,
					"EventInfo(int, int, int, TObject *)");
	// Radio button for function set selection
	// constructor(parentWindow, hotString, signalCode)
	fButtonGroup = new TGButtonGroup(this, "Mode Selection", kHorizontalFrame);
	fButtonGroup->SetLayoutManager(new TGMatrixLayout(fButtonGroup, 3, 3));
	fRadioButton[0] = new TGRadioButton(fButtonGroup, new TGHotString("&Beam_Position(X:Y)                        "), 1);
	fRadioButton[1] = new TGRadioButton(fButtonGroup, new TGHotString("&DC_Efficiency(%)                          "), 2);
	fRadioButton[2] = new TGRadioButton(fButtonGroup, new TGHotString("&PID                                       "), 3);
	fButtonGroup->Connect("Pressed(int)", "MyMainFrame", this, "SetGroupEnabled(int)");
	// Combo boxes and a list box for each function set
	fLabel[0] = new TGLabel(fButtonGroup, "Beam Position At z of:");
	fLabel[1] = new TGLabel(fButtonGroup, "DC Efficiency - Draw Option:");
	fLabel[2] = new TGLabel(fButtonGroup, "PID - Histogram Type:");
	fComboBox[0] = new TGComboBox(fButtonGroup, 100);
	fComboBox[1] = new TGComboBox(fButtonGroup, 200);
	fComboBox[2] = new TGComboBox(fButtonGroup, 300);
	// add entry to the combo and list boxes
	string optList0[] = {"VETO_0", "TOF stop", "PDC0", "PDC1", "target-U", "target-D",
		"PDC2", "PDC3", "taPosMatch-X", "taPosMatch-Y",
		"WireIdDC0X1", "WireIdDC0X2", "Pos-DC0", // DC0-X0-X1
		"WireIdDC1X1", "WireIdDC1X2", "Pos-DC1", // DC1-X0-X1
		"WireIdDC2X1", "WireIdDC2X2", "Pos-DC2", // DC2-X0-X1
		"TOFWStripId", "Pos-TOFW"};
	string optList1[] = {"textcolz", "text", "LEGO", "colz"};
	string optList2[] = {"aoz", "poz:beta", "aoz:Z"};
	for(const string &s : optList0)
		fComboBox[0]->AddEntry(s.c_str(), fComboBox[0]->GetNumberOfEntries() + 1);
	for(const string &s : optList1){
		fComboBox[1]->AddEntry(s.c_str(), fComboBox[1]->GetNumberOfEntries() + 1);
	}
	for(const string &s : optList2){
		fComboBox[2]->AddEntry(s.c_str(), fComboBox[2]->GetNumberOfEntries() + 1);
	}
	fComboBox[0]->Select(0); fComboBox[1]->Select(0); fComboBox[2]->Select(0); // set default option
	// group each combo and label in one button group
	for(int i = 0; i < 3; i++)
		fButtonGroup->AddFrame(fLabel[i], new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 2, 2, 2, 1));
	for(int i = 0; i < 3; i++){
		fButtonGroup->AddFrame(fComboBox[i], new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 2, 2, 2, 1));
		fComboBox[i]->Resize(150, 20);
		fNComboBoxEntry[i] = fComboBox[i]->GetNumberOfEntries();
	}
	fButtonGroup->SetExclusive();
	fRadioButton[0]->SetState(kButtonDown); fComboBox[1]->SetEnabled(0); fComboBox[2]->SetEnabled(0); // set the default option
	for(int i = 3; i--;) fComboBox[i]->Connect("Selected(int, int)", "MyMainFrame", this, "HandleButtonOption(int, int)");
	
	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(this, 800, 40);
	// Exit button
	fPrevious = new TGTextButton(hframe, "&Previous");
	fPrevious->Connect("Clicked()", "MyMainFrame", this, "PreviousOption()");
	fNext = new TGTextButton(hframe, "&Next");
	fNext->Connect("Clicked()", "MyMainFrame", this, "NextOption()");
	fExit = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	hframe->AddFrame(fPrevious, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	hframe->AddFrame(fNext, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	hframe->AddFrame(fExit, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	fPrevious->SetEnabled(0); fNext->SetEnabled(0);
	ULong_t ycolor; gClient->GetColorByName("yellow", ycolor);
	fExit->SetBackgroundColor(ycolor);

	// add all child frames in the main TGFrame
	AddFrame(fButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 2, 2, 2, 1));
	AddFrame(fECanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 1));
	AddFrame(fStatusBar, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 1));
	AddFrame(hframe, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	// arrange the accessories and make everything visible on screen
	MapSubwindows(); // make all the widgets appear on itself
	Resize(GetDefaultSize());
	MapWindow(); // make the main frame itself appear on screen
} // end of the constructor

void MyMainFrame::SetStatusText(const char *txt, int pi){ // pi: part id
	fStatusBar->SetText(txt, pi); // set text in status bar
}
void MyMainFrame::SetGroupEnabled(int id){
	id -= 1;
	for(int i = 0; i < 3; i++){
		fComboBox[i]->SetEnabled(0);
	}
	fComboBox[id]->SetEnabled(1);
	int id0 = fComboBox[id]->GetSelected();
	if(id0 <= 0) id0 = 1;
	fComboBox[id]->Select(0);
	fComboBox[id]->Select(id0);
}
void MyMainFrame::HandleButtonOption(int widgetId, int id){
	DoDraw(widgetId+id);
	if(!fPrevious->IsEnabled() && fCurrentOption > 0) fPrevious->SetEnabled();
	if(!fNext->IsEnabled() && fCurrentOption > 0) fNext->SetEnabled();
	cout << "widgetId: " << widgetId << "\tid: " << id << endl; // DEBUG
	cout << "fCurrentOption: " << fCurrentOption << endl; // DEBUG
}
void MyMainFrame::PreviousOption(){
	int widgetId = fCurrentOption / 100 - 1;
	int option = fCurrentOption % 100 - 1;
	option = option % fNComboBoxEntry[widgetId];
	if(0 == option) option = fNComboBoxEntry[widgetId];
	fComboBox[widgetId]->Select(option);
}
void MyMainFrame::NextOption(){
	int widgetId = fCurrentOption / 100 - 1;
	int option = fCurrentOption % 100 + 1;
	option = option % fNComboBoxEntry[widgetId];
	if(0 == option) option = fNComboBoxEntry[widgetId];
	fComboBox[widgetId]->Select(option);
}

void MyMainFrame::EventInfo(int event, int px, int py, TObject *selected){
	const char *txt0, *txt1, *txt3;
	char txt2[64];
	txt0 = selected->GetTitle();
	SetStatusText(txt0, 0);
	txt1 = selected->GetName();
	SetStatusText(txt1, 1);
	if(kKeyPress == event) sprintf(txt2, "%c", (char)px);
	else sprintf(txt2, "%d, %d", px, py);
	SetStatusText(txt2, 2);
	txt3 = selected->GetObjectInfo(px, py);
	SetStatusText(txt3, 3);
}

void MyMainFrame::DoClose(){
	exit(1);
} // end of member function DoClose()
MyMainFrame::~MyMainFrame(){
	for(auto &f : fFile) if(f) delete f;
//	if(0 == access("gui.root", F_OK)) system("rm gui.root");
	Cleanup();
} // end of the destructor

#include "custom.C"

