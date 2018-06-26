///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/MyMainFrame.C															 //
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

#include "MyMainFrame.h"
#include "TGButton.h"
#include "TCanvas.h"
#include "TRootEmbeddedCanvas.h"

#include "TGraph2D.h"

void graph3();

MyMainFrame::MyMainFrame(const TGWindow *p, int w, int h) : TGMainFrame(p, w, h){
	SetWindowName("ETF Data Analysis Display"); //  - By SUN Yazhou
	Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");

	// Create canvas widget
	fEcanvas  = new TRootEmbeddedCanvas("Ecanvas", this, 800, 600);
	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(this, 800, 40);
	// Draw and Exit button
	TGTextButton *draw = new TGTextButton(hframe, "&Draw");
	draw->Connect("Clicked()", "MyMainFrame", this, "DoDraw()");
	TGTextButton *exit = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 1));
	AddFrame(hframe, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	// arrange the accessories and make everything visible on screen
	MapSubwindows(); // make all the widgets appear on itself
	Resize(GetDefaultSize());
	MapWindow(); // make the main frame itself appear on screen
} // end of the constructor

void MyMainFrame::DoDraw(){
	graph3();
	TCanvas *fCanvas = fEcanvas->GetCanvas();
	fCanvas->cd();
	fCanvas->Update();
} // end member function draw
void MyMainFrame::DoClose(){
	exit(1);
} // end of member function DoClose()
MyMainFrame::~MyMainFrame(){
	Cleanup();
} // end of the destructor


void graph3(){
	TGraph2D *g = new TGraph2D();
	g->SetNameTitle("g", "A 3-D Line");
	g->SetFillColor(29);
	g->SetMarkerSize(0.8);
	g->SetMarkerStyle(20);
	g->SetMarkerColor(kRed);
	g->SetLineColor(kBlue-3);
	g->SetLineWidth(5);
	g->SetLineStyle(1);
//	gPad->SetLogy(1);
	
	TGraph2D *g2 = (TGraph2D*)g->Clone();
	g2->SetLineColor(kCyan);
	
	for(int i = 0; i < 10; i++){
		g->SetPoint(i, i, i, i);
		g2->SetPoint(i, i, 2 * i, 3 * i);		
	} // end for over i

	g2->Draw("P0LINE");
	g->Draw("P0LINEsame");
}

