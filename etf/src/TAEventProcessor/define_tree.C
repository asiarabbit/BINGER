///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor/define_tree.C									 //
//   define_tree.C -- a scriplet encapuslated in TAEventProcessor.C for method		 //
// TAEventProcessor::Run().															 //
//   Introduction: definition for tree storing the data reconstuction result.		 //
//																					 //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/21.															     //
// Last modified: 2018/9/6, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

	vector<TTree *> objLsTree; // a vector to manage tree pointers
	int bunchId; // trigger time
	double mag, beta; // magnetic field/Telsa, particle speed in RIBLL2
	int type[ntrMax], gGOOD[ntrMax]; // type: XUV; gGOOD: nFiredAnodePerLayer, ==2 specially...
	int id[ntrMax]; // tracks with the same track Id and different track type are projections of the same 3-D track
	int nu[ntrMax][6], sfe16Id[ntrMax][6]; // fired anode id, SFE16 chip id
	double t[ntrMax][6], TOT_DC[ntrMax][6], TOT_DC_Avrg[ntrMax], r[ntrMax][6]; // hit pattern
	double k[ntrMax], b[ntrMax], d2[ntrMax]; // track
	double aoz[ntrMax], aozdmin[ntrMax], poz[ntrMax], brho[ntrMax]; // aoz
	double yp[ntrMax][2]; // dx/dz; dy/dz, on the target hit position
	// w: weight for weight addition of chi to chi2
	// chit: drift time error suggested by fitted tracks
	double chi[ntrMax][6], chi2[ntrMax], Chi[ntrMax], w[ntrMax][6], chit[ntrMax][6];
	double TOF[ntrMax], nStripStray[ntrMax], xMiss3D[ntrMax][3];
	// beta2: w.r.t. tof2, trkLenT: total track length from the target to the TOF wall
	double beta2[ntrMax], trkLenT[ntrMax];
	double tof2[ntrMax], taHitX[ntrMax]; // tof2: from T0_1 to TOFWall; taHitX: hit pos in target
	int firedStripId[ntrMax], sipmArrStripId[ntrMax];
	// TOT: time over threshold.    for a certain strip, U: upside; D: downside,
	// V: very high resolution mode, H: high resolutio mode
	double TOTUV[ntrMax], TOTUH[ntrMax], TOTDV[ntrMax], TOTDH[ntrMax];
	double TOT_T0[6]; // [1-6: T0V, T0H, T1LV, T1LH, T1RV, T1RH]
	double TOF_T1; // time tag of T1 plastic scintillator, beside the target.
	double tRef, tRef_pos, tof1; // T reference -> ~ 500+-100 to trigger
	double tof1vme = -9999., tof1tac = -9999.; // tof1 measured by VME Daq
	double dE0 = -9999., dE1 = -9999.; // energy loss before-after TA
	int dsca10, dsca11; // pileupSCA, pileUpAMP
	int tRef_UV_NL, tRef_DV_NL;
	double tRefLT_U[5]{}, tRefLT_D[5]{}; // array of leading times belonging to multiple leading edges
	TTree *treeTrack = new TTree("treeTrack", "pattern recognition tracks");
