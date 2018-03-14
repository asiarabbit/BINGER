////////////////////////////////////////////////////////////////////////////////////////////////////
// For C++ project DETECTOR written for External Target Facility, CSR, IMP, Lanzhou, China.      //
// Author: Asia SUN(SUN YAZHOU)									//
// Date: Mar. 31, 2017									       //
// MILLEPEDE SOFTWARE ALIGHMENT PACKAGE
////////////////////////////////////////////////////////////////////////////////////////////////////

// TAMatrixSym.C -- definition of symmetic square matrix class, derived from TAMatrix class
#include "TAMatrixSym.h"

TAMatrixSym::TAMatrixSym(){
}
TAMatrixSym::TAMatrixSym(int ndim, const double *data) : TAMatrix(ndim, ndim, data){
	vector<double *> &m = GetDataVec();
	for(int i = 0; i < ndim; i++){
		for(int j = i + 1; j < ndim; j++){
			if(m[i][j] != m[j][i]){
				double mm = (m[i][j]+m[j][i])/2.;
				// 1e-15: precision of double
				if(fabs(m[i][j]-m[j][i])/mm < 1e-3 || fabs(mm) < 1e-5){
					if(m[i][j] == 0. || m[j][i] == 0.) mm = 0.;
					m[i][j] = m[j][i] = mm;
				}
				else{
					cout << "TAMatrixSym::constructor(): \n";
					cout << "\033[1mERROR 2: \033[0m";
					cout << "The input matrix is not symmetrical.\n";
					cout << "\033[31;1mm[" << i << "][" << j << "]: " << m[i][j];
					cout << "\tm[" << j << "][" << i << "]: " << m[j][i];
					cout << " fabs(m[i][j]-m[j][i])/m[i][j]: " << fabs(m[i][j]-m[j][i])/m[i][j] << "\033[0m\n";
					cout << "Enter 0 to exit, or 1 to omit the discrepancy(the two elements would be both set as their average).\nEnter your choice here: ";
					bool pass = true;
					while(!(cin >> pass)){
						cin.clear(); // reset input
						while(cin.get() != '\n')
							continue; // get rid of bad input
						cout << "Please enter 0 or 1 only: ";
					}
					if(pass){
						m[i][j] = m[j][i] = mm;
					}
					else{
						cout << "Exiting...";
						exit(1);
					}
				} // end else
			} // end if(!=)
		} // end for over j
	} // end for over i
} // end the default constructor.
TAMatrixSym::~TAMatrixSym(){
}
	
TAMatrixSym TAMatrixSym::Invert() const{ // SPMINV algorithm
	TAMatrixSym ma_t = *this;
	return SPMINV(ma_t);
} // end function Invert
TAMatrixSym &TAMatrixSym::InvertSelf(){ // SPMINV algorithm, inversion implemented in place
	return SPMINV(*this);
}

