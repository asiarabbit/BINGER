///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				   																		 		 //
// BINGER/inc/etf/TAEventProcessor/fill_pre.cxx																			 //
//   fill_pre.cxx -- a scriplet encapuslated in TAEventProcessor.cxx for method			 //
// Pre-tracking fill would tamper detector fired status if necessary.								 //
// Fill histograms and trees before tracking.																				 //
// TAEventProcessor::Run().																													 //
//   Introduction: fill histograms within the loop of runs.													 //
//																																									 //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/10/29.																													     //
// Last modified: 2018/8/27, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

//#define DEBUG
//#define DEBUG_T0_1_pos

		///////////////////// OUTPUT THE ANALYSIS RESULT //////////////////////////////////////////
		// daq statistics //
		// the last entry of a data section, then channelId stores secLen.
		hSecLen->Fill(entry_t.channelId);
		for(const tEntry *t : entry_ls) if(t->channelId > 0){
			const int chid = t->channelId;
			hCh->Fill(chid);
			if(t->nl && t->nt) htot->Fill(chid, t->TOT());
			hnxvsChid[0]->Fill(chid, t->nl); // leading edge number
			hnxvsChid[1]->Fill(chid, t->nt); // trailing edge number
			if(t->nl >= 2) hxtn_nvsChid[0]->Fill(chid, t->leadingTime[1] - t->leadingTime[0]);
			if(t->nt >= 3) hxtn_nvsChid[1]->Fill(chid, t->leadingTime[2] - t->leadingTime[1]);
			if(t->nt >= 2) hxtn_nvsChid[2]->Fill(chid, t->trailingTime[1] - t->trailingTime[0]);
			if(t->nt >= 3) hxtn_nvsChid[3]->Fill(chid, t->trailingTime[2] - t->trailingTime[1]);
			hnl_ntvsChid->Fill(chid, t->nl - t->nt);
		} // end for

		// assign TOT_T0
		tRef = -9999.;
		const double T0_1_delayAvrg = T0_1->GetDelay();
		const int nUVLEdge_T0_1 = T0_1->GetUV()->GetData()->GetNLeadingEdge();
		const int nDVLEdge_T0_1 = T0_1->GetDV()->GetData()->GetNLeadingEdge();
		tRef_UV_NL = nUVLEdge_T0_1; tRef_DV_NL = nDVLEdge_T0_1;
		// T0_1 UV validity check
		bool hasIncre_ValidityUV = false, hasIncre_ValidityDV = false; // incre only once per event
		for(int j = 0; j < nUVLEdge_T0_1; j++){
			double time = T0_1->GetUV()->GetTime(j) - T0_1_delayAvrg;
#ifdef DEBUG
			cout << "time: " << time; // DEBUG
			cout << "T0_1->GetUV()->GetTime(j): " << T0_1->GetUV()->GetTime(j) << endl; // DEBUG
			cout << "\ttimeToTrigLowBoundUV: " << timeToTrigLowBoundUV; // DEBUG
			cout << "\ttimeToTrigHighBoundUV: " << timeToTrigHighBoundUV; getchar(); // DEBUG
#endif
			hT0_1ToTrigUV->Fill(j, time);
			if(time > timeToTrigLowBoundUV && time < timeToTrigHighBoundUV){
				if(!hasIncre_ValidityUV){
					cnt_timeToTrig_T0_1UV++;
					hasIncre_ValidityUV = true;
				} // end if(!hasIncre_ValidityUV)
			}
		}
		// T0_1 DV validity check
		for(int j = 0; j < nDVLEdge_T0_1; j++){
			double time = T0_1->GetDV()->GetTime(j) - T0_1_delayAvrg;
#ifdef DEBUG
			cout << "time: " << time; // DEBUG
			cout << "T0_1->GetDV()->GetTime(j): " << T0_1->GetDV()->GetTime(j) << endl; // DEBUG
			cout << "\ttimeToTrigLowBoundDV: " << timeToTrigLowBoundDV; // DEBUG
			cout << "\ttimeToTrigHighBoundDV: " << timeToTrigHighBoundDV; // DEBUG
			getchar(); // DEBUG
#endif
			hT0_1ToTrigDV->Fill(j, time);
			if(time > timeToTrigLowBoundDV && time < timeToTrigHighBoundDV){
//				cout << "cnt_timeToTrig_T0_1DV: " << cnt_timeToTrig_T0_1DV << endl; getchar(); // DEBUG
				if(!hasIncre_ValidityDV){
					hasIncre_ValidityDV = true;
					cnt_timeToTrig_T0_1DV++;
				} // end if(!hasIncre_ValidityDV)
			}
		}
		// extract the best matched T0_1_UV and T0_1_DV
		double timeUV_T0_1 = -9999., timeDV_T0_1 = -9999., dmin_T0_1 = 1E200;
		//if(0)
		for(int j = 0; j < nUVLEdge_T0_1; j++){
			double tmpuv = T0_1->GetUV()->GetTime(j) - T0_1_delayAvrg;
			double tmpdv = T0_1->GetDV()->GetLT(tmpuv+T0_1_delayAvrg) - T0_1_delayAvrg; //
			double tmpdt = fabs(tmpuv - tmpdv);
			if(tmpuv > timeToTrigHighBoundUV || tmpuv < timeToTrigLowBoundUV) tmpdt += 20.;
			if(tmpdt < dmin_T0_1){
				dmin_T0_1 = tmpdt;
				timeUV_T0_1 = tmpuv; timeDV_T0_1 = tmpdv;
			}
		}