//	treeTrack->SetAutoSave(1e7);
	treeTrack->Branch("index", &index, "index/I");
	treeTrack->Branch("bunchId", &bunchId, "bunchId/I");
	treeTrack->Branch("mag", &mag, "mag/D");
	treeTrack->Branch("tof1", &tof1, "tof1/D"); // tof from T0_0 to T0_1
	treeTrack->Branch("tRef", &tRef, "tRef/D");
	treeTrack->Branch("tRefLT_U", tRefLT_U, "tRefLT_U[5]/D");
	treeTrack->Branch("tRefLT_D", tRefLT_D, "tRefLT_D[5]/D");
	treeTrack->Branch("tRef_pos", &tRef_pos, "tRef_pos/D");
	treeTrack->Branch("tRef_UV_NL", &tRef_UV_NL, "tRef_UV_NL/I"); // number of leading edge(s)
	treeTrack->Branch("tRef_DV_NL", &tRef_DV_NL, "tRef_DV_NL/I");
	treeTrack->Branch("beta", &beta, "beta/D");
	treeTrack->Branch("TOT_T0", TOT_T0, "TOT_T0[6]/D"); // see the comment above
	treeTrack->Branch("ntr", &ntr, "ntr/I");
	treeTrack->Branch("ntrT", &ntrT, "ntrT/I");
	treeTrack->Branch("ntrLs", ntrLs, "ntrLs[6][3]/I"); // DCArr-L-R-U-D -- [XUV - XY]
	treeTrack->Branch("nu", nu, "nu[ntrT][6]/I");
	treeTrack->Branch("SFE16Id", sfe16Id, "SFE16Id[ntrT][6]/I");
	treeTrack->Branch("gGOOD", gGOOD, "gGOOD[ntrT]/I");
	treeTrack->Branch("type", type, "type[ntrT]/I"); // track type: 1[LR][XUV]
	treeTrack->Branch("id", id, "id[ntrT]/I"); // 3-D track id
	treeTrack->Branch("xMiss3D", xMiss3D, "xMiss3D[ntrT][3]/D"); // 3D track coincidence test at z coordinates of the three DCs
	treeTrack->Branch("t", t, "t[ntrT][6]/D");
	treeTrack->Branch("w", w, "w[ntrT][6]/D");
	treeTrack->Branch("r", r, "r[ntrT][6]/D");
	treeTrack->Branch("k", k, "k[ntrT]/D"); // start for iterative fit, necessary
	treeTrack->Branch("b", b, "b[ntrT]/D"); // start for iterative fit, necessary
	treeTrack->Branch("d2", d2, "d2[ntrT]/D");
	treeTrack->Branch("chi", chi, "chi[ntrT][6]/D"); // residuals for each hit
	treeTrack->Branch("chi2", chi2, "chi2[ntrT]/D"); // sum of chi^2
	treeTrack->Branch("Chi", Chi, "Chi[ntrT]/D"); // sqrt(chi2/nFiredAnodeLayer)
	treeTrack->Branch("TOF", TOF, "TOF[ntrT]/D");
	treeTrack->Branch("chit", chit, "chit[ntrT][6]/D"); // drift time error suggested by fitted tracks
	treeTrack->Branch("tof2", tof2, "tof2[ntr]/D"); // tof from T0_1 to TOFW
	treeTrack->Branch("beta2", beta2, "beta2[ntr]/D"); // beta from T0_1 to TOFW
	treeTrack->Branch("TOT_DC", TOT_DC, "TOT_DC[ntrT][6]/D");
	treeTrack->Branch("TOT_DC_Avrg", TOT_DC_Avrg, "TOT_DC_Avrg[ntrT]/D");
	treeTrack->Branch("TOTUV", TOTUV, "TOTUV[ntrT]/D"); // time over threshold for TOFWall fired strip, up side, V
	treeTrack->Branch("TOTUH", TOTUH, "TOTUH[ntrT]/D"); // time over threshold for TOFWall fired strip, up side, H
	treeTrack->Branch("TOTDV", TOTDV, "TOTDV[ntrT]/D"); // time over threshold for TOFWall fired strip, down side, V
	treeTrack->Branch("TOTDH", TOTDH, "TOTDH[ntrT]/D"); // time over threshold for TOFWall fired strip, down side, H
	treeTrack->Branch("TOF_T1", &TOF_T1, "TOF_T1/D"); // time tag of T1 plastic scintillator
	treeTrack->Branch("firedStripId", firedStripId, "firedStripId[ntr]/I");
	treeTrack->Branch("nStripStray", nStripStray, "nStripStray[ntr]/D"); // distance of track to fired TOF Wall strip center
	// sipmArr part //
	if(sipmArr){
		treeTrack->Branch("taHitX", taHitX, "taHitX[ntr]/D");
		treeTrack->Branch("sipmArrStripId", sipmArrStripId, "sipmArrStripId[ntr]/I");
	} // end if(sipmArr)
	// PID part //
	if(IsPID()){
		treeTrack->Branch("aoz", aoz, "aoz[ntr]/D"); // mass over z; mass unit: amu
		treeTrack->Branch("poz", poz, "poz[ntr]/D"); // momentum over z; momentum unit: MeV/c
		treeTrack->Branch("brho", brho, "brho[ntr]/D"); // Tesla*m
		treeTrack->Branch("trkLenT", trkLenT, "trkLenT[ntr]/D"); // total trk len from Ta to TOFW
		treeTrack->Branch("angTaOut", yp, "angTaOut[ntr][2]/D"); // out angle at the target hit point
		treeTrack->Branch("aozdmin", aozdmin, "aozdmin[ntr]/D"); // start for iterative fit, necessary
	} // end if(IsPID())
	// for timing starts of two PDCArrays, v1190-slot9 and v1190-slot11
	double tRef_vme0ul[5], tRef_vme1ul[5], tRef_vme0dl[5], tRef_vme1dl[5];
	if(vme){ // vme - integrated Daq part
		treeTrack->Branch("tRef_vme0ul", tRef_vme0ul, "tRef_vme0ul");
		treeTrack->Branch("tRef_vme1ul", tRef_vme1ul, "tRef_vme1ul");
		treeTrack->Branch("tRef_vme0dl", tRef_vme0dl, "tRef_vme0dl");
		treeTrack->Branch("tRef_vme1dl", tRef_vme1dl, "tRef_vme1dl");
		treeTrack->Branch("tof1vme", &tof1vme, "tof1vme/D"); // tof from T0_0 to T0_1
		treeTrack->Branch("tof1tac", &tof1tac, "tof1tac/D"); // tof from T0_0 to T0_1
		treeTrack->Branch("dE0", &dE0, "dE0/D"); // energy loss before TA
		treeTrack->Branch("dE1", &dE1, "dE1/D"); // energy loss after TA
		treeTrack->Branch("dsca10", &dsca10, "dsca10/I"); // pileUp - count TOF stop
		treeTrack->Branch("dsca11", &dsca11, "dsca11/I"); // pileUp - count AMP gates
	}
	objLsTree.push_back(treeTrack);

	// Multiplicity
	short multi_DC[2][3][3][2]{}; // DCArr[L-R][DC0-1-2][XUV][X1-2]
	short multi_DCTa[2][2][2][2]{}; // DCTaArr[U-D][DC0-1][XY][X1-2]
	short multi_PDC[2][2][2][2]{}; // PDCArr[U-D][DC0-1][XY][X1-2]
	TTree *treeMulti = new TTree("treeMulti", "DC multiplicity");
	treeMulti->Branch("multi_DC", multi_DC, "multi_DC[2][3][3][2]/S");
	treeMulti->Branch("multi_DCTa", multi_DCTa, "multi_DCTa[2][2][2][2]/S");
	treeMulti->Branch("multi_PDC", multi_PDC, "multi_PDC[2][2][2][2]/S");
	objLsTree.push_back(treeMulti);

	// time to T-Reference
	// ********* this is for the last fired anode in a certain sense wire layer for convenience
	double ttRef_DC[2][3][3][2]{}; // DCArr[L-R][DC0-1-2][XUV][X1-2]
	double ttRef_DCTa[2][2][2][2]{}; // DCTaArr[U-D][DC0-1][XY][X1-2]
	double ttRef_PDC[2][2][2][2]{}; // PDCArr[U-D][DC0-1][XY][X1-2]
	double ttRef_TOFW[2]{}; // DCArr[L-R]
	TTree *treeTTRef = new TTree("treeTTRef", "Time to Reference");
	treeTTRef->Branch("ttRef_DC", ttRef_DC, "ttRef_DC[2][3][3][2]/D");
	treeTTRef->Branch("ttRef_DCTa", ttRef_DCTa, "ttRef_DCTa[2][2][2][2]/D");
	treeTTRef->Branch("ttRef_PDC", ttRef_PDC, "ttRef_PDC[2][2][2][2]/D");
	treeTTRef->Branch("ttRef_TOFW", ttRef_TOFW, "ttRef_TOFW[2]/D");
	objLsTree.push_back(treeTTRef);


	int multiSipmArr_pre,  hitIdLsSipmArr_pre[10];
	int multiSipmArr_post, hitIdLsSipmArr_post[10];
	double uvlTLsSipmArr_pre[10]; // uvl: up, Very high resl, leading edge
	if(sipmArr){
		TTree *treeSiPMPlaArr = new TTree("treeSiPMPlaArr", "SiPM Plastic Scintillator Bar Array Statistics");
	//	treeSiPMPlaArr->SetAutoSave(1e7);
		treeSiPMPlaArr->Branch("index", &index, "index/I");
		treeSiPMPlaArr->Branch("multi_pre", &multiSipmArr_pre, "multi_pre/I");
		treeSiPMPlaArr->Branch("multi_post", &multiSipmArr_post, "multi_post/I");
		treeSiPMPlaArr->Branch("hitIdLs_pre", hitIdLsSipmArr_pre, "hitIdLs_pre[multi_pre]/I");
		treeSiPMPlaArr->Branch("hitIdLs_post", hitIdLsSipmArr_post, "hitIdLs_post[multi_post]/I");
		treeSiPMPlaArr->Branch("uvlTLs_pre", uvlTLsSipmArr_pre, "uvlTLs_pre[multi_pre]/D");
		objLsTree.push_back(treeSiPMPlaArr);
	} // end if(sipmArr)

	int multiSipmBarr_pre,  hitIdLsSipmBarr_pre[24];
	int multiSipmBarr_post, hitIdLsSipmBarr_post[24];
	short hitStaLsSipmBarr_pre[24]; double uvlTLsSipmBarr_pre[24], dvlTLsSipmBarr_pre[24];
	double timeToTrigSipmBarr, timeToTRefSipmBarr;
	if(sipmBarr){
		TTree *treeSiPMPlaBarr = new TTree("treeSiPMPlaBarr", "SiPM Plastic Scintillator Strip Barrel Statistics");
	//	treeSiPMPlaBarr->SetAutoSave(1e7);
		treeSiPMPlaBarr->Branch("index", &index, "index/I");
		treeSiPMPlaBarr->Branch("tRef", &tRef, "tRef/D");
		treeSiPMPlaBarr->Branch("timeToTrig", &timeToTrigSipmBarr, "timeToTrig/D");
		treeSiPMPlaBarr->Branch("timeToTRef", &timeToTRefSipmBarr, "timeToTRef/D");
		treeSiPMPlaBarr->Branch("multi_pre", &multiSipmBarr_pre, "multi_pre/I");
		treeSiPMPlaBarr->Branch("multi_post", &multiSipmBarr_post, "multi_post/I");
		treeSiPMPlaBarr->Branch("hitIdLs_pre", hitIdLsSipmBarr_pre, "hitIdLs_pre[multi_pre]/I");
		treeSiPMPlaBarr->Branch("hitIdLs_post", hitIdLsSipmBarr_post, "hitIdLs_post[multi_post]/I");
		treeSiPMPlaBarr->Branch("hitStaLs_pre", hitStaLsSipmBarr_pre, "hitStaLs_pre[multi_pre]/S");
		treeSiPMPlaBarr->Branch("uvlTLs_pre", uvlTLsSipmBarr_pre, "uvlTLs_pre[multi_pre]/D");
		treeSiPMPlaBarr->Branch("dvlTLs_pre", dvlTLsSipmBarr_pre, "dvlTLs_pre[multi_pre]/D");
		objLsTree.push_back(treeSiPMPlaBarr);
	} // end if(sipmBarr)

	int multiTOFW_pre[2], hitIdLsTOFW_pre[2][30];
	int multiTOFW_post[2], hitIdLsTOFW_post[2][30];
	short hitStaLsTOFW_pre[2][30]; double uvlTLsTOFW_pre[2][30], dvlTLsTOFW_pre[2][30];
	TTree *treeTOFW[2]{};
	treeTOFW[0] = new TTree("treeTOFWL", "TOF Wall (L) Statistics");
	treeTOFW[1] = new TTree("treeTOFWR", "TOF Wall (R) Statistics");
	for(int i = 2; i--;) if(dcArr[i]){
//		treeTOFW[i]->SetAutoSave(1e7);
		treeTOFW[i]->Branch("index", &index, "index/I");
		treeTOFW[i]->Branch("multi_pre", multiTOFW_pre+i, "multi_pre/I");
		treeTOFW[i]->Branch("multi_post", multiTOFW_post+i, "multi_post/I");
		treeTOFW[i]->Branch("hitIdLs_pre", hitIdLsTOFW_pre[i], "hitIdLs_pre[multi_pre]/I");
		treeTOFW[i]->Branch("hitIdLs_post", hitIdLsTOFW_post[i], "hitIdLs_post[multi_post]/I");
		treeTOFW[i]->Branch("hitStaLs_pre", hitStaLsTOFW_pre[i], "hitStaLs_ptre[multi_pre]/S");
		treeTOFW[i]->Branch("uvlTLs_pre", uvlTLsTOFW_pre[i], "uvlTLs_pre[multi_pre]/D");
		treeTOFW[i]->Branch("dvlTLs_pre", dvlTLsTOFW_pre[i], "dvlTLs_pre[multi_pre]/D");
		objLsTree.push_back(treeTOFW[i]);
	}

	// trees for MUSICs
	TTree *treeMUSIC[3]{}; // [0-2]: MUSICM-L-Si: up-downstream of the target
	double deltaE[3]{}, Z[3]{}, MU_ch[3][6]{};
	for(int i = 0; i < 3; i++){
		deltaE[i] = -9999.; Z[i] = -9999.;
		for(int j = 0; j < 6; j++) MU_ch[i][j] = -9999.;
	}
	int pileUp[3]{}, nF_MU[3]{}; // N of Fired ch (pileup-ch excluded)
	int pileUpSCA; // pileup recorded by scaler
	treeMUSIC[0] = new TTree("treeMUSICM", "MUSIC upstream of the target");
	treeMUSIC[1] = new TTree("treeMUSICL", "MUSIC downstream of the target");
	treeMUSIC[2] = new TTree("treeSi", "Si downstream of the target");
	for(int i = 3; i--;) if(music[i]){
		treeMUSIC[i]->Branch("index", &index, "index/I");
		treeMUSIC[i]->Branch("deltaE", &deltaE[i], "deltaE/D");
		treeMUSIC[i]->Branch("Z", &Z[i], "Z/D");
		treeMUSIC[i]->Branch("pileUp", &pileUp[i], "pileUp/I");
		treeMUSIC[i]->Branch("pileUpSCA", &pileUpSCA, "pileUpSCA/I");
		treeMUSIC[i]->Branch("nF", &nF_MU[i], "nF/I");
	}
	if(music[0]){
		treeMUSIC[0]->Branch("ch", MU_ch[0], "ch[4]/D"); // 4 sampling units
		objLsTree.push_back(treeMUSIC[0]);
	}
	if(music[1]){
		treeMUSIC[1]->Branch("ch", MU_ch[1], "ch[8]/D"); // 8 sampling units
		objLsTree.push_back(treeMUSIC[1]);
	}
	if(music[2]){
		treeMUSIC[2]->Branch("ch", MU_ch[2], "ch[4]/D"); // 8 sampling units
		objLsTree.push_back(treeMUSIC[2]);
	}

	const int n3DtrMax = ntrMax/3;
	int n3Dtr, n3DtrT; // n3Dtr: N of trks in DCArrL-R; n3DtrT: N of trks in DCArrL-R-U-D 
	bool isDCArrR[n3DtrMax];
	double Chi3D[n3DtrMax], chi2_3D[n3DtrMax], chi3D[n3DtrMax][18];
	double k1[n3DtrMax], b1[n3DtrMax], k2[n3DtrMax], b2[n3DtrMax]; // x=k1*z+b1; y=k2*z+b2;
	// hit position in TOFW strip, rough and refine, from Down End to up end (0-1200)
	double TOF_posY[n3DtrMax], TOF_posY_refine[n3DtrMax]; // refine: calculate through 3D trks
	// 3D counterpart of treeTrack
	int firedStripId3D[n3DtrMax], dcTOT3Dcnt[n3DtrMax];
	double tof2_3D[n3DtrMax], dcTOTAvrg3D[n3DtrMax], dcTOTAvrg3D_Total; // DCArr-D + DCArr-R
	double aoz3D[n3DtrMax], aozdmin3D[n3DtrMax], beta2_3D[n3DtrMax];
	double yp3D[n3DtrMax][2], poz3D[n3DtrMax], brho3D[n3DtrMax], trkLenT3D[n3DtrMax];
	t3DTrkInfo trk3DIf[n3DtrMax]; t3DPIDInfo pid3DIf[n3DtrMax];
	TTree *treePID3D = new TTree("treePID3D", "PID using 3D Tracking and Refinement");
