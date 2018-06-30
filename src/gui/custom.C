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
// Last modified: 2018/6/30, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

void MyMainFrame::DoDraw(const int opt){
//	cout << "opt: " << opt << endl; // DEBUG
//	fCUT += " && t0_1_ok";
//	fCUT += " && 1==pileUpSCA";
	cout << "fCUT: " << fCUT << endl; // DEBUG
	fCurrentOption = opt;
	string cut;
	const char *drawopt[4] = {"textcolz", "text", "LEGO", "colz"};
	switch(opt){
		case 101:
			cut = fCUT;
			cut += " && 1==ntrLs[2][0] && 1==ntrLs[2][1]";
			cutDraw(opt, "vetoPos[1]:vetoPos[0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 102:
			cut = fCUT;
			cut += " && 1==ntrLs[2][0] && 1==ntrLs[2][1]";
			cutDraw(opt, "t0_1Pos[1]:t0_1Pos[0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 103:
			cut = fCUT;
			cut += " && 1==ntrLs[2][0] && 1==ntrLs[2][1]";
			cutDraw(opt, "PDCPos[0][1]:PDCPos[0][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 104:
			cut = fCUT;
			cut += " && 1==ntrLs[2][0] && 1==ntrLs[2][1]";
			cutDraw(opt, "PDCPos[1][1]:PDCPos[1][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 105:
			cut = fCUT;
			cut += " && 1==ntrLs[2][0] && 1==ntrLs[2][1]";
			cutDraw(opt, "taHitPos[0][1]:taHitPos[0][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 106:
			cut = fCUT;
			cut += " && 1==ntrLs[3][0] && 1==ntrLs[3][1]";
			cutDraw(opt, "taHitPos[1][1]:taHitPos[1][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 107:
			cut = fCUT;
			cut += " && 1==ntrLs[3][0] && 1==ntrLs[3][1]";
			cutDraw(opt, "PDCPos[2][1]:PDCPos[2][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 108:
			cut = fCUT;
			cut += " && 1==ntrLs[3][0] && 1==ntrLs[3][1]";
			cutDraw(opt, "PDCPos[3][1]:PDCPos[3][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";X [mm];Y [mm]");
			break;
		case 109:
			cut = fCUT;
			cut += " && 1==ntrLs[2][0] && 1==ntrLs[3][0]";
			cutDraw(opt, "taHitPos[0][0]:taHitPos[1][0]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";DCArrU-X [mm];DCArrD-X [mm]");
			break;
		case 110:
			cut = fCUT;
			cut += " && 1==ntrLs[2][1] && 1==ntrLs[3][1]";
			cutDraw(opt, "taHitPos[0][1]:taHitPos[1][1]", "(1000, -70., 70., 1000, -70., 70.)",
				cut.c_str(), "col", ";DCArrU-Y [mm];DCArrD-Y [mm]");
			break;
		case 111:
			cut = fCUT;
			cut += " && ntrLs[1][0] && nu[0][0] >= 0";
			cutDraw(opt, "nu[0][0]", "(101, -10.5, 90.5)",
				cut.c_str(), "", ";Sense Wire Id");
			break;
		case 112:
			cut = fCUT;
			cut += " && 1==ntrLs[1][0] && nu[0][1] >= 0";
			cutDraw(opt, "nu[0][1]", "(101, -10.5, 90.5)",
				cut.c_str(), "", ";Sense Wire Id");
			break;
		case 113:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1";
			cutDraw(opt, "DCRPos[0][0]", "(600, -600., 600.)",
				cut.c_str(), "", ";X(from Hit Point to DC Center) [mm]");
			break;
		case 114:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1 && nu[0][2] >= 0";
			cutDraw(opt, "nu[0][2]", "(101, -10.5, 90.5)",
				cut.c_str(), "", ";Sense Wire Id");
			break;
		case 115:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1 && nu[0][3] >= 0";
			cutDraw(opt, "nu[0][3]", "(101, -10.5, 90.5)",
				cut.c_str(), "", ";Sense Wire Id");
			break;
		case 116:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1";
			cutDraw(opt, "DCRPos[1][0]", "(600, -600., 600.)",
				cut.c_str(), "", ";X(from Hit Point to DC Center) [mm]");
			break;
		case 117:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1 && nu[0][4] >= 0";
			cutDraw(opt, "nu[0][4]", "(101, -10.5, 90.5)",
				cut.c_str(), "", ";Sense Wire Id");
			break;
		case 118:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1 && nu[0][5] >= 0";
			cutDraw(opt, "nu[0][5]", "(101, -10.5, 90.5)",
				cut.c_str(), "", ";Sense Wire Id");
			break;
		case 119:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1";
			cutDraw(opt, "DCRPos[2][0]", "(600, -600., 600.)",
				cut.c_str(), "", ";X(from Hit Point to DC Center) [mm]");
			break;
		case 120:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1";
			cutDraw(opt, "firedStripId[0]", "(32, -1.5, 30.5)",
				cut.c_str(), "", ";Strip Id");
			break;
		case 121:
			cut = fCUT;
			cut += " && ntrLs[1][0] >= 1";
			cutDraw(opt, "TOFWPos[0]", "(800, -800., 800.)",
				cut.c_str(), "", ";X [mm]");
			break;

		case 201: case 202: case 203: case 204:
			((TH2F*)fObjArr[21])->Draw(drawopt[opt%100-1]);
			break;

		case 301:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "aoz", "(500, 1., 3.)", cut.c_str(), "", ";aoz [amu]");
			break;
		case 302:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "poz:beta2", "(400, 0.6, 0.75, 400, 1100., 2300.)",
				cut.c_str(), "col", ";beta;poz [MeV/c]");
			break;
		case 303:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "firedStripId:aoz", "(1000, 1., 3., 34, -1.5, 32.5)",
				cut.c_str(), "col", ";aoz [amu];Strip Id");
			break;


		case 304:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "(adc[9]+adc[10]+adc[11]):(adc[7]+adc[8])", "(500, 0., 2500., 500, 0., 1800.)",
				cut.c_str(), "col", ";MUSIC_BeforeTA [ch];MUSIC_AfterTA [ch]");
			break;
		case 305:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "adc[12]:(adc[9]+adc[10]+adc[11])", "(500, 0., 3000., 500, 0., 3500.)",
				cut.c_str(), "col", ";MUSIC_AfterTA [ch];Si_AfterTA [ch]");
			break;
		case 306:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "adc[12]:(adc[7]+adc[8])", "(500, 0., 2500., 500, 0., 3000.)",
				cut.c_str(), "col", ";MUSIC_BeforeTA [ch];Si_AfterTA [ch]");
			break;
		case 307:
			cut = fCUT;
			cut += "";
			cutDraw(opt, "adc[12]*pow(beta2[0],2):(adc[9]+adc[10]+adc[11])*pow(beta2[0],2)", "(500, 0., 1200., 500, 0., 2000.)",
				cut.c_str(), "col", ";Z^{2}-MUSIC_AfterTA [arb.];Z^{2}-Si_AfterTA [arb.]");
			break;
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
	TTree *treeSi = (TTree*)fFile[1]->Get("treeSi");
	treeshoot = (TTree*)fFile[1]->Get("treeshoot");
	if(!treeTrack || !vme || !treeshoot){
		cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: null tree pointer(s) detected" << endl;
		cout << "treeTrack: " << treeTrack << "\tvme: " << vme << "\ttreeshoot: " << treeshoot << "\033[31[0m" << endl;
		getchar();
		exit(1);
	}
	treeTrack->AddFriend(vme);
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeSi);

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
		for(int j = 0; j < 2; j++){ // loop over XY SLayers
			for(int k = 0; k < 2; k++){ // loop over the two MWDCs
				for(int l = 0; l < 2; l++){ // loop over two sense wire layers in one SLayer
					// DC hit multiplicity
					name.str("");
					name << "/Multiplicity/hPDCMultiDCArr" << UD[i]  << "_DC" << k << "_" << xy[j] << l;
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
		TH1F *heffU = (TH1F*)fFile[2]->Get("/assessTa0U/miscTa0U/heffF");
		if(!heffU) nullError("/assessTa0U/miscTa0U/heffF");
		for(int i = 0; i < 2; i++){ // loop over X-Y
			for(int j = 0; j < 4; j++){
				hEff->SetBinContent(2+i*4+j, 3, heffU->GetBinContent(2+i*8+j));
				name.str(""); name << "ntrLs[2][" << i << "] > 0";
				hEff->SetBinContent(2+i*4+j, 4, treeTrack->GetEntries(name.str().c_str()) / double(n_ev));
			} // end for over 8 sense wire layers in DCTaU
		} // end for over i

		TH1F *heffD = (TH1F*)fFile[2]->Get("/assessTa0D/miscTa0D/heffF");
		if(!heffD) nullError("/assessTa0D/miscTa0D/heffF");
		for(int i = 0; i < 2; i++){ // loop over X-Y
			for(int j = 0; j < 4; j++){
				hEff->SetBinContent(2+8+i*4+j, 3, heffD->GetBinContent(2+i*8+j));
				name.str(""); name << "ntrLs[3][" << i << "] > 0";
				hEff->SetBinContent(2+8+i*4+j, 4, treeTrack->GetEntries(name.str().c_str()) / double(n_ev));
			} // end for over 8 sense wire layers in DCTaD
		} // end for over i

		TH1F *heffR = (TH1F*)fFile[2]->Get("/assess0R/misc0R/heffF");
		if(!heffU) nullError("/assess0R/misc0R/heffF");
		for(int i = 0; i < 6; i++){
			hEff->SetBinContent(18+i, 3, heffR->GetBinContent(2+0*8+i));
			hEff->SetBinContent(18+i, 4, treeTrack->GetEntries("ntrLs[1][0] > 0") / double(n_ev));
		} // end for over 6 sense wire layers in DCArrR
	} // end if

	// change the order of the heff bins to make it "beamwise" for PDCs //
	double effOld[22][3]{}; // bin content of hEff
	for(int i = 0; i < 22; i++){ // loop over columns
		for(int j = 0; j < 3; j++){ // loop over rows
			double tmp = hEff->GetBinContent(i+2, j+2);
//			cout << "i: " << i << "\tj: " << j << "\ttmp: " << tmp << endl; // DEBUG
			tmp = int((tmp)*100+0.5); // (2d)%
//			cout << "tmp: " << tmp << endl; getchar(); // DEBUG
			hEff->SetBinContent(i+2, j+2, tmp);
			effOld[i][j] = tmp;
		} // end for over j
	} // end for over i
	const int newOrder[16] = {
		5,   6,   1,   2,   7,   8,   3,   4,
		1+8, 2+8, 5+8, 6+8, 3+8, 4+8, 7+8, 8+8
	};
	for(int i = 0; i < 16; i++){ // loop over columns
		for(int j = 0; j < 3; j++){ // loop over rows
			hEff->SetBinContent(i+2, j+2, effOld[newOrder[i] - 1][j]);
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
	hEff->GetXaxis()->SetBinLabel(20, "R1X1"); hEff->GetXaxis()->SetBinLabel(21, "R1X2");
	hEff->GetXaxis()->SetBinLabel(22, "R2X1"); hEff->GetXaxis()->SetBinLabel(23, "R2X2");

	hEff->GetYaxis()->SetBinLabel(2, "nHit/nev");
	hEff->GetYaxis()->SetBinLabel(3, "software eff");
	hEff->GetYaxis()->SetBinLabel(4, "nTrkOK/nev");

	isCalled = true;

	fCutG[0] = (TCutG*)fFile[1]->Get("CUTG0");
	fCutG[1] = (TCutG*)fFile[1]->Get("CUTG1");
	fCutG[2] = (TCutG*)fFile[1]->Get("CUTG2");
	fCutG[3] = (TCutG*)fFile[1]->Get("CUTG3");

	fFile[0] = new TFile("gui.root", "RECREATE"); // store derivative objects
} // end member function Initialize

void nullError(const char *objName){
	cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::Initialize: ";
	cout << "null pointer in obj-extration: ";
	cout << objName << endl;
	getchar();
	exit(1);
}

// head: X or Y:X or Z:X:Y; binning: (NbinsX, xmin, xmax, NbinsY, ymin, ymax...)
void MyMainFrame::cutDraw(int id, const char *head, const char *binning, const char *cut,
		 const char *drawopt, const char *title){
	ostringstream cmd(""), hname(""); // the draw command
	hname << "h" << id << cut;
	cmd << head << ">>" << hname.str() << binning;
	if(!fFile[0]->Get(hname.str().c_str())){
		treeTrack->Draw(cmd.str().c_str(), cut, drawopt);
		TNamed *obj = dynamic_cast<TNamed*>(fFile[0]->Get(hname.str().c_str()));
		if(!obj){
			cout << "\033[31;1m\t[BINGER GUI] - MyMainFrame::cutDraw: ";
			cout << "nullpointer in object extraction: ";
			cout << hname.str() << "\033[0m" << endl;
			getchar();
			exit(1);
		}
		string titleC = obj->GetTitle(); titleC += title;
		obj->SetTitle(titleC.c_str());
		obj->Write("", TObject::kOverwrite);
	}
	else{
//		cout << "BINGO 1" << endl; // DEBUG
		((TH1*)fFile[0]->Get(hname.str().c_str()))->Draw(drawopt);
	}
}





