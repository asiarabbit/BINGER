///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAVisual.h													     //
//   TAVisual.h -- header file for class TAVisual									 //
//   Introduction: visualize the particle tracks and fired detectors. This is a user //
// interface, so is rather important to show the quality of the whole code.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/22.															     //
// Last modified: 2017/10/23, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAVISUAL_H_
#define _TAVISUAL_H_

#include <vector>

using std::vector;

// ROOT class
class TObject;
class TGraph;
class TLegend;
class TCanvas;

class TAAnode;
class TAPlaStrip;
class TAChannel;
class TH2F;

class TAVisual{
public:
	struct tHitMap{ float x, z;}; // for filling fG2HitMap

	static TAVisual *Instance();
	virtual ~TAVisual();

	void AddAnode(TAAnode *ano);
	void AddPlaStrip(TAPlaStrip *str);
	void AddChannel(TAChannel *ch);
	void FillEvent();
	void FillHitMap();
	void DrawEventSnap(int index = -1); // draw a snap of the event
	void DrawHitMap();
	TCanvas *GetCanvas();
	virtual void Configure();
protected:
	static TAVisual *fInstance;
	TAVisual();
	vector<TObject *> fObjArr;

	vector<TAAnode *> fAnodeArr;
	vector<TAPlaStrip *> fPlaStripArr;
	vector<TAChannel *> fChArr;
	TGraph *fGMainFrame;
	TGraph *fGAnodeDumb;
	TGraph *fGAnodeFired;
	TGraph *fGMagnet;
	TGraph *fGPlaStrip;
	TGraph *fGPlaStripDumb;
	TGraph *fGChannel;
	TGraph *fGTrack;
	TGraph *fGTrack_R; // drift distance circles
	TGraph *fGCurve; // distorted track in the dipole magnet
	TGraph *fGTarget; // the target
	TGraph *fGBeamline; // the central beam line
	TLegend *fGLegend; // to show some text in the graph

	TH2F *fH2HitMap;
	tHitMap *fHitMapData; // array for the function value to fH2HitMap
	TCanvas *fCanvas;
};


#endif
