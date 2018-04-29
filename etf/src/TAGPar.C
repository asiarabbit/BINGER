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
// Last modified: 2018/4/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>

// ROOT include
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"

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
	for(TObject *&p : fAgentVec) if(p){
		delete p; p = nullptr;
	}
}
void TAGPar::Write(){
	for(TObject *&p : fAgentVec) if(p){
		p->Write("", TObject::kOverwrite);
	}
}

double TAGPar::Val(unsigned int id) const{
	return Par(id)->GetValue();
}
void TAGPar::ShowPar(unsigned int id) const{
	cout << "ParName: " << Par(id)->GetName();
	cout << "\tValue: " << Val(id) << endl;
}
TAParameter *TAGPar::Parameter(unsigned int id) const{
	if(id >= kParSIZE) TAPopMsg::Error("TAGPar", "Par: input id exceeds the maximum parameter array size. M_SIZE: %d", kParSIZE);
	if(!fParVec[id]) TAPopMsg::Error("TAGPar", "Par: required pointer is null, id: %d", id);
	return fParVec[id];
}
const TAParameter *TAGPar::Par(unsigned int id) const{
	return Parameter(id);
}

short TAGPar::GetNParameter() const{
	short n = 0;
	for(TAParameter *p : fParVec) if(p) n++;
	return n;
}

