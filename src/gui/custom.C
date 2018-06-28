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
// Last modified: 2018/6/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

void MyMainFrame::DoDraw(const int opt){
//	cout << "opt: " << opt << endl; // DEBUG
	fCurrentOption = opt;
	const char *drawopt[4] = {"text", "textcolz", "LEGO", "CONTZ"};
	switch(opt){
		case 101: case 102: case 103: case 104: case 105:
		case 106: case 107: case 108: case 109: case 110:
		case 111: case 112: case 113: case 114: case 115:
		case 116: case 117: case 118: case 119: case 120: case 121:
			((TH1*)(fObjArr[opt%100 - 1]))->Draw("col"); break;
		case 201: case 202: case 203: case 204:
			((TH2F*)fObjArr[21])->Draw(drawopt[opt%100-1]); break;
			
		case 301: cout << "301" << endl; break;
		case 302: cout << "302" << endl; break;
		case 303: cout << "303" << endl; break;
		default:
			cout << "\033[31;1mUnknown User Operation Detected\033[0m" << endl;
			break;
	} // end of switch

	fMyCanvas->cd();
	fMyCanvas->Update();
} // end member function DoDraw

inline void nullError(const char *s);
void MyMainFrame::Initialize(const char *rootfile, const char *assrootfile){
	static bool isCalled = false;
	if(isCalled){
		cout << "\033[36;1m\t[BINGER GUI] - MyMainFrame::Initialize: Has been called once\033[0m" << endl;
		return;
	}
	ostringstream name(""), title("");
	fFile[0] = new TFile("gui.root", "RECREATE"); // store derivative objects
	fFile[1] = new TFile(rootfile);
	if(strcmp(assrootfile, "") || !assrootfile) fFile[2] = new TFile(assrootfile);
	if(!fFile[2]){
		name << "assess" << rootfile;
		fFile[2] = new TFile(name.str().c_str());
	}
	title << "ETFAna - SUN Yazhou - " << fFile[1]->GetName();
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

	fNObj = 0;
	fObjArr[0] = fFile[1]->Get("/SHOOT/hVetoPos"); fNObj++;
	fObjArr[1] = fFile[1]->Get("/SHOOT/hT0_1Pos"); fNObj++;
	fObjArr[2] = fFile[1]->Get("/SHOOT/hPDCPos-UDC-0"); fNObj++;
	fObjArr[3] = fFile[1]->Get("/SHOOT/hPDCPos-UDC-1"); fNObj++;
	fObjArr[4] = fFile[1]->Get("/SHOOT/hTaPos2D-U"); fNObj++;
	fObjArr[5] = fFile[1]->Get("/SHOOT/hTaPos2D-D"); fNObj++;
	fObjArr[6] = fFile[1]->Get("/SHOOT/hPDCPos-DDC-0"); fNObj++;
	fObjArr[7] = fFile[1]->Get("/SHOOT/hPDCPos-DDC-1"); fNObj++;
	fObjArr[8] = fFile[1]->Get("/SHOOT/hTaHitPos1DMatch-X"); fNObj++;
	fObjArr[9] = fFile[1]->Get("/SHOOT/hTaHitPos1DMatch-Y"); fNObj++;
	fObjArr[10] = fFile[1]->Get("/SHOOT/hNuDC0X1"); fNObj++;
	fObjArr[11] = fFile[1]->Get("/SHOOT/hNuDC0X2"); fNObj++;
	fObjArr[12] = fFile[1]->Get("/SHOOT/hDCRPosX-DC0"); fNObj++;
	fObjArr[13] = fFile[1]->Get("/SHOOT/hNuDC1X1"); fNObj++;
	fObjArr[14] = fFile[1]->Get("/SHOOT/hNuDC1X2"); fNObj++;
	fObjArr[15] = fFile[1]->Get("/SHOOT/hDCRPosX-DC1"); fNObj++;
	fObjArr[16] = fFile[1]->Get("/SHOOT/hNuDC2X1"); fNObj++;
	fObjArr[17] = fFile[1]->Get("/SHOOT/hNuDC2X2"); fNObj++;
	fObjArr[18] = fFile[1]->Get("/SHOOT/hDCRPosX-DC2"); fNObj++;
	fObjArr[19] = fFile[1]->Get("/SHOOT/hTOFWFiredStrip"); fNObj++;
	fObjArr[20] = fFile[1]->Get("/SHOOT/hTOFWPosX"); fNObj++;
	for(int i = 0; i < fNObj; i++) if(!fObjArr[i]){
		cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: null pointer(s) detected during object extraction:" << endl;
		cout << "i: " << i << "\033[0m" << endl;
	}
	
	// extract the multiplicity histograms and then get the zero multiplicity bin content //
	TH2F *hEff = new TH2F("hEff", "MWDC Efficiency Showcase;DC Type;Efficiency Type(%)", 24, -1.5, 22.5, 5, -1.5, 3.5);
	fObjArr[21] = hEff; fNObj++;
	const int n_ev = treeTrack->GetEntries();
	// ****** for PDCs ******
	const char xy[] = "XY", UD[] = "UD";
	for(int i = 0; i < 2; i++){ // loop over MWDC arrays
		for(int j = 0; j < 2; j++){ // loop over the two MWDCs
			for(int k = 0; k < 2; k++){ // loop over XY SLayers
				for(int l = 0; l < 2; l++){ // loop over two sense wire layers in one SLayer
					// DC hit multiplicity
					name.str("");
					name << "/Multiplicity/hPDCMultiDCArr" << UD[i]  << "_DC" << j << "_" << xy[k] << l;
					TH1F *hmulti = (TH1F*)fFile[1]->Get(name.str().c_str());
					if(!hmulti) nullError(name.str().c_str());
					if(0. != hmulti->GetBinCenter(5)){
						cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: the 5th bin of hmulti is not zero:" << endl;
						cout << "hmulti->GetName(): " << hmulti->GetName() << "\t";
						cout << "hmulti->GetBinCenter(5): " << hmulti->GetBinCenter(5) << "\033[0m" << endl;
					}
					hEff->SetBinContent((i*2+j)*4+(k*2+l)+2, 2, 1 - hmulti->GetBinContent(5) / n_ev); // hard efficiency
				} // end for over layer 1 and 2
			} // end for over X-Y
		} // end for over DCs
	} // end for over DC arrays
	// ****** for DCRs ******
	for(int i = 0; i < 3; i++){ // loop over the three MWDCs
		for(int j = 0; j < 2; j++){
			name.str("");
			name << "/Multiplicity/hDCMultiDCArrR_DC" << i << "_X" << j;
			TH1F *hmulti = (TH1F*)fFile[1]->Get(name.str().c_str());
			if(!hmulti) nullError(name.str().c_str());
			if(0. != hmulti->GetBinCenter(5)){
				cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: the 5th bin of hmulti is not zero:" << endl;
				cout << "hmulti->GetName(): " << hmulti->GetName() << "\t";
				cout << "hmulti->GetBinCenter(5): " << hmulti->GetBinCenter(5) << "\033[0m" << endl;
			}
			hEff->SetBinContent(i*2+j+18, 2, 1 - hmulti->GetBinContent(5) / n_ev); // hard efficiency
		} // end loop over two adjacent layers
	} // end loop over the three MWDCs
	if(fFile[2]){
		TH1F *heffU = (TH1F*)fFile[2]->Get("/assessTa0U/miscTa0U/heff");
		if(!heffU) nullError("/assessTa0U/miscTa0U/heff");
		for(int i = 0; i < 2; i++){ // loop over X-Y
			for(int j = 0; j < 4; j++){
				hEff->SetBinContent(2+i*4+j, 3, heffU->GetBinContent(5+i*8+1+j) / heffU->GetBinContent(5+i*8));
				name.str(""); name << "ntrLs[2][" << i << "] > 0";
				hEff->SetBinContent(2+i*4+j, 4, treeTrack->GetEntries(name.str().c_str()) / double(n_ev));
			} // end for over 8 sense wire layers in DCTaU
		} // end for over i

		TH1F *heffD = (TH1F*)fFile[2]->Get("/assessTa0D/miscTa0D/heff");
		if(!heffD) nullError("/assessTa0D/miscTa0D/heff");
		for(int i = 0; i < 2; i++){ // loop over X-Y
			for(int j = 0; j < 4; j++){
				hEff->SetBinContent(2+8+i*4+j, 3, heffD->GetBinContent(5+i*8+1+j) / heffD->GetBinContent(5+i*8));
				name.str(""); name << "ntrLs[3][" << i << "] > 0";
				hEff->SetBinContent(2+8+i*4+j, 4, treeTrack->GetEntries(name.str().c_str()) / double(n_ev));
			} // end for over 8 sense wire layers in DCTaD
		} // end for over i

		TH1F *heffR = (TH1F*)fFile[2]->Get("/assess0R/misc0R/heff");
		if(!heffU) nullError("/assess0R/misc0R/heff");
		for(int i = 0; i < 6; i++){
			hEff->SetBinContent(18+i, 3, heffR->GetBinContent(5+i+1) / heffR->GetBinContent(5));
			hEff->SetBinContent(18+i, 4, treeTrack->GetEntries("ntrLs[1][0] > 0") / double(n_ev));
		} // end for over 6 sense wire layers in DCArrR
	} // end if
	
	// change the order of the heff bins to make it "beamwise" for PDCs //
	double effOld[3][16]{}; // bin content of hEff
	for(int i = 0; i < 3; i++){ // loop over rows
		for(int j = 0; j < 16; j++){ // loop over columns
			effOld[i][j] = hEff->GetBinContent(i+2, j+2);
		} // end for over j
	} // end for over i
	const int newOrder[16] = {
		5,   6,   1,   2,   7,   8,   3,   4,
		1+8, 2+8, 5+8, 6+8, 3+8, 4+8, 7+8, 8+8
	};
	for(int i = 0; i < 3; i++){ // loop over rows
		for(int j = 0; j < 16; j++){ // loop over columns
			hEff->GetBinContent(i+2, j+2, effOld[i][newOrder[j] - 1]);
		} // end for over j
	} // end for over i
	hEff->GetXaxis()->SetBinLabel(2,  "U0Y1"); hEff->GetXaxis()->SetBinLabel(3,  "U0Y2");
	hEff->GetXaxis()->SetBinLabel(4,  "U0X1"); hEff->GetXaxis()->SetBinLabel(5,  "U0X2");
	hEff->GetXaxis()->SetBinLabel(6,  "U1Y1"); hEff->GetXaxis()->SetBinLabel(7,  "U1Y2");
	hEff->GetXaxis()->SetBinLabel(8,  "U1X1"); hEff->GetXaxis()->SetBinLabel(9,  "U1X2");
	hEff->GetXaxis()->SetBinLabel(10, "D0X1"); hEff->GetXaxis()->SetBinLabel(11, "D0X2");
	hEff->GetXaxis()->SetBinLabel(12, "D0Y1"); hEff->GetXaxis()->SetBinLabel(13, "D0Y2");
	hEff->GetXaxis()->SetBinLabel(14, "D1X1"); hEff->GetXaxis()->SetBinLabel(15, "D1X2");
	hEff->GetXaxis()->SetBinLabel(16, "D1Y1"); hEff->GetXaxis()->SetBinLabel(17, "D1Y2");
	hEff->GetXaxis()->SetBinLabel(18, "R0X1"); hEff->GetXaxis()->SetBinLabel(19, "R0X2");
	hEff->GetXaxis()->SetBinLabel(20, "R0X1"); hEff->GetXaxis()->SetBinLabel(21, "R0X2");
	hEff->GetXaxis()->SetBinLabel(22, "R0X1"); hEff->GetXaxis()->SetBinLabel(23, "R0X2");

	hEff->GetYaxis()->SetBinLabel(2, "nHit/n_ev");
	hEff->GetYaxis()->SetBinLabel(3, "software eff");
	hEff->GetYaxis()->SetBinLabel(4, "nWithTrk/n_ev");

	isCalled = true;
} // end member function Initialize

void nullError(const char *objName){
	cout << "null pointer in obj-extration: ";
	cout << objName << endl;
	getchar();
	exit(1);
}







