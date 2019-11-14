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
	
	const ExpData pO18(0.8487, 0.0027); // O18 dx2_tof2 rate, univerisal for all fragments
	const ExpData pN(0.8249, 0.0046); // pO18_NOTA: O18 dx2_tof2 rate, univerisal for all fragments_NOTA

	const ExpData ed_T_O(0.9141, 0.0039), ed_T_N(0.9063, 0.0313), ed_T_C(0.876, 0.033), ed_T_B(0.795, 0.042);
	// dispersion rate
	const ExpData eL_O(0.01188, 0.00024), eR_O(0.02167, 0.00033), eD_O(0.02232, 0.00033);
	const ExpData eL_N(0.01048, 0.00022), eR_N(0.01912, 0.00030), eD_N(0.02232, 0.00033);
	const ExpData eL_C(0.00786, 0.00019), eR_C(0.01435, 0.00026), eD_C(0.02232, 0.00033);
	const ExpData eL_B(0.00604, 0.00017), eR_B(0.00895, 0.00021), eD_B(0.02232, 0.00033);

	// beam info in general //
	const ExpData N0(244512, 0);

	const double nN13 = 15, nN14 = 189, nN15 = 736, nN16 = 238, nN17 = 324, nN18 = 16;
	const double nC11 = 49, nC12 = 372, nC13 = 497, nC14 = 300, nC15 = 54, nC16 = 11;
	const double nB9 = 0, nB10 = 104, nB11 = 334, nB12 = 84, nB13 = 31, nB14 = 4;
	const double nBe8 = 0, nBe9 = 50, nBe10 = 48;
	// empty TA
	const ExpData N0_n(74820, 0);
	const double nN13_n = 0, nN14_n = 14, nN15_n = 42, nN16_n = 16, nN17_n = 14, nN18_n = 1;
	const double nC11_n = 7, nC12_n = 14, nC13_n = 27, nC14_n = 7, nC15_n = 3, nC16_n = 1;
	const double nB9_n = 0, nB10_n = 2, nB11_n = 15, nB12_n = 3, nB13_n = 0, nB14_n = 0;
	const double nBe8_n = 0, nBe9_n = 4, nBe10_n = 1;

	const double dx2rate_m9_4 = 0.9009; // dx2[0]>-9&&dx2[0]<4 pass rate
	const double nO15 = 61 / dx2rate_m9_4, nO16 = 608 / dx2rate_m9_4, nO17 = 1012 / dx2rate_m9_4; // dx2[0]>-9&&dx2[0]<4
	const double nO15_n = 4 / dx2rate_m9_4, nO16_n = 79 / dx2rate_m9_4, nO17_n = 201 / dx2rate_m9_4; // dx2[0]>-9&&dx2[0]<4


	#include "O16.C"

	// ################ THE CALCULATION ROUTINE >>>>>>>>>>>>>>>>>>>>> //
	// With Target
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency
	const ExpData nL = (eL*(eR*NL*pD-eD*ND*pR+NT*pD*pR)-NL*pD*pR*pT-eL*eL*NR*pD)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nT = (pR*(eR*NL*pD+eD*ND*pL-NT*pD*pL)+eL*NR*pD*pL)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nR = (eR*(eL*NR*pD-eD*ND*pL+NT*pD*pL)-NR*pD*pL*pT-eR*eR*NL*pD)/(eL*eR*pD*(pL+pR)-pD*pL*pR*pT);
	const ExpData nD = ND/pD;
	const ExpData nTF = nT / e_e; // the final target nuclide yield
	const ExpData CS = nTF / (N0*t5mmC) * 1e27; // unit: mb
	// empty Target // pN: pL_NOTA, pR_NOTA, pT_NOTA, pD_NOTA
	const ExpData nL_n = (eL*(eR*NL_n*pN-eD*ND_n*pN+NT_n*pN*pN)-NL_n*pN*pN*pN-eL*eL*NR_n*pN)/(eL*eR*pN*(pN+pN)-pN*pN*pN*pN);
	const ExpData nT_n = (pN*(eR*NL_n*pN+eD*ND_n*pN-NT_n*pN*pN)+eL*NR_n*pN*pN)/(eL*eR*pN*(pN+pN)-pN*pN*pN*pN);
	const ExpData nR_n = (eR*(eL*NR_n*pN-eD*ND_n*pN+NT_n*pN*pN)-NR_n*pN*pN*pN-eR*eR*NL_n*pN)/(eL*eR*pN*(pN+pN)-pN*pN*pN*pN);
	const ExpData nD_n = ND_n/pN;
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

