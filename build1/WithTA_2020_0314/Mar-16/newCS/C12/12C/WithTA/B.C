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
	const char *reaction = "240 MeV/u 12C->12C => ^xB";
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.900669, 0.0165077);
	const ExpData sigma0(762.1677, 18.1800); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section
	const ExpData e_e = e_dT*e_SVT;

	// yeild, TA-in //
	const ExpData N0(129309, 0);
	ExpData B(892., sqrt(892.)); // dE1 in (1.60, 2.15)
	ExpData BeContamination(13, sqrt(13)); B -= BeContamination;
	const ExpData CS = B / (e_e*N0*t5mmC) * 1e27; // unit: mb
	// yeild, TA-out //
	const ExpData N0_n(52587, 0);
	ExpData B_n(114., sqrt(114.)); // dE1 in (1.60, 2.15)
	ExpData BeContamination_n(1, sqrt(1)); B_n -= BeContamination_n;
	B_n /= e_e;
	const ExpData CS_n = B_n / (e_e*N0*t5mmC) * 1e27; // unit: mb
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


