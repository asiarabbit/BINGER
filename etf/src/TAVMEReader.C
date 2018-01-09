///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAVMEReader.h														 //
//   TAVMEReader.h -- header file for class TAVMEReader								 //
//   Introduction: read binary data file from VME Daq and transform it into root tree//
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/1/9.															     //
// Last modified: 2018/1/9, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

//#define DEBUG

#include <iostream>
#include <cstdlib>
#include <cstring>

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

#include "readVME.h"
#include "TABUAA.h"
#include "TAVMEReader.h"
#include "TAPopMsg.h"
#include "TAMath.h"

using std::cout;
using std::endl;
using std::flush;

TAVMEReader::TAVMEReader(const string &datafile)
	 : fDatafile(datafile){
	if(strcmp("", fDatafile.c_str())) fROOTFile = fDatafile+"_vme.root";
}
TAVMEReader::~TAVMEReader(){}

void TAVMEReader::SetDatafile(const string &datafile){
	fDatafile = datafile;
	if(strcmp("", fDatafile.c_str())) fROOTFile = fDatafile+"_vme.root";
}
void TAVMEReader::ReadVME(){
	ReadVME(fDatafile);
}
void TAVMEReader::Match(const string &PXIROOTFile){
	Match(fROOTFile, PXIROOTFile);
}

