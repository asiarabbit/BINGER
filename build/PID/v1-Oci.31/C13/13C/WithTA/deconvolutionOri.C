// deconvolution.C -- to deconvolute pid spectrum for the real product nuclide yield

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.h"

using std::cout;
using std::endl;
using std::left;
using std::setw;


int main(){
	// Target information //
	const ExpData t5mmC(4.5126e22, 0.0027e22); // cm-2, target nucleus density
	// beam info in general //
	const ExpData N0(59111, 0), N0_n(34876, 0);
	const double nC10 = 2, nC11 = 62, nC12 = 399, nC13 = 47010, nC13Ori = 49722, nC14 = 0;
	const double nB9 = 0, nB10 = 25, nB11 = 87, nB12 = 83, nB13 = 2;

	const double nC10_n = 2, nC11_n = 21, nC12_n = 147, nC13_n = 29338, nC13Ori_n = 30905, nC14_n = 0;
	const double nB9_n = 0, nB10_n = 2, nB11_n = 3, nB12_n = 11, nB13_n = 1;

	#include "C13Ori.C"

	// ################ THE CALCULATION ROUTINE >>>>>>>>>>>>>>>>>>>>> //
	// With Target
	const ExpData e_e = e_dT*e_gT; // total efficiency
	ExpData nt = NT / e_e; nt.SetErr(sqrt(N0.val() - nt.val()));
	const ExpData CS = (1.-Sqrt(nt/N0)) / t5mmC * 1e27; // unit: mb
	ExpData nt_n = NT_n / e_e; nt_n.SetErr(sqrt(N0_n.val() - nt_n.val()));
	const ExpData CS_n = (1.-Sqrt(nt_n/N0_n)) / t5mmC * 1e27; // unit: mb

	// net cross section
	const ExpData CSF = CS - CS_n;
	// <<<<<<<<<<<<<<<<<<< THE CALCULATION ROUTINE #################### //



	// %%%%%%%%%%%%%%%%%%%%%%% OUTPUT THE RESULT %%%%%%%%%%%%%%%%%%%%%%%% //
	cout << "\033[36;1m-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%\033[0m" << endl;
	cout << left << endl;
	cout << "With TA, N0: " << N0 << endl;
	cout << "NO TA, N0: " << N0_n << endl;
	cout << "cross section (With TA): "; CS.print(); cout << "mb." << endl;
	cout << "cross section (NO TA): "; CS_n.print(); cout << "mb." << endl;
	cout << "\033[32;1mThe final cross section for " << reaction << " is\n\033[33;1m";
	CSF.print();
	cout << "mb.\033[0m" << endl << endl;
	
	cout << "\033[31;1me_SVT = exp(-sigma*T): ";
	Exp(CSF*(-1.)/1e27*t5mmC).print();
	cout << "\033[0m\n";
}

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.C"

