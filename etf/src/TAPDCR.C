///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPDCR.C															 //
//   TAPDCR.C -- source file for class TAPDCR										 //
//   Introduction: A class derived from TAMWDCTa. This class is for MWDCs from BUAA	 //
// which was R&D-ed by Japan researchers, and would be used around the target. R here//
// stands for "Right", out of historical reasons.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/4/20.															     //
// Last modified: 2018/5/24, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAPDCR.h"
#include "TADetectorPara.h"
#include "TAPopMsg.h"

TAPDCR::TAPDCR(const string &name, const string &title, unsigned uid)
		: TAMWDCTa(name, title, uid){
}
TAPDCR::~TAPDCR(){}

void TAPDCR::GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const{
	const int type = dcType, l = layerOption - 1;
	const int nAnoPL = GetNAnodePerLayer();
	if(0 != l && 1 != l)
		TAPopMsg::Error(GetName().c_str(), "GetAnodeCenterPosition: Invalid layerOption (only 1 or 2 is permitted): %d", layerOption);
	const int n = anodeId % nAnoPL;
	const double x0 = 15. - 10. * nAnoPL; // x of the first anode (x0 is the minimum). -65.
	double p_local[3]{}; // p_local[1] is zero (y, the height), which is about the neutral beam's
	if(TAMWDC::kX == type){
		p_local[0] = x0 + 20. * n - 10. * l; // X X(l+1)
		p_local[1] = 0.; // y
		p_local[2] = 10. * l; // z
	}
	if(TAMWDC::kY == type){
		p_local[0] = 0.; // x
		p_local[1] = x0 + 20. * n - 10. * l; // Y Y(l+1)
		p_local[2] = 10. * l + 20.; // z
	}
	GetDetPara()->GetGlobalPosition(p_local, Ag);
} // end of member function GetAnodeCenterPosition

void TAPDCR::Configure(){
	TAMWDCTa::Configure();
	// there are only 7 anodes in one layer, but let's just make it 8
	fNAnodePerLayer = 8;
}




