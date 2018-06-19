///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASiPMPlaBarrel.C												     //
//   TASiPMPlaArray.C -- source file for class TASiPMPlaBarrel					     //
//   Introduction: A barrel with a regular octagon cross section, to measure hit 	 //
// multiplicity around the target, so as to implicate impact parameter of the nuclei //
// bombardment.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/21.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "TASiPMPlaBarrel.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TADetectorPara.h"
#include "TAUIDParser.h"
#include "TAMath.h"
#include "TAChannel.h"
#include "TAChPara.h"
#include "TAPopMsg.h"

TASiPMPlaBarrel::TASiPMPlaBarrel(const string &name, const string &title, unsigned uid)
	: TATOFWall(name, title, uid), TADetUnion(uid){
}
TASiPMPlaBarrel::~TASiPMPlaBarrel(){}

void TASiPMPlaBarrel::Initialize(){
	TATOFWall::Initialize();
}
void TASiPMPlaBarrel::Configure(){ //			    C
	TADetector::Configure();    //				 D     B
	char plate[9] = "ABCDEFGH"; //			   E	    A
	char name[64]; //							 F    H
	TAPlaStrip *str;	//						   G
	const double delay = 5.; // ns, 3m signal wire compared with 2m signal wire.
	for(int i = 0; i < 8; i++){ // loop over 8 plates
		for(int j = 0; j < 3; j++){ // loop over 3 stirps
			short stripId = i*3+j;
			sprintf(name, "%s->Strip%c_%d", GetName().c_str(), plate[i], stripId);
			str = new TAPlaStrip(name, name, fUID+(i<<6)+(j<<9));
			str->SetStripId(stripId); str->Configure();
			TAPlaStripPara *pra = str->GetStripPara();
			pra->SetWidth(20.); pra->SetLength(300.);
			pra->SetVeff(1200./7.8); pra->AppendDelay(1E-10); // -471.
			str->GetDV()->GetPara()->AppendDelay(delay); // D: B(back) -> close to Magnet
			str->GetDH()->GetPara()->AppendDelay(delay); // D: B(back) -> close to Magnet
			if(stripId >= 15 && stripId <= 22){ // 3m signal wire -> add 5ns delay
				str->GetUV()->GetPara()->AppendDelay(delay); // U: F(front) -> far from Magnet
				str->GetUH()->GetPara()->AppendDelay(delay); // U: F(front) -> far from Magnet
			}
			fStripArr.push_back(str);
		} // end for over j
	} // end for over i
	// 20171026_2149, calibrated.
	GetStrip(1)->GetUV()->GetPara()->AppendDelay(-1.);
	GetStrip(1)->GetUV()->GetPara()->AppendDelay(-1.);
	GetStrip(2)->GetUV()->GetPara()->AppendDelay(-2.);
	GetStrip(2)->GetUV()->GetPara()->AppendDelay(-2.);
	
	// configure the barrel itself
	fDelayAvrg = -9.427;
	double pos[6]{};
	// The central line of the detector is supposed to be overlap the z-axis.
	pos[2] = -617.01 + 433.5/2.; // the z value. the front end (far from the magnet)
	GetDetPara()->SetPosition(pos);
	AssignStripPosition();

	// print user-defined configurations
	TAPlaStripPara *pra = GetStrip(0)->GetStripPara();
	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure: \nz0: %f\nfDelayAvrg: %f\nfWidth: %f\nfLength: %f\nfVeff: %f\n", GetDetPara()->GetZ(), fDelayAvrg, pra->GetWidth(), pra->GetLength(), pra->GetVeff());
} // end of member method Configure

TAPlaStrip *TASiPMPlaBarrel::GetStrip(short plateId, short stripId) const{
	return GetStrip(plateId*3+stripId);
}

TAStuff *TASiPMPlaBarrel::GetChannel(unsigned uid) const{
	int type[4]{}; TAUIDParser::DNS(type, uid); // parse input uid
	int TYPE[4]{}; TAUIDParser::DNS(TYPE, GetUID()); // parse uid of this

	if(type[0] == TYPE[0]){ // belongs to this object
		TAPlaStrip *str = GetStrip(type[1], type[2]);
		switch(type[3]){
			case 0: return str->GetUV();
			case 1: return str->GetUH();
			case 2: return str->GetDV();
			case 3: return str->GetDH();
			default: return nullptr;
		}
	}
	return nullptr;
}

void TASiPMPlaBarrel::GetStripProjection(int serialId, double *p) const{
	const double width = GetStrip(0)->GetStripPara()->GetWidth();
	const double r0 = width*3.*(1.+sqrt(2.))/2.; // from central point to any side
	const double theta = int(serialId / 3) * 45. * TAMath::DEGREE();
	const short subId = serialId % 3;
	double pt[3][2]; // [serilaId: 0,1,2][x,y]
	pt[0][0] = r0; pt[0][1] = -width;
	pt[1][0] = r0; pt[1][1] = 0.;
	pt[2][0] = r0; pt[2][1] = width;

	p[0] =  cos(theta) * pt[subId][0] + sin(theta) * pt[subId][1];
	p[1] = -sin(theta) * pt[subId][0] + cos(theta) * pt[subId][1];
	p[2] = GetDetPara()->GetZ();
}
void TASiPMPlaBarrel::AssignStripPosition(){
	double p[3];
	const int n = GetNStrip();
	for(int i = 0; i < n; i++){
		GetStripProjection(i, p);
		GetStrip(i)->GetStripPara()->SetGlobalProjection(p);
	}
}








