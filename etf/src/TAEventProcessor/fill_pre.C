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
// Last modified: 2017/10/29, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


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
		bunchId = entry_t.bunchId; // the last channel
		tRef = -9999.;
		double bunchIdTime = (entry_t.bunchId - (entry_t.bunchId & 0x800)) * 25.; // 11 LSB
		const int nUVLEdge_T0_1 = T0_1->GetUV()->GetData()->GetNLeadingEdge();
		const int nDVLEdge_T0_1 = T0_1->GetDV()->GetData()->GetNLeadingEdge();
		double timeToTrig_T0_1UV = -9999., timeToTrig_T0_1DV = -9999.;
		// T0_1 UV validity check
		for(int j = 0; j < nUVLEdge_T0_1; j++){
			double time = T0_1->GetUV()->GetLeadingTime(j) - bunchIdTime;
			if(time < 0.) time += 51200.;
			hT0_1ToTrigUV->Fill(j, time);
//			cout << "timeToTrig_T0_1UV: " << time << endl;
			if(time > timeToTrigLowBoundUV && time < timeToTrigHighBoundUV){ // 1214.dat: 500. 640.
				timeToTrig_T0_1UV = time;
				cnt_timeToTrig_T0_1UV++; break;
			}
		}
		// T0_1 DV validity check
		for(int j = 0; j < nDVLEdge_T0_1; j++){
			double time = T0_1->GetDV()->GetLeadingTime(j) - bunchIdTime;
			if(time < 0.) time += 51200.;
			hT0_1ToTrigDV->Fill(j, time);
//			cout << "timeToTrig_T0_1DV: " << time << endl; getchar();
			if(time > timeToTrigLowBoundDV && time < timeToTrigHighBoundDV){ // 1214.dat: 450. 650.
				timeToTrig_T0_1DV = time;
				cnt_timeToTrig_T0_1DV++; break;
			}
		}
		// extract the best matched T0_1_UV and T0_1_DV
		for(int j = 0; j < nUVLEdge_T0_1; j++){
			for(int k = 0; k < nDVLEdge_T0_1; k++){
				double timeUV = T0_1->GetUV()->GetLeadingTime(j) - bunchIdTime;
				if(timeUV < 0.) timeUV += 51200.;
				double timeDV = T0_1->GetDV()->GetLeadingTime(k) - bunchIdTime;
				if(timeDV < 0.) timeDV += 51200.;
				if((timeUV > timeToTrigLowBoundUV && timeUV < timeToTrigHighBoundUV) && 
				   (timeDV > timeToTrigLowBoundDV && timeDV < timeToTrigHighBoundDV)){
					hTOF_T1_pos->Fill(timeDV - timeUV);
					if(timeDV - timeUV < 2. && timeDV - timeUV > -2.){
						tRef = (timeUV + timeDV) / 2. + bunchIdTime;
						cntTRef++; break;
					}
				}
			}
			if(tRef != -9999.) break;
		}

