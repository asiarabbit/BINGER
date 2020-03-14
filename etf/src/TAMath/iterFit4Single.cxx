///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAMath/iterFit4Single.cxx																					 //
//   iterFit4Single.cxx -- source file for method TAMath::iterFit4Single						 //
//   Introduction: A transmutation of iterFit4. For PDCs around the target,					 //
// left-right ambiguity doesn't exist for X1-X2 both-fired cases. This should be made//
// clear in the code. Iteration only needs to revolve around the singly fried wires. //
// iterFit4Single takes care of this.	But please note that left-right ambiguity still//
// persists for Ma's DCs around Ta. So this method only suits for PDCs' tracking.		 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2020/03/12																													     //
// Last modified: 2020/03/12, SUN Yazhou.																						 //
//																																							     //
//																				 																			     //
// Copyright (C) 2017-2020, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

/// make sure that x, z, and r are initialized to -9999. for unfired wires
/// the program relis on this to function
void TAMath::IterFit4Single(const double *z, const double *x, const double *r, double *k,
	double *b, int *gGOOD, const int (*LAYER)[6], double d2ThrePD){

	short nSingle[2]{}; // number of singly fired wires [0-1]: [PDCArr-U--D]
	short singleLAYER[2]{}; // [PDCArr-U--D], layer ID of the singly fired layers
	for(int i = 2; i--;){
		if(gGOOD[i] == 1) nSingle[i] = 1;
		else if(gGOOD[i] != 2) TAPopMsg::Error("TAMath",
			"iterFit4Single: gGOOD[%d]: %d, neither 0 or 1.", i, gGOOD[i]);
	} // end for

	const short n = pow(2, nSingle[0] + nSingle[1]); // count of iterations
	double d2[2], kl[3], bl[3];
	kl[2] = k[2]; bl[2] = b[2]; // postMagTrk's parameters
	// initialize kL and bL using LSM
	// the total d2, including contributions from dxTa and dx2
	double d2Tmin = 1E200, d2T; // T: total
	double zt[2][6]{}, xt[2][6]{};
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
	// assign signs for X1-X2 bothly fired wires
	for(int I = 0; I < 2; I++){ // loop over PDCArrU-D
		for(int i = 0; i < 2; i++){ // loop over PDC0-1
			const short s = i*2, l = I*6+s; // l, l+1: X1, X2
			if(-9999. == x[l] || -9999. == x[l+1]){
				if(-9999. == x[l]) singleLAYER[I] = s+1;
				if(-9999. == x[l+1]) singleLAYER[I] = s;
				continue; // not bothly fired
			} // end if
			const double dz = fabs(z[l] - z[l+1]); // dz~10. for X1-X2 wires
			if(dz < 15. && dz > 8.){ // adjacent layers
				// X1 //
				const double R0 = r[l]   * Sign(x[l] - x[l+1]); // the signed drift distance
				zt[I][s]   = z[l]   + R0 * sinTheta[I];
				xt[I][s]   = x[l]   - R0 * cosTheta[I];
				// X2 //
				const double R1 = r[l+1] * Sign(x[l+1] - x[l]);
				zt[I][s+1] = z[l+1] + R1 * sinTheta[I];
				xt[I][s+1] = x[l+1] - R1 * cosTheta[I];
			} // end outer if
			else TAPopMsg::Error("TAMath",
				"iterFit4Single: peculiar, supposed to be adjacent X1X2.");
		} // end for over i
	} // end for over I

	for(int i = 0; i < n; i++){ // each i represents a set of r signs
		// assign signs to drift distances
		for(int I = 0; I < 2; I++){ // loop over DCTas
			if(nSingle[I]){
				const short s = singleLAYER[I], l = I*6+s;
				const double R = r[l] * // the signed drift distance
					(2 * ((i>>(I*nSingle[0])) & 1) - 1);
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
