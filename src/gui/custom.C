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
	switch(opt){
		case 0:
			cout << "Draw clicked, opt: " << opt << endl;
			((TH2F*)(fObjArr[0]))->Draw("col");
			break;
		default:
			cout << "Draw clicked" << endl; break;
	} // end of switch

	fMyCanvas->cd();
	fMyCanvas->Update();
} // end member function DoDraw

void MyMainFrame::Initialize(const char *rootfile, const char *assrootfile){
	fFile[0] = new TFile("gui.root", "RECREATE"); // store derivative objects
	fFile[1] = new TFile(rootfile);
	if(strcmp(assrootfile, "")) fFile[2] = new TFile(assrootfile);
	
	treeTrack = (TTree*)fFile[1]->Get("treeTrack");
	vme = (TTree*)fFile[1]->Get("vme");
	treeshoot = (TTree*)fFile[1]->Get("treeshoot");
	if(!treeTrack || !vme || !treeshoot){
		cout << "\t[BINGER GUI] - MyMainFrame::Initialize: null tree pointer(s) detected" << endl;
		cout << "treeTrack: " << treeTrack << "\tvme: " << vme << "\ttreeshoot: " << treeshoot << endl;
		getchar();
		exit(1);
	}
	treeTrack->AddFriend(vme);
	treeTrack->AddFriend(treeshoot);
	
	TH2F *hT0_1Pos = (TH2F*)fFile[1]->Get("/SHOOT/hT0_1Pos");
	fObjArr[0] = hT0_1Pos;
} // end member function Initialize



