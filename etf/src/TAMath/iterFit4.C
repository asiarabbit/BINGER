///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/iterFit.C													 //
//   iterFit.C -- source file for method TAMath::iterFit							 //
//   Introduction: an iterative and asymptotic fitting method. It is precise enough, //
// and currently is used for providing a starting point for further fittings, or to	 //
// give a fast estimate of the tracks.												 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2019/9/30															     //
// Last modified: 2019/9/30, SUN Yazhou.											 //
//																				     //
//																				     //
// Copyright (C) 2017-2019, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

void TAMath::IterFit4(const double *z, const double *x, const double *r, double *k,
		double *b, int *gGOOD, const int *LAYER[2], double d2ThrePD){
	int nF[2]; // number of fired sense wire layers
	for(int i = 2; i--;){ // loop over two DCTas
		if(gGOOD[i] == 2) nF[i] = 4; // 2 layers for each of the two DCs
		if(gGOOD[i] == 1) nF[i] = 3; // 1 layer for 1 DC and 2 for another
	} // end for over two DCTas
	const short n = pow(2, nF[0] + nF[1]);
	double d2[2], kl[2], bl[2];
	// initialize kL and bL using LSM
	// the total d2, including contributions from dxTa and dx2
	double d2Tmin = 1E200, d2T; // T: total
	d2min[0] = Dsquare(z, x, kL[0], bL[0], gGOOD[0], LAYER[0], deThrePD);
	d2min[1] = Dsquare(z+6, x+6, kL[1], bL[1], gGOOD[1], LAYER[1], deThrePD);
	double zt[2][6], xt[2][6];
	for(int i = 2; i--;)
		for(int j = 6; j--;){
		z[i][j] = -9999.;
		x[i][j] = -9999.;
	}
	const double cosTheta[2], sinTheta[2];
	for(int i = 0; i < 2; i++){
		cosTheta[i] = 1. / sqrt(1. + kL[i]*kL[i]);
		sinTheta[i] = kL[i] * cosTheta[i];
	}

	for(int i = 0; i < n; i++){ // each i represents a set of r signs
		// assign signs to drift distances
		for(int I = 0; I < 2; I++){ // loop over DCTas
			for(int j = 0; j < nF[I]; j++){
				const short s = LAYER[I][j], k = I*6+s;
				const short sign = 2 * ((i>>(j+I*nF[0])) & 1) - 1; // +-1
				const double R = sign * r[k]; // the signed drift distance
				zt[I][s] = z[k] + R * sinTheta[I];
				xt[I][s] = x[k] - R * cosTheta[I];
			} // end for over j
			d2[i] = Dsquare(zt[I], xt[I], kl[I], bl[I], gGOOD[I], LAYER[I], d2ThrePD);
		} // end for over I (DCTas)
		double dxTa = DxTa(kl, bl); // xTaHit0 - xTaHit1
		double x2[2]; // [0-1]: [x2-x2p]
		TAMath::rho(k1, b1, fK2, fB2, nullptr, nullptr, x2); // assign x2 array
		double dx2 = x2[0] - x2[1];
		d2T = d2[0] + d2[1] + Sum2(dxTa, dx2);
		if(d2T < d2Tmin){ // acknowledge the result
			kL[0] = kl[0]; kL[1] = kl[1];
			bL[0] = bl[0]; bL[1] = bl[1];
			d2Tmin = d2T;
		} // end if
	} // end for over i
} // end of IterFit4










