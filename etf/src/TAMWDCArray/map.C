///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDCArray/map.C													 //
//   map.C -- source file for method TAMWDCArray::Map								 //
//   Introduction: map function, mainly for pattern recognition. Core method for the //
// data analysis code, capable of multi-track high-efficiency and noise-filtering	 //
// tracking.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/7.															     //
// Last modified: 2018/4/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

//#define DEBUG_MAP

static TACtrlPara *clp = TACtrlPara::Instance();
// subordinate function of void Map();
bool TAMWDCArray::Map(TAMWDC **MWDC, vector<TATrack *> &track, int dcType){
#ifdef DEBUG_MAP
	cout << endl << "\033[32;1m" << GetName() << "\033[0m" << endl; // DEBUG
	cout << "TAMWDCArray::Map(TAMWDC **, vector<TATrack *>&, int dcType):" << endl; // DEBUG
	cout << "dcType: " << dcType << endl; // DEBUG
	getchar(); // DEBUG
#endif

	for(int i = 0; i < 3; i++) if(!MWDC[i]) return false;
//	cout << "track.size(): " << track.size() << endl; // DEBUG
//	getchar(); // DEBUG
	if(track.size() != 0) track.clear();

	bool cmpShow = false; // function compare debug
	// GetDsquareThresholdPerDot(); // stores the minimum of Dsquares of all the combinations
	const double d2Thre = clp->D2Thre();

	double z[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double x[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double t[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double r[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double chi[6] = {-9999., -9999., -9999., -9999., -9999., -9999.};
	double kl = -9999., bl = -9999., d2 = -9999., TOF = -9999.;
	double nstripStray = -1.; // count of strips from a fired strips to the fitted track, a temporary variable
	int firedStripId = -1; // serial id of the fired strip specific to certain track, a temporary variable
	const short nAnodePerLayer0 = MWDC[0]->GetNAnodePerLayer();
	const short nAnodePerLayer1 = MWDC[1]->GetNAnodePerLayer();
	const short nAnodePerLayer2 = MWDC[2]->GetNAnodePerLayer();

	// the live track for the current hit combination
	TATrack newTrack("newTrack", "Particle Track-ASIA.SUN");
	newTrack.SetFitMethod(clp->FitMethod());
	if(clp->FitMethod() == TATrack::kNormalFit){
		newTrack.SetFitPrecision(clp->Precision());
		newTrack.SetFitRotationCenter(MWDC[1]->GetDetPara()->GetZ(), MWDC[1]->GetDetPara()->GetX());
	}
	newTrack.SetDsquareThresholdPerDot(GetDsquareThresholdPerDot());
	char tail[64] = ""; // for naming newTrack
	char type; // MWDC type: V, X, or U
	if(0 == dcType) type = 'X';
	else if(1 == dcType) type = 'U';
	else if(2 == dcType) type = 'V';
	int nu[6]{}; // the counterpart of TATrack::fNu
	int gGOOD = -1, LAYER[6] = {-1, -1, -1, -1, -1, -1};
	int overlapTrackCnt = 0; // for special use
	int nFiredAnodeLayer = 0; // number of fired anode layers
	int overlap = -2; // a temporary variable for the return value of int compare(...)
	// bothFired#: the two sense wire layers of DC# is fired or not;
	// specialEvent: gGOOD == 2; normalEvent: the rest
	bool bothFired0 = false, bothFired1 = false, bothFired2 = false;
	bool normalEvent = false, specialEvent = false;

	//////////////////////////////// THE 6-FOLD NESTED LOOP ////////////////////////////////
	// to loop over all the possible combinations of fired sense wires 
	// for the least-Dsquare tracks for a specific event
	// i == nAnodePerLayer# corresponds to the one situation where all the fired anodes
	// in the layer is deemed as invalid (caused by noise, unwanted particles, etc.).
	for(int i = 0; i <= nAnodePerLayer0; i++){ nu[0] = -1; // DC0-X1
		if(i < nAnodePerLayer0 && MWDC[0]->GetAnodeL1(dcType, i)->GetFiredStatus()) nu[0] = i; // DC0-X1 --------------------------------------------------------------
		if(-1 == nu[0]) continue; // inert anodes within the anode layers would be ignored.
		
	for(int j = 0; j <= nAnodePerLayer0; j++){ nu[1] = -1; // DC0-X2 // i + 3 = i + 2 + 1, adjacency involves two anodes.
		if(j < nAnodePerLayer0 && MWDC[0]->GetAnodeL2(dcType, j)->GetFiredStatus()) nu[1] = j; // DC0-X2 ------------------------------------------
		if(-1 == nu[1]) continue;

	for(int ii = 0; ii <= nAnodePerLayer1; ii++){ nu[2] = -1; // DC1-X1
		if(ii < nAnodePerLayer1 && MWDC[1]->GetAnodeL1(dcType, ii)->GetFiredStatus()) nu[2] = ii; // DC1-X1 ------------------------------------------------------------------------------
		if(-1 == nu[2]) continue;

	for(int jj = 0; jj <= nAnodePerLayer1; jj++){ nu[3] = -1; // DC1-X2
		if(jj < nAnodePerLayer1 && MWDC[1]->GetAnodeL2(dcType, jj)->GetFiredStatus()) nu[3] = jj; // DC1-X2 ----------------------------------------
		if(-1 == nu[3]) continue;

	for(int iii = 0; iii <= nAnodePerLayer2; iii++){ nu[4] = -1; // DC2-X1
		if(iii < nAnodePerLayer2 && MWDC[2]->GetAnodeL1(dcType, iii)->GetFiredStatus()) nu[4] = iii; // DC2-X1 ----------------------------------------------------------------------------
		if(-1 == nu[4]) continue;

	for(int jjj = 0; jjj <= nAnodePerLayer2; jjj++){ nu[5] = -1; // DC2-X2
		if(jjj < nAnodePerLayer2 && MWDC[2]->GetAnodeL2(dcType, jjj)->GetFiredStatus()) nu[5] = jjj; // DC2-X2 --------------------------------------
		if(-1 == nu[5]) continue;
		
			normalEvent = (nu[0] >= 0 || nu[1] >= 0) && (nu[2] >= 0 || nu[3] >= 0) && (nu[4] >= 0 || nu[5] >= 0); // Each MWDC has fired anode(s).
//			if(!normalEvent) continue;
			specialEvent = false;
			if(!normalEvent){
				bothFired0 = nu[0] >= 0 && nu[1] >= 0; // DC0
				bothFired1 = nu[2] >= 0 && nu[3] >= 0; // DC1
				bothFired2 = nu[4] >= 0 && nu[5] >= 0; // DC2
				specialEvent = (bothFired0 && bothFired1) || (bothFired1 && bothFired2) || (bothFired0 && bothFired2);

			} // end if(!normalEvent)
//			if(!(bothFired0 && bothFired1 && bothFired2)) continue;
			if(!normalEvent && !specialEvent) continue; // Each MWDC has to be fired, or any two MWDCs are two-anode-plane fired.
#ifdef DEBUG_MAP
			cout << endl << "i: " << i << "\tj: " << j; // DEBUG
			cout << "\tii: " << ii << "\tjj: " << jj; // DEBUG
			cout << "\tiii: " << iii << "\tjjj: " << jjj << endl; // DEBUG
			for(int i = 0; i < 6; i++){ // DEBUG
				cout << "nu[" << i << "]: " << nu[i] << "\t"; // DEBUG
			} // end for over i DEBUG
			cout << endl; // DEBUG
			cout << "normalEvent: " << normalEvent << "\tspecialEvent: " << specialEvent << endl; // DEBUG
			getchar(); // DEBUG
#endif
			gGOOD = 0;
			for(int i = 0; i < 6; i++){
				x[i] = -9999.; z[i] = -9999.;  t[i] = -9999.; r[i] = -9999.; chi[i] = -9999.;
				LAYER[i] = -1;

				if(nu[i] >= 0){
					LAYER[gGOOD++] = i;
					TAAnodePara *pa = MWDC[i/2]->GetAnode(dcType, i%2+1, nu[i])->GetAnodePara();
					x[i] = pa->GetProjectionX();
					z[i] = pa->GetProjectionZ();
#ifdef DEBUG_MAP
					cout << std::fixed << std::setprecision(9);
					cout << "\tx[" << i << "] = " << x[i] << ";" << endl; // DEBUG
////					getchar(); // DEBUG
					cout << "\tz[" << i << "] = " << z[i] << ";" << endl; // DEBUG
//					getchar(); // DEBUG
#endif
				} // end if
			} // end of for over i
			nFiredAnodeLayer = gGOOD;
			if(specialEvent) gGOOD = 2; // 4 fired anode layers. But here gGOOD is specially assigned with 2 for a special treatment

			// initialization
			kl = -9999.; bl = -9999.; d2 = -9999.; TOF = -9999.;
			// weight for weighted addition of chi to chi2
			double weight[6] = {1., 1., 1., 1., 1., 1.};
			// Here it's Dsquare(z, x...), NOT Dsquare(x, z...), because the coordinate system is different from those conventional ones now
			d2 = TAMath::Dsquare(z, x, kl, bl, gGOOD, LAYER, GetDsquareThresholdPerDot());
#ifdef DEBUG_MAP
			cout << endl << "d2: " << d2; // DEBUG
			cout << "\tkl: " << kl; // DEBUG
			cout << "\tbl: " << bl << endl; // DEBUG
			cout << "d2Thre * nFiredAnodeLayer: " << d2Thre * nFiredAnodeLayer << endl; // DEBUG
//			getchar(); // DEBUG
#endif
			if(d2 < d2Thre * nFiredAnodeLayer){ // a track candidate
//				cout << endl << "d2/nFiredAnodeLayer: " << d2/nFiredAnodeLayer; getchar(); // DEBUG
#ifdef DEBUG_MAP
				cout << "\033[31;1mBINGO!\033[0m" << endl; // DEBUG
				getchar(); // DEBUG
#endif
				bool isBadTrack = false; // whether the current track is bad
				overlapTrackCnt = 0;
////				cout << "track.size(): " << track.size() << endl; // DEBUG
//				getchar(); // DEBUG

				if(0 == dcType){ // MWDC_X
					// get the lt time (t0) of the DC that is closest to the TOFWall,
					// edges of TOFW would be compared to t0 for the suitable one
					int lid = LAYER[nFiredAnodeLayer-1]; // id of the last fired DC anode layer
					TAAnode *ano = MWDC[lid/2]->GetAnode(dcType, lid%2+1, nu[lid]);
					double t0 = ano->GetTime();
					unsigned uid = ano->GetUID();
					const double delta = clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid); // minor correction
					// -20 ~ 250: speculated drift time range
					// 0+t_wire+t_drift=t_DC; 0+t_tof=t_TOF;
					// t_TOF-t_DC=(t_tof-t_wire) - t_drift; => delta-t_drift;
					// (as small and correct as possible while inclusive)
					const double t1 = delta-250., t2 = delta+20.; // the range borders
					TOF = GetTOFWall()->GetTime(kl, bl, nstripStray, firedStripId, t0, t1, t2);
#ifdef DEBUG_MAP
					cout << "firedStripId: " << firedStripId << endl; // DEBUG
					cout << "nstripStray: " << nstripStray << endl; // DEBUG
					getchar(); // DEBUG
#endif
					if(-9999. == TOF){ // no corresponding fired TOF strips are found for the X track
						isBadTrack = true; // So the validity of this track is also futile
						continue;
					}
				} // end if
				// assign drift time array and drift distance array
				if(0 == dcType){
					for(int i = 0; i < 6; i++){ // DC0X1-X2-DC1X1-X2-DC2X1-X2
						if(nu[i] >= 0){
							TAAnode *ano = MWDC[i/2]->GetAnode(dcType, i%2+1, nu[i]);
							ano->GetAnodeData()->SetTOF(TOF);
							// assign weight at the same time
							t[i] = ano->GetDriftTime(weight[i]);
//							cout << "TOF: " << TOF << endl; // DEBUG
//							cout << "1, t[i]: " << t[i] << endl; getchar(); // DEBUG
							// roughly correct time of flight from DC to TOF wall
							unsigned uid = ano->GetUID();
							t[i] +=
								clp->T_tofDCtoTOFW(uid) - clp->T_wireMean(uid);
//							cout << "dt1: " << clp->T_tofDCtoTOFW(uid) << endl;
//							cout << "dt2: " << clp->T_wireMean(uid) << endl;
//							cout << "2, t[i]: " << t[i] << endl; getchar(); // DEBUG
							if(0 == dcType && -9999. != TOF){ // X
								r[i] = ano->GetDriftDistance(t[i], kl);
							} // end if
						} // end if
					} // end for over i
				} // end if(DCtype == X)
#ifdef DEBUG_MAP
				for(double tt: t) cout << "t: " << tt << endl; // DEBUG
				getchar(); // DEBUG
				for(double rr: r) cout << "r: " << rr << endl; // DEBUG
				getchar(); // DEBUG
#endif
				// test the validity of drift time for X tracks
				if(0 == dcType) for(double tt : t){
					if(-9999. != tt && !clp->TimeThre(tt))
						isBadTrack = true;
				} // end loop over drift time
				if(isBadTrack) continue;
				// Assign newTrack
				newTrack.SetData(x, z, t, r, kl, bl, d2, gGOOD, nu, LAYER, weight);
				newTrack.SetTOF(TOF, firedStripId, nstripStray);

				// NOTE that this function would trigger TATrack::Fit() so as to increase
				// system burden
//				cout << "We're going to implement FIT function\n"; getchar(); // DEBUG
				newTrack.GetChi(chi);
//				if(newTrack.fIsDEBUG) newTrack.Show(); // DEBUG

#ifdef DEBUG_MAP
				cout << "newTrack.GetChi(): " << newTrack.GetChi() << endl; // DEBUG
				for(double cc : chi) cout << "cc: " << cc << endl; // DEBUG
				getchar(); // DEBUG
				newTrack.Show(); // DEBUG
#endif
				if(0 == dcType){
//					cout << "newTrack.GetChi(): " << newTrack.GetChi() << endl; getchar(); // DEBUG
//					cout << "clp->ChiThre(): " << clp->ChiThre() << endl; getchar(); // DEBUG
					if(fabs(newTrack.GetChi()) > clp->ChiThre()){
						isBadTrack = true; continue;
					}
					for(double cc : chi){
//						cout << "cc: " << cc << endl; getchar(); // DEBUG
//						cout << "clp->ChiThrePD(): " << clp->ChiThrePD() << endl; getchar(); // DEBUG
						if(-9999. != cc && fabs(cc) > clp->ChiThrePD()) isBadTrack = true;
					} // end for
					if(isBadTrack) continue;
				} // end if
				for(unsigned k = 0; k < track.size(); k++){ // check if two tracks derive from the same one.
					// 0: the two tracks are different; 1: newTrack is defeated by oldTrack;
					// 2: newTrack defeats oldTrack
					overlap = compare(&newTrack, track.at(k), dcType, cmpShow); // compare the two tracks, and mark the obsolete ones
					if(cmpShow){ // DEBUG
						TAPopMsg::Debug(GetName().c_str(), "map: overlap: %d", overlap); // DEBUG
					} // DEBUG
					if(2 == overlap){ // an overlap happend
						overlapTrackCnt++;
					} // end if(overlap == 2)
					if(1 == overlap) continue; // newTrack is part of oldTrack, and is dropped
				} // end for over k
				// eliminate the obsolete tracks
				for(unsigned k = 0; k < track.size(); k++){
					if(!strcmp(track[k]->GetName().c_str(), "OBSOLETE")){ // overlap == 2
						delete track[k]; track[k] = nullptr;
						track.erase(track.begin()+k); // erase track.at(k)
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
					track.push_back(new TATrack(newTrack));
#ifdef DEBUG_MAP
					cout << "track.size(): " << track.size(); // DEBUG
					track[0]->Show(); getchar(); // DEBUG
#endif
				}

				if(cmpShow){ // DEBUG
					TAPopMsg::Debug(GetName().c_str(), "map: After pushback, track.size(): %d", track.size());
				} // end if // DEBUG
//				if(track.size() >= 4) return true;
			} // end if(Dsquare is smaller than the set threshold)
	} // end of DC2-X2 for
	} // end of DC2-X1 for
	} // end of DC1-X2 for
	} // end of DC1-X1 for
	} // end of DC0-X2 for
	} // end of DC0-X1 for

#ifdef DEBUG_MAP
	sleep(3);
	TAPopMsg::Debug(GetName().c_str(), "map: track.size(): %d", track.size());
	for(auto &t : track) t->Show();
	cout << "Map returning true..." << endl; // DEBUG
	getchar(); // DEBUG
#endif
	
	return true;

} // end of function bool Map(...)


// Code Recycle: if(fabs((atan(kl) - atan(track.at(i).GetSlope())) / atan(kl)) > 10. / 5100.) // 10mm / 5100mm
// 0: the two tracks are different; 1: newTrack is defeated by oldTrack;
// 2: newTrack defeats oldTrack
// Here tracks with good == 2 are despised and discriminated
int TAMWDCArray::compare(TATrack *newTrack, TATrack *oldTrack, int dcType, bool show){
//	if(0 != dcType) return 0; // no conclusion could be reached for U(V)projs

	int nstripDeviation = fabs(newTrack->GetFiredStripId() - oldTrack->GetFiredStripId());
	const int vicinity = clp->Vicinity();
//	cout << "vicinity: " << vicinity << endl; getchar(); // DEBUG
	// TOF strip stray tolerance for discern discrete tracks
	const int stripTolerance = clp->StripTolerance();
	int nValid_nu = 0, nValid_nu_temp = 0; // count of positive elements in the array

	if(show){ // DEBUG
		cout << "nValid_nu: " << nValid_nu << "\tnValid_nu_temp: " << nValid_nu_temp << endl; // DEBUG
		cout << "newTrack->GetFiredStripId() - oldTrack->GetFiredStripId(): "; // DEBUG
		cout << newTrack->GetFiredStripId() - oldTrack->GetFiredStripId() << endl; // DEBUG
		cout << "nstripDeviation: " << nstripDeviation << endl; // DEBUG
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
			if(dcType == 0){
				if(newTrack->GetChi() >= oldTrack->GetChi()){
					return 1; // newTrack is nasty
				} // end if
				else{
					oldTrack->SetName("OBSOLETE");
					return 2; // oldTrack is nasty
				} // end else
			} // end if(dcType == 0)
			else return 0; // a conclusion cannot be reached yet for U or V tracks
		} // end if
	} // end if

	nValid_nu = newTrack->GetNFiredAnodeLayer();
	nValid_nu_temp = oldTrack->GetNFiredAnodeLayer();

	int nu[6], nu_temp[6];
	newTrack->GetNu(nu);
	oldTrack->GetNu(nu_temp);

	int vicinityVioCnt = 0; // far from vicinity
	if(nValid_nu < nValid_nu_temp){
		for(int i = 0; i < 6; i++){
			if(nu[i] >= 0 && nu_temp[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
//					cout << "vicinityVioCnt: " << vicinityVioCnt << endl;
					if(vicinityVioCnt == nValid_nu) return 0;
				} // end inner if
			} // end if(nu[i] >= 0)
		} // end for over i

		// confirmed overlapped tracks.
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
		for(int i = 0; i < 6; i++){
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
		for(int i = 0; i < 6; i++){
			if(nu_temp[i] >= 0 && nu[i] >= 0){
				if(abs(nu_temp[i] - nu[i]) > vicinity){
					vicinityVioCnt++;
					if(vicinityVioCnt == nValid_nu_temp) return 0;
				} // end inner if
			} // end if(nu_temp[i] >= 0...)
		} // end for over i

		if(nstripDeviation <= stripTolerance){
			if(0 == dcType){
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




