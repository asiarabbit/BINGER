///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor/define_hist.C									 //
//   define_hist.C -- a scriplet encapuslated in TAEventProcessor.C for method		 //
// TAEventProcessor::Run().															 //
//   Introduction: definition for histograms to be filled.							 //
//																					 //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/21.															     //
// Last modified: 2018/8/27, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

	// global singleton instance pionters
	TAVisual *vis = GetVisual();
	TAPID *pid = GetPID();
	TAGPar *gpar = GetGPar();
	// detector pointers
	TAParaManager::ArrDet_t &det_vec = GetParaManager()->GetDetList();
	TAT0_0 *T0_0 = (TAT0_0*)det_vec[0];

	// use VME TOF stop-v1190_9 or PXI TOF Stop channels as the time reference
	const short TOFStopOption = gpar->Val(106);
	if(1 != TOFStopOption && 15 != TOFStopOption)
		TAPopMsg::Error("TAEvProsr", "Run: abnormal TOFStopOption: %d", TOFStopOption);
//	cout << "TOFStopOption: " << TOFStopOption << endl; getchar(); // DEBUG
	TAT0_1 *T0_1 = (TAT0_1*)det_vec[TOFStopOption]; // [1-15-17]: [PXI-slot9-slot11]

//	TAT0_1 *str_t0_1[3] = {
//		(TAT0_1*)det_vec[1],
//		(TAT0_1*)det_vec[15], // v1190 - slot_9
//		(TAT0_1*)det_vec[17] // v1190 - slot_11
//	};



	TAT0_1 *VETO_0 = (TAT0_1*)det_vec[12];
	TAT0_1 *VETO_1 = (TAT0_1*)det_vec[13];
	TASiPMPlaArray *sipmArr = (TASiPMPlaArray*)det_vec[2];
	TASiPMPlaBarrel *sipmBarr = (TASiPMPlaBarrel*)det_vec[5];
	TAMWDCArray *dcArr[2]{0}; // MWDC arrays downstream of the dipole magnet
	TAMWDCArray2 *dcArr2[2]{0}; // MWDC arrays upstream of the dipole magnet
	TAMWDCArray2 *pdcArr2[2]{0}; // PDC arrays upstream of the dipole magnet
	dcArr[0] = (TAMWDCArray*)det_vec[3]; // dc array L
//	dcArr[1] = (TAMWDCArray*)det_vec[4]; // dc array R
	dcArr[1] = (TAMWDCArray4*)det_vec[4]; // dc array 4 // XXX: for Ma's DCs
	dcArr2[0] = (TAMWDCArray2*)det_vec[6]; // dc array U
	dcArr2[1] = (TAMWDCArray2*)det_vec[7]; // dc array D
	pdcArr2[0] = (TAMWDCArray2*)det_vec[8]; // pdc array U
	pdcArr2[1] = (TAMWDCArray2*)det_vec[9]; // pdc array D
	TAMUSIC *music[3]{0}; // MUSIC around the target
	music[0] = (TAMUSIC*)det_vec[10]; // upstream of the target
	music[1] = (TAMUSIC*)det_vec[11]; // downstream of the target
	music[2] = (TAMUSIC*)det_vec[18]; // Si-downstream of the target
	TAOpticFiberArray *opfa = (TAOpticFiberArray*)det_vec[19]; // optic fiber array made by Cheng
	TATOFWall *tofw[2]{0};
	if(dcArr[0]) tofw[0] = dcArr[0]->GetTOFWall();