//		timeUV_T0_1 = T0_1->GetUV()->GetTime(0); // DEBUG
//		timeDV_T0_1 = T0_1->GetDV()->GetTime(0); // DEBUG
//		tRef = (timeUV_T0_1 + timeDV_T0_1) / 2.; // DEBUG
#ifdef DEBUG_T0_1_pos
		cout << "dmin_T0_1: " << dmin_T0_1 << endl; // DEBUG
		cout << "timeUV_T0_1 - timeDV_T0_1: " << timeUV_T0_1 - timeDV_T0_1 << endl; // DEBUG
		getchar(); // DEBUG
#endif
		tRef_pos = timeUV_T0_1 - timeDV_T0_1;
		hTOF_T1_pos->Fill(tRef_pos);
		if(dmin_T0_1 > -20. && dmin_T0_1 < 60.){
			tRef = (timeUV_T0_1 + timeDV_T0_1) / 2.;
			cntTRef++;
		}
#ifdef DEBUG
		const double T0_1_delayUV = T0_1->GetUV()->GetPara()->GetDelay();
		cout << "T0_1_delayAvrg: " << T0_1_delayAvrg << "\tT0_1_delayUV: " << T0_1_delayUV << endl; // DEBUG
		cout << "timeUV_T0_1 - timeDV_T0_1: " << timeUV_T0_1 - timeDV_T0_1 << endl; // DEBUG
		getchar(); // DEBUG
//		if(!(tRef > timeToTrigHighBoundUV || tRef < timeToTrigLowBoundUV))
		{
			cout << "\n\nnl: " << nUVLEdge_T0_1 << "\tnDVLEdge_T0_1: " << nDVLEdge_T0_1 << endl; // DEBUG
			cout << "timeUV_T0_1: " << timeUV_T0_1 << "\ttimeDV_T0_1: " << timeDV_T0_1 << endl; // DEBUG
			cout << "dmin_T0_1: " << dmin_T0_1 << "\ttRef: " << tRef << endl; // DEBUG
			T0_1->GetUV()->GetData()->Show(); T0_1->GetDV()->GetData()->Show(); getchar(); // DEBUG
		}
#endif

		// calculate beta in RIBLL2 //
		beta = -1.; // initialization
		static const double L = 25.881 * 1000.; // the length of RIBLL2
		tof1 = -9999.; // time of flight in RIBLL2
		if(T0_0 && -9999. != tRef){
			const double t0_0 = T0_0->GetTime(tRef);
			if(-9999. != t0_0){
				tof1 = tRef - t0_0;
				beta = L / tof1 / c0;
				htof1->Fill(tof1);
//				cout << "t0_0: " << t0_0 << "\ttRef: " << tRef << endl; // DEBUG
//				cout << "index: " << index << "\ttof1: " << tof1 << endl; // DEBUG
//				cout << "beta: " << beta << endl; // DEBUG
//				getchar(); // DEBUG
			}
		} // end if

		// Time of Flight and beam energy measurement in RIBLL2
		// vme tof1 //
		if(vme){
			tof1vme = ((evt.mtdc0[1][0] + evt.mtdc0[2][0]) / 2. -  evt.mtdc0[0][0]) * 0.09765625 + 141.3;
			// 0-1-2-3: Tstart_L_R_L_R
			// tof1vme = ((evt.mtdc1[1][0] + evt.mtdc1[2][0]) / 2.
				// - (evt.mtdc1[12][0] + evt.mtdc1[13][0]) / 2.) * 0.09765625 + 141.3;
			tof1tac = (-0.010217 * evt.adc[0] -0.0104695 * evt.adc[1]) / 2. + 158.3;
			dE0 = (evt.adc[16] + evt.adc[17] + evt.adc[18] + evt.adc[19]) / 2000.;
			dE1 = (evt.adc[22] + evt.adc[23] + evt.adc[24]) / 3000.;
			dsca10 = evt.dsca[10]; dsca11 = evt.dsca[11];
			hpid00->Fill(tof1vme, dE0);
			hpid01->Fill(tof1vme, dE1);
		}
		// vme tof1

		TOF_T1 = T0_1->GetTime();
		if(T0_0){
			TOT_T0[0] = T0_0->GetUV()->GetTOT();
			TOT_T0[1] = T0_0->GetUH()->GetTOT();
		} // end if
		TOT_T0[2] = T0_1->GetUV()->GetTOT(); TOT_T0[3] = T0_1->GetUH()->GetTOT();
		TOT_T0[4] = T0_1->GetDV()->GetTOT(); TOT_T0[5] = T0_1->GetDH()->GetTOT();

		// DEBUG