void TAVMEReader::ReadVME(const string &vmeDatafile){
	TAPopMsg::Info("TAVMEReader", "VME data file to be processed: %s", vmeDatafile);

	// postion relative to the target
	const double Z_PL1 = -818.3; // z position of PL1 detector
	const double Z_PL2 = 1040.85; // z position of PL2 detector
	const double DEGREE = TAMath::DEGREE();
	vector<TObject *> objLs; // vector to write and destroy ROOT objects

	//Define Histograms and a tree
	TH1F *hzin  = new TH1F("zin" ,"Z in ",500, -1.5, 20.);
	TH1F *hzout = new TH1F("zout","Z out",500, -1.5, 20.);
	TH1F *htof  = new TH1F("tof" ,"Z in ",1000, 128., 133.);
	TH2F *hpi   = new TH2F("pi","TOF dEin",500, 120., 160., 500, 1., 20.);
	TH2F *hpid   = new TH2F("pid", "Z vs A;A;Z", 2001, 1., 45.5, 2001, 1.05, 20.05);
	TH1F *hthetaIn = new TH1F("thetaIn", "Theta Before Target", 700, -20, 20);
	TH1F *hthetaOut = new TH1F("thetaOut", "Theta After Target", 700, -20, 20);
	TH1F *hAIn  = new TH1F("hAIn", "A of Beam In;A", 2001, 10.05, 45.05);
	TH2F *hTargetXY = new TH2F("hTargetXY", "Target Position;X/mm;Y/mm", 610, -30.5, 30.5, 610, -30.5, 30.5);
	objLs.push_back(hzin); objLs.push_back(hzout); objLs.push_back(htof);
	objLs.push_back(hpid); objLs.push_back(hthetaIn); objLs.push_back(hthetaOut);
	objLs.push_back(hAIn); objLs.push_back(hTargetXY); objLs.push_back(hpi);
	TH1F *hTargetX = new TH1F("hTargetX", "Target Position;X/mm", 610, -30.5, 30.5);
	TH1F *hTargetY = new TH1F("hTargetY", "Target Position;Y/mm", 610, -30.5, 30.5);
	TH1F *hQDC_PL1L = new TH1F("hQDC_PL1L", "hQDC_PL1L", 20000, -10000., 10000.);
	TH1F *hQDC_PL1R = new TH1F("hQDC_PL1R", "hQDC_PL1R", 20000, -10000., 10000.);
	TH1F *hQDC_PL2L = new TH1F("hQDC_PL2L", "hQDC_PL2L", 20000, -10000., 10000.);
	TH1F *hQDC_PL2R = new TH1F("hQDC_PL2R", "hQDC_PL2R", 20000, -10000., 10000.); // 500, -10., 1000.);
	objLs.push_back(hTargetX); objLs.push_back(hTargetY);
	objLs.push_back(hQDC_PL1L); objLs.push_back(hQDC_PL1R);
	objLs.push_back(hQDC_PL2L); objLs.push_back(hQDC_PL2R);
	TH1F *hX_PL1 = new TH1F("hX_PL1", "x of the Hit Position in PL1", 500, -100., 100.);
	TH1F *hX_PL2 = new TH1F("hX_PL2", "x of the Hit Position in PL2", 500, -100., 100.);
	TH2F *hDTvsQ_PL1 = new TH2F("hDTvsQ_PL1", "Timing Error v.s. Integrated Charge of the Signal - PL1", 800, 0.057, 0.0574, 800, 128.5, 132.9);
	TH2F *hDTvsQ_PL2 = new TH2F("hDTvsQ_PL2", "Timing Error v.s. Integrated Charge of the Signal - PL2", 800, 0.052, 0.058, 800, 128.5, 132.9);
	TH2F *hDTvsX_PL1 = new TH2F("hDCvsX_PL1", "Timing Error v.s. Hit x Position - PL1", 800, -30., 30., 800, 128.5, 132.9);
	TH2F *hDTvsX_PL2 = new TH2F("hDCvsX_PL2", "Timing Error v.s. Hit x Position - PL2", 800, -80., 80., 2400, 129.7, 131.7);
	objLs.push_back(hDTvsQ_PL1); objLs.push_back(hDTvsQ_PL2);
	objLs.push_back(hDTvsX_PL1); objLs.push_back(hDTvsX_PL2);
	TTree *treeVME = new TTree("treeVME", "tree for VME readouts");
	double TaX, TaY, thetaIn, phiIn, thetaOut, phiOut, zin, zout, dEin, dEout, tof, Ain;
	double XP[3], YP[3]; // positions read from the three MWPCs [MWPC0-1-2]
	bool isVeto[2], isPileUp, isMWPCMiss[3];
	int sca2; // trigger accepted, serving as a trigger index
	double Q_PL[4]; // [0-3]: [PL1L, PL1R, PL2L, PL2R]
	double X_PL[2]; // [0-1]: [X_PL1, X_PL2]
	treeVME->Branch("sca2", &sca2, "sca2/I");
	treeVME->Branch("TaX", &TaX, "TaX/D");
	treeVME->Branch("TaY", &TaY, "TaY/D");
	treeVME->Branch("thetaIn", &thetaIn, "thetaIn/D"); // horizontal
	treeVME->Branch("phiIn", &phiIn, "phiIn/D"); // vertical
	treeVME->Branch("thetaOut", &thetaOut, "thetaOut/D"); // horizontal
	treeVME->Branch("phiOut", &phiOut, "phiOut/D"); // vertical
	treeVME->Branch("zin", &zin, "zin/D");
	treeVME->Branch("dEin", &dEin, "dEin/D");
	treeVME->Branch("zout", &zout, "zout/D");
	treeVME->Branch("dEout", &dEout, "dEout/D");
	treeVME->Branch("tof1VME", &tof, "tof1VME/D"); // tof over the 25.88m flight
	treeVME->Branch("Ain", &Ain, "Ain/D");
	treeVME->Branch("XP", XP, "XP[3]/D");
	treeVME->Branch("YP", YP, "YP[3]/D");
	treeVME->Branch("isPileUp", &isPileUp, "isPileUp/O");
	treeVME->Branch("isVeto", isVeto, "isVeto[2]/O");
	treeVME->Branch("isMWPCMiss", isMWPCMiss, "isMWPCMiss[3]/O");
	treeVME->Branch("Q_PL[4]", Q_PL, "Q_PL[4]/D");
	treeVME->Branch("X_PL[2]", X_PL, "X_PL[2]/D"); // x of hit position in PL detectors
	objLs.push_back(treeVME);

	TABUAA *bua = new TABUAA(vmeDatafile);
	bua->ReadOffline(); // create, fill and write the vme tree in a created rootfile
	string vmerootfile = vmeDatafile + "_vme.root";
	TFile *file = new TFile(vmerootfile.c_str(), "UPDATE");
	bua->SetROOTFile(file); // attach the vme tree in the rootfile

	TTree *vme = bua->GetTreeVME();
	int n = vme->GetEntries();
	cout << n << " entries in the raw VME tree.\n"; // DEBUG

	for(int i = 0; i < n; i++){
		bua->Assign(i); // assign fEvent with the i-th entry
		sca2 = bua->GetEvent().sca[2];
		bua->Analyze(); // analyze fEvent assined with the entry read
		isVeto[0] = bua->IsVeto(0); isVeto[1] = bua->IsVeto(1);
		isPileUp = bua->IsPileUp();
		for(int i = 3; i--;) isMWPCMiss[i] = bua->IsMWPCMiss(i);
		TaX = bua->GetTaX(); TaY = bua->GetTaY();
		thetaIn = bua->GetThetaIn(); phiIn = bua->GetPhiIn();
		thetaOut = bua->GetThetaOut(); phiOut = bua->GetPhiOut();
		zin = bua->GetZIn(); zout = bua->GetZOut();
		dEin = bua->GetDEBeforeTarget(); dEout = bua->GetDEAfterTarget();
		tof = bua->GetTOF1VME(); Ain = bua->GetAIn();
		XP[0] = bua->GetX1(); XP[1] = bua->GetX2(); XP[2] = bua->GetX3();
		YP[0] = bua->GetY1(); YP[1] = bua->GetY2(); YP[2] = bua->GetY3();
		// QDC information of the plastic scintillators
		Q_PL[0] = bua->GetQDC(0, 5); hQDC_PL1L->Fill(Q_PL[0]); // PL1L
		Q_PL[1] = bua->GetQDC(0, 7); hQDC_PL1R->Fill(Q_PL[1]); // PL1R
		Q_PL[2] = bua->GetQDC(0, 17); hQDC_PL2L->Fill(Q_PL[2]); // PL1L
		Q_PL[3] = bua->GetQDC(0, 19); hQDC_PL2R->Fill(Q_PL[3]); // PL1R
#ifdef DEBUG
		cout << "Q_PL[0]: " << Q_PL[0] << endl; // DEBUG
		cout << "Q_PL[1]: " << Q_PL[1] << endl; // DEBUG
		cout << "Q_PL[2]: " << Q_PL[2] << endl; // DEBUG
		cout << "Q_PL[3]: " << Q_PL[3] << endl; // DEBUG
		getchar(); // DEBUG
#endif
		X_PL[0] = Z_PL1*tan(thetaIn) + TaX; hX_PL1->Fill(X_PL[0]); // x of the hit position on PL1
		X_PL[1] = Z_PL2*tan(thetaIn) + TaX; hX_PL2->Fill(X_PL[1]); // x of the hit position on PL2
		hDTvsQ_PL1->Fill(pow(Q_PL[0]*Q_PL[1], -0.25), tof); // the formula is from BUAA's essay
		hDTvsQ_PL2->Fill(pow(Q_PL[2]*Q_PL[3], -0.25), tof);
		hDTvsX_PL1->Fill(X_PL[0], tof);
		hDTvsX_PL2->Fill(X_PL[1], tof);

		treeVME->Fill();
		if(!isVeto[1] // adc pile up
		 && !isPileUp // particle deviating too much from the neutral beam line
		 ){
//			cout << "Pile Up occurred." << endl; // DEBUG
//			getchar(); // DEBUG
			hzin->Fill(zin);
			hzout->Fill(zout);
			htof->Fill(tof);
			hpi->Fill(tof, zin);
			hthetaIn->Fill(thetaIn / DEGREE);
			hthetaOut->Fill(thetaOut / DEGREE);
			hAIn->Fill(Ain);
			hpid->Fill(Ain, zin);
			hTargetX->Fill(TaX);
			hTargetY->Fill(TaY);
			hTargetXY->Fill(TaX, TaY);
		} // end if
//		if(fabs(bua->GetZIn() - 11.) < 0.6 && fabs(bua->GetTOF1() - 133.25) < 1.25){ // Na
//		} // end of if
		cout << "Trigger \033[1m" << i << "\033[0m processed.";
		cout << " sca2: \033[1m" << sca2 << "\033[0m\r" << flush;
	} // end for over i

	for(TObject *&b : objLs){
		b->Write("", TObject::kOverwrite); delete b; b = nullptr;
	}
	file->Close(); delete file; file = nullptr;
	delete bua; bua = nullptr;
	
	cout << "\n\n\033[33;1mDONE\n\n\033[0m";
}

