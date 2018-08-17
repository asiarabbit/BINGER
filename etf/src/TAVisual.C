///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAVisual.C													     //
//   TAVisual.C -- source file for class TAVisual									 //
//   Introduction: visualize the particle tracks and fired detectors. This is a user //
// interface, so is rather important to show the quality of the whole code.			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/22.															     //
// Last modified: 2018/7/1, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

// ROOT include
#include "TGraph.h"
#include "TLegend.h"
#include "TH2F.h"
#include "TCanvas.h"

#include "TAVisual.h"
#include "TAAnode.h"
#include "TAAnodeData.h"
#include "TAAnodePara.h"
#include "TAEventProcessor.h"
#include "TAPopMsg.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAUIDParser.h"
#include "TAChannel.h"
#include "TADeployPara.h"
#include "TAPID.h"
#include "TAGPar.h"
#include "TAMath.h"
#include "TACtrlPara.h"


static const TAGPar *gp = TAGPar::Instance();
static const double z_SiPMArr = TADeployPara::Instance()->GetSiPMArrayZ0(); // z of SiPM Pla Array
static const double z_Ta = TADeployPara::Instance()->GetTargetZ0(); // z of the target
static const TACtrlPara *clp = TACtrlPara::Instance();

TAVisual *TAVisual::fInstance = nullptr;

TAVisual::TAVisual() : fGMainFrame(0), fGAnodeDumb(0), fGAnodeFired(0),
		fGMagnet(0), fGPlaStrip(0), fGPlaStripDumb(0), fGChannel(0),
		fGTrack(0), fGTrack_R(0), fGCurve(0), fGTarget(0), fGBeamline(0),
		fGLegend(0), fH2HitMap(0), fHitMapData(0), fCanvas(0){
	fAnodeArr.reserve(3000);
	fPlaStripArr.reserve(80);
	fChArr.reserve(10);
}
TAVisual::~TAVisual(){
	for(TObject *&obj : fObjArr){
		if(obj){ delete obj; obj = nullptr; }
	}

	if(fHitMapData){ delete fHitMapData; fHitMapData = nullptr; }
	if(fCanvas){ delete fCanvas; fCanvas = nullptr; }
}
TAVisual *TAVisual::Instance(){
	if(!fInstance) fInstance = new TAVisual();
	return fInstance;
}