//		cout << "nUVLEdge_T0_1: " << nUVLEdge_T0_1 << "\ttimeToTrig_T0_1UV: " << timeToTrig_T0_1UV << endl;
//		cout << "nDVLEdge_T0_1: " << nDVLEdge_T0_1 << "\ttimeToTrig_T0_1DV: " << timeToTrig_T0_1DV << endl;
//		cout << "tRef: " << tRef << endl; getchar(); // DEBUG

		TOF_T1 = T0_1->GetTime();
		TOT_T0[0] = T0_0->GetUV()->GetTOT(); TOT_T0[1] = T0_0->GetUH()->GetTOT();
		TOT_T0[2] = T0_1->GetUV()->GetTOT(); TOT_T0[3] = T0_1->GetUH()->GetTOT();
		TOT_T0[4] = T0_1->GetDV()->GetTOT(); TOT_T0[5] = T0_1->GetDH()->GetTOT();

		for(TAChannel *&ch : sipmArr->GetChArr())
		if(ch->GetFiredStatus()) hSiPMPlaArrFiredDist->Fill(ch->GetSerialId());
		hSiPMPlaArrMulti->Fill(sipmArr->GetNFiredStrip());
		sipmArr->GetFiredStripArr(multiSipmArr_pre, hitIdLsSipmArr_pre, uvlTLsSipmArr_pre);
		sipmBarr->GetFiredStripArr(multiSipmBarr_pre, hitIdLsSipmBarr_pre, hitStaLsSipmBarr_pre, uvlTLsSipmBarr_pre, dvlTLsSipmBarr_pre);
		// detector performance statistics //
		for(int ii = 0; ii < 2; ii++){ // loop over MWDC arrays
			tofw[ii]->GetFiredStripArr(multiTOFW_pre[ii], hitIdLsTOFW_pre[ii], hitStaLsTOFW_pre[ii], uvlTLsTOFW_pre[ii], dvlTLsTOFW_pre[ii]);
			for(TAPlaStrip *&str : tofw[ii]->GetStripArr()){
				const int sta = str->GetFiredStatus();
				const int strId = str->GetStripId();
				if(4 == sta) hTOFWFiredDist[ii]->Fill(strId);
				if(4 == sta || 3 == sta){
					if(15 == strId){
						for(int j = 0; j < str->GetUV()->GetData()->GetNLeadingEdge(); j++){
							if(tRef != -9999.){
								hTOFWToTrigUVStrip15[ii]->Fill(j, str->GetUV()->GetLeadingTime(j) - bunchIdTime);
							}
						}
					}
					double tofwToTRef = str->GetUV()->GetLeadingTime() - tRef;
					if(tRef != -9999.){
						hTOFWHitPos[ii]->Fill(strId, str->GetHitPosition());
						hTOFWToTRef[ii]->Fill(strId, tofwToTRef);
					}
					// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
					if(!(tofwToTRef > 10. && tofwToTRef < 200.)){ // belongs to the trigger-generating particle
						str->GetStripData()->SetFiredStatus(-10); // manually altered
					}
				}
			}
			tofw[ii]->GetFiredStripArr(multiTOFW_post[ii], hitIdLsTOFW_post[ii]);
			hTOFWMulti[ii]->Fill(tofw[ii]->GetNFiredStrip());
			for(int j = 0; j < 3; j++){ // loop over the three MWDCs
				for(int k = 0; k < 3; k++){ // loop over XUV SLayers
					for(int l = 0; l < 2; l++){ // loop over layer option (1, 2)
						const int na = dc[ii][j]->GetNAnodePerLayer();
						for(int m = 0; m < na; m++){ // loop over anode per layer
							TAAnode *ano = dc[ii][j]->GetAnode(k, l + 1, m);
							if(ano->GetFiredStatus()){
								hDCFiredDist[ii][j][k]->Fill(l*na+m);
								double dcToTRef = ano->GetData()->GetLeadingTime() - tRef;
								if(tRef != -9999.) hDCToTRef[ii][j][k]->Fill(dcToTRef);
								if(tRef != -9999.){
									if(0 == ii && 0 == k && 0 == j && 0 == l && 40 == m){
										for(int o = 0; o < ano->GetData()->GetNLeadingEdge(); o++)
										hDCToTrig->Fill(o, ano->GetData()->GetLeadingTime(o) - bunchIdTime);
									}
								}
								// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
								if(!(dcToTRef > 0. && dcToTRef < 500.)) ano->GetData()->SetFiredStatus(false);
//								if(1 == ii && 0 == j && 0 == k) ano->GetData()->SetFiredStatus(false);
							}
						} // end for over anode of one layer
						hDCMulti[ii][j][k][l]->Fill(dc[ii][j]->GetNFiredAnodePerLayer(k, l+1));
					} // end for over layer 1 and 2
				} // end for over X-U-V
			} // end for over DCs
		} // end for over DC arrays

		// sipmArr stastics //
		int sipmArrStripId_pre = 0, multiSipmArr = 0, multiSipmArrTRef = 0;
		for(TAChannel *&ch : sipmArr->GetChArr()){
			if(ch->GetFiredStatus()){
				double time = ch->GetTime() - sipmArr->GetDelay() - TOF_T1;
				if(-9999. != TOF_T1) hsipmStripMinusTOF_T1->Fill(sipmArrStripId_pre, time);
				if(fabs(time) < 50.) multiSipmArr++;
				time = ch->GetTime() - bunchIdTime; if(time < 0.) time += 51200.;
				hsipmArrToTrig->Fill(sipmArrStripId_pre, time);
				double timeToTRef = ch->GetLeadingTime() - tRef;
				if(tRef != -9999.){
					hsipmArrToTRef->Fill(sipmArrStripId_pre, timeToTRef);
					cntTmp++;
				}
				// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
				if(!(timeToTRef > -60. && timeToTRef < -10.)) ch->GetData()->SetFiredStatus(false);
				else multiSipmArrTRef++;
			}
			sipmArrStripId_pre++;
		} // end for over channels
		if(tRef != -9999.) hSiPMPlaArrMultiTRef->Fill(multiSipmArrTRef);
//		hSiPMPlaArrMulti->Fill(multiSipmArr);

		for(TAPlaStrip *&str : sipmBarr->GetStripArr()){
			const short sta = str->GetFiredStatus();
			const int strId = str->GetStripId();
			if(4 == sta) hSiPMPlaBarrFiredDist->Fill(strId);
			if(4 == sta || 3 == sta){
				hSiPMPlaBarrHitPos->Fill(strId, str->GetHitPosition());
				if(tRef != -9999.) hsipmBarrToTRef->Fill(strId, str->GetUV()->GetLeadingTime() - tRef);
				timeToTRefSipmBarr = str->GetTime() - tRef;
				// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
				timeToTrigSipmBarr = str->GetTime() - bunchIdTime;
				if(timeToTrigSipmBarr < 0.) timeToTrigSipmBarr += 51200.;
				if(!(timeToTRefSipmBarr > -60. && timeToTRefSipmBarr < -10.)) str->GetStripData()->SetFiredStatus(-10);
//				if(!(timeToTrigSipmBarr > 300. && timeToTrigSipmBarr < 600.)) str->GetStripData()->SetFiredStatus(-10);
			}
		} // end for over sipmArr
		sipmArr->GetFiredStripArr(multiSipmArr_post, hitIdLsSipmArr_post);
		sipmBarr->GetFiredStripArr(multiSipmBarr_post, hitIdLsSipmBarr_post);
		hSiPMPlaBarrMulti->Fill(multiSipmBarr_post);








