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

	const ExpData N0(129309, 0);
	const double nC9 = 14, nC10 = 102, nC11 = 760, nC12 = 106494, nC12Ori = 114526, nC13 = 0;
	const double nB9 = 0, nB10 = 230, nB11 = 351, nB12 = 4;
	const double nBe6 = 0, nBe7 = 93, nBe8 = 0, nBe9 = 16;
	const double nLi5   = 0;

	// empty TA
	const ExpData N0_n(52587, 0);
	const double nC9_n = 2, nC10_n = 35, nC11_n = 190, nC12_n = 44751, nC12Ori_n = 48010, nC13_n = 0;
	const double nB9_n = 0, nB10_n = 28,  nB11_n = 21, nB12_n = 1;
	const double nBe6_n = 0, nBe7_n = 4,  nBe8_n = 0,  nBe9_n = 2;
	const double nLi5_n = 0;

//	const ExpData N0(nC12Ori, sqrt(nC12Ori)), N0_n(nC12Ori_n, sqrt(nC12Ori_n));

	#include "C11.C"

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