TObject *TAGPar::Agent(unsigned int id) const{
	if(id >= kAgentSIZE) TAPopMsg::Error("TAGPar", "Agent: input id exceeds the maximum agent array size. M_SIZE: %d", kAgentSIZE);
	if(!fAgentVec[id]) TAPopMsg::Error("TAGPar", "Agent: required pointer is null, id: %d", id);
	return fAgentVec[id];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// the instructor
TAGPar::TAGPar() : fParVec{0}, fHasRead(false){
	DefineParameters();
	DefineAgents();
}
void TAGPar::DefineParameters(){
	// XXX: NOTE THAT values set here would be overwritten by the values given in confi fils
	TAParameter *p = nullptr; // a temporary variable
	// $$$$$ time to trigger range $$$$$ //
	// ---- PARAMETER 0 --- //
	p = new TAParameter("T0_1ToTrigLBUV", "T0_1ToTrigLBUV");
	p->SetValue(450.); fParVec[0] = p; p = nullptr;
	// ---- PARAMETER 1 --- //
	p = new TAParameter("T0_1ToTrigHBUV", "T0_1ToTrigHBUV");
	p->SetValue(620.); fParVec[1] = p; p = nullptr;
	// ---- PARAMETER 2 --- //
	p = new TAParameter("T0_1ToTrigLBDV", "T0_1ToTrigLBDV");
	p->SetValue(450.); fParVec[2] = p; p = nullptr;
	// ---- PARAMETER 3 --- //
	p = new TAParameter("T0_1ToTrigHBDV", "T0_1ToTrigHBDV");
	p->SetValue(620.); fParVec[3] = p; p = nullptr;
	// ---- PARAMETER 4 --- //
	p = new TAParameter("T0_1DelayAvrg", "T0_1DelayAvrg");
	p->SetValue(0.); fParVec[4] = p; p = nullptr;
	// ---- PARAMETER 5 --- //
	p = new TAParameter("tofwToTrigLB", "tofwToTrigLB");
	p->SetValue(300.); fParVec[5] = p; p = nullptr;
	// ---- PARAMETER 6 --- //
	p = new TAParameter("tofwToTrigHB", "tofwToTrigHB");
	p->SetValue(700.); fParVec[6] = p; p = nullptr;
	// ---- PARAMETER 7 --- //
	p = new TAParameter("dcToTrigLB", "dcToTrigLB");
	p->SetValue(450.); fParVec[7] = p; p = nullptr;
	// ---- PARAMETER 8 --- //
	p = new TAParameter("dcToTrigHB", "dcToTrigHB");
	p->SetValue(920.); fParVec[8] = p; p = nullptr;
	// ---- PARAMETER 9 --- //
	p = new TAParameter("sipmPlaArrToTrigLB", "sipmPlaArrToTrigLB");
	p->SetValue(200.); fParVec[9] = p; p = nullptr;
	// ---- PARAMETER 10 --- //
	p = new TAParameter("sipmPlaArrToTrigHB", "sipmPlaArrToTrigHB");
	p->SetValue(750.); fParVec[10] = p; p = nullptr;
	// ---- PARAMETER 11 --- //
	p = new TAParameter("sipmPlaBarrToTrigLB", "sipmPlaBarrToTrigLB");
	p->SetValue(250.); fParVec[11] = p; p = nullptr;
	// ---- PARAMETER 12 --- //
	p = new TAParameter("sipmPlaBarrToTrigHB", "sipmPlaBarrToTrigHB");
	p->SetValue(700.); fParVec[12] = p; p = nullptr;
	// $$$$$ DC HV: 0,1,2,3,4: 900,1000,1300,1350,1500 $$$$$ //
	// $ DCL0-2 $ //
	// ---- PARAMETER 13 --- //
	p = new TAParameter("HVDCL0X", "HVDCL0X");
	p->SetValue(3); fParVec[13] = p; p = nullptr;
	// ---- PARAMETER 14 --- //
	p = new TAParameter("HVDCL0U", "HVDCL0U");
	p->SetValue(-1); fParVec[14] = p; p = nullptr;
	// ---- PARAMETER 15 --- //
	p = new TAParameter("HVDCL0V", "HVDCL0V");
	p->SetValue(3); fParVec[15] = p; p = nullptr;
	// ---- PARAMETER 16 --- //
	p = new TAParameter("HVDCL1X", "HVDCL1X");
	p->SetValue(2); fParVec[16] = p; p = nullptr;
	// ---- PARAMETER 17 --- //
	p = new TAParameter("HVDCL1U", "HVDCL1U");
	p->SetValue(2); fParVec[17] = p; p = nullptr;
	// ---- PARAMETER 18 --- //
	p = new TAParameter("HVDCL1V", "HVDCL1V");
	p->SetValue(2); fParVec[18] = p; p = nullptr;
	// ---- PARAMETER 19 --- //
	p = new TAParameter("HVDCL2X", "HVDCL2X");
	p->SetValue(4); fParVec[19] = p; p = nullptr;
	// ---- PARAMETER 20 --- //
	p = new TAParameter("HVDCL2U", "HVDCL2U");
	p->SetValue(4); fParVec[20] = p; p = nullptr;
	// ---- PARAMETER 21 --- //
	p = new TAParameter("HVDCL2V", "HVDCR2V");
	p->SetValue(4); fParVec[21] = p; p = nullptr;
	// $ DCR0-2 $ //
	// ---- PARAMETER 22 --- //
	p = new TAParameter("HVDCR0X", "HVDCR0X");
	p->SetValue(3); fParVec[22] = p; p = nullptr;
	// ---- PARAMETER 23 --- //
	p = new TAParameter("HVDCR0U", "HVDCR0U");
	p->SetValue(-1); fParVec[23] = p; p = nullptr;
	// ---- PARAMETER 24 --- //
	p = new TAParameter("HVDCR0V", "HVDCR0V");
	p->SetValue(3); fParVec[24] = p; p = nullptr;
	// ---- PARAMETER 25 --- //
	p = new TAParameter("HVDCR1X", "HVDCR1X");
	p->SetValue(3); fParVec[25] = p; p = nullptr;
	// ---- PARAMETER 26 --- //
	p = new TAParameter("HVDCR1U", "HVDCR1U");
	p->SetValue(3); fParVec[26] = p; p = nullptr;
	// ---- PARAMETER 27 --- //
	p = new TAParameter("HVDCR1V", "HVDCR1V");
	p->SetValue(3); fParVec[27] = p; p = nullptr;
	// ---- PARAMETER 28 --- //
	p = new TAParameter("HVDCR2X", "HVDCR2X");
	p->SetValue(3); fParVec[28] = p; p = nullptr;
	// ---- PARAMETER 29 --- //
	p = new TAParameter("HVDCR2U", "HVDCR2U");
	p->SetValue(3); fParVec[29] = p; p = nullptr;
	// ---- PARAMETER 30 --- //
	p = new TAParameter("HVDCR2V", "HVDCR2V");
	p->SetValue(3); fParVec[30] = p; p = nullptr;
	// $$$$$ Exp visualization Canvas Size (unit: mm) $$$$$ //
	// ---- PARAMETER 31 --- //
	p = new TAParameter("L-DownstreamMag", "L-DownstreamMag");
	p->SetValue(5000.); fParVec[31] = p; p = nullptr;
	// ---- PARAMETER 32 --- //
	p = new TAParameter("W-DownstreamMag", "W-DownstreamMag");
	p->SetValue(3000.); fParVec[32] = p; p = nullptr;
	// ---- PARAMETER 33 --- //
	p = new TAParameter("T0_1UV_delay", "T0_1UV_delay");
	p->SetValue(0.); fParVec[33] = p; p = nullptr;
	// $$$$$ T_tofDCToTOFWall
	// ---- PARAMETER 34 --- //
	p = new TAParameter("T_tofDC0toTOFWLX", "T_tofDC0toTOFWLX");
	p->SetValue(6.6); fParVec[34] = p; p = nullptr;
	// ---- PARAMETER 35 --- //
	p = new TAParameter("T_tofDC1toTOFWLX", "T_tofDC1toTOFWLX");
	p->SetValue(4.1); fParVec[35] = p; p = nullptr;
	// ---- PARAMETER 36 --- //
	p = new TAParameter("T_tofDC2toTOFWLX", "T_tofDC2toTOFWLX");
	p->SetValue(1.4); fParVec[36] = p; p = nullptr;
	// ---- PARAMETER 37 --- //
	p = new TAParameter("T_tofDC0toTOFWRX", "T_tofDC0toTOFWRX");
	p->SetValue(8.5); fParVec[37] = p; p = nullptr;
	// ---- PARAMETER 38 --- //
	p = new TAParameter("T_tofDC1toTOFWRX", "T_tofDC1toTOFWRX");
	p->SetValue(5.5); fParVec[38] = p; p = nullptr;
	// ---- PARAMETER 39 --- //
	p = new TAParameter("T_tofDC2toTOFWRX", "T_tofDC2toTOFWRX");
	p->SetValue(2.2); fParVec[39] = p; p = nullptr;
	// $$$$$$$$$$ Parameters owned by class TACtrlPara $$$$$$$$$$$$$$$ //
	// ---- PARAMETER 40 --- //
	p = new TAParameter("3DCoincideWindow", "3DCoincideWindow");
	p->SetValue(10.); fParVec[40] = p; p = nullptr;
	// ---- PARAMETER 41 --- //
	p = new TAParameter("D2Thre", "D2Thre");
	p->SetValue(17.); fParVec[41] = p; p = nullptr;
	// ---- PARAMETER 42 --- //
	p = new TAParameter("TimeThreLB", "TimeThreLB");
	p->SetValue(-40.); fParVec[42] = p; p = nullptr;
	// ---- PARAMETER 43 --- //
	p = new TAParameter("TimeThreHB", "TimeThreHB");
	p->SetValue(350.); fParVec[43] = p; p = nullptr;
	// ---- PARAMETER 44 --- //
	p = new TAParameter("ChiThrePD", "ChiThrePD");
	p->SetValue(3.); fParVec[44] = p; p = nullptr;
	// ---- PARAMETER 45 --- //
	p = new TAParameter("Vicinity", "Vicinity");
	p->SetValue(2); fParVec[45] = p; p = nullptr;
	// ---- PARAMETER 46 --- //
	p = new TAParameter("StripTolerance", "StripTolerance");
	p->SetValue(1); fParVec[46] = p; p = nullptr;
	// ---- PARAMETER 47 --- //
	p = new TAParameter("FitMethod", "FitMethod");
	p->SetValue(1); fParVec[47] = p; p = nullptr;
	// ---- PARAMETER 48 --- //
	p = new TAParameter("Precision", "Precision");
	p->SetValue(2); fParVec[48] = p; p = nullptr;
	// ---- PARAMETER 49 --- //
	p = new TAParameter("NStripStrayMinL", "NStripStrayMinL");
	p->SetValue(-0.8); fParVec[49] = p; p = nullptr;
	// ---- PARAMETER 50 --- //
	p = new TAParameter("NStripStrayMaxL", "NStripStrayMaxL");
	p->SetValue(0.8); fParVec[50] = p; p = nullptr;
	// ---- PARAMETER 51 --- //
	p = new TAParameter("NStripStrayMinR", "NStripStrayMinR");
	p->SetValue(-0.8); fParVec[51] = p; p = nullptr;
	// ---- PARAMETER 52 --- //
	p = new TAParameter("NStripStrayMaxR", "NStripStrayMaxR");
	p->SetValue(0.8); fParVec[52] = p; p = nullptr;

	// ---- PARAMETER 53 --- //
	p = new TAParameter("T0_0Delay", "T0_0Delay");
	p->SetValue(-853.716); fParVec[53] = p; p = nullptr;
	// ---- PARAMETER 54 --- //
	p = new TAParameter("dcTOTNoiseLevel", "dcTOTNoiseLevel");
	p->SetValue(250.); fParVec[54] = p; p = nullptr; // this value is for beam test

	//////////////////// for DCs around the target ////////////////////////////////
	// DCs made by P. Ma //
	// tof -> DCArr U-D
	// ---- PARAMETER 55 --- //
	p = new TAParameter("T_tofDC0toTOFWUX", "T_tofDC0toTOFWUX");
	p->SetValue(0.); fParVec[55] = p; p = nullptr;
	// ---- PARAMETER 56 --- //
	p = new TAParameter("T_tofDC1toTOFWUX", "T_tofDC1toTOFWUX");
	p->SetValue(0.); fParVec[56] = p; p = nullptr;
	// ---- PARAMETER 57 --- //
	p = new TAParameter("T_tofDC0toTOFWDX", "T_tofDC0toTOFWDX");
	p->SetValue(0.); fParVec[57] = p; p = nullptr;
	// ---- PARAMETER 58 --- //
	p = new TAParameter("T_tofDC1toTOFWDX", "T_tofDC1toTOFWDX");
	p->SetValue(0.); fParVec[58] = p; p = nullptr;

	// $$$$$ DC HV: 0,1,2,3,4: 900,1000,1300,1350,1500 $$$$$ //
	// $ DCU0-1 $ //
	// ---- PARAMETER 59 --- //
	p = new TAParameter("HVDCU0X", "HVDCU0X");
	p->SetValue(3); fParVec[59] = p; p = nullptr;
	// ---- PARAMETER 60 --- //
	p = new TAParameter("HVDCU0Y", "HVDCU0Y");
	p->SetValue(3); fParVec[60] = p; p = nullptr;
	// ---- PARAMETER 61 --- //
	p = new TAParameter("HVDCU1X", "HVDCU1X");
	p->SetValue(3); fParVec[61] = p; p = nullptr;
	// ---- PARAMETER 62 --- //
	p = new TAParameter("HVDCU1Y", "HVDCU1Y");
	p->SetValue(3); fParVec[62] = p; p = nullptr;
	// $ DCD0-1 $ //
	// ---- PARAMETER 63 --- //
	p = new TAParameter("HVDCD0X", "HVDCD0X");
	p->SetValue(3); fParVec[63] = p; p = nullptr;
	// ---- PARAMETER 64 --- //
	p = new TAParameter("HVDCD0Y", "HVDCD0Y");
	p->SetValue(3); fParVec[64] = p; p = nullptr;
	// ---- PARAMETER 65 --- //
	p = new TAParameter("HVDCD1X", "HVDCD1X");
	p->SetValue(3); fParVec[65] = p; p = nullptr;
	// ---- PARAMETER 66 --- //
	p = new TAParameter("HVDCD1Y", "HVDCD1Y");
	p->SetValue(3); fParVec[66] = p; p = nullptr;

	// $$$$$ time to trigger range - for DCs around the target $$$$$ //
	// ---- PARAMETER 67 --- //
	p = new TAParameter("dcTaToTrigLB", "dcTaToTrigLB");
	p->SetValue(450.); fParVec[67] = p; p = nullptr;
	// ---- PARAMETER 68 --- //
	p = new TAParameter("dcTaToTrigHB", "dcTaToTrigHB");
	p->SetValue(920.); fParVec[68] = p; p = nullptr;

	// --------------------------------------------------------- //
	// DCs from BUAA, made in Japan //
	// tof -> DCArr U-D
	// ---- PARAMETER 69 --- //
	p = new TAParameter("T_tofPDC0toTOFWUX", "T_tofPDC0toTOFWUX");
	p->SetValue(0.); fParVec[69] = p; p = nullptr;
	// ---- PARAMETER 70 --- //
	p = new TAParameter("T_tofPDC1toTOFWUX", "T_tofPDC1toTOFWUX");
	p->SetValue(0.); fParVec[70] = p; p = nullptr;
	// ---- PARAMETER 71 --- //
	p = new TAParameter("T_tofPDC0toTOFWDX", "T_tofPDC0toTOFWDX");
	p->SetValue(0.); fParVec[71] = p; p = nullptr;
	// ---- PARAMETER 72 --- //
	p = new TAParameter("T_tofPDC1toTOFWDX", "T_tofPDC1toTOFWDX");
	p->SetValue(0.); fParVec[72] = p; p = nullptr;

	// $$$$$ DC HV: 0,1,2,3,4: 900,1000,1300,1350,1500 $$$$$ //
	// $ DCU0-1 $ //
	// ---- PARAMETER 73 --- //
	p = new TAParameter("HVPDCU0X", "HVPDCU0X");
	p->SetValue(3); fParVec[73] = p; p = nullptr;
	// ---- PARAMETER 74 --- //
	p = new TAParameter("HVPDCU0Y", "HVPDCU0Y");
	p->SetValue(3); fParVec[74] = p; p = nullptr;
	// ---- PARAMETER 75 --- //
	p = new TAParameter("HVPDCU1X", "HVPDCU1X");
	p->SetValue(3); fParVec[75] = p; p = nullptr;
	// ---- PARAMETER 76 --- //
	p = new TAParameter("HVPDCU1Y", "HVPDCU1Y");
	p->SetValue(3); fParVec[76] = p; p = nullptr;
	// $ DCD0-1 $ //
	// ---- PARAMETER 77 --- //
	p = new TAParameter("HVPDCD0X", "HVPDCD0X");
	p->SetValue(3); fParVec[77] = p; p = nullptr;
	// ---- PARAMETER 78 --- //
	p = new TAParameter("HVPDCD0Y", "HVPDCD0Y");
	p->SetValue(3); fParVec[78] = p; p = nullptr;
	// ---- PARAMETER 79 --- //
	p = new TAParameter("HVPDCD1X", "HVPDCD1X");
	p->SetValue(3); fParVec[79] = p; p = nullptr;
	// ---- PARAMETER 80 --- //
	p = new TAParameter("HVPDCD1Y", "HVPDCD1Y");
	p->SetValue(3); fParVec[80] = p; p = nullptr;

	// $$$$$ time to trigger range - for DCs around the target $$$$$ //
	// ---- PARAMETER 81 --- //
	p = new TAParameter("pdcToTrigLB", "pdcToTrigLB");
	p->SetValue(450.); fParVec[81] = p; p = nullptr;
	// ---- PARAMETER 82 --- //
	p = new TAParameter("pdcToTrigHB", "pdcToTrigHB");
	p->SetValue(920.); fParVec[82] = p; p = nullptr;
	
	// ---- PARAMETER 83 --- //
	p = new TAParameter("usingPDC", "usingPDC"); // this is a boolean
	p->SetValue(0); fParVec[83] = p; p = nullptr;

} // end of member function DefineParameters

void TAGPar::DefineAgents(){
	TObject *p = nullptr;
	p = new TH1F("hUprojCnt", "Uproj Count Entering TrackerMeger", 13, -1.5, 11.5);
	fAgentVec[0] = p; p = nullptr;
	p = new TH1F("hVprojCnt", "Vproj Count Entering TrackerMeger", 13, -1.5, 11.5);
	fAgentVec[1] = p; p = nullptr;
}





