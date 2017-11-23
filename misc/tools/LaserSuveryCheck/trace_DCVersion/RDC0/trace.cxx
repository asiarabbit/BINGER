// trace.cxx -- tracing the position and angles of a rigid body.

#include "header.C"

//#define DEBUG

int main(){
	// [DCArrL-R][DC0-1-2][A0-1-2-3][x-y-z]; default is Back(facing MG), f -> Front
	// in the global coordinate system
	double A[2][3][4][3]{}, Af[2][3][4][3]{}, B[2][3][4][3]{}, Bf[2][3][4][3]{};
	// in the local coordinate system
	double Ai[2][3][4][3]{}, Aif[2][3][4][3]{}, Bi[2][3][4][3]{}, Bif[2][3][4][3]{};
	// [DCArrL-R][L-R][U-D][x-y-z], default is Back.
	double TOFWi[2][2][2][3]{}, TOFW[2][2][2][3]{};
	offsetGap = 0.7; dcThickness = 132.6; // exclusive to the current DC
//	skewAng = 0. * DEGREE; // exclusive to the current DC
	#include "inputLocal.C" // assign Ai, Bi, Aif, Bif (local coordinate system)
	#include "inputGlobal.C" // assign A, B, Af, Bf (global coordinate system)


	double P[2][3][3]{}, ang[2][3][3]{}; // [DCArr][DC#][X-Y-Z; phi-theta-psi]

	// search for the best position and angle set
	// DCArrR
	// DC0
	P[1][0][0] = 793.13; P[1][0][1] = 16.59; P[1][0][2] = 1462.47;
	ang[1][0][0] = 22.82 * DEGREE; ang[1][0][1] = 0.; ang[1][0][2] = 0.;

	double Pt[3], At[3], chi2t = 0.; // the temporary Pos and ang set
	double Pm[3], Am[3], chi2m = 1E200; // the optimal Pos and ang set
	double spanP[3] = {5., 5., 5.}, spanA[3] = {1. * DEGREE, 1. * DEGREE, 1. * DEGREE};
	const int nP[3] = {20, 20, 20}, nA[3] = {20, 20, 20};
	double dP[3]{}, dA[3]{}; // the position and angle variations
	double *pIn[16], *pOut_m[16], pOut[3]{}; // pOut_m: the measured value
	memset(pIn, 0, sizeof(pIn)); memset(pOut_m, 0, sizeof(pOut_m));
	pIn[0] = Ai[1][0][0]; pOut_m[0] = A[1][0][0]; // A0: 0
	pIn[1] = Ai[1][0][1]; pOut_m[1] = A[1][0][1]; // A1: 1
	pIn[2] = Ai[1][0][2]; pOut_m[2] = A[1][0][2]; // A2: 2
	pIn[3] = Ai[1][0][3]; pOut_m[3] = A[1][0][3]; // A3: 3
	pIn[4] = Bi[1][0][1]; pOut_m[4] = B[1][0][1]; //  B1: 4
	pIn[5] = Bi[1][0][2]; pOut_m[5] = B[1][0][2]; //  B2: 5
	pIn[6] = Bi[1][0][3]; pOut_m[6] = B[1][0][3]; //  B3: 6
//	vec(pIn[4], pIn[1]); // DEBUG
//	vec(pOut_m[4], pOut_m[1]); // DEBUG
//	cout << "LIn: " << L(pIn[6], pIn[0]) << "\tLOut: " << L(pOut_m[6], pOut_m[0]) << endl; // DEBUG
//	getchar(); // DEBUG

//	if(0)
	for(int I = 0; I < 5; I++){
	for(int iA = 0; iA <= nA[0]; iA++){ dA[0] = (1. - 2. * iA / nA[0]) * spanA[0];
	for(int jA = 0; jA <= nA[0]; jA++){ dA[1] = (1. - 2. * jA / nA[1]) * spanA[1];
	for(int kA = 0; kA <= nA[0]; kA++){ dA[2] = (1. - 2. * kA / nA[2]) * spanA[2];
//		dA[0] = 0; dA[1] = 0; dA[2] = 0; // DEBUG
		double pvIn[3], pvOut_m[3]; chi2t = 0.;
		for(int l = 3; l--;) At[l] = ang[1][0][l] + dA[l];
//		At[0] = 0.398282; At[1] = 0.00680678; At[2] = -0.0102974; // DEBUG

		int a, b; // temporary variables
		// B1, A1: 4-1
		a = 4; b = 1;
		vec(pIn[a], pIn[b], pvIn); rotate(At, pvIn, pOut); // assign pOut = R(At).pvIn
		vec(pOut_m[a], pOut_m[b], pvOut_m); // measured vector
		chi2t += chi2(pOut, pvOut_m); // distance of pOut and the measured counterpart
//		cout << "0.pOut: "; show(pOut); cout << "pvOut_m: "; show(pvOut_m); getchar(); // DEBUG
		// B2, A2: 5-2
		a = 5; b = 2;
		vec(pIn[a], pIn[b], pvIn); rotate(At, pvIn, pOut); // assign pOut = R(At).pvIn
		vec(pOut_m[a], pOut_m[b], pvOut_m); // measured vector
		chi2t += chi2(pOut, pvOut_m); // distance of pOut and the measured counterpart
//		cout << "pOut: "; show(pOut); cout << "pvOut_m: "; show(pvOut_m); getchar(); // DEBUG
		// B3, A3: 6-3
		a = 6; b = 3;
		vec(pIn[a], pIn[b], pvIn); rotate(At, pvIn, pOut); // assign pOut = R(At).pvIn
		vec(pOut_m[a], pOut_m[b], pvOut_m); // measured vector
		chi2t += chi2(pOut, pvOut_m); // distance of pOut and the measured counterpart
//		cout << "pOut: "; show(pOut); cout << "pvOut_m: "; show(pvOut_m); getchar(); // DEBUG
		// A0, A3: 0-3
		a = 0; b = 3;
		vec(pIn[a], pIn[b], pvIn); rotate(At, pvIn, pOut); // assign pOut = R(At).pvIn
		vec(pOut_m[a], pOut_m[b], pvOut_m); // measured vector
		chi2t += chi2(pOut, pvOut_m); // distance of pOut and the measured counterpart
//		cout << "pOut: "; show(pOut); cout << "pvOut_m: "; show(pvOut_m); getchar(); // DEBUG
		// B1, B3: 4-6
		a = 4; b = 6;
		vec(pIn[a], pIn[b], pvIn); rotate(At, pvIn, pOut); // assign pOut = R(At).pvIn
		vec(pOut_m[a], pOut_m[b], pvOut_m); // measured vector
		chi2t += chi2(pOut, pvOut_m); // distance of pOut and the measured counterpart
//		cout << "1.pOut: "; show(pOut); cout << "pvOut_m: "; show(pvOut_m); getchar(); // DEBUG

		if(chi2t < chi2m){
			chi2m = chi2t;
			memcpy(Am, At, sizeof(Am));
		}
//		cout << "iA: " << iA << "\tjA: " << jA << "\tkA: " << kA << "\r" << flush;

	} } }
	for(int i = 3; i--;){
		spanA[i] /= nA[i] / 2.5;
		ang[1][0][i] = Am[i];
	}
#ifdef DEBUG
	cout << "\nI: " << I << endl;
	cout << "Am: "; showAngle(Am);
#endif
	} // end the outer loop

//	Am[0] = 0.398282; Am[1] = 0.; Am[2] = 0.; // DEBUG 0.00680678 -0.0102974
//	cout << "Am:\n"; showAngle(Am); cout << "rad: "; show(Am);
//	cout << "Am[0]-ang[1][0][0]: " << (Am[0]-ang[1][0][0]) / DEGREE << endl;
//	cout << "Am[1]-ang[1][0][1]: " << (Am[1]-ang[1][0][1]) / DEGREE << endl;
//	cout << "Am[2]-ang[1][0][2]: " << (Am[2]-ang[1][0][2]) / DEGREE << endl;
//	getchar();

	chi2m = 1E200;
	for(int I = 0; I < 4; I++){
	for(int iP = 0; iP <= nP[0]; iP++){ dP[0] = (1. - 2. * iP / nP[0]) * spanP[0];
	for(int jP = 0; jP <= nP[0]; jP++){ dP[1] = (1. - 2. * jP / nP[1]) * spanP[1];
	for(int kP = 0; kP <= nP[0]; kP++){ dP[2] = (1. - 2. * kP / nP[2]) * spanP[2];
//		dP[0] = 0; dP[1] = 0; dP[2] = 0; // DEBUG
		chi2t = 0.;
		for(int l = 3; l--;) Pt[l] = P[1][0][l] + dP[l];
		for(int ii = 0; ii < 16; ii++) if(pIn[ii]){
//			cout << "pIn[ii]: "; show(pIn[ii]); // DEBUG
//			rotate(Am, pIn[ii], pOut); // DEBUG
//			cout << "rotate, pOut: "; show(pOut);
			globalPos(Pt, Am, pIn[ii], pOut);
			chi2t += chi2(pOut, pOut_m[ii]); // distance of pOut and the measured counterpart
//			if(3 != I) continue; // DEBUG
//			cout << "Pt: "; show(Pt); // DEBUG
//			cout << "pOut: "; show(pOut); cout << "pOut_m: "; show(pOut_m[ii]); // DEBUG
//			cout << "chi2: " << chi2(pOut, pOut_m[ii]) << endl; // DEBUG
//			cout << "ch2t: " << chi2t << endl; getchar(); // DEBUG
		}
		if(chi2t < chi2m){
			chi2m = chi2t;
			memcpy(Pm, Pt, sizeof(Pm));
//			cout << "\tchi2m: " << chi2m << endl; // DEBUG
//			show(dP); getchar();
		}
//		cout << "iP: " << iP << "\tjP: " << jP << "\tkP: " << kP << "\r" << flush;

	} } }
	for(int i = 3; i--;){
		spanP[i] /= nP[i] / 2.5;
		P[1][0][i] = Pm[i];
	}
#ifdef DEBUG
	cout << "\nI: " << I << endl;
	cout << "Pm: "; show(Pm);
#endif
	} // end the outer loop


	cout << "MWDC Array R, DC#0\n";
	cout << "\033[32;1m";
	cout << "Am:\n"; showAngle(Am); cout << "rad: "; show(Am);
	cout << "Pm:\n"; show(Pm);
	cout << "chi2m: " << chi2m << endl;
	cout << "\x1b[0m";
	// output in format
	ofstream fout; fout.open("detPos/detpos.001", ios::app);
	fout << "# __________ ETF -> MWDCArrayR -> MWDC0 __________\n";
	fout << Pm[0] << " \t" << Pm[1] << " \t" << Pm[2] << " \t";
	fout << Am[0] / DEGREE << " \t" << Am[1] / DEGREE << " \t" << Am[2] / DEGREE << " \t4\n";
	fout.close();
//	cout << "Pm[0]-P[1][0][0]: " << Pm[0]-P[1][0][0] << endl;
//	cout << "Pm[1]-P[1][0][1]: " << Pm[1]-P[1][0][1] << endl;
	cout << "Pm[2]-P[1][0][2]: " << Pm[2]-P[1][0][2] << endl;
//	cout << "Am[0]-ang[1][0][0]: " << (Am[0]-ang[1][0][0]) / DEGREE << endl;
//	cout << "Am[1]-ang[1][0][1]: " << (Am[1]-ang[1][0][1]) / DEGREE << endl;
	cout << "Am[2]-ang[1][0][2]: " << (Am[2]-ang[1][0][2]) / DEGREE << endl;
//	getchar();

	return 0;
}


#include "math.C"

