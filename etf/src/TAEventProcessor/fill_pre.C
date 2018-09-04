///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor/fill_pre.C										 //
//   fill_pre.C -- a scriplet encapuslated in TAEventProcessor.C for method			 //
// Pre-tracking fill would tamper detector fired status if necessary.				 //
// Fill histograms and trees before tracking.										 //
// TAEventProcessor::Run().															 //
//   Introduction: fill histograms within the loop of runs.							 //
//																					 //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/29.															     //
// Last modified: 2018/8/27, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
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
			if(j < 5) tRefLT_U[j] = time;
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
			if(j < 5) tRefLT_D[j] = time;
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
		if(!(tRef > timeToTrigHighBoundUV || tRef < timeToTrigLowBoundUV)){
			cout << "\n\nnl: " << nUVLEdge_T0_1 << "\tnDVLEdge_T0_1: " << nDVLEdge_T0_1 << endl; // DEBUG
			cout << "timeUV_T0_1: " << timeUV_T0_1 << "\ttimeDV_T0_1: " << timeDV_T0_1 << endl; // DEBUG
			cout << "dmin_T0_1: " << dmin_T0_1 << "\ttRef: " << tRef << endl; // DEBUG
			T0_1->GetUV()->GetData()->Show(); T0_1->GetDV()->GetData()->Show(); getchar(); // DEBUG
		}
#endif
		
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
		}

		// calculate beta in RIBLL2 //
		// Time of Flight and beam energy measurement in RIBLL2
		// vme tof1 //
		if(vme){
			tof1vme = ((evt.mtdc0[1][0] + evt.mtdc0[2][0]) / 2. -  evt.mtdc0[0][0]) * 0.09765625 + 141.3;
			tof1tac = (-0.010217 * evt.adc[0] -0.0104695 * evt.adc[1]) / 2. + 158.3;
			dE0 = (evt.adc[16] + evt.adc[17]) / 2000.;
			dE1 = (evt.adc[22] + evt.adc[23] + evt.adc[24]) / 3000.;
			dsca10 = evt.dsca[10]; dsca11 = evt.dsca[11];
			hpid00->Fill(tof1vme, dE0);
			hpid01->Fill(tof1vme, dE1);
		}
		// vme tof1

		TOF_T1 = T0_1->GetTime();
