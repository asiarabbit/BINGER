///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/TABUAA.C																	 //
//   TABUAA.C -- source file for class TABUAA										 //
//   Introduction: This class is written for data analysis acquired from detector	 //
// clusters installed upstream the dipole magnet along beamline, which is an integral//
// part for ETF system to complete particle identification. So it is incorporated in //
// the Detector system.																 //
// *** BUAA: Beijing University of Aeronautics and Astronautics ***					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2016/10/15, transported: 2018/1/9.									     //
// Last modified: 2018/1/9, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h> // for access function
#include <ctime>

// ROOT includes
#include "TFile.h"
#include "TTree.h"

#include "readVME.h"
#include "TABUAA.h"
#include "TAMath.h"
#include "TAPopMsg.h"

using std::cout;
using std::endl;
using std::flush;

constexpr double Pi = TAMath::Pi();
constexpr double DEGREE = TAMath::DEGREE();
inline double rand0_5(){ return rand()*1./RAND_MAX; } // bin smoothing

TABUAA::TABUAA(const string &datafile, const string &rootfile)
	 : fDatafile(datafile), fROOTFileName(rootfile){
	fROOTFile = NULL;
	fVME = NULL;
	Initialize();
} // end of the default constructor.
TABUAA::~TABUAA(){} // the destructor.
#include "TABUAA/read.cxx" // definition of TABUAA::ReadOffline()
#include "TABUAA/VMEAnalyze.cxx" // read one block

