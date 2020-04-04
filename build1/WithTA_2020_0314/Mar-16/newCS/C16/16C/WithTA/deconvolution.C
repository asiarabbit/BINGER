// deconvolution.C -- to deconvolute pid spectrum for the real product nuclide yield
// Adopted at Nov. 1st, cs is calculated via nuclObj/nuclOri

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
	const ExpData N0(190950, 0);
//	const double nC12 = 13, nC13 = 255, nC14 = 1336, nC15 = 1310, nC16 = 155766, nC16Ori = 164097, nC17 = 0;
	const double nC12 = 13, nC13 = 223.306, nC14 = 1360.47, nC15 = 1329.56, nC16 = 155766, nC16Ori = 164097, nC17 = 0; // only C13-C14-15, Refine - with PDCTa4 Tracking, gaus fit
	const double nB11 = 205, nB12 = 228, nB13 = 348, nB14 = 93, nB15 = 142, nB16 = 0;
	const double nBe10 = 246, nBe11 = 59, nBe12 = 47, nBe13 = 0;
	const double nLi8 = 0, nLi9 = 0;

	const ExpData N0_n(82493, 0);
	const double nC12_n = 2, nC13_n = 82, nC14_n = 339, nC15_n = 332, nC16_n = 69461, nC16Ori_n = 73647, nC17_n = 0;
	const double nB11_n = 13, nB12_n = 12, nB13_n = 21, nB14_n = 8, nB15_n = 7, nB16_n = 0;
	const double nBe10_n = 12, nBe11_n = 2, nBe12_n = 1, nBe13_n = 0;
	const double nLi8_n = 0, nLi9_n = 0;

	#include "C15.C"

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

