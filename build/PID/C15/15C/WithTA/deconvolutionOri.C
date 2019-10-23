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
	const ExpData N0(152420, 0), N0_n(57824, 0);
	const double nC12 = 100, nC13 = 575, nC14 = 1382, nC15 = 119320, nC15Ori = 125437, nC16 = 0;
	const double nB11 = 208, nB12 = 114, nB13 = 165, nB14 = 129, nB15 = 3;
	const double nBe9 = 52, nBe10 = 84, nBe11 = 14, nBe12 = 10;
	const double nLi7 = 14, nLi8 = 4;

	const double nC12_n = 1923, nC13_n = 129, nC14_n = 308, nC15_n = 46965, nC15Ori_n = 49522, nC16_n = 0;
	const double nB11_n = 9, nB12_n = 13, nB13_n = 5, nB14_n = 3, nB15_n = 0;
	const double nBe9_n = 1, nBe10_n = 4, nBe11_n = 0, nBe12_n = 0;
	const double nLi7_n = 0, nLi8_n = 0;

	#include "C15Ori.C"

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