//	if(dcArr[1]) tofw[1] = dcArr[1]->GetTOFWall();
	// dc downstream and upstream of the dipole magnet, pdc: same role as dc2
	TAMWDC *dc[2][4]{0}, *dc2[2][2]{0}, *pdc2[2][2]{0};
	for(int i = 2; i--;) for(int j = 4; j--;) if(dcArr[i]) dc[i][j] = dcArr[i]->GetMWDC(j);
	for(int i = 2; i--;) for(int j = 2; j--;) if(dcArr2[i]) dc2[i][j] = dcArr2[i]->GetMWDC(j);
	for(int i = 2; i--;) for(int j = 2; j--;) if(pdcArr2[i]) pdc2[i][j] = pdcArr2[i]->GetMWDC(j);
	dc2[0][0] = dcArr[1]->GetMWDC(0);
	dc2[0][1] = dcArr[1]->GetMWDC(1);
	dc2[1][0] = dcArr[1]->GetMWDC(2);
	dc2[1][1] = dcArr[1]->GetMWDC(3);
//	TAAnode *ano = dc[1][1]->GetAnodeL1(1, 58); // DEBUG
//	cout << ano->GetAnodePara()->GetSTR(0)->GetTitle() << endl; getchar(); // DEBUG
//	cout << ano->GetDriftDistance(70., 0) << endl; getchar(); // DEBUG

