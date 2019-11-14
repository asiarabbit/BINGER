// deconvolution.C -- to deconvolute pid spectrum for the real product nuclide yield

void deconvolution(){
	// contamination rate
	const double  L[4] = {0.04017, 0.02647, 0.01632, 0.01632}; // N-C-B-Be, left
	const double dL[4] = {0.00038, 0.00031, 0.00024, 0.00024}; // the error
	const double  R[4] = {0.03043, 0.02451, 0.01110, 0.01110}; // N-C-B-Be, right
	const double dR[4] = {0.00033, 0.00030, 0.00020, 0.00020}; // the error
	const double  D[4] = {0.01410, 0.02041, 0.02800, 0.02800}; // N-C-B-Be, down
	const double dD[4] = {0.00022, 0.00027, 0.00032, 0.00032}; // the error

	// survival rate; T: target nuclide
	const double  T[4] = {0.8717, 0.8717, 0.8717, 0.8869}; // N13-N14-N15-C12: L-R-T-D
	const double dT[4] = {0.0046, 0.0046, 0.0046, 0.0039}; // N13-N14-N15-C12: L-R-T-D
	// detecting efficiency
	const double  DetEff[4] = {0.9345, 0.9345, 0.9345, 0.9211}; // N13-N15-N14-C12: L-R-T-D
	const double dDetEff[4] = {0.0066, 0.0066, 0.0066, 0.0054}; // N13-N15-N14-C12: L-R-T-D
	const double e_g[4]     = {0.97,   1.,     1.,     1.    }; // N13-N15-N14-C12: L-R-T-D

	// USER INPUT - THE REAL AND SMEARED YIELD //
	const double NL = 63., NR = 337., NT = 255., ND = 310.;
	const short OP = 0; // 0-1-2-3: N-C-B-Be


	// THE PROGRAM OUTPUT - THE DECONVOLUTED YIELD //
	const double nL = (L[OP]*R[OP]*NL*T[OP+3] - L[OP]*L[OP]*NR*T[OP+3] - D[OP]*L[OP]*ND*T[OP+1] + 
		L[OP]*NT*T[OP+3]*T[OP+1] - NL*T[OP+3]*T[OP+1]*T[OP])/(L[OP]*R[OP]*T[OP+3]*T[OP] + L[OP]*R[OP]*T[OP+3]*T[OP+1] - 
		T[OP+3]*T[OP]*T[OP+1]*T[OP]);
	const double nR = -(R[OP]*R[OP]*NL*T[OP+3] - L[OP]*R[OP]*NR*T[OP+3] + D[OP]*R[OP]*ND*T[OP] - 
		R[OP]*NT*T[OP+3]*T[OP] + NR*T[OP+3]*T[OP]*T[OP])/(L[OP]*R[OP]*T[OP+3]*T[OP] + L[OP]*R[OP]*T[OP+3]*T[OP+1] - 
		T[OP+3]*T[OP]*T[OP+1]*T[OP]);
	const double nT = (L[OP]*NR*T[OP+3]*T[OP] + R[OP]*NL*T[OP+3]*T[OP+1] + D[OP]*ND*T[OP]*T[OP+1] - NT*T[OP+3]*T[OP]*T[OP+1])/(
		L[OP]*R[OP]*T[OP+3]*T[OP] + L[OP]*R[OP]*T[OP+3]*T[OP+1] - T[OP+3]*T[OP]*T[OP+1]*T[OP]);
	const double nD = ND/T[OP+3];

	const double nLF = nL / (DetEff[0]*e_g[0]), nRF = nR / (DetEff[1]*e_g[1]);
	const double nTF = nT / (DetEff[2]*e_g[2]), nDF = nD / (DetEff[3]*e_g[3]);

	cout << "\033[36;1m-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%\033[0m" << endl;
	cout << left << endl;
	cout << setw(20) << "NL" << setw(20) << "NR" << setw(20) << "NT" << setw(20) << "ND" << endl;
	cout << setw(20) <<  NL  << setw(20) <<  NR  << setw(20) <<  NT  << setw(20) <<  ND  << endl;
	cout << setw(20) << "nL" << setw(20) << "nR" << setw(20) << "nT" << setw(20) << "nD" << endl;
	cout << setw(20) <<  nL  << setw(20) <<  nR  << setw(20) <<  nT  << setw(20) <<  nD  << endl;
	cout << setw(20) << "nLF" << setw(20) << "nRF" << setw(20) << "nTF" << setw(20) << "nDF" << endl;
	cout << setw(20) <<  nLF  << setw(20) <<  nRF  << setw(20) <<  nTF  << setw(20) <<  nDF  << endl;
}


