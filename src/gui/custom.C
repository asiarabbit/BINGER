///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// src/gui/custom.C																	 //
//   custom.C -- source file for definitions of some methods of class MyMainFrame	 //
//   Introduction: definitions of initialize() and DoDraw() function for class 		 //
// MyMainFrame. ROOT objects extraction from input root files are defined in this 	 //
// scriplet, together with some button actions.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/6/25.															     //
// Last modified: 2018/6/25, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

void MyMainFrame::DoDraw(const int opt){
//	cout << "opt: " << opt << endl; // DEBUG
	switch(opt){
		case 101: ((TH2F*)(fObjArr[0]))->Draw("col"); break;
		case 102: ((TH2F*)(fObjArr[1]))->Draw("col"); break;
		case 103: ((TH2F*)(fObjArr[2]))->Draw("col"); break;
		case 104: ((TH2F*)(fObjArr[3]))->Draw("col"); break;
		case 105: ((TH2F*)(fObjArr[4]))->Draw("col"); break;
		case 106: ((TH2F*)(fObjArr[5]))->Draw("col"); break;
		case 107: ((TH2F*)(fObjArr[6]))->Draw("col"); break;
		case 108: ((TH2F*)(fObjArr[7]))->Draw("col"); break;
		case 109: ((TH2F*)(fObjArr[8]))->Draw("col"); break;
		case 110: ((TH2F*)(fObjArr[9]))->Draw("col"); break;
		case 201: cout << "201" << endl; break;
		case 202: cout << "202" << endl; break;
		case 203: cout << "203" << endl; break;
		case 204: cout << "204" << endl; break;
		case 301: cout << "301" << endl; break;
		case 302: cout << "302" << endl; break;
		case 303: cout << "303" << endl; break;
		default:
			cout << "\033[31;1mUnknown User Operation Detected\033[0m" << endl; break;
	} // end of switch

	fMyCanvas->cd();
	fMyCanvas->Update();
} // end member function DoDraw

void MyMainFrame::Initialize(const char *rootfile, const char *assrootfile){
	fFile[0] = new TFile("gui.root", "RECREATE"); // store derivative objects
	fFile[1] = new TFile(rootfile);
	if(strcmp(assrootfile, "")) fFile[2] = new TFile(assrootfile);
	ostringstream title;
	title << "ETFAna-Y.Z.SUN - " << fFile[1]->GetName();
	SetWindowName(title.str().c_str()); //  - By SUN Yazhou
	
	treeTrack = (TTree*)fFile[1]->Get("treeTrack");
	vme = (TTree*)fFile[1]->Get("vme");
	treeshoot = (TTree*)fFile[1]->Get("treeshoot");
	if(!treeTrack || !vme || !treeshoot){
		cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: null tree pointer(s) detected" << endl;
		cout << "treeTrack: " << treeTrack << "\tvme: " << vme << "\ttreeshoot: " << treeshoot << "\033[31[0m" << endl;
		getchar();
		exit(1);
	}
	treeTrack->AddFriend(vme);
	treeTrack->AddFriend(treeshoot);
	
	int n = 0;
	fObjArr[0] = fFile[1]->Get("/SHOOT/hVetoPos"); n++;
	fObjArr[1] = fFile[1]->Get("/SHOOT/hT0_1Pos"); n++;
	fObjArr[2] = fFile[1]->Get("/SHOOT/hPDCPos-UDC-0"); n++;
	fObjArr[3] = fFile[1]->Get("/SHOOT/hPDCPos-UDC-1"); n++;
	fObjArr[4] = fFile[1]->Get("/SHOOT/hTaPos2D-U"); n++;
	fObjArr[5] = fFile[1]->Get("/SHOOT/hTaPos2D-D"); n++;
	fObjArr[6] = fFile[1]->Get("/SHOOT/hPDCPos-DDC-0"); n++;
	fObjArr[7] = fFile[1]->Get("/SHOOT/hPDCPos-DDC-1"); n++;
	fObjArr[8] = fFile[1]->Get("/SHOOT/hTaHitPos1DMatch-X"); n++;
	fObjArr[9] = fFile[1]->Get("/SHOOT/hTaHitPos1DMatch-Y"); n++;
	for(int i = n; i--;) if(!fObjArr[9]){
		cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: null pointer(s) detected during object extraction:" << endl;
		cout << "i: " << i << "\033[0m" << endl;
	}
} // end member function Initialize



