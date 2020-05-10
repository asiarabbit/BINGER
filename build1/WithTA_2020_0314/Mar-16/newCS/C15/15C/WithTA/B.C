// B.C -- partial charge-chaning cross section for 240MeV/u C16 on a carbon target

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
	// experiment information
	const char *reaction = "240 MeV/u 15C->12C => ^xB";
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_PU(0.9809, 5e-5);
	// interaction c.s. in mb, not any more, using CCCS instead
	const ExpData sigma0(731.8413, 11.9774);
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC));
	const ExpData e_e = e_PU*e_SVT;

	// yeild, TA-in //
	const ExpData N0(152766, 0);
	ExpData B(1410., sqrt(1410.)); // dE1 in (1.10, 1.50)
	const ExpData CS = B / (e_e*N0*t5mmC) * 1e27; // unit: mb
	// yeild, TA-out //
	const ExpData N0_n(129523, 0);
	ExpData B_n(211., sqrt(211.)); // dE1 in (1.10, 1.46)
	B_n /= e_e;
	const ExpData CS_n = B_n / (e_e*N0_n*t5mmC) * 1e27; // unit: mb
	// net cross section
	const ExpData CSF = CS - CS_n;

	cout << "cross section (With TA): "; CS.print(); cout << "mb." << endl;
	cout << "cross section (NO TA): "; CS_n.print(); cout << "mb." << endl;
	cout << "\033[32;1mThe final cross section for " << reaction << " is\n\033[33;1m";
	CSF.print();
	cout << "mb.\033[0m" << endl;

	return 0;
}

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.C"


