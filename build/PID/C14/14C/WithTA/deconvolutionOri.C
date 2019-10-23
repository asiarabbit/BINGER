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
	const ExpData N0(331969, 0), N0_n(57824, 0);
	const double nC11 = 37, nC12 = 530, nC13 = 2149, nC14 = 264006, nC14Ori = 278253, nC15 = 0;
	const double nB10 = 69, nB11 = 454, nB12 = 223, nB13 = 413, nB14 = 14, nB15 = 1;
	const double nBe9 = 68, nBe10 = 134, nBe11 = 25, nBe12 = 12;
	const double nLi7 = 31, nLi8 = 22;

	const double nC11_n = 19, nC12_n = 254, nC13_n = 971, nC14_n = 204695, nC14Ori_n = 216847, nC15_n = 0;
	const double nB10_n = 5, nB11_n = 44, nB12_n = 27, nB13_n = 35, nB14_n = 0, nB15_n = 0;
	const double nBe9_n = 1, nBe10_n = 9, nBe11_n = 0, nBe12_n = 0;
	const double nLi7_n = 8, nLi8_n = 2;

	#include "C14Ori.C"

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
	
	cout << "\033[31;1me_SVT = exp(-sigma*T): " << Exp(CSF*(-1.)/1e27*t5mmC) << "\033[0m\n" << endl;
}

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.C"

