// check.cxx -- check the consistency and validity of the laser suvery
// measurements for External Target Facility detectors.

#include "header.C"

// direction cosines x
void cos0(const double *p0, const double *p1);
// direction cosines y
void cos1(const double *p0, const double *p1);
// direction cosines z
void cos2(const double *p0, const double *p1);

const double epslon = 23.;
const double lnorm[2][3] = {{359., 650., 742.55}, {218., 750., 781.04}};

int main(){
	// [DCArrL-R][DC0-1-2][A0-1-2-3][x-y-z]; default is Back(facing MG), f -> Front
	// in the global coordinate system
	double A[2][3][4][3]{}, Af[2][3][4][3]{}, B[2][3][4][3]{}, Bf[2][3][4][3]{};
	// in the local coordinate system
	double Ai[2][3][4][3]{}, Aif[2][3][4][3]{}, Bi[2][3][4][3]{}, Bif[2][3][4][3]{};
	// [DCArrL-R][L-R][U-D][x-y-z], default is Back.
	double TOFWi[2][2][2][3]{}, TOFW[2][2][2][3]{};
	#include "inputLocal.C" // assign Ai, Bi, Aif, Bif (local coordinate system)
	#include "inputGlobal.C" // assign A, B, Af, Bf (global coordinate system)


	for(int i = 0; i < 2; i++){ // loop over MWDC arrays
		for(int j = 0; j < 3; j++){ // loop over MWDCs
//		if(1 == i && 0 == j) continue;
//		if(1 == i && 1 == j) continue;
//		if(1 == i && 2 == j) continue;

			// in a certain MWDC //
			for(int k0 = 0; k0 < 4; k0++){ // loop over four pin poles in the MWDC plate
				for(int k1 = k0+1; k1 < 4; k1++){ // loop over four pin poles in the MWDC plate
					double lA = L(A[i][j][k0], A[i][j][k1]), lB = L(B[i][j][k0], B[i][j][k1]); // horizontal
					double lAB0 = L(A[i][j][k0], B[i][j][k0]), lAB1 = L(A[i][j][k1], B[i][j][k1]); // vertical
					double lAB2 = L(A[i][j][k0], B[i][j][k1]), lAB3 = L(A[i][j][k1], B[i][j][k0]); // cross
					double err = 1E200;

					if((k0 == 0 && k1 == 1) || (k0 == 2 && k1 == 3)){
						if(i == 0 && j == 2){ // MWDCArrR-DC2
							if(lA > 0. && ((err = fabs(lA - 359.)) > epslon)) BEE(i, j, k0, k1, err, 0);
							if(lB > 0. && ((err = fabs(lB - 359.)) > epslon)) BEE(i, j, k0, k1, err, 1);
							if(lAB0 > 0. && ((err = fabs(lAB0 - 650.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB1 > 0. && ((err = fabs(lAB1 - 650.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB2 > 0. && ((err = fabs(lAB2 - 742.55)) > epslon)) BEE(i, j, k0, k1, err, 3);
							if(lAB3 > 0. && ((err = fabs(lAB3 - 742.55)) > epslon)) BEE(i, j, k0, k1, err, 3);
						} // end if(MWDCArrR-DC2)
						else{
							if(lA > 0. && ((err = fabs(lA - 218.)) > epslon)) BEE(i, j, k0, k1, err, 0);
							if(lB > 0. && ((err = fabs(lB - 218.)) > epslon)) BEE(i, j, k0, k1, err, 1);
							if(lAB0 > 0. && ((err = fabs(lAB0 - 750.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB1 > 0. && ((err = fabs(lAB1 - 750.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB2 > 0. && ((err = fabs(lAB2 - 781.04)) > epslon)) BEE(i, j, k0, k1, err, 3);
							if(lAB3 > 0. && ((err = fabs(lAB3 - 781.04)) > epslon)) BEE(i, j, k0, k1, err, 3);
						} // end else
					} // end if((k0 == 0 && k1 == 1) || (k0 == 2 && k1 == 3))


					if((k0 == 0 && k1 == 2) || (k0 == 1 && k1 == 3)){
						if(i == 0 && j == 2){ // MWDCArrR-DC2
							if(lA > 0. && ((err = fabs(lA - 557.)) > epslon)) BEE(i, j, k0, k1, err, 0);
							if(lB > 0. && ((err = fabs(lB - 557.)) > epslon)) BEE(i, j, k0, k1, err, 1);
							if(lAB0 > 0. && ((err = fabs(lAB0 - 650.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB1 > 0. && ((err = fabs(lAB1 - 650.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB2 > 0. && ((err = fabs(lAB2 - 856.01)) > epslon)) BEE(i, j, k0, k1, err, 3);
							if(lAB3 > 0. && ((err = fabs(lAB3 - 856.01)) > epslon)) BEE(i, j, k0, k1, err, 3);
						} // end if(MWDCArrR-DC2)
						else{
							if(lA > 0. && ((err = fabs(lA - 488.)) > epslon)) BEE(i, j, k0, k1, err, 0);
							if(lB > 0. && ((err = fabs(lB - 488.)) > epslon)) BEE(i, j, k0, k1, err, 1);
							if(lAB0 > 0. && ((err = fabs(lAB0 - 750.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB1 > 0. && ((err = fabs(lAB1 - 750.)) > epslon)) BEE(i, j, k0, k1, err, 2);
							if(lAB2 > 0. && ((err = fabs(lAB2 - 894.79)) > epslon)) BEE(i, j, k0, k1, err, 3);
							if(lAB3 > 0. && ((err = fabs(lAB3 - 894.79)) > epslon)) BEE(i, j, k0, k1, err, 3);
						} // end else
					} // end if((k0 == 0 && k1 == 1) || (k0 == 2 && k1 == 3))

				} // end of loop over k1
			} // end of loop over k0
			if(1 == i && 0 == j){
				cout << "DCArr: " << i << "\tDC#j: " << j << "\tphiA: " << phi(A[i][j][0], A[i][j][3]) << endl;
				cout << "DCArr: " << i << "\tDC#j: " << j << "\tphiB: " << phi(B[i][j][1], B[i][j][3]) << endl;
				getchar();
			} // end if
			double dd = d(B[i][j][1],B[i][j][3],B[i][j][2]);
			double ddp = dp(A[i][j][0], A[i][j][3], B[i][j][2], B[i][j][0]);
			vec(A[i][j][1], B[i][j][1]);
			vec(A[i][j][2], B[i][j][2]);
			vec(A[i][j][3], B[i][j][3]);
			if(ddp > 0.){
				cout << "i: " << i << "\tj: " << j << endl;
//				cout << "A[i][j][2]: "; show(A[i][j][2]); cout << endl;
//				cout << "A[i][j][3]: "; show(A[i][j][3]); cout << endl;
//				cout << "A[i][j][1]: "; show(A[i][j][1]); cout << endl;
				cout << "D: A0->A2_A3: " << ddp << endl; getchar();
			}
			// in a certain MWDC //

		} // end of loop over MWDCs
	} // end of loop over MWDC arrays

	return 0;
} // end of the main function


#include "math.C"


