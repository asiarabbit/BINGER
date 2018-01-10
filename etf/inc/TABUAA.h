///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/TABUAA.h																	 //
//   TABUAA.h -- header file for structs used in class TABUAA						 //
//   Introduction: This class is written for data analysis acquired from detector	 //
// clusters installed upstream the dipole magnet along beamline, which is an integral//
// part for ETF system to complete particle identification. So it is incorporated in //
// the Detector system.																 //
// *** BUAA: Beijing University of Aeronautics and Astronautics ***					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2016/10/15, transported: 2018/1/9.									     //
// Last modified: 2018/1/10, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __TABUAA_H__
#define __TABUAA_H__

#include <string>

using std::string;

class TFile;
class TTree;
struct tVME_event;

class TABUAA{
public:
	TABUAA(const string &datafile, const string &rootfile = ""); // the default constructor.
	~TABUAA(); // the destructor.

	void ReadOffline();
	void Assign(int entryId); // assign member variables.
	void Analyze(); // give physical result
	// Energy deposit information recorded by Music
	double GetDEBeforeTarget() const;
	double GetDEAfterTarget() const;
	double GetZIn() const; // return fZIn;
	double GetAIn() const; // return fZIn;
	double GetAoQ1() const; // return fAoQ1;
	double GetZOut() const; // return fZOut;
	// position information recorded by MWPC
	double GetX1() const; // from MWPC1, before target.
	double GetY1() const; // from MWPC1, before target.
	double GetX2() const; // from MWPC2, before target.
	double GetY2() const; // from MWPC2, before target.
	double GetX3() const; // from MWPC3, after target.
	double GetY3() const; // from MWPC3, after target.
	double GetTaX() const; // X of the target
	double GetTaY() const; // Y of the target
	// For angles here, anti-clockwise is positive.
	double GetThetaIn() const; // from z-axis to incident particle tracks.
	double GetThetaOut() const; // from z-axis to exit particle tracks.
	double GetPhiIn() const; // from z-axis to incident particle tracks.
	double GetPhiOut() const; // from z-axis to exit particle tracks.
	double GetBetaIn() const; // fBeta1
	bool IsMWPCMiss(int id) const{ return fIsMWPCMiss[id%3]; }
	const tVME_event& GetfEvent() const{ return fEvent; }
	double GetQDC(int i, int j) const; // { return fEvent.qdc[i][j]; }
	// time tag and time of flight information recorded by TOF_start and TOF_stop
	double GetTOF1PXI() const; // time tag recorded by TOF_start, TOF1 from PXI
	double GetTOF1VME() const; // time tag recorded by TOF_start, TOF1 from VME
	bool IsPileUp() const; // return fIsPileUp
	bool IsVeto(int id) const; // return fIsVeto[id%2]; [veto1-2]
	const tVME_event &GetEvent() const; // return fEvent;
	TTree *GetTreeVME(); // return fVME
	void Output(double &AIn, double &ZIn, double &ToFIn, double &betaIn, double &pIn, double &dEIn, double &thetaIn, double &phiIn, double &TaX, double &TaY); // output the result.

	void SetTOF1PXI(double tof1pxi);
	void SetROOTFile(TFile *f);
	// initiazlie function
	void Initialize();

private:
	static void analyze(const int *data, tVME_event &event);

	TTree *fVME; // point to the data tree.
	tVME_event fEvent; // considered as corresponding to one particle.
	string fDatafile; // raw binary data file
	string fROOTFileName; // root file name
	TFile *fROOTFile; // root file pointer
	bool fIsPileUp;
	bool fIsVeto[2];
	bool fIsMWPCMiss[3]; // [MWPC0-1-2]: if the MWPC misses the particle
	bool fIsAssigned;
	bool fIsAnalyzed;
	int fBlockIndex; // index of the block the stored event belongs.
	double fDEBeforeTarget; // energy deposit in the Music before the target.
	double fDEAfterTarget; // energy deposit in the Music after the target.
	double fTOF1PXI; // from TOF start to T0_0, to be assigned by PXI
	double fTOF1VME; // from TOF start to T0_0, from VME
	double fBeta1; // from TOF start to T0_0
	double fZIn; // nuclear charge number, calculated from fDEBeforeTarget
	double fAoQ1; // A over Q ratio for incident particle.
	double fAIn; // nuclear mass number.
	double fZOut; // nuclear charge number, calculated from fDEAfterTarget
	double fX1; // x recorded by MWPC1
	double fY1; // y recorded by MWPC1
	double fX2; // x recorded by MWPC2
	double fY2; // y recorded by MWPC2
	double fX3; // x recorded by MWPC3
	double fY3; // y recorded by MWPC3
	double fTaX; // X of the target
	double fTaY; // Y of the target
	double fThetaIn;
	double fThetaOut;
	double fPhiIn;
	double fPhiOut;
};
#endif
