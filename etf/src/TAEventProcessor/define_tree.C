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
// Last modified: 2017/12/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

	vector<TTree *> objLsTree; // a vector to manage tree pointers
	int bunchId; // trigger time
	double beta; // particle speed in RIBLL2
	int type[ntrMax], gGOOD[ntrMax]; // type: XUV; gGOOD: nFiredAnodePerLayer, ==2 specially...
	int id[ntrMax]; // tracks with the same track Id and different track type are projections of the same 3-D track.
	int nu[ntrMax][6], sfe16Id[ntrMax][6]; // fired anode id, SFE16 chip id
	double t[ntrMax][6], TOT_DC[ntrMax][6], TOT_DC_Avrg[ntrMax], r[ntrMax][6]; // hit pattern
	double k[ntrMax], b[ntrMax]; // track
	double aoz[ntrMax], aozdmin[ntrMax], poz[ntrMax]; // aoz
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
	double tRef; // T reference -> ~ 500+-100 to trigger
	TTree *treeTrack = new TTree("treeTrack", "pattern recognition tracks");
//	treeTrack->SetAutoSave(1e7);
	treeTrack->Branch("index", &index, "index/I");
	treeTrack->Branch("bunchId", &bunchId, "bunchId/I");
	treeTrack->Branch("tRef", &tRef, "tRef/D");
	treeTrack->Branch("beta", &beta, "beta/D");
	treeTrack->Branch("TOT_T0", TOT_T0, "TOT_T0[6]/D"); // see the comment above
	treeTrack->Branch("ntr", &ntr, "ntr/I");
	treeTrack->Branch("ntrT", &ntrT, "ntrT/I");
	treeTrack->Branch("ntrLs", ntrLs, "ntrLs[4][3]/I"); // DCArr-L-R-U-D -- [XUV - XY]
	treeTrack->Branch("nu", nu, "nu[ntr][6]/I");
	treeTrack->Branch("SFE16Id", sfe16Id, "SFE16Id[ntr][6]/I");
	treeTrack->Branch("gGOOD", gGOOD, "gGOOD[ntr]/I");
	treeTrack->Branch("type", type, "type[ntr]/I"); // track type: 1[LR][XUV]
	treeTrack->Branch("id", id, "id[ntr]/I"); // 3-D track id
	treeTrack->Branch("xMiss3D", xMiss3D, "xMiss3D[ntr][3]/D"); // 3D track coincidence test at z coordinates of the three DCs
	treeTrack->Branch("t", t, "t[ntr][6]/D");
	treeTrack->Branch("w", w, "w[ntr][6]/D");
	treeTrack->Branch("r", r, "r[ntr][6]/D");
	treeTrack->Branch("k", k, "k[ntr]/D"); // start for iterative fit, necessary
	treeTrack->Branch("b", b, "b[ntr]/D"); // start for iterative fit, necessary
	treeTrack->Branch("chi", chi, "chi[ntr][6]/D"); // residuals for each hit
	treeTrack->Branch("chi2", chi2, "chi2[ntr]/D"); // sum of chi^2
	treeTrack->Branch("Chi", Chi, "Chi[ntr]/D"); // sqrt(chi2/nFiredAnodeLayer)
	treeTrack->Branch("TOF", TOF, "TOF[ntr]/D");
	treeTrack->Branch("chit", chit, "chit[ntr][6]/D"); // drift time error suggested by fitted tracks
	treeTrack->Branch("tof2", tof2, "tof2[ntr]/D"); // tof from T0_1 to TOFW
	treeTrack->Branch("beta2", beta2, "beta2[ntr]/D"); // beta from T0_1 to TOFW
	treeTrack->Branch("TOT_DC", TOT_DC, "TOT_DC[ntr][6]/D");
	treeTrack->Branch("TOT_DC_Avrg", TOT_DC_Avrg, "TOT_DC_Avrg[ntr]/D");
	treeTrack->Branch("TOTUV", TOTUV, "TOTUV[ntr]/D"); // time over threshold, up side, V
	treeTrack->Branch("TOTUH", TOTUH, "TOTUH[ntr]/D"); // time over threshold, up side, H
	treeTrack->Branch("TOTDV", TOTDV, "TOTDV[ntr]/D"); // time over threshold, down side, V
	treeTrack->Branch("TOTDH", TOTDH, "TOTDH[ntr]/D"); // time over threshold, down side, H
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
		treeTrack->Branch("trkLenT", trkLenT, "trkLenT[ntr]/D"); // total trk len from Ta to TOFW
		treeTrack->Branch("angTaOut", yp, "angTaOut[ntr][2]/D"); // out angle at the target hit point
		treeTrack->Branch("aozdmin", aozdmin, "aozdmin[ntr]/D"); // start for iterative fit, necessary
	} // end if(IsPID())
	objLsTree.push_back(treeTrack);

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

	const int n3DtrMax = ntrMax/3;
	int n3Dtr, n3DtrT; // n3Dtr: N of trks in DCArrL-R; n3DtrT: N of trks in DCArrL-R-U-D 
	bool isDCArrR[n3DtrMax];
	double Chi3D[n3DtrMax], chi2_3D[n3DtrMax], chi3D[n3DtrMax][18];
	double k1[n3DtrMax], b1[n3DtrMax], k2[n3DtrMax], b2[n3DtrMax]; // x=k1*z+b1; y=k2*z+b2;
	// hit position in TOFW strip, rough and refine, from Down End to up end (0-1200).
	double TOF_posY[n3DtrMax], TOF_posY_refine[n3DtrMax]; // refine: calculate through 3D trks
	// 3D counterpart of treeTrack
	int firedStripId3D[n3DtrMax];
	double tof2_3D[n3DtrMax], dcTOTAvrg3D[n3DtrMax];
	double aoz3D[n3DtrMax], aozdmin3D[n3DtrMax], beta2_3D[n3DtrMax];
	double yp3D[n3DtrMax][2], poz3D[n3DtrMax], trkLenT3D[n3DtrMax];
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
	// 3D PID result using the same PID method
	if(IsPID()){
		treePID3D->Branch("aoz", aoz3D, "aoz[n3Dtr]/D");
		treePID3D->Branch("aozdmin", aozdmin3D, "aozdmin[n3Dtr]/D");
		treePID3D->Branch("beta2", beta2_3D, "beta2[n3Dtr]/D");
		treePID3D->Branch("angTaOut", yp3D, "angTaOut[n3Dtr][2]/D"); // out angle at the target hit point
		treePID3D->Branch("trkLenT", trkLenT3D, "trkLenT[n3Dtr]/D");
		treePID3D->Branch("poz", poz3D, "poz[n3Dtr]/D"); // MeV/c
	} // end if(IsPID())
	objLsTree.push_back(treePID3D);







