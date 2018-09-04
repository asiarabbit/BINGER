///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray4/map.C												 //
//   map.C -- source file for method TAMWDCArray4::Map								 //
//   Introduction: map function, mainly for pattern recognition. Core method for the //
// data analysis code. Note that this is a simplified map function for TAMWDCArray4. //
// This class contains 4 MWDCs.														 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/9/3.															     //
// Last modified: 2018/9/3, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

//#define DEBUG_MAP

static TACtrlPara *clp = TACtrlPara::Instance();
// subordinate function of void Map();
bool TAMWDCArray4::Map4(TAMWDC **MWDC, vector<TATrack *> &track, int dcType){
#ifdef DEBUG_MAP
	cout << endl << "\033[32;1m" << GetName() << "\033[0m" << endl; // DEBUG
	cout << "TAMWDCArray4::Map(TAMWDC **, vector<TATrack *>&, int dcType):" << endl; // DEBUG
	cout << "dcType: " << dcType << endl; // DEBUG
	getchar(); // DEBUG
#endif

	for(int i = 0; i < 4; i++) if(!MWDC[i])
		TAPopMsg::Error("TAMWDCArray4::Map4", "one of the 4 MWDCs is null pointer");
	if(track.size() != 0) track.clear();
	const bool cmpShow = false; // debug for function int compare4(...)
	const int UID = GetUID();
	const double d2Thre = clp->D2Thre(UID);
	const double chiThre = clp->ChiThre(UID);
	const double chiThrePD = clp->ChiThrePD(UID);

	// z,x: sense wire position; t,r: drift time, drift distance; chi: fitting residual
	double z[8], x[8], t[8], r[8], chi[8];
	// weight for weighted addition of chi to chi2
	double weight[8] = {1., 1., 1., 1., 1., 1., 1., 1.};
	for(int i = 0; i < 8; i++){
		z[i] = -9999.; x[i] = -9999.; t[i] = -9999.; r[i] = -9999.; chi[i] = -9999.;
	}
	double kl = -9999., bl = -9999., d2 = -9999., TOF = -9999.;

	const short nAnodePerLayer0 = MWDC[0]->GetNAnodePerLayer();
	const short nAnodePerLayer1 = MWDC[1]->GetNAnodePerLayer();
	const short nAnodePerLayer2 = MWDC[2]->GetNAnodePerLayer();
	const short nAnodePerLayer3 = MWDC[3]->GetNAnodePerLayer();

	// the live track for the current hit combination
	TATrack4 newTrack("newTrack", "Particle Track4-ASIA.SUN");
	newTrack.SetFitMethod(clp->FitMethod());
	if(clp->FitMethod() == TATrack::kNormalFit){
		newTrack.SetFitPrecision(clp->Precision());
		const double zc = (MWDC[1]->GetDetPara()->GetZ()+MWDC[2]->GetDetPara()->GetZ())/2.;
		const double xc = (MWDC[1]->GetDetPara()->GetX()+MWDC[2]->GetDetPara()->GetX())/2.;
		newTrack.SetFitRotationCenter(zc, xc);
	}
	newTrack.SetDsquareThresholdPerDot(GetDsquareThresholdPerDot());
	char tail[64] = ""; // for naming newTrack
	char type; // MWDC type: X or Y
	if(0 == dcType) type = 'X';
	else if(1 == dcType) type = 'Y';
	int nu[8]{}; // the counterpart of TATrack::fNu
	// gGOOD: indicator for different fired sense wire layer scenarios
	int gGOOD = -1, LAYER[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
	int overlapTrackCnt = 0; // for special use (checking of the tracking process)
	int nFiredAnodeLayer = 0; // number of fired sense wire layers
	int overlap = -2; // a temporary variable for the return value of int compare2(...)

	//////////////////////////////// THE 8-FOLD NESTED LOOP ////////////////////////////////
	// to loop over all the possible combinations of fired sense wires 
	// for the least-Dsquare tracks for a specific event
	// i == nAnodePerLayer# corresponds to the one situation where all the fired anodes
	// in the layer is deemed as invalid (caused by noise, unwanted particles, etc.).
	for(int i = 0; i <= nAnodePerLayer0; i++){ nu[0] = -1; // DC0-X1
		if(i < nAnodePerLayer0 && MWDC[0]->GetAnodeL1(dcType, i)->GetFiredStatus()) nu[0] = i; // DC0-X1 --------------------------------------------------------------
		if(-1 == nu[0] && i < nAnodePerLayer0) continue; // inert anodes within the anode layers would be ignored

	for(int j = 0; j <= nAnodePerLayer0; j++){ nu[1] = -1; // DC0-X2
		if(j < nAnodePerLayer0 && MWDC[0]->GetAnodeL2(dcType, j)->GetFiredStatus()) nu[1] = j; // DC0-X2 --------------------------------------------------------------
		if(-1 == nu[1] && j < nAnodePerLayer0) continue;

	for(int ii = 0; ii <= nAnodePerLayer1; ii++){ nu[2] = -1; // DC1-X1
		if(ii < nAnodePerLayer1 && MWDC[1]->GetAnodeL1(dcType, ii)->GetFiredStatus()) nu[2] = ii; // DC1-X1 --------------------------------------------------------------
		if(-1 == nu[2] && ii < nAnodePerLayer1) continue;

	for(int jj = 0; jj <= nAnodePerLayer1; jj++){ nu[3] = -1; // DC1-X2
		if(jj < nAnodePerLayer1 && MWDC[1]->GetAnodeL2(dcType, jj)->GetFiredStatus()) nu[3] = jj; // DC1-X2 --------------------------------------------------------------
		if(-1 == nu[3] && jj < nAnodePerLayer1) continue;

	for(int iii = 0; iii <= nAnodePerLayer2; iii++){ nu[4] = -1; // DC2-X1
		if(iii < nAnodePerLayer2 && MWDC[2]->GetAnodeL1(dcType, iii)->GetFiredStatus()) nu[4] = iii; // DC2-X1 --------------------------------------------------------------
		if(-1 == nu[4] && iii < nAnodePerLayer2) continue;

	for(int jjj = 0; jjj <= nAnodePerLayer2; jjj++){ nu[5] = -1; // DC2-X2
		if(jjj < nAnodePerLayer2 && MWDC[2]->GetAnodeL2(dcType, jjj)->GetFiredStatus()) nu[5] = jjj; // DC2-X2 --------------------------------------------------------------
		if(-1 == nu[5] && jjj < nAnodePerLayer2) continue;

	for(int iiii = 0; iiii <= nAnodePerLayer3; iiii++){ nu[6] = -1; // DC3-X1
		if(iiii < nAnodePerLayer3 && MWDC[3]->GetAnodeL1(dcType, iiii)->GetFiredStatus()) nu[6] = iiii; // DC3-X1 --------------------------------------------------------------
		if(-1 == nu[6] && iiii < nAnodePerLayer3) continue;

	for(int jjjj = 0; jjjj <= nAnodePerLayer3; jjjj++){ nu[7] = -1; // DC3-X2
		if(jjjj < nAnodePerLayer3 && MWDC[3]->GetAnodeL2(dcType, jjjj)->GetFiredStatus()) nu[7] = jjjj; // DC3-X2 --------------------------------------------------------------
		if(-1 == nu[7] && jjjj < nAnodePerLayer3) continue;

			nFiredAnodeLayer = 0;
			for(int i = 0; i < 8; i++){
				LAYER[i] = -1;
				if(nu[i] >= 0) LAYER[nFiredAnodeLayer++] = i;
			}
			if(nFiredAnodeLayer < 4) continue;
			gGOOD = nFiredAnodeLayer;
			if(4 == nFiredAnodeLayer) if(nu[LAYER[0]]/2 == nu[LAYER[1]]/2 && nu[LAYER[2]]/2 == nu[LAYER[3]]/2) gGOOD = 2;
#ifdef DEBUG_MAP
			cout << endl << "i: " << i << "\tj: " << j; // DEBUG
			cout << "\tii: " << ii << "\tjj: " << jj << endl; // DEBUG
			cout << "\tiii: " << iii << "\tjjj: " << jjj; // DEBUG
			cout << "\tiiii: " << iiii << "\tjjjj: " << jjjj << endl; // DEBUG
			for(int i = 0; i < 8; i++){ // DEBUG
				cout << "nu[" << i << "]: " << nu[i] << "\t"; // DEBUG
			} // end for over i DEBUG
			getchar(); // DEBUG
#endif
			for(int i = 0; i < 8; i++){
				x[i] = -9999.; z[i] = -9999.; t[i] = -9999.; r[i] = -9999.;
				r[i] = -9999.; chi[i] = -9999.; weight[i] = 1.;
				if(nu[i] >= 0){
					TAAnodePara *pa = MWDC[i/2]->GetAnode(dcType, i%2+1, nu[i])->GetAnodePara();
					x[i] = pa->GetProjectionX();
					z[i] = pa->GetProjectionZ();
#ifdef DEBUG_MAP
					cout << std::fixed << std::setprecision(9);
					cout << "\tx[" << i << "] = " << x[i] << ";" << endl; // DEBUG
					cout << "\tz[" << i << "] = " << z[i] << ";" << endl; // DEBUG
#endif
				} // end if
			}
			
			// initialization
			kl = -9999.; bl = -9999.; d2 = -9999.; TOF = -9999.;
			// Here it's Dsquare(z, x...), NOT Dsquare(x, z...), because the coordinate system is different from those conventional ones now
			d2 = TAMath::Dsquare8(z, x, kl, bl, gGOOD, LAYER, GetDsquareThresholdPerDot());
#ifdef DEBUG_MAP
			cout << endl << "d2: " << d2; // DEBUG
			cout << "\tkl: " << kl; // DEBUG
			cout << "\tbl: " << bl << endl; // DEBUG
			cout << "d2Thre * nFiredAnodeLayer: " << d2Thre * nFiredAnodeLayer << endl; // DEBUG
#endif
			if(d2 < d2Thre * nFiredAnodeLayer){ // a track candidate
#ifdef DEBUG_MAP
				cout << "\033[31;1mBINGO!\033[0m" << endl; // DEBUG
				getchar(); // DEBUG
#endif
				bool isBadTrack = false; // whether the current track is bad
				overlapTrackCnt = 0;

				// get the lt time of the DC that is closest to the PlaT0,
				// edges of PlaT0 would be compared to t0 for the suitable one
				int lid = LAYER[0]; // using the layer closet to fPlaT0
				TAAnode *ano = MWDC[lid/2]->GetAnode(dcType, lid%2+1, nu[lid]);
				const double t0 = ano->GetTime();
				const unsigned uid = ano->GetUID();
				const double delta = clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid); // minor correction
				// -20 ~ 250: speculated drift time range
				// 0+t_wire_t_drift=t_DC; 0+t_tof=t_TOF;
				// t_TOF-t_DC=(t_tof-t_wire) - t_drift; => delta-t_drift;
				// (as small and correct as possible while inclusive)
				// drift time bound for drift time start selection
				double drfTA = gp->Val(42), drfTB = gp->Val(43);
				static const short detId = GetDetId(); // 8-9: PDCArr; 6-7: DCTaArr
				if(8 == detId || 9 == detId){ // PDCArr
					drfTA = gp->Val(103); drfTB = gp->Val(104); // corresponds to larger drfit time
				}
				const double t1 = delta - drfTB, t2 = delta - drfTA; // the range borders
				TOF = GetPlaT0()->GetTime(t0, t1, t2);
				if(-9999. == TOF){ // drift time start is not available
					isBadTrack = true;
					continue;
				}
				// assign drift time array and drift distance array
				for(int i = 0; i < 8; i++) if(nu[i] >= 0){ // DC0-X1-X2-DC1-X1-X2-DC2-X1-X2-DC3-X1-X2
					TAAnode *ano = MWDC[i/2]->GetAnode(dcType, i%2+1, nu[i]);
					ano->GetAnodeData()->SetTOF(TOF);
					// assign weight at the same time
					t[i] = ano->GetDriftTime(weight[i]);
//					cout << "TOF: " << TOF << endl; // DEBUG
//					cout << "1, t[i]: " << t[i] << endl; getchar(); // DEBUG
					// roughly correct time of flight from DC to PlaT0
					unsigned uid = ano->GetUID();
					t[i] +=
						clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid);
//					cout << "dt1: " << clp->T_tofDCtoTOFW(uid) << endl;
//					cout << "dt2: " << clp->T_wireMean(uid) << endl;
//					cout << "2, t[i]: " << t[i] << endl; getchar(); // DEBUG
					if(-9999. != TOF) r[i] = ano->GetDriftDistance(t[i], kl);
				} // end for over i and if
#ifdef DEBUG_MAP
				for(double tt : t) cout << "t: " << tt << endl; // DEBUG
				getchar(); // DEBUG
				for(double rr : r) cout << "r: " << rr << endl; // DEBUG
				getchar(); // DEBUG
#endif
				// test the validity of drift time for X tracks
				for(double tt : t){
					if(-9999. != tt && !clp->TimeThre(tt, GetUID()))
						isBadTrack = true;
				} // end loop over drift time
				if(isBadTrack) continue;
				// assign newTrack
				newTrack.SetData(x, z, t, r, kl, bl, d2, gGOOD, nu, LAYER, weight);
				newTrack.SetTOF(TOF, -1, -9999.);
				
				// NOTE that this function would trigger TATrack::Fit() so as to increase
				// system burden
//				cout << "We're going to implement FIT function\n"; getchar(); // DEBUG
				newTrack.GetChi(chi);
//				if(newTrack.fIsDEBUG) newTrack.Show(); // DEBUG
#ifdef DEBUG_MAP
				cout << "newTrack.GetChi(): " <<  newTrack.GetChi() << endl; // DEBUG
				for(double cc : chi) cout << "cc: " << cc << endl; // DEBUG
				getchar(); // DEBUG
				newTrack.Show(); // DEBUG
#endif
				if(fabs(newTrack.GetChi()) > chiThre){
					isBadTrack = true; continue;
				}
				for(double cc : chi){
//					cout << "cc: " << cc << endl; getchar(); // DEBUG
//					cout << "clp->ChiThrePD(): " << clp->ChiThrePD() << endl; getchar(); // DEBUG
					if(-9999. != cc && fabs(cc) > chiThrePD) isBadTrack = true;
				} // end for
				if(isBadTrack) continue;
				////// INCOMPATIBILITY CHECK //////
				for(unsigned k = 0; k < track.size(); k++){ // check if two tracks derive from the same one
					// 0: the two tracks are different; 1: newTrack is defeated by oldTrack;
					// 2: newTrack defeats oldTrack
					overlap = compare4(&newTrack, track.at(k), type, cmpShow); // compare the two tracks, and mark the obsolete ones
					if(cmpShow){ // DEBUG
						TAPopMsg::Debug(GetName().c_str(), "map: overlap: %d", overlap); // DEBUG
					} // DEBUG
					if(2 == overlap){ // an overlap happened
						overlapTrackCnt++;
					} // end if
					if(1 == overlap) continue; // newTrack is part of oldTrack, and is dropped
				} // end for over k
				// eliminate the obsolete tracks
				for(unsigned k = 0; k < track.size(); k++){
					if(!strcmp(track[k]->GetName().c_str(), "OBSOLETE")){ // overlap == 2
						delete track[k]; track[k] = nullptr;
						track.erase(track.begin()+k); // erase track.at[k];
						k--;
					} // end if
				} // end for over k
#ifdef DEBUG_MAP
				if(1 != overlap){
					TAPopMsg::Debug(GetName().c_str(), "map: New track confirmed.");
				}
#endif
				if(cmpShow){ // DEBUG
					TAPopMsg::Debug(GetName().c_str(), "map: Before pushback, track.size(): %d", track.size());
				} // end if // DEBUG
				if(1 != overlap){ // new track accepted
					newTrack.SetName(GetName());
					sprintf(tail, "->Track%c_%lu", type, track.size());
					newTrack.AppendName(tail);
					track.push_back(new TATrack4(newTrack));
#ifdef DEBUG_MAP
					cout << "track.size(): " << track.size(); // DEBUG
					track[0]->Show(); getchar(); // DEBUG
#endif
				}
				if(cmpShow){ // DEBUG
					TAPopMsg::Debug(GetName().c_str(), "map: After pushback, track.size(): %d", track.size());
				} // end if // DEBUG
			} // end if(Dsquare is smaller than the set threshold)

	} // end of DC3-X2 loop
	} // end of DC3-X1 loop
	} // end of DC2-X2 loop
	} // end of DC2-X1 loop
	} // end of DC1-X2 loop
	} // end of DC1-X1 loop
	} // end of DC0-X2 loop
	} // end of DC0-X1 loop