void TABUAA::Assign(int entryId){ // assign member variables.
	Initialize();
	if(fROOTFile == NULL){
		string vmerootfile = fDatafile + "_vme.root";
		if (access(vmerootfile.c_str(), 0) == -1){ // file does not exist.
			cout << "File " << vmerootfile << " cannot be opened." << endl;
			getchar();
			exit(EXIT_FAILURE);
		} // end if
		fROOTFile = new TFile(vmerootfile.c_str(), "update");
	} // end if
	else if(fVME == NULL){
		if(!fROOTFile->FindObjectAny("vme")){
			cout << "\033[31;1mTObject vme doesn't exist. Exiting...\033[0m" << endl;
			getchar();
			exit(1);
		} // end if
		fVME = (TTree*)fROOTFile->Get("vme");
		fVME->SetBranchAddress("adc", fEvent.adc);
		fVME->SetBranchAddress("tdc0", fEvent.tdc[0]);
		fVME->SetBranchAddress("tdc1", fEvent.tdc[1]);
		fVME->SetBranchAddress("qdc0", fEvent.qdc[0]);
		fVME->SetBranchAddress("qdc1", fEvent.qdc[1]);
		fVME->SetBranchAddress("qdc2", fEvent.qdc[2]);
		fVME->SetBranchAddress("hit", fEvent.hit);
		fVME->SetBranchAddress("sca", fEvent.sca);
		fVME->SetBranchAddress("dsca", fEvent.dsca);
	} // end if
	if(entryId < 0 || entryId >= fVME->GetEntries()){
		cout << "TABUAA::Assign: \033[31;1mentryId out of range!\033[0m" << endl;
		getchar();
		return;
	} // end if
	fVME->GetEntry(entryId);
	fIsAssigned = true;
} // end of function Assign
void TABUAA::Analyze(){
	srand(time(0));
	if(!fIsAssigned){
		cout << "TABUAA::Analyze: fEvent is not assigned." << endl;
		getchar();
		return;
	} // end if
	int cnt = 0; // temporary variables.
	double deTemp; // temporary variables to store energy deposit.

	// calibration coefficients and offsets
	double coe_tac[3] = {-1.026e-2, -1.054e-2, -1.000e-2}; // ch->ns
	double tof1ofst = 159.3; // offset, unit: ns.
	fTOF1VME = ((fEvent.adc[0] + rand0_5()) * coe_tac[0] + (fEvent.adc[2] + rand0_5()) * coe_tac[1]) / 2. + tof1ofst;
//	fTOF1VME = (fEvent.adc[0] * coe_tac[0] + fEvent.adc[2] * coe_tac[1]) / 2. + tof1ofst;

//	if(fTOF1PXI == -9999.){
//		cout << "TABUAA::Analyze(): fTOF1PXI has not been assigned." << endl;
//		getchar();
//	} // end if
//	fTOF1PXI = fTOF1VME;
	double L1 = 25.88 * 1000.; // from TOF_start to T0_0, unit: mm
	double beta1 = 0.; // of the incident particle.
	beta1 = L1 / fTOF1VME / 300.; // 300.: velocity of light, mm/ns
	fBeta1 = beta1;
	double gamma1 = 1./sqrt(1-beta1*beta1);
	double brho = 4.;
	double coe_aoq[2] = {0.06, 1.0476}; // calibrated by 16O 11141619
	fAoQ1 = coe_aoq[0] + coe_aoq[1]*(brho*0.3/beta1/gamma1);

	//////////////////----------- assign energy deposit -------------//////////////////
	double ofst_adc[6] = {-51, +21, +20, +30, -30, +12};
	double gain_adc[6] = {1.061, 0.991, 0.967, 0.989, 1.059, 0.954};
	double coeZIn = 0.5, ofstZIn = -0.36;
	double coeZOut = 0.42, ofstZOut = 2.29;
	double dENoiseLevel = 216.; // channel
	if(fEvent.adc[31] < 1500.){ // no pulses pile up
		////// assign fDEBeforeTarget. ////// Music1
		fDEBeforeTarget = 0.; cnt = 0;
		for(int i = 0; i < 4; i++){ // chid: 21~24
			if(fEvent.adc[21 + i] > dENoiseLevel){
				deTemp = fEvent.adc[21 + i] + rand0_5();
//				cout << "i: " << i << endl; // DEBUG
//				cout << "deTemp: " << deTemp << endl; // DEBUG
//				getchar(); // DEBUG
				fDEBeforeTarget += deTemp;
				cnt++;
			} // end if
		} // end for over i
		fDEBeforeTarget /= cnt; // for the average.
		fZIn = coeZIn * pow(beta1, 0.8) * sqrt(fDEBeforeTarget) + ofstZIn;
		double AIn = fZIn * fAoQ1 + 5.;
		fAIn = 0.717425 * AIn + 17.1869; // calibration. Nov. 27, 2016. SUN Yazhou.
//		cout << "fDEBeforeTarget: " << fDEBeforeTarget << endl; // DEBUG
//		cout << "fZIn: " << fZIn << endl; // DEBUG
//		getchar(); // DEBUG
		////// assign fDEAfterTarget. ////// Music2
		fDEAfterTarget = 0.; cnt = 0;
		for(int i = 0; i < 6; i++){ // chid: 25~30
			if(fEvent.adc[25 + i] > dENoiseLevel){
				deTemp = gain_adc[i] * (fEvent.adc[25 + i] + rand0_5()) + ofst_adc[i];
//				cout << "i: " << i << endl; // DEBUG
//				cout << "deTemp: " << deTemp << endl; // DEBUG
//				getchar(); // DEBUG
				fDEAfterTarget += deTemp;
				cnt++;
			} // end if
		} // end for over i
		fDEAfterTarget /= cnt; // for the average.
		fZOut = coeZOut * pow(beta1, 0.8) * sqrt(fDEAfterTarget) + ofstZOut;
//		cout << "fDEAfterTarget: " << fDEAfterTarget << endl; // DEBUG
//		cout << "fZOut: " << fZOut << endl; // DEBUG
//		getchar(); // DEBUG

	} // end if not pile up
	else{
//		cout << "fEvent.adc[31]: " << fEvent.adc[31] << endl; // DEBUG
//		getchar(); // DEBUG
		fIsPileUp = true;
	} // end else



	//////////////////------------- assign particle position -------------//////////////////
	// standing beside the target, facing dipole magnet, +x is right, +y is up.
	// x0 = (t1 - t2) * v / 2.; unit: mm
	double X1MWPC[3], Y1MWPC[3], X2MWPC[3], Y2MWPC[3]; // MWPC0, 1, 2
	const double coeXMWPC[3] = {+3.550e-2, +3.368e-2, -3.944e-2}; // MWPC0, 1, 2, ch->mm
	const double coeYMWPC[3] = {-3.750e-2, -3.420e-2, +3.679e-2}; // MWPC0, 1, 2, ch->mm
	const double ofstXMWPC[3] = {4.642, 0.816, 0.982}; // offset of MWPC0, 1, 2. unit: mm.
	const double ofstYMWPC[3] = {5.277, 4.122, 3.188}; // offset of MWPC1, 1, 2. unit: mm.
	// {-740,-200,+160} unit: mm
	// position of the MWPCs along the beam line, relative to the target.
	const double off_mwz[3] = {-710.3, -169.91, 134.68};
	// MWPC1, in front of the target 
	X1MWPC[0] = fEvent.tdc[0][16]; X2MWPC[0] = fEvent.tdc[0][17];
	Y1MWPC[0] = fEvent.tdc[0][18]; Y2MWPC[0] = fEvent.tdc[0][19];
	fX1 = coeXMWPC[0] * (X2MWPC[0] - X1MWPC[0] + rand0_5()) + ofstXMWPC[0];
	fY1 = coeYMWPC[0] * (Y2MWPC[0] - Y1MWPC[0] + rand0_5()) + ofstYMWPC[0];
	double x1 = fX1, y1 = fY1;
	double z1 = off_mwz[0]; // -740.9 + 27.5; // 30. - 2.5; // 649.60 - 1389.90.
	// MWPC2, in front of the target 
	X1MWPC[1] = fEvent.tdc[0][22]; X2MWPC[1] = fEvent.tdc[0][23];
	Y1MWPC[1] = fEvent.tdc[0][24]; Y2MWPC[1] = fEvent.tdc[0][25];
	fX2 = coeXMWPC[1] * (X2MWPC[1] - X1MWPC[1] + rand0_5()) + ofstXMWPC[1];
	fY2 = coeYMWPC[1] * (Y2MWPC[1] - Y1MWPC[1] + rand0_5()) + ofstYMWPC[1];
	double x2 = fX2, y2 = fY2;
	double z2 = off_mwz[1]; // -199.91 + 27.5; // 30. - 2.5; // 1189.99 - 1389.90;
	// MWPC3, behind the target 
	X1MWPC[2] = fEvent.tdc[0][28]; X2MWPC[2] = fEvent.tdc[0][29];
	Y1MWPC[2] = fEvent.tdc[0][30]; Y2MWPC[2] = fEvent.tdc[0][31];
	fX3 = coeXMWPC[2] * (X2MWPC[2] - X1MWPC[2] + rand0_5()) + ofstXMWPC[2];
	fY3 = coeYMWPC[2] * (Y2MWPC[2] - Y1MWPC[2] + rand0_5()) + ofstYMWPC[2];
	double x3 = fX3, y3 = fY3;
	double z3 = off_mwz[2]; // 160 + 27.5; // 104.68 + 27.5; // 30. - 2.5; // 160.; // 104.68; // 1494.58 - 1389.90;
	double x0 = 0., y0 = 0., z0 = 0.; // target position
	x0 = (x1*z2-x2*z1)/(z2-z1); // target X coordinate
	y0 = (y1*z2-y2*z1)/(z2-z1); // target Y coordinate
	fThetaIn = atan((x2-x1)/(z2-z1));
	fThetaOut = atan((x3-x0)/(z3-z0));
	fPhiIn = atan((y2-y1)/(z2-z1));
	fPhiOut = atan((y3-y0)/(z3-z0));
	fTaX = x0; fTaY = y0;

	// reverse x direction to accommodate Detector code project.
	fX1 *= -1.; fX2 *= -1.; fX3 *= -1.; fTaX *= -1.; fThetaIn *= -1.; fThetaOut *= -1.;

	// to tell if the MWPCs miss the particle
	for(int i = 0; i < 3; i++){
//		cout << "X1MWPC["<< i << "]: " << X1MWPC[i] << "\tX2MWPC["<< i << "]: " << X2MWPC[i] << endl; // DEBUG
//		cout << "Y1MWPC["<< i << "]: " << Y1MWPC[i] << "\tY2MWPC["<< i << "]: " << Y2MWPC[i] << endl; // DEBUG
		if(X1MWPC[i] < 0 || X2MWPC[i] < 0 || Y1MWPC[i] < 0 || Y2MWPC[i] < 0){ // loss by efficiency
			fIsMWPCMiss[i] = true;
		} // end if
	} // end for over i // DEBUG

//	cout << "z1: " << z1 << "\tx1: " << x1 << "\ty1: " << y1 << endl; // DEBUG
//	cout << "z2: " << z2 << "\tx2: " << x2 << "\ty2: " << y2 << endl; // DEBUG
//	cout << "z0: " << z0 << "\tx0: " << x0 << endl; // DEBUG
//	cout << "z3: " << z3 << "\tx3: " << x3 << "\ty3: " << y3 << endl; // DEBUG

//	cout << "fTaX: " << fTaX << endl; // DEBUG
//	cout << "fTaY: " << fTaY << endl; // DEBUG
//	cout << "fThetaIn: " << fThetaIn / DEGREE << endl; // DEBUG
//	cout << "fThetaOut: " << fThetaOut / DEGREE << endl; // DEBUG
//	getchar(); // DEBUG


	//////////////////------------- assign TOF information -------------//////////////////
	double VETO01L = fEvent.tdc[0][0] + rand0_5(), VETO01R = fEvent.tdc[0][1] + rand0_5();
	double VETO02L = fEvent.tdc[0][2] + rand0_5(), VETO02R = fEvent.tdc[0][3] + rand0_5();
//	double PL2L = fEvent.tdc[0][4], PL2R = fEvent.tdc[0][5];
//	if(VETO01L < 0 && VETO01R < 0){ // DEBUG
//		cout << "VETO01L: " << VETO01L << "\tVETO01R: " << VETO01R << endl; // DEBUG
//		cout << "BINGO!\n"; getchar(); // DEBUG
//	} // end if
//	if((VETO01L >= 0. && VETO01R >= 0.) || (VETO02L >= 0. && VETO02R >= 0.)){ // DEBUG
//		cout << "Mark 2\n"; getchar();
//		cout << "VETO01L: " << VETO01L << "\tVETO01R: " << VETO01R << endl; // DEBUG
//		cout << "VETO02L: " << VETO02L << "\tVETO02R: " << VETO02R << endl; // DEBUG
//		getchar(); // DEBUG
//	} // end if // DEBUG
	if(VETO01L > 0. && VETO01R > 0.) fIsVeto[0] = true;
	if(VETO02L > 0. && VETO02R > 0.) fIsVeto[1] = true;

	fIsAnalyzed = true;
} // end of function Analyze
// Energy deposit information recorded by Music
double TABUAA::GetDEBeforeTarget() const{ return fDEBeforeTarget; }
double TABUAA::GetDEAfterTarget() const{ return fDEAfterTarget; }
double TABUAA::GetZIn() const{ return fZIn; }
double TABUAA::GetAIn() const{ return fAIn; }
double TABUAA::GetAoQ1() const{ return fAoQ1; }
double TABUAA::GetZOut() const{ return fZOut; }
// position information recorded by MWPCs
double TABUAA::GetX1() const{ return fX1; }
double TABUAA::GetY1() const{ return fY1; }
double TABUAA::GetX2() const{ return fX2; }
double TABUAA::GetY2() const{ return fY2; }
double TABUAA::GetX3() const{ return fX3; }
double TABUAA::GetY3() const{ return fY3; }
double TABUAA::GetTaX() const{ return fTaX; } // X of the target
double TABUAA::GetTaY() const{ return fTaY; } // Y of the target
// For angles here, anti-clockwise is positive.
double TABUAA::GetThetaIn() const{ return fThetaIn; }
double TABUAA::GetThetaOut() const{ return fThetaOut; }
double TABUAA::GetPhiIn() const{ return fPhiIn; }
double TABUAA::GetPhiOut() const{ return fPhiOut; }
// time tag and time of flight information recorded by TOF_start and TOF_stop
// time tag recorded by TOF_start, TOF1 from PXI
double TABUAA::GetTOF1PXI() const{
	if(fTOF1PXI == -9999.){
		cout << "TABUAA::GetTOF1(): fTOF1(from PXI) has not been assigned." << endl;
		getchar();
	} // end if
	return fTOF1PXI;
} // end of function GetTOF1.
double TABUAA::GetTOF1VME() const{
	if(fTOF1VME == -9999.){
		cout << "TABUAA::GetTOF1VME(): fTOF1VME(from VME) has not been assigned." << endl;
		getchar();
	} // end if
	return fTOF1VME;
} // end of function GetTOF1.
double TABUAA::GetBetaIn() const{ return fBeta1; }
bool TABUAA::IsPileUp() const{
	if(!fIsAnalyzed) {
		cout << "TABUAA::IsPileUp(): Data not analyzed or assigned." << endl;
		getchar();
	} // end else
	return fIsPileUp;
} // end function IsPileUp
bool TABUAA::IsVeto(int id) const{
	if(!fIsAnalyzed){
		cout << "TABUAA::IsVeto(): Data not analyzed or assigned." << endl;
		getchar();
	} // end else
	return fIsVeto[id%2];
} // end function IsVeto