//		TOT_T0[0] = T0_0->GetUV()->GetTOT(); TOT_T0[1] = T0_0->GetUH()->GetTOT();
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
		// the MWDC arrays downstream of the target //
		for(int ii = 0; ii < 2; ii++) if(dcArr[ii]){ // loop over MWDC arrays
			if(tofw[ii]){
				ttRef_TOFW[ii] = -9999.;
				tofw[ii]->GetFiredStripArr(multiTOFW_pre[ii], hitIdLsTOFW_pre[ii], hitStaLsTOFW_pre[ii], uvlTLsTOFW_pre[ii], dvlTLsTOFW_pre[ii]);
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
						double tofwToTrig = str->GetTime(0., gpar->Val(5), gpar->Val(6));
						double tofwToTRef = tofwToTrig - tofw[ii]->GetDelayAvrg() - tRef;
						if(-9999. != tRef){
							hTOFWToTRef[ii]->Fill(strId, tofwToTRef);
							ttRef_TOFW[ii] = tofwToTRef;
						}
						// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
	//					cout << "tofwToTrig: " << tofwToTrig << endl; getchar(); // DEBUG
						if(!(tofwToTrig > gpar->Val(5) && tofwToTrig < gpar->Val(6))){ // belongs to the trigger-generating particle // (300., 700.)->pion2017; (1120., 1160.)->beamTest2016
							str->GetStripData()->SetFiredStatus(-10); // manually altered
						}
					}
				} // end for over TOFW strips
				tofw[ii]->GetFiredStripArr(multiTOFW_post[ii], hitIdLsTOFW_post[ii]);
				hTOFWMulti[ii]->Fill(tofw[ii]->GetNFiredStrip());
			} // end if(tofw[ii])
			for(int j = 0; j < 4; j++){ // loop over the four MWDCs
				const int nsl = dc[ii][j]->GetNSLayer();
				const int na = dc[ii][j]->GetNAnodePerLayer();
				for(int k = 0; k < nsl; k++){ // loop over XUV SLayers
					for(int l = 0; l < 2; l++){ // loop over layer option (1, 2)
						ttRef_DC[ii][j][k][l] = -9999.;
						DC_NLM[ii][j][k][l] = 0; // maximum number of leading edges in a wire layer
						DC_LTM[ii][j][k][l] = 0.; // maximum leading edge time over all the edges of a wire layer
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = dc[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
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
						hDCMulti[ii][j][k][l]->Fill(multi_DC[ii][j][k][l]);
//cout << "mark 1" << endl; getchar();
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
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = dc2[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
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
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = pdc2[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
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
						hPDCMulti[ii][j][k][l]->Fill(pdc2[ii][j]->GetNFiredAnodePerLayer(k, l+1));
					} // end for over layer 1 and 2
				} // end for over X-U-V
			} // end for over DCs
		} // end for over DC arrays





		// sipmArr stastics //
		if(sipmArr){
			hSiPMPlaArrMulti->Fill(sipmArr->GetNFiredStrip());
			sipmArr->GetFiredStripArr(multiSipmArr_pre, hitIdLsSipmArr_pre, uvlTLsSipmArr_pre);
			for(TAChannel *&ch : sipmArr->GetChArr()){
				if(ch->GetFiredStatus()){
					hSiPMPlaArrFiredDist->Fill(ch->GetSerialId());
					double timeToTrig = ch->GetLT(0., 200., 650.);
					// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
					if(!(timeToTrig > gpar->Val(9) && gpar->Val(10))) ch->GetData()->SetFiredStatus(false);
					if(tRef != -9999.){
						hsipmArrToTRef->Fill(ch->GetSerialId(), timeToTrig - tRef);
					}
					hsipmArrToTrig->Fill(ch->GetSerialId(), timeToTrig);
				}
			} // end for over channels
			sipmArr->GetFiredStripArr(multiSipmArr_post, hitIdLsSipmArr_post);
			hSiPMPlaArrMulti->Fill(multiSipmArr_post);
		} // end if(sipmArr)

		if(sipmBarr){
			sipmBarr->GetFiredStripArr(multiSipmBarr_pre, hitIdLsSipmBarr_pre, hitStaLsSipmBarr_pre, uvlTLsSipmBarr_pre, dvlTLsSipmBarr_pre);
			for(TAPlaStrip *&str : sipmBarr->GetStripArr()){
				const short sta = str->GetFiredStatus();
				const int strId = str->GetStripId();
				if(4 == sta) hSiPMPlaBarrFiredDist->Fill(strId);
				if(4 == sta || 3 == sta){
					hSiPMPlaBarrHitPos->Fill(strId, str->GetHitPosition());
					// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
					timeToTrigSipmBarr = str->GetTime(0., 300., 600.);
					hsipmBarrToTrig->Fill(strId, timeToTrigSipmBarr);
					if(tRef != -9999.){
						timeToTRefSipmBarr = timeToTrigSipmBarr - tRef;
						hsipmBarrToTRef->Fill(strId, timeToTRefSipmBarr);
					}
	//				if(tRef != -9999. && !(timeToTRefSipmBarr > -600. && timeToTRefSipmBarr < 300.))
					if(!(timeToTrigSipmBarr > gpar->Val(11) && timeToTrigSipmBarr < gpar->Val(12)))
					{
						str->GetStripData()->SetFiredStatus(-10);
					}
				}
			} // end for over sipmArr
			sipmBarr->GetFiredStripArr(multiSipmBarr_post, hitIdLsSipmBarr_post);
			hSiPMPlaBarrMulti->Fill(multiSipmBarr_post);
		} // end if(sipmBarr)

		// MUSIC statistics;   psca: previous value of sca
		static unsigned psca[16];
		pileUpSCA = sca[10] - psca[10];
		for(int j = 0; j < 3; j++) if(music[j]){ // loop over two MUSICs
			nF_MU[j] = music[j]->GetNFiredChannel();
			if(0 == nF_MU[j]) continue;
			if(pileUpSCA >= 2) music[j]->SetPileUp(true);
			else if(pileUpSCA == 1) music[j]->SetPileUp(false);
			pileUp[j] = music[j]->GetPileUp();
//			else TAPopMsg::Error("TAEvProsr", "Run: MUSIC PileUpSCA anomaly: %d", pileUpSCA);
			deltaE[j] = music[j]->GetDeltaE();
//			music[j]->SetBeta(beta);
//			Z[j] = music[j]->GetZ();
			for(double &t : MU_ch[j]) t = -9999.;
			int sub = 0;
			for(TAChannel *c : music[j]->GetChArr()) MU_ch[j][sub++] = c->GetLeadingTime();
		} // end loop over two MUSICs
		memcpy(psca, sca, sizeof(sca));

		// optical fiber array statistics
		if(opfa){
			multi_opfa = 0;
			for(int j = 0; j < 40; j++){
				pos_opfa[j] = -9999.;
				for(double &x : ul_opfa[j]) x = -9999.;
				for(double &x : dl_opfa[j]) x = -9999.;
				TAPlaStrip *str = opfa->GetStrip(j);
				const short sta = str->GetFiredStatus();
				if(3 == sta || 4 == sta){
					multi_opfa++;
					TAChannel *uv = str->GetUV();
					TAChannel *dv = str->GetDV();
//					uv->GetData()->Show(); // DEBUG
//					dv->GetData()->Show(); // DEBUG
//					getchar(); // DEBUG
					for(int k = 0; k < 5; k++){
						double tu = uv->GetLeadingTime(k);
						double td = dv->GetLeadingTime(k);
						ul_opfa[j][k] = tu;
						dl_opfa[j][k] = td;
						if(0 == k && -9999. != tu && -9999. != td)
							pos_opfa[j] = tu - td;
					}
				}
			}
			int tmp = 0;
			for(int i = 0; i < 40; i++){
				if(-9999. == pos_opfa[i]) tmp++;
			}
		} // end if(opfa)