#ifdef DEBUG_MAP
//	sleep(3);
	TAPopMsg::Debug(GetName().c_str(), "map: track.size(): %d", track.size());
	for(auto &t : track) t->Show();
	cout << "Map returning true..." << endl; // DEBUG
	getchar(); // DEBUG
#endif
	
	return true;

} // end of Map function

int TAMWDCArray4::compare4(TATrack *newTrack, TATrack *oldTrack, char type, bool show){
	if('X' != type && 'Y' != type) return 0; // no conclusion could be reached for U(V)projs

	int nstripDeviation = 0;
	const int vicinity = clp->Vicinity();
//	cout << "vicinity: " << vicinity << endl; getchar(); // DEBUG
	// TOF strip stray tolerance for discern discrete tracks
	const int stripTolerance = clp->StripTolerance();
	int nValid_nu = 0, nValid_nu_temp = 0; // count of positive elements in the array

	nValid_nu = newTrack->GetNFiredAnodeLayer();
	nValid_nu_temp = oldTrack->GetNFiredAnodeLayer();

	int nu[8], nu_temp[8];
	newTrack->GetNu(nu);
	oldTrack->GetNu(nu_temp);

	if(show){ // DEBUG
		cout << "nValid_nu: " << nValid_nu << "\tnValid_nu_temp: " << nValid_nu_temp << endl; // DEBUG
		cout << "newTrack->GetFiredStripId() - oldTrack->GetFiredStripId(): "; // DEBUG
		newTrack->Show(); oldTrack->Show();
		getchar(); // DEBUG
	} // end if(show) // DEBUG

	// special elimination treatment for good == 2 tracks
	if(nstripDeviation <= stripTolerance){
		if(2 == newTrack->GetgGOOD() && oldTrack->GetgGOOD() > 2){
			return 1; // newTrack is nasty
		} // end if
		if(2 == oldTrack->GetgGOOD() && newTrack->GetgGOOD() > 2){
			oldTrack->SetName("OBSOLETE");
			return 2; // oldTrack is nasty
		} // end if
		if(2 == newTrack->GetgGOOD() && 2 == oldTrack->GetgGOOD()){
			if('X' == type || 'Y' == type){
				if(newTrack->GetChi() >= oldTrack->GetChi()){
					return 1; // newTrack is nasty
				} // end if
				else{
					oldTrack->SetName("OBSOLETE");
					return 2; // oldTrack is nasty
				} // end else
			} // end if('X' == type || 'Y' == type)
			else return 0; // a conclusion cannot be reached yet for U or V tracks
		} // end if
	} // end if

	int vicinityVioCnt = 0; // far from vicinity
	if(nValid_nu < nValid_nu_temp){
		for(int i = 0; i < 8; i++){
			if(nu[i] >= 0 && nu_temp[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
//					cout << "vicinityVioCnt: " << vicinityVioCnt << endl;
					if(vicinityVioCnt == nValid_nu) return 0;
				} // end inner if
			} // end if(nu[i] >= 0)
		} // end for over i

		// confirmed overlapped tracks
		if(nstripDeviation <= stripTolerance){
	   		if(show){ // DEBUG
	   			cout << "mark2" << endl; // DEBUG
	   			getchar(); // DEBUG
	   		} // DEBUG
			return 1;
		} // end if
		else{ // not sure
			return 0;
		} // end else
	} // end if

	vicinityVioCnt = 0;
	if(nValid_nu > nValid_nu_temp){
		for(int i = 0; i < 8; i++){
			if(nu_temp[i] >= 0 && nu[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
					if(vicinityVioCnt == nValid_nu_temp) return 0;
				} // end inner if
			} // end if(nu_temp[i] >= 0...)
		} // end for over i
		if(nstripDeviation <= stripTolerance){
			oldTrack->SetName("OBSOLETE"); // OBSOLETE marks the track to be eliminated
			return 2;
		} // end if
		else{
			return 0;
		} // end else
	} // end if

	vicinityVioCnt = 0;
	if(nValid_nu == nValid_nu_temp){
		for(int i = 0; i < 8; i++){
			if(nu_temp[i] >= 0 && nu[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
					if(vicinityVioCnt == nValid_nu_temp) return 0;
				} // end inner if
			} // end if(nu_temp[i] >= 0...)
		} // end for over i

		if(nstripDeviation <= stripTolerance){
			if('X' == type || 'Y' == type){
				if(newTrack->GetChi() < oldTrack->GetChi()){
					oldTrack->SetName("OBSOLETE");
					return 2;
				} // end if
				else{
			   		if(show){ // DEBUG
			   			cout << "mark3" << endl; // DEBUG
			   			getchar(); // DEBUG
			   		} // DEBUG
					return 1;
				} // end else
			} // end if(dcType == 0)
			else return 0; // a conclusion cannot be reached yet for U or V tracks
		} // end if
		else{
			return 0;
		} // end else

	} // end if
	
	return 0;
} // end of function compare



