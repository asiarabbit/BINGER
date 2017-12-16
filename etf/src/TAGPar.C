///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAGPar.C															 //
//   TAGPar.C -- source file for class TAGPar										 //
//   Introduction: class to store global parameters. Each parameter would be a		 //
// TAParameter object.																 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/12/13.															     //
// Last modified: 2017/12/12, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>

#include "TAGPar.h"
#include "TAParameter.h"
#include "TAPopMsg.h"

using std::cout;
using std::endl;

TAGPar *TAGPar::fInstance = nullptr;

TAGPar *TAGPar::Instance(){
	if(!fInstance) fInstance = new TAGPar();
	return fInstance;
}
TAGPar::~TAGPar(){
	for(TAParameter *&p : fParVec) if(p){
		delete p; p = nullptr;
	}
}

double TAGPar::Val(unsigned int id) const{
	return Par(id)->GetValue();
}
void TAGPar::ShowPar(unsigned int id) const{
	cout << "ParName: " << Par(id)->GetName();
	cout << "\tValue: " << Val(id) << endl;
	getchar();
}
const TAParameter *TAGPar::Par(unsigned int id) const{
	if(id >= kSIZE) TAPopMsg::Error("TAGPar", "Par: input id exceeds the maximum parameter array size. M_SIZE: %d", kSIZE);
	if(!fParVec[id]) TAPopMsg::Error("TAGPar", "Par: required pointer is null");
	return fParVec[id];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// the instructor
TAGPar::TAGPar() : fParVec{0}{
	TAParameter *p = nullptr; // a temporary variable
	// $$$$$ time to trigger range $$$$$ //
	// ---- PARAMETER 0 --- //
	p = new TAParameter("T0_1ToTrigLBUV", "T0_1ToTrigLBUV");
	p->SetValue(1350.); fParVec[0] = p; p = nullptr;
	// ---- PARAMETER 1 --- //
	p = new TAParameter("T0_1ToTrigHBUV", "T0_1ToTrigHBUV");
	p->SetValue(1500.); fParVec[1] = p; p = nullptr;
	// ---- PARAMETER 2 --- //
	p = new TAParameter("T0_1ToTrigLBDV", "T0_1ToTrigLBDV");
	p->SetValue(1350.); fParVec[2] = p; p = nullptr;
	// ---- PARAMETER 3 --- //
	p = new TAParameter("T0_1ToTrigHBDV", "T0_1ToTrigHBDV");
	p->SetValue(1500.); fParVec[3] = p; p = nullptr;
	// ---- PARAMETER 4 --- //
	p = new TAParameter("T0_1DelayAvrg", "T0_1DelayAvrg");
	p->SetValue(-343.7); fParVec[4] = p; p = nullptr;
	// ---- PARAMETER 5 --- //
	p = new TAParameter("tofwToTrigLB", "tofwToTrigLB");
	p->SetValue(1120.); fParVec[5] = p; p = nullptr;
	// ---- PARAMETER 6 --- //
	p = new TAParameter("tofwToTrigHB", "tofwToTrigHB");
	p->SetValue(1160.); fParVec[6] = p; p = nullptr;
	// ---- PARAMETER 7 --- //
	p = new TAParameter("dcToTrigLB", "dcToTrigLB");
	p->SetValue(1000.); fParVec[7] = p; p = nullptr;
	// ---- PARAMETER 8 --- //
	p = new TAParameter("dcToTrigHB", "dcToTrigHB");
	p->SetValue(1400.); fParVec[8] = p; p = nullptr;
	// ---- PARAMETER 9 --- //
	p = new TAParameter("sipmPlaArrToTrigLB", "sipmPlaArrToTrigLB");
	p->SetValue(200.); fParVec[9] = p; p = nullptr;
	// ---- PARAMETER 10 --- //
	p = new TAParameter("sipmPlaArrToTrigHB", "sipmPlaArrToTrigHB");
	p->SetValue(650.); fParVec[10] = p; p = nullptr;
	// ---- PARAMETER 11 --- //
	p = new TAParameter("sipmPlaBarrToTrigLB", "sipmPlaBarrToTrigLB");
	p->SetValue(200.); fParVec[11] = p; p = nullptr;
	// ---- PARAMETER 12 --- //
	p = new TAParameter("sipmPlaBarrToTrigHB", "sipmPlaBarrToTrigHB");
	p->SetValue(700.); fParVec[12] = p; p = nullptr;
	// $$$$$ DC HV: 0,1,2,3,4: 900,1000,1300,1350,1500 $$$$$ //
	// $ DCL0-2 $ //
	// ---- PARAMETER 13 --- //
	p = new TAParameter("HVDCL0X", "HVDCL0X");
	p->SetValue(0); fParVec[13] = p; p = nullptr;
	// ---- PARAMETER 14 --- //
	p = new TAParameter("HVDCL0U", "HVDCL0U");
	p->SetValue(0); fParVec[14] = p; p = nullptr;
	// ---- PARAMETER 15 --- //
	p = new TAParameter("HVDCL0V", "HVDCL0V");
	p->SetValue(0); fParVec[15] = p; p = nullptr;
	// ---- PARAMETER 16 --- //
	p = new TAParameter("HVDCL1X", "HVDCL1X");
	p->SetValue(0); fParVec[16] = p; p = nullptr;
	// ---- PARAMETER 17 --- //
	p = new TAParameter("HVDCL1U", "HVDCL1U");
	p->SetValue(0); fParVec[17] = p; p = nullptr;
	// ---- PARAMETER 18 --- //
	p = new TAParameter("HVDCL1V", "HVDCL1V");
	p->SetValue(0); fParVec[18] = p; p = nullptr;
	// ---- PARAMETER 19 --- //
	p = new TAParameter("HVDCL2X", "HVDCL2X");
	p->SetValue(0); fParVec[19] = p; p = nullptr;
	// ---- PARAMETER 20 --- //
	p = new TAParameter("HVDCL2U", "HVDCL2U");
	p->SetValue(0); fParVec[20] = p; p = nullptr;
	// ---- PARAMETER 21 --- //
	// $ DCR0-2 $ //
	p = new TAParameter("HVDCR2V", "HVDCR2V");
	p->SetValue(0); fParVec[21] = p; p = nullptr;
	// ---- PARAMETER 22 --- //
	p = new TAParameter("HVDCR0X", "HVDCR0X");
	p->SetValue(0); fParVec[22] = p; p = nullptr;
	// ---- PARAMETER 23 --- //
	p = new TAParameter("HVDCR0U", "HVDCR0U");
	p->SetValue(0); fParVec[23] = p; p = nullptr;
	// ---- PARAMETER 24 --- //
	p = new TAParameter("HVDCR0V", "HVDCR0V");
	p->SetValue(0); fParVec[24] = p; p = nullptr;
	// ---- PARAMETER 25 --- //
	p = new TAParameter("HVDCR1X", "HVDCR1X");
	p->SetValue(0); fParVec[25] = p; p = nullptr;
	// ---- PARAMETER 26 --- //
	p = new TAParameter("HVDCR1U", "HVDCR1U");
	p->SetValue(0); fParVec[26] = p; p = nullptr;
	// ---- PARAMETER 27 --- //
	p = new TAParameter("HVDCR1V", "HVDCR1V");
	p->SetValue(0); fParVec[27] = p; p = nullptr;
	// ---- PARAMETER 28 --- //
	p = new TAParameter("HVDCR2X", "HVDCR2X");
	p->SetValue(0); fParVec[28] = p; p = nullptr;
	// ---- PARAMETER 29 --- //
	p = new TAParameter("HVDCR2U", "HVDCR2U");
	p->SetValue(0); fParVec[29] = p; p = nullptr;
	// ---- PARAMETER 30 --- //
	p = new TAParameter("HVDCR2V", "HVDCR2V");
	p->SetValue(0); fParVec[30] = p; p = nullptr;
	// $$$$$ Exp visualization Canvas Size (unit: mm) $$$$$ //
	// ---- PARAMETER 31 --- //
	p = new TAParameter("L-DownstreamMag", "L-DownstreamMag");
	p->SetValue(10000.); fParVec[31] = p; p = nullptr;
	// ---- PARAMETER 32 --- //
	p = new TAParameter("W-DownstreamMag", "W-DownstreamMag");
	p->SetValue(3000.); fParVec[32] = p; p = nullptr;
	
}







