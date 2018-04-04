///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCTaL.C														 //
//   TAMWDCTaL.C -- source file for class TAMWDCTaL									 //
//   Introduction: A class derived from TAMWDCTa. This class is for large size DC	 //
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

#include "TAMWDCTaL.h"
#include "TADetectorPara.h"
#include "TAPopMsg.h"

TAMWDCTaL::TAMWDCTaL(const string &name, const string &title, unsigned uid)
		: TAMWDCTa(name, title, uid){
}
TAMWDCTaL::~TAMWDCTaL(){}

void TAMWDCTaL::GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const{
	const int type = dcType, l = layerOption - 1;
	const int nAnoPL = GetNAnodePerLayer();
	if(0 != l && 1 != l)
		TAPopMsg::Error(GetName().c_str(), "GetAnodeCenterPosition: Invalid layerOption (only 1 or 2 is permitted)");
	int n = anodeId % nAnoPL;
	double x0 = 10. - 5. * nAnoPL; // x of the first anode (x0 is the minimum). -70.
	double p_local[3]{}; // p_local[1] is zero (y, the height), which is about the neutral beam's
	if(TAMWDC::kX == type){
		p_local[0] = x0 + 10. * n - 5. * l; // x X(l+1)
		p_local[1] = 0.; // y
		p_local[2] = 10. * l; // z
	}
	if(TAMWDC::kY == type){
		p_local[0] = 0.; // x
		p_local[1] = x0 + 10. * n - 5. * l; // Y Y(l+1)
		p_local[2] = 10. * l + 25.; // z
	}
	GetDetPara()->GetGlobalPosition(p_local, Ag);
} // end of member function GetAnodeCenterPosition

