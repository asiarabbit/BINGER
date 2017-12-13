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
// Last modified: 2017/12/13, SUN Yazhou.										     //
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
		tRef = -9999.;
		const int nUVLEdge_T0_1 = T0_1->GetUV()->GetData()->GetNLeadingEdge();
		const int nDVLEdge_T0_1 = T0_1->GetDV()->GetData()->GetNLeadingEdge();
		// T0_1 UV validity check
		for(int j = 0; j < nUVLEdge_T0_1; j++){
			double time = T0_1->GetUV()->GetLeadingTime(j);
			hT0_1ToTrigUV->Fill(j, time);
			if(time > timeToTrigLowBoundUV && time < timeToTrigHighBoundUV){
				cnt_timeToTrig_T0_1UV++; break;
			}
		}
		// T0_1 DV validity check
		for(int j = 0; j < nDVLEdge_T0_1; j++){
			double time = T0_1->GetDV()->GetLeadingTime(j);
			hT0_1ToTrigDV->Fill(j, time);
			if(time > timeToTrigLowBoundDV && time < timeToTrigHighBoundDV){
				cnt_timeToTrig_T0_1DV++; break;
			}
		}
		// extract the best matched T0_1_UV and T0_1_DV
		double timeUV_T0_1 = -9999., timeDV_T0_1 = -9999., dmin_T0_1 = 1E200;
		for(int j = 0; j < nUVLEdge_T0_1; j++){
			double tmpuv = T0_1->GetUV()->GetLeadingTime(j)+2.7; // XXX: +2.7: for beamTest2016 only
			double tmpdv = T0_1->GetDV()->GetLT(tmpuv);
			double tmpdt = fabs(tmpuv - tmpdv);
			if(tmpuv > timeToTrigHighBoundUV || tmpuv < timeToTrigLowBoundUV) tmpdt += 20.;
			if(tmpdt < dmin_T0_1){
				dmin_T0_1 = tmpdt;
				timeUV_T0_1 = tmpuv; timeDV_T0_1 = tmpdv;
			}
		}
		
		dmin_T0_1 = timeUV_T0_1 - timeDV_T0_1;
		hTOF_T1_pos->Fill(dmin_T0_1);
		if(dmin_T0_1 > -20. && dmin_T0_1 < 60.){
			// XXX +gpar->Val(4): -343.7: for beamTest2016 only
			tRef = (timeUV_T0_1 + timeDV_T0_1) / 2.+gpar->Val(4);
			cntTRef++;
		}
//		if(tRef > timeToTrigHighBoundUV || tRef < timeToTrigLowBoundUV){
//			cout << "\n\nnl: " << nUVLEdge_T0_1 << "\tnDVLEdge_T0_1: " << nDVLEdge_T0_1 << endl; // DEBUG
//			cout << "timeUV_T0_1: " << timeUV_T0_1 << "\ttimeDV_T0_1: " << timeDV_T0_1 << endl; // DEBUG
//			cout << "dmin_T0_1: " << dmin_T0_1 << "\ttRef: " << tRef << endl; // DEBUG
//			T0_1->GetUV()->GetData()->Show(); T0_1->GetDV()->GetData()->Show(); getchar(); // DEBUG
//		}

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
//					if(15 == strId)
					{
						for(int j = 0; j < str->GetUV()->GetData()->GetNLeadingEdge(); j++){
							hTOFWToTrigUV[ii]->Fill(j, str->GetUV()->GetLeadingTime(j));
						}
					}
					hTOFWHitPos[ii]->Fill(strId, str->GetHitPosition());
					double tofwToTrig = str->GetUV()->GetLeadingTime();
					if(-9999. != tRef) hTOFWToTRef[ii]->Fill(strId, tofwToTrig - tRef);
					// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
					if(!(tofwToTrig > gpar->Val(5) && tofwToTrig < gpar->Val(6))){ // belongs to the trigger-generating particle // (300., 700.)->pion2017; (1120., 1160.)->beamTest2016
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
								double dcToTrig = ano->GetData()->GetLeadingTime();
								if(tRef != -9999.) hDCToTRef[ii][j][k]->Fill(dcToTrig - tRef);
//								if(0 == ii && 0 == k && 0 == j)
								{
									for(int i = 0; i < ano->GetData()->GetNLeadingEdge(); i++)
									hDCToTrig->Fill(i, ano->GetData()->GetLeadingTime(i));
								}
								// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
								if(!(dcToTrig > gpar->Val(7) && dcToTrig < gpar->Val(8))) ano->GetData()->SetFiredStatus(false); // (340., 840.)->pion2017; (1000., 1400.)->beamTest2016
//								if(1 == ii && 0 == j && 0 == k) ano->GetData()->SetFiredStatus(false);
							}
						} // end for over anode of one layer
						hDCMulti[ii][j][k][l]->Fill(dc[ii][j]->GetNFiredAnodePerLayer(k, l+1));
					} // end for over layer 1 and 2
				} // end for over X-U-V
			} // end for over DCs
		} // end for over DC arrays

		// sipmArr stastics //
		for(TAChannel *&ch : sipmArr->GetChArr()){
			if(ch->GetFiredStatus()){
				double timeToTrig = ch->GetLT(0., 200., 650.);
				// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
				if(!(timeToTrig > gpar->Val(9) && gpar->Val(10))) ch->GetData()->SetFiredStatus(false);
				if(tRef != -9999.){
					hsipmArrToTRef->Fill(ch->GetSerialId(), timeToTrig - tRef);
				}
				hsipmArrToTrig->Fill(ch->GetSerialId(), timeToTrig);
			}
		} // end for over channels

		for(TAPlaStrip *&str : sipmBarr->GetStripArr()){
			const short sta = str->GetFiredStatus();
			const int strId = str->GetStripId();
			if(4 == sta) hSiPMPlaBarrFiredDist->Fill(strId);
			if(4 == sta || 3 == sta){
				hSiPMPlaBarrHitPos->Fill(strId, str->GetHitPosition());
				// NOTE THAT FIRED STATUS ALTERING SHOULD BE PUT IN THE LAST OF THIS SCRIPTLET! //
				timeToTrigSipmBarr = str->GetTime(0., 300., 600.);
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
		sipmArr->GetFiredStripArr(multiSipmArr_post, hitIdLsSipmArr_post);
		sipmBarr->GetFiredStripArr(multiSipmBarr_post, hitIdLsSipmBarr_post);
		hSiPMPlaArrMulti->Fill(multiSipmArr_post);
		hSiPMPlaBarrMulti->Fill(multiSipmBarr_post);




