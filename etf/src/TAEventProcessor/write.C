///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor/write.C.C										 //
//   write.C -- a scriplet encapuslated in TAEventProcessor.C for method			 //
// TAEventProcessor::Run().															 //
//   Introduction: write histograms, trees and other ROOT objects.					 //
//																					 //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/21.															     //
// Last modified: 2017/12/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


	char dir[8][64] = {"DaqStatistics", "FiredDistribution", "Multiplicity",
		 "DriftTimeDist", "PlaScintHitPos", "PID", "MISC", "TimeToTRef"};
	for(int i = 0; i < 8; i++){
		if(!f->FindObjectAny(dir[i])) f->mkdir(dir[i]); f->cd(dir[i]);
		for(TObject *&b : objLs[i]) if(b) b->Write("", TObject::kOverwrite);
	}
	f->cd("/"); for(TTree *&tree : objLsTree) tree->Write("", TObject::kOverwrite);
	treeTrack->Write("", TObject::kOverwrite);
	treePID3D->Write("", TObject::kOverwrite);

	for(auto &objls : objLs) for(TObject *&obj : objls){
		delete obj; obj = nullptr;
	}
	for(TTree *&tree : objLsTree){
		delete tree; tree = nullptr;
	}
	delete treeTrack; treeTrack = nullptr;
	delete treePID3D; treePID3D = nullptr;

	if(!f->FindObjectAny("VISUAL")) f->mkdir("VISUAL");
	f->cd("VISUAL");
	vis->DrawHitMap();

	cout << "\n";
	cout << "cntTRefCoincide: " << cntTRef << endl;
	cout << "cnt_timeToTrig_T0_1UV: " << cnt_timeToTrig_T0_1UV << endl;
	cout << "cnt_timeToTrig_T0_1DV: " << cnt_timeToTrig_T0_1DV << endl;
	
