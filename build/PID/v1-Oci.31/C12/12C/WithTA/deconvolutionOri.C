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
	const ExpData t5mmC(4.5126e22, 0.0027e22); // cm-2, target density

	const ExpData N0(106735, 0);
	const double nC9 = 2, nC10 = 42, nC11 = 628, nC12 = 84019, nC12Ori = 90257, nC13 = 0;
	const double nB9 = 0, nB10 = 137, nB11 = 208, nB12 = 4;
	const double nBe6 = 0, nBe7 = 45, nBe8 = 0, nBe9 = 15;
	const double nLi5   = 0;

	// empty TA
	const ExpData N0_n(74437, 0);
	const double nC9_n = 0, nC10_n = 25, nC11_n = 346, nC12_n = 60105, nC12Ori_n = 64576, nC13_n = 0;
	const double nB9_n = 0, nB10_n = 41,  nB11_n = 55,  nB12_n = 2;
	const double nBe6_n = 0, nBe7_n = 9,  nBe8_n = 0, nBe9_n = 3;
	const double nLi5_n = 0;

	#include "C12Ori.C"

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

