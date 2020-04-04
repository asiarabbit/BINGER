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
	const ExpData N0(152766, 0);
	const double nC12 = 155, nC13 = 742, nC14 = 1495, nC15 = 123439, nC15Ori = 130905;
	const double nB11 = 311, nB12 = 197, nB13 = 240, nB14 = 167, nB15 = 4;
	const double nBe9 = 88, nBe10 = 166, nBe11 = 38, nBe12 = 197;
	const double nLi7 = 56, nLi8 = 23;

	const ExpData N0_n(129523, 0);
	const double nC12_n = 72, nC13_n = 352, nC14_n = 793, nC15_n = 109087, nC15Ori_n = 115526;
	const double nB11_n = 34, nB12_n = 26, nB13_n = 27, nB14_n = 11, nB15_n = 2;
	const double nBe9_n = 4, nBe10_n = 17, nBe11_n = 2, nBe12_n = 0;
	const double nLi7_n = 0, nLi8_n = 0;

	#include "C13.C"

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