TAMatrixSym &TAMatrixSym::SPMINV(TAMatrixSym &ma){
	const int n = ma.GetNRow();
	vector<double *> &m = ma.GetDataVec();
	int pvReg[n]; // register used pivots
	for(int &x : pvReg) x = -1;

	for(int i = 0; i < n; i++){ // invert the matrix in n iterations
		int pivotRo = -1; // row number of pivotal element for i-th step
		
		///////############## CHOOSE THE PIVOTAL ELEMENT FOR STEP I ##############////////
		// the following two arrays is for choosing pivot for i-th step.
		int pvReg_t[n]; // row numbers of candidate pivots
		double epsl_min[n]; // C[i][i](i+1) / C[i][i](i)
		for(int &x : pvReg_t) x = -1;
		for(double &x : epsl_min) x = 1E200;
		int pvReg_tCnt = 0;
		int virgi[n]; // subscripts of virginal rows (not chosen or tried)
		for(int &x : virgi) x = -1;
//		cout << "Mark 1\n"; getchar(); // DEBUG
//		for(int &x : pvReg) cout << "i: " << i << "\tpvReg: " << x << endl; // DEBUG
		int l;
		for(l = 0; l < n; l++){ // loop over candidate diagonal elements
//			cout << "l: " << l << endl; // DEBUG
//			for(int &x : pvReg) cout << "pvReg: " << x << endl; // DEBUG
//			for(int &x : pvReg_t) cout << "pvReg_t: " << x << endl; // DEBUG
			//// so for each loop pvReg_t[loop-l] and epsl_min[loop-l] is assigned. ////
			int virgiCnt = 0;
			for(int j = 0; j < n; j++){
				bool isVirgi = true;
				for(int k = 0; k < n; k++){
					if((pvReg_t[k] >= 0 && j == pvReg_t[k])
					|| (pvReg[k]   >= 0 && j == pvReg[k])) // l-th pivot has been chosen or tried.
					isVirgi = false;
				} // end for over k
				if(isVirgi){
//					cout << "Virgi: " << j << endl; // DEBUG
					virgi[virgiCnt++] = j;
				} // end if
			} // end for over j
//			cout << "virgiCnt: " << virgiCnt << endl; // DEBUG
//			cout << "Mark 2\n"; getchar(); // DEBUG

			// extract the maximum diagonal element in virginal rows //
			double pivot_t = 1E-200; // temporary variable for storing candidate pivot
			for(int j = 0; j < virgiCnt; j++){
				int k = virgi[j];
				if(fabs(m[k][k]) > fabs(pivot_t)){
					pivot_t = m[k][k];
					pvReg_t[pvReg_tCnt] = k;
				}
			} // end for over j
//			cout << "pvReg_tCnt: " << pvReg_tCnt << "\tpvReg_t[pvReg_tCnt]: " << pvReg_t[pvReg_tCnt] << endl; // DEBUG
//			getchar(); // DEBUG
			int pvN = pvReg_t[pvReg_tCnt]; // temporary

			// get the minimum ratio for pivot candidate obtained above. //
			for(int j = 0; j < n; j++){
				if(j != pvN){
					int r, c; // row and column number, temporary
					r = j < pvN ? j: pvN; c = j > pvN ? j: pvN;
					double Cjj = m[j][j] - m[r][c]*m[r][c]/pivot_t;
					double ratio_t = Cjj / m[j][j];
//					cout << "j: " << j; // DEBUG
//					cout << "\nratio_t: " << ratio_t; // DEBUG
//					cout << "\tr: " << r << "\tc: " << c << endl; // DEBUG
//					getchar(); //DEBUG
					if(fabs(ratio_t) < fabs(epsl_min[pvReg_tCnt])){
						epsl_min[pvReg_tCnt] = ratio_t;
					}
				} // end if
			} // end for over j
//			cout << "epsl_min[" << pvReg_tCnt << "]: " << epsl_min[pvReg_tCnt] << endl; // DEBUG
//			getchar(); // DEBUG
			pvReg_tCnt++; // pvReg_tCnt: id of the pivot candidate
			if(fabs(epsl_min[pvReg_tCnt]) > 1E-7){ // current pivot approved!
				pivotRo = pvN; break;
			}
		} // end for over l
		// possibly all the ratios are not eligible. Then the maximum one would be chosen.
//		cout << "l: " << l << endl; getchar(); // DEBUG
		if(l == n - 1){
			double epsl_max = 1E-200;
			for(int j = 0; j < n; j++){
				if(epsl_min[j] != 1E200 && fabs(epsl_min[j]) > epsl_max){
					epsl_max = epsl_min[j];
					pivotRo = pvReg_t[j];
				}
			} // end for over j
		} // end if
		pvReg[i] = pivotRo; // register the selected pivot
		// So, after so much labor, the output pivot is the pivotRo-th diagonal element.
		///////////////############## END OF PIVOT SEARCH ##############/////////////////
		

		////////////^^^^^^^^^^^^^^ IMPLEMENT THE SPMINV ALGORITHM ^^^^^^^^^^^^^^^^^/////////
		// the non-diagonal elements
		int P = pvReg[i];
		int r1, c1, r2, c2; // row and column number, temporary variables
		// // // // // // STEP 1 // // // // // STEP 1 // // // // // STEP 1 // // // //
		for(int j = 0; j < n; j++){
			if(j == P) continue;
			for(int k = j; k < n; k++){
				if(k == P) continue;
				// to calculate in the upper triangle,
				// row number has to be less than column number.
				r1 = j < P ? j : P; c1 = j > P ? j : P;
				r2 = k < P ? k : P; c2 = k > P ? k : P;
				m[j][k] = m[j][k] - m[r1][c1]*m[r2][c2]/m[P][P];
			} // end for over k
		} // end for over j
//		cout << "step 1, m:\n"; // DEBUG
//		ma.Show(); getchar(); // DEBUG
		// // // // // // STEP 2 // // // // // STEP 2 // // // // // STEP 2 // // // //
		for(int j = 0; j < P; j++){
			m[j][P] /= m[P][P];
		}
		for(int j = P + 1; j < n; j++){
			m[P][j] /= m[P][P];
		}
//		cout << "step 2, m:\n"; // DEBUG
//		ma.Show(); getchar(); // DEBUG
		// // // // // // STEP 3 // // // // // STEP 3 // // // // // STEP 3 // // // //
		m[P][P] = -1./m[P][P];
//		cout << "step 3, m:\n"; // DEBUG
//		ma.Show(); getchar(); // DEBUG
	} // end for over i

	// copy the upper triangle to the lower triangle to complete the inverted matrix.
	for(int i = 0; i < n; i++){
		for(int j = i + 1; j < n; j++){
			m[j][i] = m[i][j];
		} // end for over j
	} // end for over i
	if(n == 12){
		cout << "povit:\n"; // DEBUG
		for(int &x : pvReg) cout << x << "\t"; // DEBUG
		cout << endl << endl; // DEBUG
	}
	ma *= -1.;
	return ma;
} // end of function SPMINV