//	cout << "T0_0->GetName(): " << T0_0->GetName() << endl; // DEBUG
//	cout << "T0_0->UV: " << T0_0->GetUV()->GetPara()->GetChannelId() << endl; getchar(); // DEBUG
	// to select the trigger-generating particle
	// (204., 857.)->pion2017; (1350., 1500.)->beamTest2016
	const double timeToTrigLowBoundUV = gpar->Val(0), timeToTrigHighBoundUV = gpar->Val(1);
	// (204., 857.)->pion2017; (1350., 1500.)->beamTest2016
	const double timeToTrigLowBoundDV = gpar->Val(2), timeToTrigHighBoundDV = gpar->Val(3);




	// ######################### RAW DETECTOR STATISTICS & PID ############################## //
	// 0: eve stat(misc); 1: FiredDist; 2: multi; 3: hdt; 4: pla hit pos; 5: PID 6: misc 7: timeToTRef
	vector<TObject *> objLs[8];
	// ------- overall view of the whole detector system in the perspective of the Daq ------- //
	TH1F *hSecLen = new TH1F("hSecLen", "Data Section Length;words", 2001, -0.5, 2000.5);
	TH1F *hCh = new TH1F("hCh", "Signal Channel Distribution among Channel IDs;channelId", 10001, -0.5, 10000.5);
	TH2F *htot = new TH2F("htot", "Time Over Threshold v.s. Channel ID Distribution-1st Pulse-Overall;channelId;Tot", 10001, -0.5, 10000.5, 800, -500., 2000.);
	objLs[0].push_back(hSecLen); objLs[0].push_back(hCh); objLs[0].push_back(htot);
	TH2F *hnxvsChid[2], *hxtn_nvsChid[4];
	char name[64], title[128], xuv[] = "XUV", LR[] = "LR", lt[] = "lt";
	char s_lt[2][16] = {"Leading", "Trailing"};
	for(int i = 0; i < 2; i++){
		sprintf(name, "hn%cvsChid", lt[i]);
		sprintf(title, "Number of %s Edges v.s. Channel Id;channel Id;N Edge", s_lt[i]);
		hnxvsChid[i] = new TH2F(name, title, 10001, -0.5, 10000.5, 15, -1.5, 13.5);
		objLs[1].push_back(hnxvsChid[i]);
		for(int j = 0; j < 2; j++){
			sprintf(name, "h%ct%d_%dvsChid", lt[i], j+1, j);
			sprintf(title, "%s Edge %d - %d v.s. Channel Id;channel id;lt[1]-lt[0] [ns]", s_lt[i], j+1, j);
			hxtn_nvsChid[2*i+j] = new TH2F(name, title, 10001, -0.5, 10000.5, 1000, -1500., 1500.);
			objLs[1].push_back(hxtn_nvsChid[2*i+j]);
		} // end for over leading and trailing edges
	}
	TH2F *hnl_ntvsChid = new TH2F("hnl_ntvsChid", "N-Leading_Edge - N-Trailing-Edge v.s. Channel Id;channel Id;nl - nt", 10001, -0.5, 10000.5, 17, -8.5, 8.5);
	objLs[0].push_back(hnl_ntvsChid);

	// ------- comprehensive performance indicators for individual detectors ------- //
	// fired distributions, multiplicities and drift time distributions specifically for MWDCs
	TH1F *hTOFWFiredDist[2], *hDCFiredDist[2][4][3]; // [dcArrL-R], [dcArrL-R][DC0-1-2][X-U-V];
	TH2F *hTOFWToTRef[2]; TH1F *hDCToTRef[2][4][3]; // [dcArrL-R], [dcArrL-R][DC0-1-2][X-U-V];
	TH1F *hTOFWMulti[2], *hDCMulti[2][4][3][2]; // [dcArrL-R], [dcArrL-R][DC0-1-2][X-U-V][1,2];
	// PosCmp: hit position in TOFW strip, rough and refine, from Down End to up end (0-1200).
	TH2F *hTOFWHitPos[2], *hTOFWHitPosCmp[2]; // [dcArrL-R], Cmp: posY calculated via dt v.s. 3D trks
	TH1F *hdt[2][4][3]; // [dcArrL-R][DC0-1-2][X-U-V]
	for(int i = 0; i < 2; i++){ // loop over MWDC arrays
		// TOFWall fired distribution
		sprintf(name, "hTOFWFiredDistDCArr%c", LR[i]);
		sprintf(title, "Fired TOF Wall Strip Distribution - DC Array %c;Strip Serial Id", LR[i]);
		hTOFWFiredDist[i] = new TH1F(name, title, 33, -1.5, 31.5);
		objLs[1].push_back(hTOFWFiredDist[i]);
		// TOFWall hit multiplicity
		sprintf(name, "hTOFWMultiDCArr%c", LR[i]);
		sprintf(title, "TOF Wall Hit Multiplicity - DC Array %c;multiplicity", LR[i]);
		hTOFWMulti[i] = new TH1F(name, title, 17, -1.5, 15.5);
		objLs[2].push_back(hTOFWMulti[i]);
		// TOFWall hit position, utilizing both-end readouts
		sprintf(name, "hTOFWHitPosDCArr%c", LR[i]);
		sprintf(title, "TOF Wall Hit Position - DC Array %c;Strip Serial Id;Hit Pos (from Down end) [mm]", LR[i]);
		hTOFWHitPos[i] = new TH2F(name, title, 33, -1.5, 31.5, 500, -300., 1700.);
		objLs[4].push_back(hTOFWHitPos[i]);
		sprintf(name, "hTOFWHitPosCmpDCArr%c", LR[i]);
		sprintf(title, "TOF Wall Hit Position Compare - DC Array %c;Y - time difference method [mm];Y - Calculated from 3D tracks [mm]", LR[i]);
		hTOFWHitPosCmp[i] = new TH2F(name, title, 500, -300., 1700., 500, -300., 1700.);
		objLs[4].push_back(hTOFWHitPosCmp[i]);
		// TOFWall time to T-reference
		sprintf(name, "h%cTOFWToTRefUV", LR[i]);
		sprintf(title, "h%cTOFWToTRefUV;stripId;timeToTRef [ns]", LR[i]);
		hTOFWToTRef[i] = new TH2F(name, title, 33, -1.5, 31.5, 30000, -200., 800.);
		objLs[7].push_back(hTOFWToTRef[i]);
		for(int j = 0; j < 4; j++){ // loop over the four MWDCs
			for(int k = 0; k < 3; k++){ // loop over XUV SLayers
				for(int l = 0; l < 2; l++){ // loop over X[UV]1-2
					// DC hit multiplicity
					sprintf(name, "hDCMultiDCArr%c_DC%d_%c%d", LR[i], j, xuv[k], l);
					sprintf(title, "DC Hit Multiplicity of DC Array%c - DC%d - %c%d;multiplicity", LR[i], j, xuv[k], l);
					hDCMulti[i][j][k][l] = new TH1F(name, title, 35, -4.5, 30.5);
					objLs[2].push_back(hDCMulti[i][j][k][l]);
				} // end for over layer 1 and 2
				// DC fired distribution
				sprintf(name, "hDCFiredDistDCArr%c_DC%d_%c", LR[i], j, xuv[k]);
				sprintf(title, "Fired DC Anode Distribution of DC Array%c - DC%d - %c;DC Anode Serial Id", LR[i], j, xuv[k]);
				hDCFiredDist[i][j][k] = new TH1F(name, title, 205, -4.5, 200.5);
				objLs[1].push_back(hDCFiredDist[i][j][k]);
				// DC drift time distribution
				sprintf(name, "HdtDCArr%c_DC%d_%c", LR[i], j, xuv[k]);
				sprintf(title, "Drift Time Distribution of DC Array%c - DC%d - %c;drift time [ns]", LR[i], j, xuv[k]);
				hdt[i][j][k] = new TH1F(name, title, 500, -100., 400.);
				objLs[3].push_back(hdt[i][j][k]);
				// DC time to T-reference
				sprintf(name, "hDCToTRefArr%c_DC%d_%c", LR[i], j, xuv[k]);
				sprintf(title, "DC Time to T-reterence - DC Array%c - DC%d - %c;timeToTRef [ns]", LR[i], j, xuv[k]);
				hDCToTRef[i][j][k] = new TH1F(name, title, 4000, -2000., 2000.);
				objLs[7].push_back(hDCToTRef[i][j][k]);
			} // end for over X-U-V
		} // end for over DCs
	} // end for over DC arrays
	TH1F *hSiPMPlaArrFiredDist = new TH1F("hSiPMPlaArrFiredDist", "Fired SiPM Plastic Scintillator Strip Array Distribution;Strip Serial Id", 13, -1.5, 11.5);
	TH1F *hSiPMPlaBarrFiredDist = new TH1F("hSiPMPlaBarrFiredDist", "Fired SiPM Plastic Scintillator Strip Barrel Distribution;Strip Serial Id", 27, -1.5, 25.5);
	objLs[1].push_back(hSiPMPlaArrFiredDist);
	objLs[1].push_back(hSiPMPlaBarrFiredDist);
	TH1F *hSiPMPlaArrMulti = new TH1F("hSiPMPlaArrMultipost", "SiPM Plastic Scintillator Strip Array Hit Multiplicity post-selection;Multiplicity", 8, -1.5, 6.5);
	TH1F *hSiPMPlaBarrMulti = new TH1F("hSiPMPlaBarrMultipost", "SiPM Plastic Scintillator Strip Barrel Hit Multiplicity post-selection;Multiplicity", 25, -1.5, 23.5);
	objLs[2].push_back(hSiPMPlaArrMulti);
	objLs[2].push_back(hSiPMPlaBarrMulti);
	TH2F *hSiPMPlaBarrHitPos = new TH2F("hSiPMPlaBarrHitPos", "SiPM Plastic Scintillator Strip Barrel Hit Position;Strip Serial Id;Hit Pos (from front end (z is smaller)) [mm]", 27, -1.5, 25.5, 501, -70.5, 430.5);
	objLs[4].push_back(hSiPMPlaBarrHitPos);

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$$ PID spectrum $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ //
	TH1F *hpoz = new TH1F("hpoz", "Momentum over Z;p/z MeV/c", 1000, -1000., 4000.);
	TH1F *haoz = new TH1F("haoz", "Mass(u) over Z;aoz", 500, -5.0, 5.0);
	TH1F *htof2 = new TH1F("htof2", "Time of Flight from T0_1 to TOFWall;tof2 [ns]", 500, -20., 100.);
	TH1F *hbeta2 = new TH1F("hbeta2", "beta from from T0_1 to TOFWall;beta2", 500, -0.5, 1.3);
	TH1F *hdcTOT = new TH1F("hdcTOT", "Averaged TOT of DC Anodes along One X Track;TOT [ns]", 1000, -100., 1000.);
	TH2F *htof2_vs_poz = new TH2F("htof2_vs_poz", "tof2 v.s. poz;p/z/ MeV/c;tof2 [ns]", 5000, -1000., 4000., 500, -20., 100.);
	TH2F *haoz_vs_poz = new TH2F("haoz_vs_poz", "aoz v.s. poz;p/z/ MeV/c;aoz", 5000, -1000., 4000., 500, -5.0, 5.0);
	TH2F *hdcTOT_vs_poz = new TH2F("hdcTOT_vs_poz", "Averaged DC Anode TOT v.s. poz;p/z/ MeV/c;dcTOT [ns]", 5000, -1000., 4000., 500, -100., 1000.);
	TH1F *hTOF_T1_pos = new TH1F("TOF_T1_pos", "TOF_T1_pos(U - D);pos [ns]", 10000, -50., 50.);
	TH2F *hpid00 = new TH2F("hpid00", "PID TOF-dE0 - RIBLL2;tof1 [ns];dE0 arb.", 2000, 110., 160., 1000, 0., 5.);
	TH2F *hpid01 = new TH2F("hpid01", "PID TOF-dE1 - RIBLL2;tof1 [ns];dE1 arb.", 2000, 110., 160., 1000, 0., 5.);
	objLs[5].push_back(hpoz); objLs[5].push_back(haoz); objLs[5].push_back(htof2);
	objLs[5].push_back(hbeta2); objLs[5].push_back(hdcTOT); objLs[5].push_back(htof2_vs_poz);
	objLs[5].push_back(haoz_vs_poz); objLs[5].push_back(hdcTOT_vs_poz);
	objLs[5].push_back(hpid00); objLs[5].push_back(hpid01);

	// ************************ MISC miscellaneous ****************************************** //
	TH2F *htof2sipmArr = new TH2F("htof2sipmArr", "htof2sipmArr;stripId;tof2 [ns]", 13, -1.5, 11.5, 500, -300., 500.);
	TH2F *hsipmArrToTrig = new TH2F("hsipmArrToTrig", "hsipmArrToTrig;stripId;timeToTrig [ns]", 13, -1.5, 11.5, 4000, -500., 2000.);
	TH2F *hsipmArrToTRef = new TH2F("hsipmArrToTRef", "hsipmArrToTRef;stripId;timeToTRef [ns]", 13, -1.5, 11.5, 3000, -500., 1500.);
	TH2F *hsipmBarrToTrig = new TH2F("hsipmBarrToTrig", "hsipmBarrToTrig;stripId;timeToTrig [ns]", 27, -1.5, 25.5, 3000, -500., 2000.);
	TH2F *hsipmBarrToTRef = new TH2F("hsipmBarrToTRef", "hsipmBarrToTRef;stripId;timeToTRef [ns]", 27, -1.5, 25.5, 3000, -500., 1500.);
	TH2F *hT0_1ToTrigUV = new TH2F("hT0_1ToTrigUV", "hT0_1ToTrigUV;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	TH2F *hT0_1ToTrigDV = new TH2F("hT0_1ToTrigDV", "hT0_1ToTrigDV;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	TH2F *hTOFWToTrigUV[2], *hTOFWTOTUV[2];
	hTOFWToTrigUV[0] = new TH2F("hLTOFWToTrigUV", "hLTOFWToTrigUV;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	hTOFWToTrigUV[1] = new TH2F("hRTOFWToTrigUV", "hRTOFWToTrigUV;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	hTOFWTOTUV[0] = new TH2F("hLTOFWTOTUV", "hLTOFWTOTUV;edgeNumId;TOT [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	hTOFWTOTUV[1] = new TH2F("hRTOFWTOTUV", "hRTOFWTOTUV;edgeNumId;TOT [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	TH2F *hDCToTrig = new TH2F("hDCToTrig", "hDCToTrig;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	TH2F *hDCToTrigNoPU = new TH2F("hDCToTrigNoPU", "hDCToTrig - dsca11==0;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	TH1F *htof1 = new TH1F("htof1", "tof1", 1000, -10., 300.);
	objLs[6].push_back(htof2sipmArr); objLs[6].push_back(hsipmArrToTrig);
	objLs[6].push_back(hTOF_T1_pos); objLs[6].push_back(hDCToTrig);
	if(vme) objLs[6].push_back(hDCToTrigNoPU);
	objLs[6].push_back(hT0_1ToTrigUV); objLs[6].push_back(hT0_1ToTrigDV);
	objLs[6].push_back(hTOFWToTrigUV[0]); objLs[6].push_back(hTOFWToTrigUV[1]);
	objLs[6].push_back(hTOFWTOTUV[0]); objLs[6].push_back(hTOFWTOTUV[1]);
	objLs[7].push_back(hsipmArrToTRef); objLs[7].push_back(hsipmBarrToTRef);
	objLs[6].push_back(hsipmBarrToTrig);
	objLs[6].push_back(htof1);

	TGraph *gTrkEff = new TGraph();
	gTrkEff->SetNameTitle("gTrkEff", "index-Tracking Efficiency diagram;event index;3D Efficiency");
	objLs[1].push_back(gTrkEff);

	int cnt_timeToTrig_T0_1UV = 0, cnt_timeToTrig_T0_1DV = 0;
	int cntTRef = 0;



	//////////////////// histograms for DCs around the target //////////////////////
	// DCs made by P. Ma //
	const char xy[] = "XY", UD[] = "UD";
	TH1F *hDCTaFiredDist[2][2][2], *hDCTaToTRef[2][2][2]; // [dcArrU-D], [dcArrU-D][DC0-1][X-Y];
	TH1F *hDCTaMulti[2][2][2][2], *hdtTa[2][3][3]; // [dcArrU-D], [dcArrU-D][DC0-1][X-Y][1-2];
	for(int i = 0; i < 2; i++){ // loop over MWDC arrays
		for(int j = 0; j < 2; j++){ // loop over the two MWDCs
			for(int k = 0; k < 2; k++){ // loop over XY SLayers
				for(int l = 0; l < 2; l++){ // loop over two sense wire layers in one SLayer
					// DC hit multiplicity
					sprintf(name, "hDCTaMultiDCArr%c_DC%d_%c%d", UD[i], j, xy[k], l);
					sprintf(title, "DC_Ta Hit Multiplicity of DC Array%c - DC%d - %c%d;multiplicity", UD[i], j, xy[k], l);
					hDCTaMulti[i][j][k][l] = new TH1F(name, title, 15, -4.5, 10.5);
					objLs[2].push_back(hDCTaMulti[i][j][k][l]);
				} // end for over layer 1 and 2
				// DC fired distribution
				sprintf(name, "hDCTaFiredDistDCArr%c_DC%d_%c", UD[i], j, xy[k]);
				sprintf(title, "Fired DC_Ta Anode Distribution of DC Array%c - DC%d - %c;DC Anode Serial Id", UD[i], j, xy[k]);
				hDCTaFiredDist[i][j][k] = new TH1F(name, title, 55, -4.5, 50.5);
				objLs[1].push_back(hDCTaFiredDist[i][j][k]);
				// DC drift time distribution
				sprintf(name, "HdtDC_TaArr%c_DC%d_%c", UD[i], j, xy[k]);
				sprintf(title, "Drift Time Distribution of DC_Ta Array%c - DC%d - %c;drift time [ns]", UD[i], j, xy[k]);
				hdtTa[i][j][k] = new TH1F(name, title, 500, -100., 400.);
				objLs[3].push_back(hdtTa[i][j][k]);
				// DC time to T-reference
				sprintf(name, "hDCTaToTRefArr%c_DC%d_%c", UD[i], j, xy[k]);
				sprintf(title, "DC_Ta Time to T-reterence - DC Array%c - DC%d - %c;timeToTRef [ns]", UD[i], j, xy[k]);
				hDCTaToTRef[i][j][k] = new TH1F(name, title, 4000, -2000., 2000.);
				objLs[7].push_back(hDCTaToTRef[i][j][k]);
			} // end for over X-Y
		} // end for over DCs
	} // end for over DC arrays
	TH2F *hDCTaToTrig = new TH2F("hDCTaToTrig", "hDCTaToTrig;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 8000, -4000., 6000.);
	objLs[6].push_back(hDCTaToTrig);


	// DCs from BUAA, made in Japan //
	TH1F *hPDCFiredDist[2][2][2], *hPDCToTRef[2][2][2]; // [pdcArrU-D], [pdcArrU-D][DC0-1][X-Y];
	TH1F *hPDCMulti[2][2][2][2], *hdtp[2][3][3]; // [pdcArrU-D], [pdcArrU-D][DC0-1][X-Y][1-2];
	for(int i = 0; i < 2; i++){ // loop over MWDC arrays
		for(int j = 0; j < 2; j++){ // loop over the two MWDCs
			for(int k = 0; k < 2; k++){ // loop over XY SLayers
				for(int l = 0; l < 2; l++){ // loop over two sense wire layers in one SLayer
					// DC hit multiplicity
					sprintf(name, "hPDCMultiDCArr%c_DC%d_%c%d", UD[i], j, xy[k], l);
					sprintf(title, "PDC Hit Multiplicity of DC Array%c - DC%d - %c%d;multiplicity", UD[i], j, xy[k], l);
					hPDCMulti[i][j][k][l] = new TH1F(name, title, 15, -4.5, 10.5);
					objLs[2].push_back(hPDCMulti[i][j][k][l]);
				} // end for over layer 1 and 2
				// DC fired distribution
				sprintf(name, "hPDCFiredDistDCArr%c_DC%d_%c", UD[i], j, xy[k]);
				sprintf(title, "Fired PDC Anode Distribution of DC Array%c - DC%d - %c;DC Anode Serial Id", UD[i], j, xy[k]);
				hPDCFiredDist[i][j][k] = new TH1F(name, title, 55, -4.5, 50.5);
				objLs[1].push_back(hPDCFiredDist[i][j][k]);
				// DC drift time distribution
				sprintf(name, "HdtPDCArr%c_DC%d_%c", UD[i], j, xy[k]);
				sprintf(title, "Drift Time Distribution of PDC Array%c - DC%d - %c;drift time [ns]", UD[i], j, xy[k]);
				hdtp[i][j][k] = new TH1F(name, title, 500, -100., 600.);
				objLs[3].push_back(hdtp[i][j][k]);
				// DC time to T-reference
				sprintf(name, "hPDCToTRefArr%c_DC%d_%c", UD[i], j, xy[k]);
				sprintf(title, "PDC Time to T-reterence - DC Array%c - DC%d - %c;timeToTRef [ns]", UD[i], j, xy[k]);
				hPDCToTRef[i][j][k] = new TH1F(name, title, 4000, -2000., 2000.);
				objLs[7].push_back(hPDCToTRef[i][j][k]);
			} // end for over X-Y
		} // end for over DCs
	} // end for over DC arrays
	TH2F *hPDCToTrig = new TH2F("hPDCToTrig", "hPDCToTrig;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 10000, -4000., 16000.);
	objLs[6].push_back(hPDCToTrig);
	TH2F *hPDCToTrigNoPU = new TH2F("hPDCToTrigNoPU", "hPDCToTrig - dsca11==0;edgeNumId;timeToTrig [ns]", 7, -1.5, 5.5, 10000, -4000., 16000.);
	if(vme) objLs[6].push_back(hPDCToTrigNoPU);
