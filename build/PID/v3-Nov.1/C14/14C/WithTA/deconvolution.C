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
	const ExpData N0(334315, 0);
	const double nC11 = 80, nC12 = 819, nC13 = 2652, nC14 = 275523, nC14Ori = 291907;
	const double nB10 = 114, nB11 = 820, nB12 = 359, nB13 = 593, nB14 = 15, nB15 = 1;
	const double nBe9 = 162, nBe10 = 291, nBe11 = 46, nBe12 = 22;
	const double nLi7 = 116, nLi8 = 24;

	const ExpData N0_n(251927, 0);
	const double nC11_n = 42, nC12_n = 405, nC13_n = 1212, nC14_n = 212263, nC14Ori_n = 226672;
	const double nB10_n = 7, nB11_n = 78, nB12_n = 41, nB13_n = 46, nB14_n = 10, nB15_n = 0;
	const double nBe9_n = 14, nBe10_n = 22, nBe11_n = 4, nBe12_n = 3;
	const double nLi7_n = 12, nLi8_n = 2;

	#include "B12.C"

	// ################ THE CALCULATION ROUTINE >>>>>>>>>>>>>>>>>>>>> //
	// With Target
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency
	const ExpData nL = (eL*(eR*NL*pD-eD*ND*pR+NT*pD*pR)-NL*pD*pR*pT-eL*eL*NR*pD)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nT = (pR*(eR*NL*pD+eD*ND*pL-NT*pD*pL)+eL*NR*pD*pL)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nR = (eR*(eL*NR*pD-eD*ND*pL+NT*pD*pL)-NR*pD*pL*pT-eR*eR*NL*pD)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nD = ND/pD;
	const ExpData nTF = nT / e_e; // the final target nuclide yield
	const ExpData CS = nTF / (N0*t5mmC) * 1e27; // unit: mb
	// empty Target
	const ExpData nL_n = (eL*(eR*NL_n*pD-eD*ND_n*pR+NT_n*pD*pR)-NL_n*pD*pR*pT-eL*eL*NR_n*pD)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nT_n = (pR*(eR*NL_n*pD+eD*ND_n*pL-NT_n*pD*pL)+eL*NR_n*pD*pL)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nR_n = (eR*(eL*NR_n*pD-eD*ND_n*pL+NT_n*pD*pL)-NR_n*pD*pL*pT-eR*eR*NL_n*pD)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nD_n = ND_n/pD;
	const ExpData nTF_n = nT_n / e_e; // the final target nuclide yield
	const ExpData CS_n = nTF_n / (N0_n*t5mmC) * 1e27; // unit: mb
	// net cross section
	const ExpData CSF = CS - CS_n;
	// <<<<<<<<<<<<<<<<<<< THE CALCULATION ROUTINE #################### //




	// %%%%%%%%%%%%%%%%%%%%%%% OUTPUT THE RESULT %%%%%%%%%%%%%%%%%%%%%%%% //
	cout << "\033[36;1m-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%\033[0m" << endl;
	cout << left << endl;
	cout << "With TA, N0: " << N0 << endl;
	cout << setw(20) << "NL" << setw(20) << "NT" << setw(20) << "NR" << setw(20) << "ND" << endl;
	cout << setw(20) <<  NL  << setw(20) <<  NT  << setw(20) <<  NR  << setw(20) <<  ND  << endl;
	cout << setw(20) << "nL" << setw(20) << "nT" << setw(20) << "nR" << setw(20) << "nD" << endl;
	cout << setw(20) <<  nL  << setw(20) <<  nT  << setw(20) <<  nR  << setw(20) <<  nD  << endl;	

	cout << "NO TA, N0: " << N0_n << endl;
	cout << setw(20) << "NL_n" << setw(20) << "NT_n" << setw(20) << "NR_n" << setw(20) << "ND_n" << endl;
	cout << setw(20) <<  NL_n  << setw(20) <<  NT_n  << setw(20) <<  NR_n  << setw(20) <<  ND_n  << endl;
	cout << setw(20) << "nL_n" << setw(20) << "nT_n" << setw(20) << "nR_n" << setw(20) << "nD_n" << endl;
	cout << setw(20) <<  nL_n  << setw(20) <<  nT_n  << setw(20) <<  nR_n  << setw(20) <<  nD_n  << endl;

	cout << "cross section (With TA): "; CS.print(); cout << "mb." << endl;
	cout << "cross section (NO TA): "; CS_n.print(); cout << "mb." << endl;
	cout << "\033[32;1mThe final cross section for " << reaction << " is\n\033[33;1m";
	CSF.print();
	cout << "mb.\033[0m" << endl;
}

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.C"

