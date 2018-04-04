///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCTa.C														 //
//   TAMWDCTa.C -- source file for class TAMWDCTa									 //
//   Introduction: A class derived from TAMWDC. S stands for small. This class is for//
// MWDCs around the target.															 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/27.															     //
// Last modified: 2018/3/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TADCSFE16.h"
#include "TADCCable.h"
#include "TADCSuperLayer.h"
#include "TAMWDCTa.h"
#include "TADetectorPara.h"
#include "TAPopMsg.h"
#include "TAMWDCArray.h"
#include "TACtrlPara.h"
#include "TAMath.h"
#include "TAUIDParser.h"

TAMWDCTa::TAMWDCTa(const string &name, const string &title, unsigned uid)
		: TAMWDC(name, title, uid){
}
TAMWDCTa::~TAMWDCTa(){}

void TAMWDCTa::GetAnodeGlobalDirection(int dcType, double *ag) const{
	double a[2][3] = {{0., 1., 0.}, {1., 0., 0.}}; // X-Y
	GetDetPara()->GetGlobalRotation(a[dcType], ag);
}
void TAMWDCTa::GetAnodeGlobalProjection(int dcType, const double *globalCenter, double *globalProjection) const{
	const double *p = globalCenter;
	if(0. == p[0] && 0. == p[1] && 0. == p[2])
		TAPopMsg::Warn(GetName().c_str(), "GetAnodeGlobalProjection: input anode global Center is zero array~ Not assigned?");
	if(dcType == TAMWDC::kX){ // [0-1-2] -> [X-Y-Z]
		globalProjection[0] = p[0];
		globalProjection[1] = p[1]; // Y value would not be used in tracking
		globalProjection[2] = p[2];
	}
	if(dcType == TAMWDC::kY){ // [0-1-2] -> [Y-X-Z]
		globalProjection[0] = p[1];
		globalProjection[1] = p[0]; // Y value would not be used in tracking
		globalProjection[2] = p[2];
	}
}

void TAMWDCTa::Configure(){
	TADetector::Configure();
	if(0 != fSLayerArr.size()){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}

	TADCSuperLayer *sl[2]; // [0-1]: [X-Y]
	sl[0] = new TADCSuperLayer(fName+"->SLayerX", fTitle+"->SLayerX", fUID+(0<<8));
	sl[1] = new TADCSuperLayer(fName+"->SLayerY", fTitle+"->SLayerY", fUID+(1<<8));

	for(int i = 2; i--;){ // X-Y
		sl[i]->SetSLayerId(i);
		sl[i]->Configure();
		fSLayerArr.push_back(sl[i]);
		fNAnodePerLayer = sl[i]->GetNCable() * 16;

		// set mother dc
		for(int j = fNAnodePerLayer; j--;){ // loop over anodes in a super layer
			GetAnodeL1(i, j)->GetAnodePara()->SetMotherDC(this);
			GetAnodeL2(i, j)->GetAnodePara()->SetMotherDC(this);
		}
	}
//	TAPopMsg::Debug(GetName().c_str(), "Configure: showcase: fNAnodePerLayer: %d", fNAnodePerLayer);
}