void TAVMEReader::Match(const string &PXIROOTFile, const string &VMEROOTFile){
	TFile *fvme = new TFile(VMEROOTFile.c_str());
	TFile *fpxi = new TFile(PXIROOTFile.c_str(), "update");
	TTree *treeVME = (TTree*)fvme->Get("treeVME");
	TTree *treeTrack = (TTree*)fpxi->Get("treeTrack");
	TTree *treeVME_M = new TTree("treeVME_M", "Matched VME tree with treeTrack");
	int index, sca2;
	double beta; // variables to extracted from treeTrack
	// variables to be extracted from treeVME_M. In fact it is all of them.
	double TaX, TaY, thetaIn, phiIn, thetaOut, phiOut, zin, zout, tof, Ain;
	double XP[3], YP[3]; // positions read from the three MWPCs [MWPC0-1-2]
	bool isVeto[2], isPileUp, isMWPCMiss[3], isFutile; // isFutile: mark fuitle VME entries.
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("beta", &beta); // to calculate tof1PXI

	treeVME->SetBranchAddress("sca2", &sca2);
	treeVME->SetBranchAddress("TaX", &TaX);
	treeVME->SetBranchAddress("TaY", &TaY);
	treeVME->SetBranchAddress("thetaIn", &thetaIn);
	treeVME->SetBranchAddress("phiIn", &phiIn);
	treeVME->SetBranchAddress("thetaOut", &thetaOut);
	treeVME->SetBranchAddress("phiOut", &phiOut);
	treeVME->SetBranchAddress("zin", &zin);
	treeVME->SetBranchAddress("zout", &zout);
	treeVME->SetBranchAddress("tof1VME", &tof); // tof over the 25.88m flight
	treeVME->SetBranchAddress("Ain", &Ain);
	treeVME->SetBranchAddress("XP", XP);
	treeVME->SetBranchAddress("YP", YP);
	treeVME->SetBranchAddress("isPileUp", &isPileUp);
	treeVME->SetBranchAddress("isVeto", isVeto);
	treeVME->SetBranchAddress("isMWPCMiss", isMWPCMiss);

	treeVME_M->Branch("sca2", &sca2, "sca2/I");
	treeVME_M->Branch("TaX", &TaX, "TaX/D");
	treeVME_M->Branch("TaY", &TaY, "TaY/D");
	treeVME_M->Branch("thetaIn", &thetaIn, "thetaIn/D");
	treeVME_M->Branch("phiIn", &phiIn, "phiIn/D");
	treeVME_M->Branch("thetaOut", &thetaOut, "thetaOut/D");
	treeVME_M->Branch("phiOut", &phiOut, "phiOut/D");
	treeVME_M->Branch("zin", &zin, "zin/D");
	treeVME_M->Branch("zout", &zout, "zout/D");
	treeVME_M->Branch("tof1VME", &tof, "tof1VME/D"); // tof over the 25.88m flight.
	treeVME_M->Branch("Ain", &Ain, "Ain/D");
	treeVME_M->Branch("XP", XP, "XP[3]/D");
	treeVME_M->Branch("YP", YP, "YP[3]/D");
	treeVME_M->Branch("isPileUp", &isPileUp, "isPileUp/O");
	treeVME_M->Branch("isVeto", isVeto, "isVeto[2]/O");
	treeVME_M->Branch("isMWPCMiss", isMWPCMiss, "isMWPCMiss[3]/O");
	// futile entries don't match any PXI entries, which is caused by trigger loss
	treeVME_M->Branch("isFutile", &isFutile, "isFutile/O");

	// for VME-PXI alignment test
	TH2F *hTOFInVMEvsTOFInPXI  = new TH2F("hTOFInVMEvsTOFInPXI", "hTOFIn-VME vs TOFIn-PXI of Beam In;TOFIn-VME/ns;TOFIn-PXI/ns", 800, 120., 165., 800, 120., 165.);
	TH2F *hEvIndexTimeDevi = new TH2F("hEvIndexTimeDevi", "Event Index v.s. TOF1 Deviation;event index;dt/ns", 2000, 0., 42000., 500, -20., 30.);
	TH2F *hLminusR_VMEvsPXI = new TH2F("hLminusR_VMEvsPXI", "hLminusR_VME vs PXI;L-R_VME/channel;L-R_PXI/ns", 80, 2190.5, 2270.5, 48, 2.2, 3.4);
	TH1F *hdd = new TH1F("hdd", "Difference of Trigger Indices between PXI and VME data;PXI;VME", 2001, -1000.5, 1000.5);

	const int n = treeTrack->GetEntries();
	const int vmeN = treeVME->GetEntries();
	cout << n << " entries in treeTrack.\n";
	cout << vmeN << " entries in treeVME.\n";
	if(vmeN > n){
		cout << "\033[31;1mThis is strange. There are more entries in VME data than in PXI counterpart.\033[0m\n";
		cout << "Something is wrong.\n";
		getchar();
	} // end if

	int vme_i = -1; // the entry id of VME data tree
	int vme_offset = 0; // the difference between VME and PXI synchronization.
	int cntAlignment = 0; // number of successfully aligned triggers between PXI and VME data
	int cntFutile = 0; // futile entries count 
	for(int i = 0; i < n; i++){
		treeTrack->GetEntry(i);
		double tofInPXI = 86.32638784 / beta; // 25.88 * 1000. / (beta*c0)
		isFutile = true;
		if(vme_i >= vmeN) break; // end of treeVME reached
		vme_i = index - vme_offset;
		treeVME->GetEntry(vme_i);
		
		// // error map // //
		// to trace the trigger error along events, exclusive to specific data file.
		if(sca2 >= 82478) sca2 -= 1;
		if(sca2 >= 91288) sca2 -= 1;
		// // error map // //
		
		
		int dd = sca2 - 1 - index;
//		cout << "\nsca2: " << sca2 << "\tindex: " << index << endl; getchar(); // DEBUG
		hdd->Fill(dd);
		if(dd != 0){ // DEBUG
#ifdef DEBUG
			cout << endl << endl;
			cout << "i: " << i << endl;
			cout << "index: " << index << endl;
			cout << "sca2: " << sca2 << endl;
			cout << "\n\033[33;1mdd: " << dd << "\n\033[0m"; // DEBUG
			getchar(); // DEBUG
#endif
		} // end if // DEBUG
		// the maximum normal dd caused by abnormal scaler increments or trigger loss.
		int ddNormal = 10;
		if(dd != 0 && fabs(dd) <= ddNormal){ // trigger loss confirmed
			cout << "\n\033[33;1m_____________________\033[0m\n"; // DEBUG
			cout << "\033[32;1mvme_offset: " << vme_offset << endl; // DEBUG
			cout << "sca2: " << sca2 << endl; // DEBUG
			cout << "PXI index: " << index << "\n\033[0m"; // DEBUG
			if(dd != 1){
				cout << "dd: " << dd << endl;
				getchar(); // DEBUG
			} // end if
			vme_offset += dd;
			cntFutile++;
			treeVME_M->Fill(); // this entry is futile, only filled to match PXI entry
		} // end if
		else {
			if(dd == 0){
				cntAlignment++; // event alignment success
//				if(sca2 < 100000)
					isFutile = false;
			} // end if
//			if(sca2 >= 80000) // && sca2 < 10000000) // 
				hTOFInVMEvsTOFInPXI->Fill(tof, tofInPXI);
//			isFutile = true;
			treeVME_M->Fill();
			hEvIndexTimeDevi->Fill(index, tof - tofInPXI);
		} // end else
		cout << "Processing PXI index " << index << " and VME entry id " << vme_i << "\r" << flush;
	} // end for over i

	treeVME_M->Write("", TObject::kOverwrite);
	hTOFInVMEvsTOFInPXI->Write("", TObject::kOverwrite);
	hEvIndexTimeDevi->Write("", TObject::kOverwrite);
	hdd->Write("", TObject::kOverwrite);

	fvme->Close();
	fpxi->Close();
	
	cout << "\033[33;1mCount of successfully matched events between PXI and VME data: " << cntAlignment << "\n\033[0m";
	cout << "\033[33;1mFutile count: " << cntFutile << "\n\033[0m";
}