const tVME_event &TABUAA::GetEvent() const{ return fEvent; }
TTree *TABUAA::GetTreeVME(){
	if(fROOTFile == NULL){
		string vmerootfile = fDatafile + "_vme.root";
		if (access(vmerootfile.c_str(), 0) == -1){ // file does not exist.
			cout << "File " << vmerootfile << " cannot be opened." << endl;
			getchar();
			exit(EXIT_FAILURE);
		} // end if
		fROOTFile = new TFile(vmerootfile.c_str(), "update");
	} // end if
	else if(fVME == NULL){
		if(!fROOTFile->FindObjectAny("vme")){
			cout << "\033[31;1mTObject vme doesn't exist. Exiting...\033[0m" << endl;
			getchar();
			exit(1);
		} // end if
		fVME = (TTree*)fROOTFile->Get("vme");
		fVME->SetBranchAddress("adc", fEvent.adc);
		fVME->SetBranchAddress("tdc0", fEvent.tdc[0]);
		fVME->SetBranchAddress("tdc1", fEvent.tdc[1]);
		fVME->SetBranchAddress("qdc0", fEvent.qdc[0]);
		fVME->SetBranchAddress("qdc1", fEvent.qdc[1]);
		fVME->SetBranchAddress("qdc2", fEvent.qdc[2]);
		fVME->SetBranchAddress("hit", fEvent.hit);
		fVME->SetBranchAddress("sca", fEvent.sca);
		fVME->SetBranchAddress("dsca", fEvent.dsca);
	} // end if
	return fVME;
} // end of function GetTreeVME
double TABUAA::GetQDC(int i, int j) const{
	const double qdc = fEvent.qdc[abs(i%3)][abs(j%32)];
	if(-9999. == qdc) return -9999.;
	return qdc + rand0_5();
} // end of the member function

void TABUAA::SetTOF1PXI(double tof1pxi){ fTOF1PXI = tof1pxi; }
void TABUAA::SetROOTFile(TFile *f){ fROOTFile = f; }


void TABUAA::Initialize(){
	fEvent.initialize();
	fIsAssigned = false; fIsAnalyzed = false;
	fIsPileUp = false; fIsVeto[0] = false; fIsVeto[1] = false;
	for(bool &o : fIsMWPCMiss) o = false;
	fBlockIndex = -1;
	fDEBeforeTarget = -9999.; fDEAfterTarget = -9999.;
	fZIn = -1.; fZOut = -1.; fAIn = -1.; fAoQ1 = -1.;
	fX1 = -9999.; fY1 = -9999.;
	fX2 = -9999.; fY2 = -9999.;
	fX3 = -9999.; fY3 = -9999.;
	fTaX = -9999.; fTaY = -9999.;
	fThetaIn = -9999.; fThetaOut = -9999.;
	fPhiIn = -9999.; fPhiOut = -9999.;
	fTOF1PXI = -9999.; fTOF1VME = -9999.; fBeta1 = -1.;
} // end of function Initialize.



