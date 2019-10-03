///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMUSICL.C														 //
//   TAMUSICL.C -- source file for class TAMUSICL									 //
//   Introduction: A derived class from TAMUSIC. "L" stands for large. This MUSIC has//
// 8 channels, i.e., 8 sampling unit.												 //
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

#include "TAMUSICL.h"
#include "TAPopMsg.h"
#include "TAChannel.h"

TAMUSICL::TAMUSICL(const string &name, const string &title, unsigned uid)
	: TAMUSIC(name, title, uid){
}
TAMUSICL::~TAMUSICL(){}

double TAMUSICL::GetDeltaE(){
	if(-9999 != fDeltaE) return fDeltaE; // fDeltaE has been calculated once
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

	// XXX: temporary for beam experment due June, 2018
	if(fChArr[fNChannel]->GetFiredStatus()) cnt++; // the last channel of this MUSIC is 2-multiplexed

	if(0 != cnt){
		fDeltaE /= cnt;
		return fDeltaE;
	}

	return -9999.;
} // end of member function GetDeltaE()
// paticle charge number
double TAMUSICL::GetZ(){
	if(-9999. != fZ) return fZ; // fZ has been calculated once
	double de = GetDeltaE();
	if(-9999. == de) return -9999.; // particle energy deposit information not available
	double beta = GetBeta();
	if(beta < 0.) return -9999.; // particle velocity not available
	static double coeZ = 0.42, ofstZ = 2.29;
	fZ = coeZ * pow(beta, 0.8) * sqrt(de) + ofstZ;

	return fZ;
}

void TAMUSICL::Configure(){
	if(fChArr.size()){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return;
	}
	
	char name[64]; TAChannel *c;
	fNChannel = 5;
	for(int i = 0; i <= fNChannel; i++){ // the first channel is for pile up mark
		sprintf(name, "%s->Ch%d", GetName().c_str(), i);
		c = new TAChannel(name, name, GetUID()+(i<<6));
		fChArr.push_back(c);
	}
}


