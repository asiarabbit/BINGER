///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAT0_1.C														     //
//   TAT0_1.C -- source file for class TAT0_1									     //
//   Introduction: start time detector, located at the end of RIBLL2 to measure		 //
// particle time of flight. It is actually a plastic scintillator with both-end PMT	 //
// readouts.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/10.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAT0_1.h"
#include "TAUIDParser.h"
#include "TAPopMsg.h"
#include "TAChannel.h"
#include "TAPlaStripPara.h"
#include "TAPlaStripData.h"
#include "TAGPar.h"

static const TAGPar *gp = TAGPar::Instance();

TAT0_1::TAT0_1(const string &name, const string &title, unsigned uid)
		: TAPlaStrip(name, title, uid), TADetUnion(uid){
	fZ0 = -9999; fDistanceOfFlight = -9999.;
}
TAT0_1::~TAT0_1(){}
// position along the beam line in the global coordinate system
double TAT0_1::GetZ0() const{
	if(-9999 == fZ0) TAPopMsg::Warn(GetName().c_str(), "GetZ0: not assigned.");
	return fZ0;
}
// from T0_0 to T0_1
double TAT0_1::GetDistanceOfFlight() const{
	if(-9999 == fDistanceOfFlight)
		TAPopMsg::Warn(GetName().c_str(), "GetDistanceOfFlight: not assigned.");
	return fDistanceOfFlight;
}

// get the channel that belongs to this with uid
TAStuff *TAT0_1::GetChannel(unsigned uid) const{
	if(uid > 0xFF) return nullptr; // not a uid belonging to this object
	int type[2]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[2]{}; TAUIDParser::DNS(TYPE, fUID); // parse uid of this
	if(type[0] == TYPE[0]){ // belongs to this
		if(0 == type[1]) return GetUV();
		else if(1 == type[1]) return GetUH();
		else if(2 == type[1]) return GetDV();
		else if(3 == type[1]) return GetDH();
	}
	return nullptr;
}

void TAT0_1::Configure(){
	if(fUV){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	SetZ0(gp->Val(85)); // from this to 32Q02 unit: mm 20171026_2157, pionExp
	fUV = new TAChannel(fName+"->UV", fName+"->UV", fUID+(0<<6)); fUV->SetSerialId(0);
	fUH = new TAChannel(fName+"->UH", fName+"->UH", fUID+(1<<6)); fUV->SetSerialId(1);
	fDV = new TAChannel(fName+"->DV", fName+"->DV", fUID+(2<<6)); fUV->SetSerialId(2);
	fDH = new TAChannel(fName+"->DH", fName+"->DH", fUID+(3<<6)); fUV->SetSerialId(3);

	fStripPara = new TAPlaStripPara(fName+"->Para", fName+"->Para", fUID);
	fStripData = new TAPlaStripData(fName+"->Data", fName+"->Data", fUID);
	double p[3] = {0., 0., GetZ0()}; GetStripPara()->SetGlobalProjection(p);

	fStripPara->SetWidth(400.); // mm, not accurate
	fStripPara->SetLength(400.); // mm
	fStripPara->AppendDelay(1.E-7); // set as the time reference
	fStripPara->SetVeff(1200. / 7.8); // mm/ns, roughly calibrated

	GetStripPara()->AppendDelay(gp->Val(4));
	GetUV()->GetPara()->AppendDelay(gp->Val(33));
	SetStripId(0);

	// print user-defined configurations
	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure: \nfZ0: %f\nfDelay: %f\nfWidth: %f\nfLength: %f\nfVeff: %f\n", fZ0, GetDelay(), fStripPara->GetWidth(), fStripPara->GetLength(), fStripPara->GetVeff());
} // end of member method Configure













