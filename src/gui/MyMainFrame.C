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
// Last modified: 2018/6/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "MyMainFrame.h"
#include "TGButton.h"
#include "TCanvas.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGStatusBar.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCut.h"
#include "TCutG.h"

using std::cout;
using std::endl;

MyMainFrame::MyMainFrame(const TGWindow *p, int w, int h)
	 : TGMainFrame(p, w, h), fECanvas(0), fMyCanvas(0), fFile{0},
		treeTrack(0), vme(0), treeshoot(0){
	SetWindowName("ETF Data Analysis Display"); //  - By SUN Yazhou
	Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");

	// Create canvas widget
	fECanvas  = new TRootEmbeddedCanvas("ECanvas", this, 800, 600);
	fMyCanvas = fECanvas->GetCanvas();
	// add a status bar
	int parts[] = {33, 10, 10, 47}; // length proportions of the bar parts
	fStatusBar = new TGStatusBar(this, 800, 10);
	fStatusBar->Draw3DCorner(false);
	fStatusBar->SetParts(parts, 4);
	fMyCanvas->Connect("ProcessedEvent(int, int, int, TObject *)", "MyMainFrame", this,
					"EventInfo(int, int, int, TObject *)");
	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(this, 800, 40);
	// Draw and Exit button
	TGTextButton *drawB = new TGTextButton(hframe, "&Draw");
	drawB->Connect("Clicked()", "MyMainFrame", this, "DoDraw(=0)");
	TGTextButton *exitB = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	hframe->AddFrame(drawB, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	hframe->AddFrame(exitB, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));

	// add all child frames in the main TGFrame
	AddFrame(fECanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 1));
	AddFrame(fStatusBar, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 0));
	AddFrame(hframe, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	// arrange the accessories and make everything visible on screen
	MapSubwindows(); // make all the widgets appear on itself
	Resize(GetDefaultSize());
	MapWindow(); // make the main frame itself appear on screen
} // end of the constructor

void MyMainFrame::SetStatusText(const char *txt, int pi){ // pi: part id
	fStatusBar->SetText(txt, pi); // set text in status bar
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
	Cleanup();
} // end of the destructor

#include "custom.C"

