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
	if(id >= kSIZE) TAPopMsg::Error("TAGpar", "Par: input id exceeds the maximum parameter array size. M_SIZE: %d", kSIZE);
	return fParVec[id];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// the instructor
TAGPar::TAGPar() : fParVec{0}{
	TAParameter *p = nullptr; // a temporary variable
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

}







