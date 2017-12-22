///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor/fill_post.C										 //
//   fill_post.C -- a scriplet encapuslated in TAEventProcessor.C for method		 //
// Fill histograms and trees after tracking.										 //
// TAEventProcessor::Run().															 //
//   Introduction: fill histograms within the loop of runs.							 //
//																					 //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/29.															     //
// Last modified: 2017/11/24, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


		///////////////////// OUTPUT THE TRACKING RESULT //////////////////////////////////////////
		ntr = track_ls.size() < ntrMax ? track_ls.size() : ntrMax;
		// TAVisual::Fill //
		if(ntr > ntrMax){
			TAPopMsg::Warn("TAEventProcessor", "Run: ntr is larger than ntrMax: secLen: %d  index: %d", entry_t.channelId, index);
		}
		for(int j = 0; j < ntr; j++){ // assign track_ls to treeTrack. loop over tracks
			tTrack *&tra = track_ls[j];
			type[j] = tra->type; id[j] = tra->id;
			k[j] = tra->k; b[j] = tra->b; TOF[j] = tra->TOF;
			gGOOD[j] = tra->gGOOD; chi2[j] = tra->chi2; Chi[j] = tra->Chi;
			firedStripId[j] = tra->firedStripId; nStripStray[j] = tra->nStripStray;			
			memcpy(xMiss3D[j], tra->xMiss3D, sizeof(xMiss3D[j]));

			const short dcArrId = (type[j]/10)%10; // 0: dcArrL; 1: dcArrR
			if(0 != dcArrId && 1 != dcArrId)
				TAPopMsg::Error("TAEvPro", "Run: invalid dcArrId: %d", dcArrId);
			const int dcType = type[j]%10; // [0-1-2]: [X-U-V]
			if(firedStripId[j] >= 0){
				TAPlaStrip *strip = tofw[dcArrId]->GetStrip(firedStripId[j]);
				TOTUV[j] = strip->GetUV()->GetTOT(); TOTUH[j] = strip->GetUH()->GetTOT();
				TOTDV[j] = strip->GetDV()->GetTOT(); TOTDH[j] = strip->GetDH()->GetTOT();
			}
			// particle identification //

			int ii = 0; taHitX[j] = -9999.; tof2[j] = -9999.; sipmArrStripId[j] = -1;
			int priority = 0, priorityM = 1000; // to select the optimal fired sipmArr strip time
			if(sipmArr->GetNFiredStrip() >= 1){
				for(TAChannel *&ch : sipmArr->GetChArr()){
					if(ch->GetFiredStatus()){
						// sipm -> TOFWall
						double time = tra->TOF - sipmArr->GetStripTime(ch->GetSerialId());
						if(time < 50. && time > 30.) priority = 1;
						else if(time < 60. && time > 20.) priority = 2;
						else if(time < 80. && time > 10.) priority = 3;
						else if(time < 100. && time > 0.) priority = 4;
						else if(time < 200. && time > -10.) priority = 5;
						else if(time < 400. && time > -100.) priority = 6;
						else if(time < 800. && time > -200.) priority = 7;
						else priority = 8;
						if(priority < priorityM){
							priorityM = priority;
							sipmArrStripId[j] = ii;
							taHitX[j] = sipmArr->GetStripX(ii);
						}
						if(8 == priorityM){ // abnormally fired
							taHitX[j] = -9999.;
						}
						htof2sipmArr->Fill(ii, time);
					} // end if(ch->...)
					ii++;
				} // end for over channels
			} // end outer if
			if(tRef != -9999. && firedStripId[j] >= 0){
				tof2[j] = tofw[dcArrId]->GetStripTime(firedStripId[j], tRef, 9., 40.) - tRef;
			}

			yp[j][0] = -9999.; yp[j][1] = -9999.; trkLenT[j] -9999.;
			aoz[j] = -9999.; aozdmin[j] = -9999.; beta2[j] = -1.;
			if(0 == dcType && tof2[j] > 0. && -9999. != taHitX[j]){ // X tracks
				if(IsPID()){
					double p[4] = {k[j], 0., b[j], 0.}; // {k1, k2, b1, b2}
					pid->Fly(tof2[j], taHitX[j], p, dcArrId); // , false
					aoz[j] = pid->GetAoZ(); aozdmin[j] = pid->GetChi();
					beta2[j] = pid->GetBeta(); poz[j] = pid->GetPoZ(); // MeV/c
					pid->GetTargetExitAngle(yp[j]); trkLenT[j] = pid->GetTotalTrackLength();
					if(aozdmin[j] > 0.5 || -9999. == aoz[j]) cntaozWrong++;
//					cout << "aozdmin[j]: " << aozdmin[j] << endl; getchar(); // DEBUG
					cntaoz++;
				}
			} // end the lengthy if
			// track information
			for(int k = 0; k < 6; k++){ // loop over anode layers
				short dcId = k/2; // DC[0-1-2]
				short layerOption = k%2+1; // 1: X(U,V)1; 2: X(U,V)2
				nu[j][k] = tra->nu[k];
				t[j][k] = tra->t[k];
				w[j][k] = tra->w[k];
				r[j][k] = tra->r[k];
				chi[j][k] = tra->chi[k];
				hdt[dcArrId][dcId][dcType]->Fill(tra->t[k]);
				// TOT of DC signals
				if(nu[j][k] >= 0){
					TAMWDC *dc = dcArr[dcArrId]->GetMWDC(dcId);
					TAAnode *ano = dc->GetAnode(dcType, layerOption, nu[j][k]);
					TOT_DC[j][k] = tra->dcTOT[k] = ano->GetTOT();
					sfe16Id[j][k] = ((TAAnodePara*)ano->GetPara())->GetSFE16Id();
				} // end if
				else TOT_DC[j][k] = -9999.;
			} // end for over k
			TOT_DC_Avrg[j] = tra->dcTOTAvrg();
			index = tra->index; // indexes are the same in the loop.
			tra->beta = beta2[j];
			// pid fill //
			if(-9999. != tof2[j]) htof2->Fill(tof2[j]);
			if(beta2[j] >= 0.) hbeta2->Fill(beta2[j]);
			if(-9999. != TOT_DC_Avrg[j]) hdcTOT->Fill(TOT_DC_Avrg[j]);
			if(aoz[j] != -9999. && aozdmin[j] < 0.1){
				haoz->Fill(aoz[j]);
				if(beta2[j] >= 0.){
					hpoz->Fill(poz[j]);
					haoz_vs_poz->Fill(poz[j], aoz[j]);
					if(-9999. != tof2[j]) htof2_vs_poz->Fill(poz[j], tof2[j]);
					if(-9999. != TOT_DC_Avrg[j]) hdcTOT_vs_poz->Fill(poz[j], TOT_DC_Avrg[j]);
				}
			}
			cntTrk++;
			if(id[j] != -1) cnt3DTrk++;
		} // end for over j
		// correct drift time and refit with the update, together with pareticle identification //