//	treePID3D->SetAutoSave(1e7);
	treePID3D->Branch("index", &index, "index/I");
	treePID3D->Branch("n3Dtr", &n3Dtr, "n3Dtr/I");
	treePID3D->Branch("n3DtrT", &n3DtrT, "n3DtrT/I");
	treePID3D->Branch("isDCArrR", isDCArrR, "isDCArrR[n3DtrT]/O");
	treePID3D->Branch("Chi", Chi3D, "Chi[n3DtrT]/D");
	treePID3D->Branch("chi2", chi2_3D, "chi2[n3DtrT]/D");
	treePID3D->Branch("chi", chi3D, "chi[n3DtrT][18]/D"); // residuum [x6->u6->v6]
	treePID3D->Branch("k1", k1, "k1[n3DtrT]/D");
	treePID3D->Branch("b1", b1, "b1[n3DtrT]/D");
	treePID3D->Branch("k2", k2, "k2[n3DtrT]/D");
	treePID3D->Branch("b2", b2, "b2[n3DtrT]/D");
	treePID3D->Branch("TOF_posY", TOF_posY, "TOF_posY[n3DtrT]/D"); // rough TOF hit postion
	treePID3D->Branch("TOF_posY_refine", TOF_posY_refine, "TOF_posY_refine[n3DtrT]/D");
	treePID3D->Branch("firedStripId", firedStripId3D, "firedStripId[n3DtrT]/I");
	treePID3D->Branch("tof2", tof2_3D, "tof2[n3DtrT]/D");
	treePID3D->Branch("TOT_DC_Avrg", dcTOTAvrg3D, "TOT_DC_Avrg[n3DtrT]/D");
	treePID3D->Branch("TOT_DC_cnt", dcTOT3Dcnt, "TOT_DC_cnt[n3DtrT]/I"); // number of valid TOTs
	treePID3D->Branch("TOT_DC_Avrg_Total", &dcTOTAvrg3D_Total, "TOT_DC_Avrg_Total/D"); // TOT: DCArr-D + DCArr-R
	// 3D PID result using the same PID method
	if(IsPID()){
		treePID3D->Branch("aoz", aoz3D, "aoz[n3Dtr]/D");
		treePID3D->Branch("aozdmin", aozdmin3D, "aozdmin[n3Dtr]/D");
		treePID3D->Branch("beta2", beta2_3D, "beta2[n3Dtr]/D");
		treePID3D->Branch("poz", poz3D, "poz[n3Dtr]/D"); // MeV/c
		treePID3D->Branch("brho", brho3D, "brho[n3Dtr]/D"); // T.m
		treePID3D->Branch("angTaOut", yp3D, "angTaOut[n3Dtr][2]/D"); // out angle at the target hit point
		treePID3D->Branch("trkLenT", trkLenT3D, "trkLenT[n3Dtr]/D");
	} // end if(IsPID())
	objLsTree.push_back(treePID3D);


	int multi_opfa;
	double ul_opfa[40][5], dl_opfa[40][5];
	double ut_opfa[40][5], dt_opfa[40][5];
	double pos_opfa[40];
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 5; j++){
			ul_opfa[i][j] = -9999; dl_opfa[i][j] = -9999;
		}
		pos_opfa[i] = -9999.;
	}
	TTree *treeOpticFiberArr = new TTree("treeOpticFiber", "Optic Fiber array");
	if(opfa){
		treeOpticFiberArr->Branch("index", &index, "index/I");
		treeOpticFiberArr->Branch("multi", &multi_opfa, "multi/I");
		treeOpticFiberArr->Branch("ul", ul_opfa, "ul[40][5]/D");
		treeOpticFiberArr->Branch("dl", dl_opfa, "dl[40][5]/D");
		treeOpticFiberArr->Branch("ut", ut_opfa, "ut[40][5]/D");
		treeOpticFiberArr->Branch("dt", dt_opfa, "dt[40][5]/D");
		treeOpticFiberArr->Branch("pos", pos_opfa, "pos[40]/D");
		objLsTree.push_back(treeOpticFiberArr);
	}
	
	
	// pile up evidence provided by DCs //
	// for PDCs
	// NLM: maximum number of leading edges
	// LTM: maximum leading time
	int PDC_NLM[2][2][2][2]{}; // [U-D][PDC0-1][X-Y][X1-X2]
	double PDC_LTM[2][2][2][2]{};
	TTree *treePDCPileUp = new TTree("treePDCPileUp", "treePDCPileUp");
	if(vme){
		treePDCPileUp->Branch("index", &index, "index/I");
		treePDCPileUp->Branch("PDC_NLM", PDC_NLM, "PDC_NLM[2][2][2][2]/I");
		treePDCPileUp->Branch("PDC_LTM", PDC_LTM, "PDC_LTM[2][2][2][2]/D");
		objLsTree.push_back(treePDCPileUp);
	}
	// for DCs
	int DC_NLM[2][3][3][2]{}; // [L-R][DC0-1-2][X-U-V][X1-X2]
	double DC_LTM[2][3][3][2]{};
	TTree *treeDCPileUp[2]{};
	treeDCPileUp[0] = new TTree("treeDCLPileUp", "PileUp tree for DCArr L");
	treeDCPileUp[1] = new TTree("treeDCRPileUp", "PileUp tree for DCArr R");
	for(int i = 0; i < 2; i++){
		if(dcArr[i]){
			treeDCPileUp[i]->Branch("index", &index, "index/I");
			treeDCPileUp[i]->Branch("DC_NLM", DC_NLM[i], "DC_NLM[3][3][2]/I");
			treeDCPileUp[i]->Branch("DC_LTM", DC_LTM[i], "DC_LTM[3][3][2]/D");
			objLsTree.push_back(treeDCPileUp[i]);
		} // end if
	} // end for over DCArrL-R