// note that this function can only be called after the TAEventProcessor::Configure is called.
void TAVisual::Configure(){
	if(0 == fAnodeArr.size())
		TAPopMsg::Warn("TAVisual", "Configure: DC Anode array is empty. DC not configured?");

	fGMainFrame = new TGraph(); fGMainFrame->SetNameTitle("MainFrame", "Experiment Snap");
	const double L = gp->Val(31), W = gp->Val(32);
	fGMainFrame->SetPoint(0, L, W); fGMainFrame->SetPoint(1, L, -W);
	fGMainFrame->SetPoint(2, -W, -W); fGMainFrame->SetPoint(3, -W, W);
	fGMainFrame->SetPoint(4, L, W);
	fGAnodeDumb = new TGraph(); fGAnodeDumb->SetNameTitle("AnodeDumb", "AnodeDumb");
	fGAnodeDumb->SetMarkerColor(15); fGAnodeDumb->SetMarkerStyle(7);
	for(TAAnode *&ano : fAnodeArr){
		TAAnodePara *para = ano->GetAnodePara();
		if(para->GetChannelId() < 0) continue;
		fGAnodeDumb->SetPoint(fGAnodeDumb->GetN(), para->GetProjectionZ(), para->GetProjectionX());
	}
	fGAnodeFired = new TGraph(); fGAnodeFired->SetNameTitle("AnodeFired", "AnodeFired");
	fGAnodeFired->SetMarkerColor(kGreen); fGAnodeFired->SetMarkerStyle(7);
	fGTrack = new TGraph(); fGTrack->SetNameTitle("Track", "Track");
	fGTrack->SetMarkerColor(4); fGTrack->SetMarkerStyle(1);
	fGTrack_R = new TGraph(); fGTrack_R->SetNameTitle("Track_R", "Track_R");
	fGTrack_R->SetMarkerColor(6); fGTrack_R->SetMarkerStyle(6); fGTrack->SetMarkerSize(4);
	fGCurve = new TGraph(); fGCurve->SetNameTitle("aoz", "Curve in the Diple Magnet");
	fGCurve->SetMarkerStyle(1); fGCurve->SetMarkerSize(3);
	fGCurve->SetMarkerColor(6); fGCurve->SetLineColor(6);
	TAPID::Instance()->SetCurveGraph(fGCurve); // fill fGCurve in TAPID::TransportIon method
	fGTarget = new TGraph(); fGTarget->SetNameTitle("Target", "Target");
	fGTarget->SetMarkerStyle(29); fGTarget->SetMarkerColor(6); fGTarget->SetMarkerSize(1);
	fGTarget->SetPoint(0, z_Ta, 0.);
	fGBeamline = new TGraph(); fGBeamline->SetNameTitle("Beamline", "Beamline");
	fGBeamline->SetLineColor(8); fGBeamline->SetLineStyle(3); fGBeamline->SetLineWidth(2);
	fGBeamline->SetPoint(0, -10000., 0.); fGBeamline->SetPoint(1, 15000., 0.);
	fGLegend = new TLegend(0.4,0.8,0.6,0.89); fGLegend->SetBorderSize(0);
	fGLegend->AddEntry(fGCurve, fGCurve->GetName(), "lp");

	fGMagnet = new TGraph(); fGMagnet->SetNameTitle("MagnetFrame", "MagnetFrame");
	fGMagnet->SetMarkerColor(5); fGMagnet->SetMarkerStyle(29);
	fGMagnet->SetLineWidth(3); fGMagnet->SetLineColor(3);
	fGMagnet->SetPoint(0, 475., 500.); fGMagnet->SetPoint(1, 475., -500.);
	fGMagnet->SetPoint(2, -475., -500.); fGMagnet->SetPoint(3, -475., 500.);
	fGMagnet->SetPoint(4, 475., 500.);
	fGPlaStrip = new TGraph(); fGPlaStrip->SetNameTitle("PlaStrip", "PlaStrip");
	fGPlaStrip->SetMarkerStyle(29); fGPlaStrip->SetMarkerColor(2);
	fGPlaStripDumb = new TGraph(); fGPlaStripDumb->SetNameTitle("PlaStripDumb", "PlaStripDumb");
	fGPlaStripDumb->SetMarkerStyle(29); fGPlaStripDumb->SetMarkerColor(30);
	TGraph *&gpla = fGPlaStripDumb;
	for(TAPlaStrip *&str : fPlaStripArr){
		if(str->GetUV()->GetPara()->GetChannelId() < 0) continue;
		double pos[3]; str->GetStripPara()->GetGlobalProjection(pos);
		// identify the strip
		int type[6]{}; TAUIDParser::DNS(type, str->GetUID());
		if(3 == type[0] || 4 == type[0]) // TOFWall strip
			gpla->SetPoint(gpla->GetN(), pos[2], pos[0]); // z, x
		if(5 == type[0]) // SiPMPlaBarrel strip
			gpla->SetPoint(gpla->GetN(), pos[1], pos[0]); // y, x
	} // end for over strips
	// specifically for SiPM plastic scintillator array
	for(TAChannel *&ch : fChArr){
			gpla->SetPoint(gpla->GetN(), z_SiPMArr, ch->GetPara()->GetValue());
	}

	fGChannel = new TGraph(); fGChannel->SetNameTitle("Channel", "Channel");
	fGChannel->SetMarkerStyle(21); fGChannel->SetMarkerColor(4);

	fH2HitMap = new TH2F("HitDistri", "HitDistri;Z/mm;X/mm", 800, 0., L, 500, -W, W);
	fHitMapData = new tHitMap[fAnodeArr.size()];
	const int n = fAnodeArr.size();
//	TAPopMsg::Debug("TAVisual", "Configure: Anode array size: %d", n); // DEBUG
	for(int i = 0; i < n; i++){
		TAAnodePara *para = (TAAnodePara*)fAnodeArr[i]->GetPara();
		if(para->GetChannelId() < 0) continue;
		fHitMapData[i].z = para->GetProjectionZ();
		fHitMapData[i].x = para->GetProjectionX();
	}

	fObjArr.push_back(fGMainFrame); fObjArr.push_back(fGMagnet);
	fObjArr.push_back(fGAnodeDumb); fObjArr.push_back(fGAnodeFired);
	fObjArr.push_back(fGPlaStrip); fObjArr.push_back(fGPlaStripDumb);
	fObjArr.push_back(fGTrack); fObjArr.push_back(fGTrack_R);
	fObjArr.push_back(fGCurve); fObjArr.push_back(fGTarget);
	fObjArr.push_back(fGChannel); fObjArr.push_back(fGBeamline);
	fObjArr.push_back(fGLegend); fObjArr.push_back(fH2HitMap);
} // end of the memeber method Configure
void TAVisual::FillEvent(){
	// fill fired anodes
	if(!fGAnodeFired) TAPopMsg::Error("TAVisual", "Fill: fGAnodeFired is null");
	TGraph *&gaf = fGAnodeFired;
	while(gaf->GetN()) gaf->RemovePoint(0);
	for(TAAnode *&ano : fAnodeArr){
		TAAnodePara *pra = (TAAnodePara*)ano->GetPara();
		if(ano->GetFiredStatus()){
			// draw anode projection
			const double zc = pra->GetProjectionZ(), xc = pra->GetProjectionX();
			gaf->SetPoint(gaf->GetN(), zc, xc);
			// draw all drift distance circles
			double t = ano->GetDriftTime();
			unsigned uid = ano->GetUID();
			t += clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid);
			const double r = ano->GetDriftDistance(t, 3); // 3: STR_id
			const int nCir = r/5.*20 < 5 ? 5 : r/5.*20;
//			cout << "ano->GetName(): " << ano->GetName() << endl; // DEBUG
//			cout << "t: " << t << "\tr: " << r << endl; getchar(); // DEBUG
			double z, x, theta; // temporary variables
			if(r > 0.) for(int i = nCir; i--;){
				theta = (2.* i / nCir - 1.) * TAMath::Pi(); // +-Pi
				z = zc + r * cos(theta);
				x = xc + r * sin(theta);
				gaf->SetPoint(gaf->GetN(), z, x);
			} // end for over i
//			TAPopMsg::Debug("TAVisual", "Fill: GetN(): %d\t%f\t%f)", gaf->GetN(), pra->GetProjectionZ(), pra->GetProjectionX());
		}
	} // end for over anodes
	// fill tracks and drift distances
	if(!fGTrack) TAPopMsg::Error("TAVisual", "Fill: fGTrack is null");
	if(!fGTrack_R) TAPopMsg::Error("TAVisual", "Fill: fGTrack_R is null");
	while(fGTrack->GetN()) fGTrack->RemovePoint(0); // clear the graph
	while(fGTrack_R->GetN()) fGTrack_R->RemovePoint(0); // clear the graph
	TAEventProcessor::Instance()->FillTrack(fGTrack, fGTrack_R);
	// fill fired TOFWall strips
	if(!fGPlaStrip) TAPopMsg::Error("TAVisual", "Fill: fGPlaStrip is null");
	TGraph *gpla = fGPlaStrip;
	while(gpla->GetN()) gpla->RemovePoint(0); // clear the graph
	for(TAPlaStrip *&str : fPlaStripArr){
		if(4 == str->GetFiredStatus()){
			double pos[3]; str->GetStripPara()->GetGlobalProjection(pos);
			// identify the strip
			int type[6]{}; TAUIDParser::DNS(type, str->GetUID());
			if(3 == type[0] || 4 == type[0]) // TOFWall strip
				gpla->SetPoint(gpla->GetN(), pos[2], pos[0]); // z, x
			if(5 == type[0]) // SiPMPlaBarrel strip
				gpla->SetPoint(gpla->GetN(), pos[1], pos[0]); // y, x
		}
	}
	// fill channels
	if(!fGChannel) TAPopMsg::Error("TAVisual", "Fill: fGChannel is null");
	TGraph *&gch = fGChannel;
	while(gch->GetN()) gch->RemovePoint(0); // clear the graph
	for(TAChannel *&ch : fChArr){
		if(ch->GetFiredStatus()){
			gch->SetPoint(gch->GetN(), z_SiPMArr, ch->GetPara()->GetValue());
		}
	}
} // end of member function FillEvent(...)
void TAVisual::FillHitMap(){
	// fill the hit distribution 2-D graph
	static const int n = fAnodeArr.size();
	for(int i = 0; i < n; i++){
		if(fAnodeArr[i]->GetFiredStatus()) fH2HitMap->Fill(fHitMapData[i].z, fHitMapData[i].x);
	}
} // end of member function FillHitMap
void TAVisual::DrawHitMap(){
	TCanvas *c = GetCanvas();
	char name[64], title[128];
	sprintf(name, "%s_Hit_Map", c->GetName());
	sprintf(title, "%s - Hit Map", c->GetTitle());
	c->SetName(name); c->SetTitle(title);
	c->Clear();

	if(!fH2HitMap) TAPopMsg::Error("TAVisual", "Fill: fH2HitMap is null");
//	for(TAAnode *&ano : fAnodeArr){
//		h2->SetPoint(g2->GetN(), da[i].z, da[i].x, da[i].cnt); i++;
//	}
	fH2HitMap->Draw("colz");
	c->Update();
	c->Write("", TObject::kOverwrite);
}
TCanvas *TAVisual::GetCanvas(){
	if(!fCanvas) fCanvas = new TCanvas("ETF-VISUAL", "ETF-VISUAL", 800, 600);
	return fCanvas;
}
// index: event index
void TAVisual::DrawEventSnap(int index){
	TCanvas *c = GetCanvas();
	char name[64], title[128];
	
	sprintf(name, "Event_Snap_Event%d", index);
	sprintf(title, "Event Snap - Event%d", index);
	c->SetName(name); c->SetTitle(title);
//	c->Clear();

	// establish the main frame and skeleton
	char drawOption[] = "Psame";
	sprintf(title, "Experiment Snap - Event#%d", index);
	fGMainFrame->SetTitle(title);
	fGMainFrame->Draw("APL"); fGMagnet->Draw("PLsame");
	fGTarget->Draw(drawOption); fGBeamline->Draw("PLsame");
	fGPlaStripDumb->Draw(drawOption); fGAnodeDumb->Draw(drawOption);
	if(fGTrack->GetN()) fGTrack->Draw(drawOption);
	if(fGTrack_R->GetN()) fGTrack_R->Draw(drawOption);
	if(fGAnodeFired->GetN()) fGAnodeFired->Draw(drawOption);
	if(fGPlaStrip->GetN()) fGPlaStrip->Draw(drawOption);
	if(fGChannel->GetN()) fGChannel->Draw(drawOption);

	if(fGCurve->GetN()){
		fGLegend->SetEntryLabel(fGCurve->GetName());
		fGCurve->Draw(drawOption); fGLegend->Draw();
		fGCurve->SetName("aoz");
	}
//	c->Update();
	c->Write("", TObject::kOverwrite);
	while(fGCurve->GetN()) fGCurve->RemovePoint(0);
}
void TAVisual::AddAnode(TAAnode *ano){
	fAnodeArr.push_back(ano);
}
void TAVisual::AddPlaStrip(TAPlaStrip *str){
	fPlaStripArr.push_back(str);
}
void TAVisual::AddChannel(TAChannel *ch){
	fChArr.push_back(ch);
}







