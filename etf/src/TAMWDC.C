///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDC.C															 //
//   TAMWDC.C -- source file for class TAMWDC										 //
//   Introduction: MWDC class, providing position calculation service for wires in	 //
// the MWDC. A container class, storing TADCSuperLayer objects.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/3.															     //
// Last modified: 2017/11/13, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "TF1.h" // ROOT class

#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TADCSFE16.h"
#include "TADCCable.h"
#include "TADCSuperLayer.h"
#include "TAMWDC.h"
#include "TAPopMsg.h"
#include "TAMWDCArray.h"
#include "TACtrlPara.h"
#include "TAMath.h"
#include "TAUIDParser.h"
#include "TADeployPara.h"

TAMWDC::TAMWDC(const string &name, const string &title, unsigned uid)
		: TADetector(name, title, uid), 
		fSLayerArr{0}, fMWDCId(-1), fNAnodePerLayer(-1), fMotherDCArr(0){
}
TAMWDC::~TAMWDC(){
	for(TADCSuperLayer *sl : fSLayerArr) if(sl){
		delete sl; sl = nullptr;
	}
}
short TAMWDC::GetMWDCId() const{
	if(fMWDCId < 0) TAPopMsg::Error(GetName().c_str(), "GetMWDCId: MWDC id not assigend");
	return fMWDCId;
}
short TAMWDC::GetNAnodePerLayer() const{
	if(fNAnodePerLayer < 0) TAPopMsg::Error(GetName().c_str(), "GetNAnodePerLayer: Not assigend, %d", fNAnodePerLayer);
	return fNAnodePerLayer;
}
// acceptance angle of the X1 SLayer of the MWDC
// (from the coordinate system origin to the both ends of the SLayer)
double TAMWDC::Acceptance() const{
	if(!GetDetPara()->IsAssigned())
		TAPopMsg::Error(GetName().c_str(), "Acceptance: detector position not assigned.");
	double p0[3]{}, p1[3]{};
	((TAAnodePara*)GetAnode(0, 1, 0)->GetPara())->GetGlobalProjection(p0);
	((TAAnodePara*)GetAnode(0, 1, GetNAnodePerLayer() - 1)->GetPara())->GetGlobalProjection(p1);
	double pp0[2] = {p0[2], p0[0]}, pp1[2] = {p1[2], p1[0]}; // z, x, to fit the format of the method
	return TAMath::acceptance(pp0, pp1);
}
int TAMWDC::GetNFiredAnodePerLayer(int dcType, int layerOption) const{
	int cnt = 0;
	const int n = GetNAnodePerLayer();
	for(int i = 0; i < n; i++){
		if(GetAnode(dcType, layerOption, i)->GetFiredStatus()) cnt++;
	}
	return cnt;
}
TADCSuperLayer *TAMWDC::GetSLayer(int n) const{
	if(n >= fSLayerArr.size())
		TAPopMsg::Error(GetName().c_str(), "GetSLayer: The input subscript is too large: %d", n);
	if(!fSLayerArr[n])
		TAPopMsg::Error(GetName().c_str(), "GetSLayer: Super-layer#%d not assigend", n);
	return fSLayerArr[n];
}
TAMWDCArray *TAMWDC::GetMotherDCArr() const{
	if(!fMotherDCArr) TAPopMsg::Error(GetName().c_str(), "GetMotherDCArr: pointer is null.");
	return fMotherDCArr;
}
void TAMWDC::SetSLayer(int n, TADCSuperLayer *sl){
	if(n >= fSLayerArr.size())
		TAPopMsg::Error(GetName().c_str(), "SetSLayer: The input subscript is too large: %d", n);
	if(!sl) TAPopMsg::Error(GetName().c_str(), "SetSLayer: The input pointer is null.");
	fSLayerArr[n] = sl;
}
void TAMWDC::SetMWDCId(int id){
	if(id < 0) TAPopMsg::Warn(GetName().c_str(), "SetMWDCId: Input id is minus: %d", id);
	fMWDCId = id;
}

