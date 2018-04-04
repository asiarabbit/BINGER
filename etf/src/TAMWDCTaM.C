///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCTaM.C														 //
//   TAMWDCTaM.C -- source file for class TAMWDCTaM									 //
//   Introduction: A class derived from TAMWDCTa. This class is for medium size DC	 //
// downstream of the target.														 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/3/28.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAMWDCTaM.h"
#include "TADetectorPara.h"
#include "TAPopMsg.h"

TAMWDCTaM::TAMWDCTaM(const string &name, const string &title, unsigned uid)
		: TAMWDCTa(name, title, uid){
}
TAMWDCTaM::~TAMWDCTaM(){}

void TAMWDCTaM::GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const{
	const int type = dcType, l = layerOption - 1;
	const int nAnoPL = GetNAnodePerLayer();
	if(0 != l && 1 != l)
		TAPopMsg::Error(GetName().c_str(), "GetAnodeCenterPosition: Invalid layerOption (only 1 or 2 is permitted)");
	int n = anodeId % nAnoPL;
	double x0 = 5. - 2.5 * nAnoPL; // x of the first anode (x0 is the minimum). -35.
	double p_local[3]{}; // p_local[1] is zero (y, the height), which is about the neutral beam's
	if(TAMWDC::kX == type){
		p_local[0] = x0 + 5. * n - 2.5 * l; // x X(l+1)
		p_local[1] = 0.; // y
		p_local[2] = 5. * l; // z
	}
	if(TAMWDC::kY == type){
		p_local[0] = 0.; // x
		p_local[1] = x0 + 5. * n - 2.5 * l; // Y Y(l+1)
		p_local[2] = 5. * l + 12.5; // z
	}
	GetDetPara()->GetGlobalPosition(p_local, Ag);
} // end of member function GetAnodeCenterPosition

