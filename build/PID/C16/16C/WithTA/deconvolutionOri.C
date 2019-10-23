// deconvolution.C -- to deconvolute pid spectrum for the real product nuclide yield
// for calculation of interaction cross section only

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
	// beam info in general //
	const ExpData N0(190828., 0.), N0_n(82490., 0.);

	const double nC13 = 190, nC14 = 1126, nC15 = 1165, nC16 = 150869, nC16Ori = 157600, nC17 = 0;
	const double nC13_n = 64, nC14_n = 277, nC15_n = 300, nC16_n = 67236, nC16Ori_n = 70825, nC17_n = 0;


	const double nB12 = 167, nB13 = 217, nB14 = 71, nB15 = 112, nB16 = 0;
	const double nB12_n = 9, nB13_n = 14, nB14_n = 6, nB15_n = 7, nB16_n = 0;
	const double nBe10 = 135, nBe11 = 28, nBe12 = 29, nBe13 = 0;
	const double nBe10_n = 10, nBe11_n = 2, nBe12_n = 1, nBe13_n = 0;
	const double nLi8 = 0, nLi9 = 0;
	const double nLi8_n = 0, nLi9_n = 0;

	#include "C16Ori.C"

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