static const double degree = TAMath::DEGREE();
// serialId is unique in one MWDC(X, U or V). p stores global coordinates.
void TAMWDC::GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const{
	const int type = dcType, l = layerOption - 1;
	if(0 != l && 1 != l) TAPopMsg::Error(GetName().c_str(), "GetAnodeCenterPosition: Invalid layerOption (only 1 or 2 is permitted)");
	int n = anodeId % GetNAnodePerLayer();
	double x0 = 7. - 5. * fNAnodePerLayer; // x of the first anode (x0 is the minimum). -393
	double p_local[3]; // p_local[1] is zero (y, the height), which is about the neutral beam's.
	if(TAMWDC::kX == type){
		p_local[0] = x0 + 10. * n - 4. * l; // x X(l+1)
		p_local[1] = 0.; // y
		p_local[2] = 10. * l; // z
	}
	// XXX: 0.8: thickness of 6 dc plate gaps. (2.8/21*6 (mm)) 2017/11/13
	if(TAMWDC::kU == type){
		p_local[0] = (x0 + 10. * n - 4. * l) * cos(30. * degree); // x  U(l+1)
		p_local[1] = -p_local[0] * tan(30. * degree); // y
		p_local[2] = 10. * l - 40.8; // z

		// modified in May 23rd, 2017.
		// installation error, verified by exp data 20161125_2031.dat.
		int whoami[6]; TAUIDParser::DNS(whoami, fUID);
		if(4 == whoami[0] && 1 == whoami[1]) // DCArr(R) && DC(1)
			p_local[0] += TACtrlPara::DCArrR_DC1UHorizontalDeviation();
	}
	if(TAMWDC::kV == type){
		// + 4. -> DEBUG
		p_local[0] = (x0 + 10. * n - 4. + 4. * l) * cos(30. * degree); // x V(l+1) 
		p_local[1] = p_local[0] * tan(30. * degree); // y
		p_local[2] = 10. * l + 40.8; // z
	}
	GetDetPara()->GetGlobalPosition(p_local, Ag);
}
void TAMWDC::GetAnodeGlobalDirection(int dcType, double *ag) const{
	double a[3][3] = {{0., 1., 0.}, {1./2., sqrt(3.)/2., 0.}, {-1./2., sqrt(3.)/2., 0.}}; // X-U-V
	GetDetPara()->GetGlobalRotation(a[dcType], ag);
}

void TAMWDC::GetAnodeGlobalProjection(int dcType, const double *globalCenter, double *globalProjection) const{
	double phi = GetMotherDCArr()->GetPhiAvrg();
	double theta = 0.;
	if(dcType == TAMWDC::kU) theta = -30. * degree;
	if(dcType == TAMWDC::kV) theta = 30. * degree;
	if(dcType == TAMWDC::kX) phi = 0.; // X anodes don't need backward rotation.
	double cp = cos(phi), sp = sin(phi);
	double ct = cos(theta), st = sin(theta);
	const double *p = globalCenter;
	if(0. == p[0] && 0. == p[1] && 0. == p[2])
		TAPopMsg::Warn(GetName().c_str(), "GetAnodeGlobalProjection: input anode global Center is zero array~ Not assigned?");
	// Au = Rz[-theta].Ry[-phi].A;
	globalProjection[0] = ct*cp  *p[0] +st *p[1] -ct*sp *p[2];
	globalProjection[1] = -st*cp *p[0] +ct *p[1] +st*sp *p[2];
	globalProjection[2] = sp     *p[0] +0. *p[1] +cp    *p[2];
}
void TAMWDC::AssignAnodePosition(){
	if(!GetDetPara()->IsAssigned())
		TAPopMsg::Warn(GetName().c_str(), "AssignAnodePosition: Detector Position not assigned.");
	const int n = GetNAnodePerLayer();
	// anode global direction, position and projection, temporary variable
	double ag[3]{}, Ag[3]{}, proj[3]{};
	for(int i = 3; i--;){ // dc type
		GetAnodeGlobalDirection(i, ag); // assign anode orientation
		TADCSuperLayer *sl = GetSLayer(i);
		sl->SetGlobalDirection(ag);
		for(int j = 2; j--;){ // layer option
			for(int k = n; k--;){ // anode id
				// NOTE: the sequence of the calling of the two funcs canNOT be reversed
				GetAnodeCenterPosition(i, j+1, k, Ag);
				GetAnodeGlobalProjection(i, Ag, proj);

				TAAnodePara *para = (TAAnodePara*)GetAnode(i, j+1, k)->GetPara();
				para->SetGlobalCenter(Ag);
				para->SetGlobalDirection(sl->GetGlobalDirection());
				para->SetGlobalProjection(proj);
			} // anode id
		} // layer option
	} // dc type
} // end of the member function
int TAMWDC::GetSTRid(double k, int dcType) const{
	return GetAnode(dcType, 1, 0)->GetAnodePara()->GetSTRid(k, dcType);
}

