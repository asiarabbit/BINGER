///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAT0_0.cxx																										     //
//   TAT0_0.cxx -- source file for class TAT0_0																	     //
//   Introduction: start time detector, located at the middle of RIBLL2 to measure	 //
// particle time of flight. It is actually a plastic scintillator with one-end PMT	 //
// readout.																																					 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/10/10.																													     //
// Last modified: 2020/04/24, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAUIDParser.h"
#include "TAPopMsg.h"
#include "TAT0_0.h"
#include "TAChannel.h"
#include "TAGPar.h"

static const TAGPar *gp = TAGPar::Instance();

TAT0_0::TAT0_0(const string &name, const string &title, unsigned uid)
		: TAStrip(name, title, uid), TADetUnion(uid){
	fFiredStatus = -2; fDelay = -9999.;
}
TAT0_0::~TAT0_0(){}

// get the channel that belongs to this with uid.
TAStuff *TAT0_0::GetChannel(unsigned uid) const{
	if(uid > 0x7F) return nullptr; // not a uid belonging to this object
	int type[2]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[2]{}; TAUIDParser::DNS(TYPE, fUID); // parse uid of this
	if(type[0] == TYPE[0]){ // belongs to this
		if(0 == type[1])
			return GetUV();
		else if(1 == type[1])
			return GetUH();
	}
	return nullptr;
}
void TAT0_0::Initialize(){
	if(fUV && fUV->GetFiredStatus()) fUV->Initialize();
	if(fUH && fUH->GetFiredStatus()) fUH->Initialize();
}
void TAT0_0::Configure(){
	if(fUV){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}
	fUV = new TAChannel(fName+"->UV", fName+"->UV", fUID+(0<<6)); fUV->SetSerialId(0);
	fUH = new TAChannel(fName+"->UH", fName+"->UH", fUID+(1<<6)); fUV->SetSerialId(1);
	fDV = new TAChannel(fName+"->DV", fName+"->DV", fUID+(2<<6)); fUV->SetSerialId(2);
	fDH = new TAChannel(fName+"->DH", fName+"->DH", fUID+(3<<6)); fUV->SetSerialId(3);

	fStripPara = new TAPlaStripPara(fName+"->Para", fName+"->Para", fUID);
	fStripData = new TAPlaStripData(fName+"->Data", fName+"->Data", fUID);
	double p[3] = {0., 0., GetZ0()}; GetStripPara()->SetGlobalProjection(p);

	fStripPara->SetWidth(100.); // mm, not accurate
	fStripPara->SetLength(100.); // mm，
	fStripPara->AppendDelay(1.E-7);
	fStripPara->SetVeff(1200. / 7.8); // mm/ns, roughly calibrated

	GetStripPara()->AppendDelay(gp->Val(53));
	SetStripId(0);

	// print user-defined configurations
	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure: \nfZ0: %f\nfDelay: %f\nfWidth: %f\nfLength: %f\nfVeff: %f\n",
	 fZ0, GetDelay(), fStripPara->GetWidth(), fStripPara->GetLength(), fStripPara->GetVeff());
} // end of member method Configure