//		for(int i = 0; i < 3; i++){
//			int fired = str_t0_1[i]->GetFiredStatus();
//			cout << str_t0_1[i]->GetName() << endl;
//			cout << "FiredStatus(): " << fired << endl;
//			if(4 == fired){
//				double uv = str_t0_1[i]->GetUV()->GetLeadingTime();
//				double dv = str_t0_1[i]->GetDV()->GetLeadingTime();
//				cout << "UV: " << uv << "\tDV: " << dv << endl;
//				cout << "UV - DV: " << uv - dv << endl;
//			}
//		}
//		getchar(); // DEBUG

		////////////// detector performance statistics //////////////
		// initialize multiplicity //
		memset(multi_DC, 0, sizeof(multi_DC));
		memset(multi_PDC, 0, sizeof(multi_DC));
		memset(multi_DCTa, 0, sizeof(multi_DC));
		memset(multi_DC_invalid, 0, sizeof(multi_DC_invalid));
		memset(multi_PDC_invalid, 0, sizeof(multi_DC_invalid));
		memset(multi_DCTa_invalid, 0, sizeof(multi_DC_invalid));
		memset(multiTOFW_pre, 0, sizeof(multiTOFW_pre));
		memset(multiTOFW_pre, 0, sizeof(multiTOFW_post));
		// the MWDC arrays downstream of the target //
		for(int ii = 0; ii < 2; ii++) if(dcArr[ii]){ // loop over MWDC arrays
			ttRef_TOFW[ii] = -9999.;
			tofw[ii]->GetFiredStripArr(multiTOFW_pre[ii], hitIdLsTOFW_pre[ii],
				hitStaLsTOFW_pre[ii], uvlTLsTOFW_pre[ii], dvlTLsTOFW_pre[ii]);
			const int nStrip = tofw[ii]->GetNStrip();
			for(TAPlaStrip *&str : tofw[ii]->GetStripArr()){
				const int sta = str->GetFiredStatus();
				const int strId = str->GetStripId();
//				cout << "strId: " << strId << "\tsta: " << sta << endl; getchar(); // DEBUG
				if(4 == sta) hTOFWFiredDist[ii]->Fill(strId);
				if(4 == sta || 3 == sta){
//					if(15 == strId)
					{
						for(int j = 0; j < str->GetUV()->GetData()->GetNLeadingEdge(); j++){
							hTOFWToTrigUV[ii]->Fill(j, str->GetUV()->GetTime(j));
							hTOFWTOTUV[ii]->Fill(j, str->GetUV()->GetTOT(j));
						}
					}
					hTOFWHitPos[ii]->Fill(strId, str->GetHitPosition());
					const double tofwToTrig = str->GetTime(); // 0., gpar->Val(5), gpar->Val(6)
					const double tofwToTRef = tofwToTrig - tofw[ii]->GetDelayAvrg() - tRef;
					if(-9999. != tRef){
						hTOFWToTRef[ii]->Fill(strId, tofwToTRef);
						ttRef_TOFW[ii] = tofwToTRef;
					}
					// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
//					cout << "tofwToTrig: " << tofwToTrig << endl; getchar(); // DEBUG
					// belongs to the trigger-generating particle
					// (300., 700.)->pion2017; (1120., 1160.)->beamTest2016
					if(!(tofwToTrig > gpar->Val(5) && tofwToTrig < gpar->Val(6))){
						str->GetStripData()->SetFiredStatus(-10); // manually altered
					}
					// for two adjacent strips, set the one with later timing invalid
					// for very possibly it's not directly hit by the trig-particle
					if(0) if(strId != 0){ //
						TAPlaStrip *strBefore = tofw[ii]->GetStrip(strId - 1);
						const int staBefore = strBefore->GetFiredStatus();
						if((4 == staBefore || 3 == staBefore) && strBefore->GetTime() < tofwToTrig)
							str->GetStripData()->SetFiredStatus(-10);
					} // end if
					if(0) if(strId < nStrip - 1){ //
						TAPlaStrip *strNext = tofw[ii]->GetStrip(strId + 1);
						const int staNext = strNext->GetFiredStatus();
						if((4 == staNext || 3 == staNext) && strNext->GetTime() < tofwToTrig)
							str->GetStripData()->SetFiredStatus(-10);
					} // end if
				} // end if(4 == sta || 3 == sta)
			} // end for over TOFW strips
			tofw[ii]->GetFiredStripArr(multiTOFW_post[ii], hitIdLsTOFW_post[ii]);
			hTOFWMulti[ii]->Fill(tofw[ii]->GetNFiredStrip());
			for(int j = 0; j < 3; j++){ // loop over the three MWDCs
				const int nsl = dc[ii][j]->GetNSLayer();
				const int na = dc[ii][j]->GetNAnodePerLayer();
				for(int k = 0; k < nsl; k++){ // loop over XUV SLayers
					for(int l = 0; l < 2; l++){ // loop over layer option (1, 2)
						ttRef_DC[ii][j][k][l] = -9999.;
						DC_NLM[ii][j][k][l] = 0; // maximum number of leading edges in a wire layer
						DC_LTM[ii][j][k][l] = 0.; // maximum leading edge time over all the edges of a wire layer
						int nuCnt = 0; // for counting fired anodes per layer
						for(auto &x : nu_DC[ii][j][k][l]) x = -9999;
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = dc[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
								if(nuCnt < maxMultiDC) nu_DC[ii][j][k][l][nuCnt++] = m;
								hDCFiredDist[ii][j][k]->Fill(l*na+m);
								double dcToTrig = ano->GetTime();
								if(tRef != -9999.){
									hDCToTRef[ii][j][k]->Fill(dcToTrig - tRef);
									ttRef_DC[ii][j][k][l] = dcToTrig - tRef;
								}
//								if(0 == ii && 0 == k && 0 == j)
								{
									int DC_NL = ano->GetData()->GetNLeadingEdge();
									if(DC_NL > DC_NLM[ii][j][k][l]) DC_NLM[ii][j][k][l] = DC_NL;
									for(int i = 0; i < DC_NL; i++){
										const double tt = ano->GetTime(i);
										hDCToTrig->Fill(i, tt);
										if(vme && dsca11 == 0)
											hDCToTrigNoPU->Fill(i, tt);
										if(tt > DC_LTM[ii][j][k][l]) DC_LTM[ii][j][k][l] = tt;
									}
								}
								// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
								if(!(dcToTrig > gpar->Val(7) && dcToTrig < gpar->Val(8))) ano->GetData()->SetFiredStatus(false); // (340., 840.)->pion2017; (1000., 1400.)->beamTest2016
//								if(1 == ii && 0 == j && 0 == k) ano->GetData()->SetFiredStatus(false);
							}
						} // end for over anode of one layer
						multi_DC[ii][j][k][l] = dc[ii][j]->GetNFiredAnodePerLayer(k, l+1);
						multi_DC_invalid[ii][j][k][l] = multi_DC[ii][j][k][l];
						hDCMulti[ii][j][k][l]->Fill(dc[ii][j]->GetNFiredAnodePerLayer(k, l+1));
					} // end for over layer 1 and 2
				} // end for over X-U-V
			} // end for over DCs
		} // end for over DC arrays

		// MWDC arrays around the target //
		// DCs made by P. Ma
		for(int ii = 0; ii < 2; ii++) if(dcArr2[ii]){ // loop over MWDC arrays around the target
			for(int j = 0; j < 2; j++){ // loop over two MWDCs
				const int na = dc2[ii][j]->GetNAnodePerLayer();
				for(int k = 0; k < 2; k++){ // loop over XY SLayers
					for(int l = 0; l < 2; l++){ // loop over layer option (1, 2)
						ttRef_DCTa[ii][j][k][l] = -9999.;
						int nuCnt = 0; // for counting fired anodes per layer
						for(auto &x : nu_DCTa[ii][j][k][l]) x = -9999;
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = dc2[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
								if(nuCnt < maxMultiDCTa) nu_DCTa[ii][j][k][l][nuCnt++] = m;
								hDCTaFiredDist[ii][j][k]->Fill(l*na+m);
								double dcToTrig = ano->GetTime();
								if(tRef != -9999.){
									hDCTaToTRef[ii][j][k]->Fill(dcToTrig - tRef);
									ttRef_DCTa[ii][j][k][l] = dcToTrig - tRef;
								}
//								if(0 == ii && 0 == k && 0 == j)
								{
									for(int i = 0; i < ano->GetData()->GetNLeadingEdge(); i++)
									hDCTaToTrig->Fill(i, ano->GetTime(i));
								}
								// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
								if(!(dcToTrig > gpar->Val(67) && dcToTrig < gpar->Val(68))) ano->GetData()->SetFiredStatus(false); // (340., 840.)->pion2017; (1000., 1400.)->beamTest2016
//								if(1 == ii && 0 == j && 0 == k) ano->GetData()->SetFiredStatus(false);
							}
						} // end for over anode of one layer
						multi_DCTa[ii][j][k][l] = dc2[ii][j]->GetNFiredAnodePerLayer(k, l+1);
						multi_DCTa_invalid[ii][j][k][l] = multi_DCTa[ii][j][k][l];
						hDCTaMulti[ii][j][k][l]->Fill(dc2[ii][j]->GetNFiredAnodePerLayer(k, l+1));
					} // end for over layer 1 and 2
				} // end for over X-U-V
			} // end for over DCs
		} // end for over DC arrays
		// DCs made from BUAA, made by Japan
		for(int ii = 0; ii < 2; ii++) if(pdcArr2[ii]){ // loop over MWDC arrays around the target
			for(int j = 0; j < 2; j++){ // loop over two MWDCs
				const int na = pdc2[ii][j]->GetNAnodePerLayer();
				for(int k = 0; k < 2; k++){ // loop over XY SLayers
					for(int l = 0; l < 2; l++){ // loop over layer option (1, 2)
						ttRef_PDC[ii][j][k][l] = -9999.;
						PDC_NLM[ii][j][k][l] = 0; // maximum number of leading edges in a wire layer
						PDC_LTM[ii][j][k][l] = 0.; // maximum leading edge time over all the edges of a wire layer
						int nuCnt = 0; // for counting fired anodes per layer
						for(auto &x : nu_PDC[ii][j][k][l]) x = -9999;
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = pdc2[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
								if(nuCnt < maxMultiPDC) nu_PDC[ii][j][k][l][nuCnt++] = m;
								hPDCFiredDist[ii][j][k]->Fill(l*na+m);
								const double t0 = pdcArr2[ii]->GetPlaT0()->GetTime();
								const double dcToTrig = ano->GetTime();
								const double dcToTRef = dcToTrig - t0;
								// tRef has been subtracted from dc meansuresments by VME Daq
								if(-9999. != t0){
									hPDCToTRef[ii][j][k]->Fill(dcToTRef);
									ttRef_PDC[ii][j][k][l] = dcToTRef;
								}
//								if(0 == ii && 0 == k && 0 == j)
								{
									int PDC_NL = ano->GetData()->GetNLeadingEdge();
									if(PDC_NL > PDC_NLM[ii][j][k][l]) PDC_NLM[ii][j][k][l] = PDC_NL;
									for(int i = 0; i < PDC_NL; i++){
										const double tt = ano->GetTime(i);
										hPDCToTrig->Fill(i, tt);
										if(vme && dsca11 == 0)
											hPDCToTrigNoPU->Fill(i, tt);
										if(tt > PDC_LTM[ii][j][k][l]) PDC_LTM[ii][j][k][l] = tt;
									} // end for over PDC LED edges
								}
								// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
								if(!(dcToTrig > gpar->Val(81) && dcToTrig < gpar->Val(82))) ano->GetData()->SetFiredStatus(false);
//								if(1 == ii && 0 == j && 0 == k) ano->GetData()->SetFiredStatus(false);
							} // end if the anode is fired
						} // end for over anode of one layer
						multi_PDC[ii][j][k][l] = pdc2[ii][j]->GetNFiredAnodePerLayer(k, l+1);
						multi_PDC_invalid[ii][j][k][l] = multi_PDC[ii][j][k][l];
						hPDCMulti[ii][j][k][l]->Fill(pdc2[ii][j]->GetNFiredAnodePerLayer(k, l+1));
					} // end for over layer 1 and 2
				} // end for over X-U-V
			} // end for over DCs
		} // end for over DC arrays