//		for(auto &t : trk3DIf) t.initialize(); for(auto &t : pid3DIf) t.initialize();
		RefineTracks(n3Dtr, trk3DIf, tof2, taHitX);
		RefinePID(n3Dtr, trk3DIf, pid3DIf);
//		cout << "n3Dtr: " << n3Dtr << endl; getchar(); // DEBUG
		// assignment for the filling of treePID3D
		for(int jj = 0; jj < n3Dtr; jj++){
			isDCArrR[jj] = trk3DIf[jj].isDCArrR;
			Chi3D[jj] = trk3DIf[jj].Chi; chi2_3D[jj] = trk3DIf[jj].chi2;
			memcpy(chi3D[jj], trk3DIf[jj].chi, sizeof(chi3D[jj]));
			k1[jj] = trk3DIf[jj].k1; k2[jj] = trk3DIf[jj].k2;
			b1[jj] = trk3DIf[jj].b1; b2[jj] = trk3DIf[jj].b2;
			aoz3D[jj] = pid3DIf[jj].aoz; aozdmin3D[jj] = pid3DIf[jj].aozdmin;
			beta2_3D[jj] = pid3DIf[jj].beta2; poz3D[jj] = pid3DIf[jj].poz;
			yp3D[jj][0] = pid3DIf[jj].angTaOut[0]; yp3D[jj][1] = pid3DIf[jj].angTaOut[1];
			trkLenT3D[jj] = pid3DIf[jj].trkLenT;
		} // end for over 3D tracks
		// update drift time and drift distance
		for(int j = 0; j < ntr; j++){
			tTrack *&tra = track_ls[j];
			for(int k = 0; k < 6; k++){
				t[j][k] = tra->t[k];
				r[j][k] = tra->r[k];
			}
		} // end for over tracks
		treePID3D->Fill();
		for(TTree *&tree : objLsTree) tree->Fill();

		if(0) vis->FillHitMap();
		static int jj = 0;
		if(0) if(jj < 50){
			jj++;
			static int i0 = 0;
			if(0 == i0){ // to make sure that this block would only be carried out once
				if(!f->FindObjectAny("VISUAL")) f->mkdir("VISUAL");
				f->cd("VISUAL"); i0++;
			}
			vis->FillEvent();
			vis->DrawEventSnap(index);
		} // end if
		// stop filling the curved track in the dipole magnet
		else TAPID::Instance()->SetCurveGraph(nullptr);


		if(index % 1 == 0){
			cout << "Processing idx " << index << " dataSec " << cntSec;
			cout << " trk " << cntTrk << " 3Dtrk " << cnt3DTrk / 3;
			cout << " naoz " << cntaoz << " naozBad " << cntaozWrong << "\r" << flush; // cntaozWrong
		}















