///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMUSICM.C														 //
//   TAMUSICM.C -- source file for class TAMUSICM									 //
//   Introduction: A derived class from TAMUSIC. "M" stands for Medium. This MUSIC	 //
// has 4 channels, i.e., 4 sampling unit.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/22.															     //
// Last modified: 2018/5/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#define USE_ADC_PILEUP

#include <cmath>

#include "TAMUSICM.h"
#include "TAPopMsg.h"
#include "TAChannel.h"

TAMUSICM::TAMUSICM(const string &name, const string &title, unsigned uid)
	: TAMUSIC(name, title, uid){
}
TAMUSICM::~TAMUSICM(){}

double TAMUSICM::GetDeltaE(){
	if(-9999. != fDeltaE) return fDeltaE; // fDeltaE has been calculated once
	if(!GetFiredStatus()) return -9999.; // not in valid fired state
#ifdef USE_ADC_PILEUP
	double pileUp = GetPileUp();
	if(pileUp > 1500.) return -9999.; // pile up on, data spoiled
#endif
	if(IsPileUp()) return -9999.; // sca pile up

	int cnt = 0; // number of assigned channels
	fDeltaE = 0.;
	const double dENoiseLevel = 216.; // to rule out noise;   unit: ch

	int i = 0; // to identify the first channel
	for(TAChannel *c : fChArr){
		if(0 == i) continue; // the first channel contains pileUp info, so is dropped
		i++;
		if(c && c->GetFiredStatus()){
			double adc = c->GetLeadingTime(); // the first leading time measurement stores the adc value
			if(adc < dENoiseLevel) continue; // noise
			fDeltaE += adc;
			cnt++;
		} // end for and if
	} // end for over channels

	if(0 != cnt){
		fDeltaE /= cnt;
		return fDeltaE;
	}
	
	return -9999.;
} // end of member function GetDeltaE()
// paticle charge number
double TAMUSICM::GetZ(){
	if(-9999. != fZ) return fZ; // fZ has been calculated once
	double de = GetDeltaE();
	if(-9999. == de) return -9999.; // particle energy deposit information not available
	double beta = GetBeta();
	if(beta < 0.) return -9999.; // particle velocity not available
	static double coeZ = 0.5, ofstZ = -0.36;
	fZ = coeZ * pow(beta, 0.8) * sqrt(de) + ofstZ;

	return fZ;
}

void TAMUSICM::Configure(){
	if(fChArr.size()){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return;
	}
	
	char name[64]; TAChannel *c;
	fNChannel = 4;
	for(int i = 0; i <= fNChannel; i++){ // the first channel is for pile up mark
		sprintf(name, "%s->Ch%d", GetName().c_str(), i);
		c = new TAChannel(name, name, GetUID()+(i<<6));
		fChArr.push_back(c);
	}
}
