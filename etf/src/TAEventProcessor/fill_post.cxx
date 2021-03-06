///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAEventProcessor/fill_post.cxx																		 //
//   fill_post.C -- a scriplet encapuslated in TAEventProcessor.C for method				 //
// Fill histograms and trees after tracking.																				 //
// TAEventProcessor::Run()																													 //
//   Introduction: fill histograms within the loop of runs.													 //
//																																									 //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/10/29.																													     //
// Last modified: 2018/9/13, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////




		///////////////////// OUTPUT THE TRACKING RESULT //////////////////////////////////////////
		if(ntr > ntrMax){
			TAPopMsg::Warn("TAEventProcessor", "Run: ntr is larger than ntrMax: secLen: %d  index: %d", entry_t.channelId, index);
		}
		ntrT = track_ls.size();
		for(int j = 0; j < ntrT; j++){ // assign track_ls to treeTrack. loop over tracks
			tTrack *&tra = track_ls[j];
			type[j] = tra->type; id[j] = tra->id;
			k[j] = tra->k; b[j] = tra->b; TOF[j] = tra->TOF; d2[j] = tra->dsquare;
			gGOOD[j] = tra->gGOOD; chi2[j] = tra->chi2; Chi[j] = tra->Chi;

			const short dcArrId = (type[j]/10)%10; // 0: dcArrL; 1: dcArrR; 2: dcArrrU; 3: dcArrD;
			if(   0 != dcArrId && 1 != dcArrId // DCArr-LR
			   && 2 != dcArrId && 3 != dcArrId // DCArr-UD
			   && 4 != dcArrId && 5 != dcArrId ) // DCArr-PDC-UD
				TAPopMsg::Error("TAEventProcessor", "Run: invalid dcArrId: %d", dcArrId);
			if(0 == dcArrId || 1 == dcArrId){
				firedStripId[j] = tra->firedStripId; nStripStray[j] = tra->nStripStray;
				memcpy(xMiss3D[j], tra->xMiss3D, sizeof(xMiss3D[j]));
			} // end if(0 == dcArrId || 1 == dcArrId)
			const int dcType = type[j]%10; // [0-1-2 or 0-1]: [X-U-V or X-Y]
			if(dcArrId < 2 && type[j]%10 == 0 && firedStripId[j] >= 0){ // X trk
				TAPlaStrip *strip = tofw[dcArrId]->GetStrip(firedStripId[j]);
				TOTUV[j] = strip->GetUV()->GetTOT(); TOTUH[j] = strip->GetUH()->GetTOT();
				TOTDV[j] = strip->GetDV()->GetTOT(); TOTDH[j] = strip->GetDH()->GetTOT();
			}
			// track information
			const double &kl = k[j]; // to distinguish slope k from incremental k below
			static const bool usingPDC = bool(GetGPar()->Val(83));
			for(int k = 0; k < 6; k++){ // loop over anode layers
				short dcId = k/2; // DC[0-1-2]
				short layerOption = k%2+1; // 1: X(U,V)1; 2: X(U,V)2
				nu[j][k] = tra->nu[k];
				t[j][k] = tra->t[k];
				w[j][k] = tra->w[k];
				r[j][k] = tra->r[k];
				chi[j][k] = tra->chi[k];
				const double dt = tra->t[k];
				if(-9999. != dt){
					if(0 == dcArrId || 1 == dcArrId) hdt[dcArrId][dcId][dcType]->Fill(dt);
					if(2 == dcArrId || 3 == dcArrId){
						if(!usingPDC) hdtTa[dcArrId-2][dcId][dcType]->Fill(dt);
						else hdtp[dcArrId-2][dcId][dcType]->Fill(dt);
					}
					if(4 == dcArrId || 5 == dcArrId){
						if(!usingPDC) hdtp[dcArrId-2][dcId][dcType]->Fill(dt);
						else hdtTa[dcArrId-2][dcId][dcType]->Fill(dt);
					}
				} // end if
				// TOT of DC signals
				if(nu[j][k] >= 0){
					TAMWDC *dc = nullptr;
					if(0 == dcArrId || 1 == dcArrId) dc = dcArr[dcArrId]->GetMWDC(dcId);
					if((usingPDC && (2 == dcArrId || 3 == dcArrId))){
						dc = pdcArr2[dcArrId-2]->GetMWDC(dcId);
					}
					if((!usingPDC && (2 == dcArrId || 3 == dcArrId))){
						dc = dcArr2[dcArrId-2]->GetMWDC(dcId);
					}
					TAAnode *ano = dc->GetAnode(dcType, layerOption, nu[j][k]);
					TOT_DC[j][k] = tra->dcTOT[k] = ano->GetTOT();
					sfe16Id[j][k] = ano->GetAnodePara()->GetSFE16Id();
					chit[j][k] = ano->GetDriftTime(r[j][k]+chi[j][k], kl) - t[j][k];
				} // end if
				else{
					TOT_DC[j][k] = -9999.; chit[j][k] = -9999.;
					sfe16Id[j][k] = -1;
				}
			} // end for over k
			TOT_DC_Avrg[j] = tra->dcTOTAvrg();

			// particle identification //
			int ii = 0; taHitX[j] = -9999.; tof2[j] = -9999.; sipmArrStripId[j] = -1;
			int priority = 0, priorityM = 1000; // to select the optimal fired sipmArr strip time
			if(sipmArr && sipmArr->GetNFiredStrip() >= 1){
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
			if(tRef != -9999. && firedStripId[j] >= 0 && dcArrId < 2){
				tof2[j] = tofw[dcArrId]->GetStripTime(firedStripId[j], tRef, 40., 90.) - tRef;
				tof2[j] += GetGPar()->Val(107);
			}
			// initialization //
			yp[j][0] = -9999.; yp[j][1] = -9999.; trkLenT[j] = -9999.;
			aoz[j] = -9999.; aozdmin[j] = -9999.; beta2[j] = -1.;
			poz[j] = -9999.; brho[j] = -9999.;
			x2_PID[j][0] = -9999.; x2_PID[j][1] = -9999.; dx2_PID[j] = -9999.;
			if(0 == dcType && tof2[j] > 0. && -9999. != taHitX[j]){ // X tracks
				if(IsPID()){
					double p[4] = {k[j], 0., b[j], 0.}; // {k1, k2, b1, b2}
					pid->FlyPion(tof2[j], taHitX[j], p, dcArrId, (TAPID::OPTION)(GetGPar()->Val(108)));
					aoz[j] = pid->GetAoZ(); aozdmin[j] = pid->GetChi();
					beta2[j] = pid->GetBeta(); poz[j] = pid->GetPoZ(); // MeV/c
					brho[j] = pid->GetBrho(); // T.m
					pid->GetTargetExitAngle(yp[j]); trkLenT[j] = pid->GetTotalTrackLength();
					if(aozdmin[j] > 1. || -9999. == aoz[j]) cntaozWrong++;
//					cout << "aozdmin[j]: " << aozdmin[j] << endl; // DEBUG
//					cout << "aoz[j]: " << aoz[j] << endl; // DEBUG
//					getchar(); // DEBUG
					cntaoz++;
				}
			} // end the lengthy if
			tra->beta = beta2[j];
			if(-9999. != tof2[j]) htof2->Fill(tof2[j]);
			if(beta2[j] >= 0.) hbeta2->Fill(beta2[j]);
			if(-9999. != TOT_DC_Avrg[j]) hdcTOT->Fill(TOT_DC_Avrg[j]);
			if(-9999. != aoz[j] && aozdmin[j] < 0.1){
				haoz->Fill(aoz[j]);
				if(beta2[j] >= 0.){
					hpoz->Fill(poz[j]);
					haoz_vs_poz->Fill(poz[j], aoz[j]);
					if(-9999. != tof2[j]) htof2_vs_poz->Fill(poz[j], tof2[j]);
					if(-9999. != TOT_DC_Avrg[j]) hdcTOT_vs_poz->Fill(poz[j], TOT_DC_Avrg[j]);
				}
			}
		} // end for over j

		// correct drift time and refit with the update, together with particle identification //
//		for(auto &t : trk3DIf) t.initialize(); for(auto &t : pid3DIf) t.initialize();
		n3Dtr = 0;
		if(ntr >= 3) RefineTracks(n3Dtr, trk3DIf, tof2, taHitX);
		if(IsPID() && n3Dtr > 0) RefinePID(n3Dtr, trk3DIf, pid3DIf);
		// count 3D trks from DCArrL and DCArrR
		for(int j = 0; j < n3Dtr; j++){
			if(trk3DIf[j].isDCArrR) n3DtrLs[1]++; // DCArrR
			else n3DtrLs[0]++; // DCArrL
		}

		//////// refine tracks from DCArrU-D if there's only one trkX and one trkY in U or D /////////
		n3DtrT = n3Dtr;
		if((1 == ntrLs[2][0] && 1 == ntrLs[2][1]) || (1 == ntrLs[3][0] && 1 == ntrLs[3][1])){
			// obtain the track id of the two 3D tracks //
			tTrack *tArr[4]{}; // [0-1-2-3]: [UX-UY-DX-DY]
			for(tTrack *&t : track_ls){
				const int dcArrId = t->type / 10 % 10; // [0-1-2-3]: [L-R-U-D]
				if(2 == dcArrId || 3 == dcArrId){ // DCArrU-D
					const int dcType = t->type % 10;
					const int sub = (dcArrId - 2) * 2 + dcType;
					if(sub > 3) TAPopMsg::Error("TAEventProcessor", "Run: DCArrUD 3D trk \
fitting, sub is abnormal: sub: %d", sub);
					tArr[sub] = t;
				} // end if
			} // end for
			//////////// do the 3D fitting ///////////////
			if(Is3DTracking() && ((tArr[0] && tArr[1]) || (tArr[2] && tArr[3]))){
//				cout << "here we go" << endl; getchar(); // DEBUG
				for(int jj = 2; jj--;){ // loop over 3D tracks (or DCArr) jj 0-1 -> U-D
//					cout << "jj: " << jj << endl; // DEBUG
//					cout << "!tArr[jj*2]: " << (!tArr[jj*2]) << endl; // DEBUG
					if(nullptr == tArr[jj*2] || nullptr == tArr[jj*2+1]) continue; // select valid DCArr (U or D)
					//////  Assign anode position and track information for 3D fitting /////////////
					const int nF = tArr[jj*2]->nFiredAnodeLayer + tArr[jj*2+1]->nFiredAnodeLayer;
					double Ag[nF][3], ag[nF][3], rr[nF], trkVec[4];
					trkVec[0] = tArr[jj*2]->k;   trkVec[2] = tArr[jj*2]->b;   // X
					trkVec[1] = tArr[jj*2+1]->k; trkVec[3] = tArr[jj*2+1]->b; // Y
					int tmp = 0;
					for(int l = 0; l < 2; l++){ // loop over X-Y
						for(int k = 0; k < 4; k++){ // loop over 4 andoe layers
							const int nu = tArr[jj*2+l]->nu[k];
							if(-1 == nu) continue;
							static bool usingPDC = GetGPar()->Val(83);
							TAMWDC *dc = dc2[jj][k/2];
							if(usingPDC) dc = pdc2[jj][k/2];
							TAAnode *ano = dc->GetAnode(l, k%2 + 1, nu);
							TAAnodePara *anoPar = ano->GetAnodePara();
							anoPar->GetGlobalCenter(Ag[tmp]);
							anoPar->GetGlobalDirection(ag[tmp]);
							rr[tmp] = tArr[jj*2+l]->r[k];
							tmp++;
						} // end for over k (anode layer)
					} // end for over l (X-Y)
//					cout << "UD, Before correction,\n"; // DEBUG
//					cout << "UD, k1: " << trkVec[0] << "\tk2: " << trkVec[1] << "\tb1: " << trkVec[2] << "\tb2: " << trkVec[3] << endl; // DEBUG
					TAMath::BFGS4(Ag, ag, trkVec, rr, nF); // refine and refresh trkVec
//					cout << "UD, After correction,\n"; // DEBUG
//					cout << "UD, k1: " << trkVec[0] << "\tk2: " << trkVec[1] << "\tb1: " << trkVec[2] << "\tb2: " << trkVec[3] << endl; // DEBUG
//					getchar(); // DEBUG
					// pass the fitting result to treePID3D //

					trk3DIf[n3DtrT].initialize(); // initialization

					tmp = 0; trk3DIf[n3DtrT].chi2 = 0.;
					// assign residuals and prepare for the tree filling
					for(int l = 0; l < 2; l++){ // loop over X-Y
						for(int j = 0; j < 4; j++){ // DC0X1X2-DC1X1X2
							if(tArr[jj*2+l]->nu[j] != -1){ // one measure point
								trk3DIf[n3DtrT].chi[l*6+j] = TAMath::dSkew(Ag[tmp], ag[tmp], trkVec) - rr[tmp];
								trk3DIf[n3DtrT].chi2 += trk3DIf[n3DtrT].chi[l*6+j] * trk3DIf[n3DtrT].chi[l*6+j];
								tmp++;
//								cout << "chi[" << l*6+j << "]: " << trk3DIf[n3DtrT].chi[l*6+j] << endl; // DEBUG
							} // end if
						} // end for over j
					} // end for over l
					trk3DIf[n3DtrT].Chi = sqrt(trk3DIf[n3DtrT].chi2/(nF-4)); // nF-4: ndf
					trk3DIf[n3DtrT].k1 = trkVec[0]; trk3DIf[n3DtrT].b1 = trkVec[2];
					trk3DIf[n3DtrT].k2 = trkVec[1]; trk3DIf[n3DtrT].b2 = trkVec[3];
					trk3DIf[n3DtrT].isDCArrR = jj;
					trk3DIf[n3DtrT].firedStripId = -2; // -2: typical value for trks in DCArrUD
					//// Get averaged TOT of DC signals ////
					// calculate averaged TOT over all the hit anode layers
					double TOTAvrgtmp = 0.; int TOTcnt = 0;
					// get the average, temporary, for the following filtering
					for(int j = 0; j < 2; j++){ // loop over XY
						for(int k = 0; k < 4; k++){ // loop over four anode layers in the two MWDCs
							if(tArr[jj*2+j]->dcTOT[k] >= GetGPar()->Val(54)){
								TOTAvrgtmp += tArr[jj*2+j]->dcTOT[k]; TOTcnt++;
							}
						} // end for over k
					} // end for over XY
					if(0 != TOTcnt) TOTAvrgtmp /= TOTcnt; // the temporary average
					TOTcnt = 0; trk3DIf[n3DtrT].dcTOTAvrg = 0.; // initialization for average update
					for(int j = 0; j < 2; j++){ // loop over XY
						for(int k = 0; k < 4; k++){ // loop over six anode layers in the two MWDCs
							if(tArr[jj*2+j]->dcTOT[k] >= TOTAvrgtmp*0.6){ // or it is deemed as noise
								trk3DIf[n3DtrT].dcTOTAvrg += tArr[jj*2+j]->dcTOT[k]; TOTcnt++;
							}
							else tArr[jj*2+j]->dcTOT[k] = -9999.; // 2018-01-15, slick, rule out noise-like TOTs
						} // end for over k
					} // end for voer XY
					if(0 == TOTcnt) trk3DIf[n3DtrT].dcTOTAvrg = -9999.; // failed
					else{
						trk3DIf[n3DtrT].dcTOTAvrg /= TOTcnt; // the updated average
						trk3DIf[n3DtrT].dcTOTcnt = TOTcnt;
					}

					// increment the number of tracks //
					n3DtrT++;
					n3DtrLs[2+jj]++;
				} // end for over jj (3D tracks, or DC Arr)
			} // end if(there's only one track in DCArrU or DCArrD)
		} // end outer if

		/////////////////////// PID DOWNSTREAM THE TARGET ////////////////////////////////////////
		// PID using the DC array downstream the target and the DC array downstream the dipole magnet
		// veto before target //
		if(1 || (4 != VETO_0->GetFiredStatus() && 4 != VETO_1->GetFiredStatus()))
		if(IsPID() && ntrLs[3][0] >= 1){ // at least one trk in DCArrD, or no pid is possible
			// only one trk in DCArrR, or no pid is possible
			if(1 == n3DtrLs[1] || (0 == n3DtrLs[1] && 1 == ntrLs[1][0])){

				///// ameliorate tracks around TA using new algorithm ///////
				// - Oct., 2019 //
				// extract pOut: info of trkPostMag
				double pOut[4];
				for(double &x : pOut) x = -9999.;
				for(tTrack *&t : track_ls){
					const int dcArrId = t->type / 10 % 10; // [0-1-2-3]: [L-R-U-D]
					const int dcType = t->type % 10;
					// assign DCArrR trks //
					if(0 == n3DtrLs[1] && 1 == ntrLs[1][0]){ // DCArrR, only one Xproj is found, no 3D trks
						if(1 == dcArrId && 0 == dcType){
							pOut[0] = t->k; pOut[2] = t->b;
						} // end if(1 == dcArrId && 0 == dcType)
					} // end if(0 == n3DtrLs[1] && 1 == ntrLs[1][0])
				} // end for over tracks
				if(1 == n3DtrLs[1]){ // found only one DCArrR 3D trk
					pOut[0] = trk3DIf[0].k1; pOut[2] = trk3DIf[0].b1;
					pOut[1] = trk3DIf[0].k2; pOut[3] = trk3DIf[0].b2;
				}
				// commence the new tracking //
				pdcArrayTa4->SetIsReady(true); /// XXX-2019-09-26
				pdcArrayTa4->SetPostMagXproj(pOut[0], pOut[2]); // k, b
				pdcArrayTa4->Map();

				// update ntr variables //
				memset(ntrLs, 0, sizeof(ntrLs));
				dcntTrkX = 0; dcntTrkY = 0;
				for(tTrack *&t : track_ls){
					const int dcArrId = t->type / 10 % 10, dcType = t->type % 10;
					ntrLs[dcArrId][dcType]++;
					if(dcArrId > 1 && 1 == dcType) dcntTrkY++;
					if(0 == dcType) dcntTrkX++;
				}
				ntr = 0; ntrT = track_ls.size();
				for(tTrack *&t : track_ls){
					const short dcArrId = t->type / 10 % 10; // dcArrId
					if(0 == dcArrId || 1 == dcArrId) ntr++; // ntr for MWDCArrayL-R
				}
				ntr = ntr < ntrMax ? ntr : ntrMax;

//				bool GOING = false; // DEBUG
//				cout << "index: " << index << endl; // DEBUG
//				cout << "Enter your option: "; // DEBUG
//				std::cin >> GOING; // DEBUG
//				if(!GOING) continue; // DEBUG
				double pIn[4]; // [0-1-2-3]: [k1, k2, b1, b2]; pIn: into the magnet
				double pIn0[4]; // trk info before the target
				for(int j = 4; j--;){
					pIn[j] = -9999.; pIn0[j] = -9999.;
				}
				for(tTrack *&t : track_ls){
					const int dcArrId = t->type / 10 % 10; // [0-1-2-3]: [L-R-U-D]
					const int dcType = t->type % 10;
					// assign DCArrU trks //
					if(2 == dcArrId && 0 == dcType){ // only one UX trk is found
						pIn0[0] = t->k; pIn0[2] = t->b;
					} // end if(2 == dcArrId)
					if(1 == ntrLs[2][1] && 2 == dcArrId && 1 == dcType){ // only one UY trk is found
						pIn0[1] = t->k; pIn0[3] = t->b;
					} // end if(1 == ntrLs[2][1])
					// assign DCArrD trks //
					if(3 == dcArrId && 0 == dcType){ // only one DX trk is found
						pIn[0] = t->k; pIn[2] = t->b;
					} // end if(3 == dcArrId)
					if(1 == ntrLs[3][1] && 3 == dcArrId && 1 == dcType){ // only one DY trk is found
						pIn[1] = t->k; pIn[3] = t->b;
					} // end if(1 == ntrLs[3][1])
				} // end for over tracks
				// XXX XXX XXX XXX //
				if(1 == n3DtrLs[3]){ // use DCArrU-3D trk or not
					for(int jj = n3Dtr; jj < n3DtrT; jj++){
						if(0 == trk3DIf[jj].isDCArrR){ // DCArrU
							if(-2 != trk3DIf[jj].firedStripId)
								TAPopMsg::Error("TAEventProcessor", "Run: fill_post: abnormal firedStripId\
 for 3D DCArrUD tracks: %d", trk3DIf[jj].firedStripId);
							pIn0[0] = trk3DIf[jj].k1; pIn0[2] = trk3DIf[jj].b1;
							pIn0[1] = trk3DIf[jj].k2; pIn0[3] = trk3DIf[jj].b2;
							break;
						} // end if
					} // end for over jj
				} // end outer if
				if(1 == n3DtrLs[3]){ // use DCArrD-3D trk or not
					for(int jj = n3Dtr; jj < n3DtrT; jj++){
						if(1 == trk3DIf[jj].isDCArrR){ // DCArrD
							if(-2 != trk3DIf[jj].firedStripId)
								TAPopMsg::Error("TAEventProcessor", "Run: fill_post: abnormal firedStripId for\
 3D DCArrUD tracks: %d", trk3DIf[jj].firedStripId);
							pIn[0] = trk3DIf[jj].k1; pIn[2] = trk3DIf[jj].b1;
							pIn[1] = trk3DIf[jj].k2; pIn[3] = trk3DIf[jj].b2;
							break;
						} // end if
					} // end for over jj
				} // end outer if

				if(0 && pdcArrayTa4->ZeroTrack()){  //
					static int n;
					TAPopMsg::Warn("TAEvProsr", "Run: pdcArrTa4 zero track! n = %d, index: %d", n++, index);
//					cout << pOut[0] << " " << pOut[1] << endl;
//					cout << pIn[0] << " " << pIn[1] << endl;
				}
				if(1 == ntrLs[3][0]){ // 1 || !pdcArrayTa4->ZeroTrack()
					static TAPID::OPTION pidOpt = (TAPID::OPTION)(GetGPar()->Val(109));
					pid->Fly(tof2[0], -9999., pOut, 1, pidOpt, pIn, pIn0);
					aoz[0] = pid->GetAoZ(); aozdmin[0] = pid->GetChi();
					beta2[0] = pid->GetBeta(); poz[0] = pid->GetPoZ(); // MeV/c
					brho[0] = pid->GetBrho(); // T.m
					pid->GetTargetExitAngle(yp[0]); trkLenT[0] = pid->GetTotalTrackLength();
					pid->GetX2Arr(x2_PID[0]);
					if(-9999. == x2_PID[0][0] || -9999. == x2_PID[0][1]) dx2_PID[0] = -9999.;
					else{
						if(TAPID::kOpt4 == pidOpt) dx2_PID[0] = x2_PID[0][0] - x2_PID[0][1];
						if(TAPID::kOpt0 == pidOpt || TAPID::kOpt1 == pidOpt || TAPID::kOpt3 == pidOpt)
							dx2_PID[0] = sqrt(x2_PID[0][0]*x2_PID[0][0] + x2_PID[0][1]*x2_PID[0][1]); // d - taHitCal v.s. taHitReal
					}
	//				cout << "x2_PID[0][0]: " << x2_PID[0][0] << "\tx2_PID[0][1]: " << x2_PID[0][1] << endl; // DEBUG
	//				cout << "dx2_PID[0]: " << dx2_PID[0] << endl; // DEBUG
	//				getchar(); // DEBUG
					if(aozdmin[0] > 100. || -9999. == aoz[0]){
						cntaozWrong++;
	//					cout << "index: " << index << endl; // DEBUG
					}
					// assign 3D PID result //
					if(Is3DTracking()){
						t3DPIDInfo &pi = pid3DIf[0];
						pi.aoz = pid->GetAoZ();
						pi.aozdmin = pid->GetChi();
						pi.beta2 = pid->GetBeta();
						pi.poz = pid->GetPoZ();
						pi.brho = pid->GetBrho();
						pid->GetTargetExitAngle(pi.angTaOut);
						pi.trkLenT = pid->GetTotalTrackLength();
					}
					if(0) if(aoz[0] > 0.){ //
						cout << "index: " << index << endl; // DEBUG
						cout << "aoz: " << aoz[0] << "\ttrkLenT: " << trkLenT[0] << endl; // DEBUG
						cout << "aozdmin: " << aozdmin[0] << endl; // DEBUG
						cout << "poz[0]: " << poz[0] << endl; // DEBUG
						cout << "brho[0]: " << brho[0] << " poz[0]: " << poz[0] << endl; // DEBUG
						cout << "brhoc[0]: " << poz[0] / (0.321840605 * 931.493582); // DEBUG
						cout << "rho: " << brho[0]*1000./1.3848 << endl; // DEBUG
						cout << "rhoc: " << brho[0]*1000./1.3848 << endl; // DEBUG
						cout << "beta: " << pid->GetBeta() << endl; // DEBUG
						cout << "tof2[0]: " << tof2[0] << endl; // DEBUG
						getchar(); // DEBUG
					} // end if

					cntaoz++;
				} // end if no splined track is found
//				cout << "mark 1, cntaoz: " << cntaoz << endl; getchar(); // DEBUG
			} // end inner if
		} // end the outer if




//		cout << "n3Dtr: " << n3Dtr << endl; getchar(); // DEBUG
		// assignment for the filling of treePID3D
		dcTOTAvrg3D_Total = 0.; // TOT: DCArr-D + DCArr-R
		for(int jj = 0; jj < n3DtrT; jj++){
			const t3DTrkInfo &t = trk3DIf[jj];
			// 3D trk
			isDCArrR[jj] = t.isDCArrR;
			Chi3D[jj] = t.Chi; chi2_3D[jj] = t.chi2;
			memcpy(chi3D[jj], t.chi, sizeof(chi3D[jj]));
			k1[jj] = t.k1; k2[jj] = t.k2; b1[jj] = t.b1; b2[jj] = t.b2;
			// hit Y position in TOFW strips, calculated via dt or 3D trks
			TOF_posY[jj] = t.TOF_posY;
			TOF_posY_refine[jj] = t.TOF_posY_refine;
			firedStripId3D[jj] = t.firedStripId; tof2_3D[jj] = t.tof2;
			dcTOTAvrg3D[jj] = t.dcTOTAvrg;
			dcTOT3Dcnt[jj] = t.dcTOTcnt;
			if(t.isDCArrR) dcTOTAvrg3D_Total += t.dcTOTAvrg; // DCArrR or DCArrD - for secondary beam experiment
			hTOFWHitPosCmp[isDCArrR[jj]]->Fill(TOF_posY[jj], TOF_posY_refine[jj]);
//			cout << "t.firedStripId: " << t.firedStripId << endl; // DEBUG
//			cout << "t.tof2: " << t.tof2 << endl; // DEBUG
//			getchar(); // DEBUG
			// 3D trk PID
			const t3DPIDInfo &p = pid3DIf[jj];
			if(IsPID() && p.aoz != -9999.){
				aoz3D[jj] = p.aoz; aozdmin3D[jj] = p.aozdmin;
				beta2_3D[jj] = p.beta2; poz3D[jj] = p.poz; brho3D[jj] = p.brho;
				yp3D[jj][0] = p.angTaOut[0]; yp3D[jj][1] = p.angTaOut[1];
				trkLenT3D[jj] = p.trkLenT;
			} // end if(IsPID())
		} // end for over 3D tracks


		////////////////// UPDATE AFTER RE-TRACKING ////////////////////////////
		// update drift time, drift distance and dcTOTs before filling the trees
		for(int j = 0; j < ntrT; j++){ // assign track_ls to treeTrack. loop over tracks
			tTrack *&tra = track_ls[j];
			type[j] = tra->type; id[j] = tra->id;
			k[j] = tra->k; b[j] = tra->b; TOF[j] = tra->TOF; d2[j] = tra->dsquare;
			gGOOD[j] = tra->gGOOD; chi2[j] = tra->chi2; Chi[j] = tra->Chi;
			for(int k = 0; k < 6; k++){ // loop over anode layers
				nu[j][k] = tra->nu[k];
				t[j][k] = tra->t[k];
				w[j][k] = tra->w[k];
				r[j][k] = tra->r[k];
				chi[j][k] = tra->chi[k];

				/////////// mark those sense wires that ////////////////
				//////// don't belong to any track with a minus sign in treeMulti ////////////
				// dcArrId: 0-1-2-3 => dcArrL-R--U-D; dcType: 0-1 => X-Y, 0-1-2: X-U-V
				short dcArrId = type[j]/10%10, dcType = type[j]%10, dcId = k/2, layerId = k%2;
				// it's the same layer for sure
				const double NU = nu[j][k];
				if(NU >= 0){
					if(dcArrId <= 1){ // DCArrL-R
						multi_DC_invalid[dcArrId][dcId][dcType][layerId]--;
						for(short &x : nu_DC[dcArrId][dcId][dcType][layerId]) if(NU == x) x += 1000;
					} // end if
					if(dcArrId <= 3 && dcArrId >= 2){ // PDCs
						multi_PDC_invalid[dcArrId][dcId][dcType][layerId]--;
						for(short &x : nu_PDC[dcArrId-2][dcId][dcType][layerId]) if(NU == x) x += 1000;
					} // end if
				} // end outer if
				//////////////////////////////////////////////////////////////////////////////

			} // end for over k
			TOT_DC_Avrg[j] = tra->dcTOTAvrg();
		} // end for over tracks


		/// XXX THE TREE FILLING XXX ///
		for(TTree *&tree : objLsTree) tree->Fill();
		/// XXX XXX XXX XXX XXX XXX ///




//		double eff3D = cnt3DTrk/3.;
//		if(0 == cntTrk) eff3D = 0.; else eff3D /= cntTrk;
//		gTrkEff->SetPoint(gTrkEff->GetN(), cntSec, eff3D);


		// the CONDITION for visualization //
		static const int indexArr1[] = {};
		static const int indexArr2[] = {};
		static const int indexArr3[] = {};
		// to find a match
		bool BINGO = false;
		for(const int t : indexArr1) if(t == index) { BINGO = true; break; }
		for(const int t : indexArr2) if(t == index) { BINGO = true; break; }
		for(const int t : indexArr3) if(t == index) { BINGO = true; break; }


		if(0) vis->FillHitMap();
		static int jj = 0;
		static const int jjM = 10;

		if(jj < jjM && (BINGO || vis->OnTheList(index))){ // index permitted by vis
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
		if(jjM == jj) TAPID::Instance()->SetCurveGraph(nullptr);