TAAnode *TAMWDC::GetAnodeL1(int dcType, int anodeId) const{
	short nap = GetNAnodePerLayer();
	if(anodeId >= nap || anodeId < 0)
		TAPopMsg::Error(GetName().c_str(), "GetAnodeL1: Required anode Id out of range: %d", anodeId);
	TAAnode *ano = GetSLayer(dcType)->GetCable(anodeId/16)->GetSFE16(0)->GetAnode(anodeId%16);
	if(!ano) TAPopMsg::Error(GetName().c_str(), "GetAnodeL1: pointer is null.");
	return ano;
}
TAAnode *TAMWDC::GetAnodeL2(int dcType, int anodeId) const{
	short nap = GetNAnodePerLayer();
	if(anodeId >= nap || anodeId < 0)
		TAPopMsg::Error(GetName().c_str(), "GetAnodeL2: Required anode Id out of range: %d", anodeId);
	TAAnode *ano = GetSLayer(dcType)->GetCable(anodeId/16)->GetSFE16(1)->GetAnode(anodeId%16);
	if(!ano) TAPopMsg::Error(GetName().c_str(), "GetAnodeL2: pointer is null.");
	return ano;
}
TAAnode *TAMWDC::GetAnode(int dcType, int layerOption, int anodeId) const{
	if(1 == layerOption) return GetAnodeL1(dcType, anodeId);
	else if(2 == layerOption) return GetAnodeL2(dcType, anodeId);
	else TAPopMsg::Error(GetName().c_str(), "GetAnode: layer option out of range: %d", layerOption);
	return nullptr;
}
TAAnode *TAMWDC::GetAnode(int dcType, int anodeSerialId) const{
	return GetAnode(dcType, anodeSerialId/GetNAnodePerLayer()+1, anodeSerialId%GetNAnodePerLayer());
}

// create anodes, and assign them with position information.
void TAMWDC::Configure(){
	TADetector::Configure();
	if(fSLayerArr[0]){
		TAPopMsg::Warn(GetName().c_str(), "Configure: has been called once");
		return; // Configure() has been called
	}

	TADCSuperLayer *sl[3]; // [0-1-2]: [X-U-V]
	sl[0] = new TADCSuperLayer(fName+"->SLayerX", fTitle+"->SLayerX", fUID+(0<<9));
	sl[1] = new TADCSuperLayer(fName+"->SLayerU", fTitle+"->SLayerU", fUID+(1<<9));
	sl[2] = new TADCSuperLayer(fName+"->SLayerV", fTitle+"->SLayerV", fUID+(2<<9));

	for(int i = 3; i--;){ // X-U-V
		sl[i]->SetSLayerId(i);
		sl[i]->Configure();
		fSLayerArr[i] = sl[i];
		fNAnodePerLayer = sl[i]->GetNCable() * 16;

		// set mother dc
		for(int j = fNAnodePerLayer; j--;){ // loop over anodes in a super layer
			((TAAnodePara*)GetAnodeL1(i, j)->GetPara())->SetMotherDC(this);
			((TAAnodePara*)GetAnodeL2(i, j)->GetPara())->SetMotherDC(this);
		}
	}
//	TAPopMsg::Debug(GetName().c_str(), "Configure: showcase: fNAnodePerLayer: %d", fNAnodePerLayer);
}
void TAMWDC::Info() const{
	TAPopMsg::ConfigInfo(GetName().c_str(), "Configure:\ndelay: %f\nphi: %f\nacceptance: %f\n", GetAnode(0, 1, 0)->GetPara()->GetDelay(), GetDetPara()->GetPhi() / TAMath::DEGREE(), Acceptance() / TAMath::DEGREE());
}
void TAMWDC::Initialize(){
	for(TADCSuperLayer *&su : fSLayerArr) if(su) su->Initialize();
}







