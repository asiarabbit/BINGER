///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAMath/iterFit4.cxx																								 //
//   iterFit.cxx -- source file for method TAMath::iterFit4													 //
//   Introduction: an iterative and asymptotic fitting method. It is precise enough, //
// and currently is used for providing a starting point for further fittings, or to	 //
// give a fast estimate of the tracks.																							 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2019/9/30																														     //
// Last modified: 2020/03/12, SUN Yazhou.																						 //
//																																							     //
//																				 																			     //
// Copyright (C) 2017-2019, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

void TAMath::IterFit4(const double *z, const double *x, const double *r, double *k,
		double *b, int *gGOOD, const int (*LAYER)[6], double d2ThrePD){
	int nF[2]; // number of fired sense wire layers
	for(int i = 2; i--;){ // loop over two DCTas
		if(gGOOD[i] == 2) nF[i] = 4; // 2 layers for each of the two DCs
		if(gGOOD[i] == 1) nF[i] = 3; // 1 layer for 1 DC and 2 for another
	} // end for over two DCTas
	const short n = pow(2, nF[0] + nF[1]);
	double d2[2], kl[3], bl[3];
	kl[2] = k[2]; bl[2] = b[2];
	// initialize kL and bL using LSM
	// the total d2, including contributions from dxTa and dx2
	double d2Tmin = 1E200, d2T; // T: total
	double zt[2][6], xt[2][6];
	for(int i = 2; i--;){
		for(int j = 6; j--;){
			zt[i][j] = -9999.;
			xt[i][j] = -9999.;
		}
	} // end the initialization
	// assign k and b, so as to calculate theta
	Dsquare(z, x, k[0], b[0], gGOOD[0], LAYER[0], d2ThrePD);
	Dsquare(z+6, x+6, k[1], b[1], gGOOD[1], LAYER[1], d2ThrePD);
	double cosTheta[2], sinTheta[2];
	for(int i = 0; i < 2; i++){
		cosTheta[i] = 1. / sqrt(1. + k[i]*k[i]);
		sinTheta[i] = k[i] * cosTheta[i];
	}

	for(int i = 0; i < n; i++){ // each i represents a set of r signs
		// assign signs to drift distances
		for(int I = 0; I < 2; I++){ // loop over DCTas
			for(int j = 0; j < nF[I]; j++){
				const short s = LAYER[I][j], l = I*6+s;
				const double R = r[l] * // the signed drift distance
					(2 * ((i>>(j+I*nF[0])) & 1) - 1); // the sign (left-right ambiguity)
				zt[I][s] = z[l] + R * sinTheta[I];
				xt[I][s] = x[l] - R * cosTheta[I];
			} // end for over j
			d2[I] = Dsquare(zt[I], xt[I], kl[I], bl[I], gGOOD[I], LAYER[I], d2ThrePD);
		} // end for over I (DCTas)
		d2T = d2[0] + d2[1] + Dx2DxTa_2(kl, bl);
		if(d2T < d2Tmin){ // acknowledge the result
			k[0] = kl[0]; k[1] = kl[1];
			b[0] = bl[0]; b[1] = bl[1];
			d2Tmin = d2T;
		} // end if
	} // end for over i

} // end of IterFit4
