// interactionCS.C -- calculate interaction cross section - i.e. N-Z changing CS

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.h"

using std::cout;
using std::endl;
using std::left;
using std::setw;

#define CCCS // calculate charge-changing cross section

int main(){
	const ExpData t5mmC(4.5126e22, 0.0027e22); // cm-2, target nucleus density
	const ExpData e_dT(0.923763, 0.00582709); // detecting efficiency
	// survival rate: exp(-sigma0*t)
	const ExpData sigma0(762.1677, 18.1800); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	cout << "e_SVT: "; e_SVT.print(); cout << endl;
	const ExpData N0(129309, 0);
	const ExpData CC(4512, sqrt(4512)); // charge-changing yield
	ExpData NR(1025, sqrt(1025)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC10(102, sqrt(102)), e_gC10(0.89, 0.05);
	const ExpData nC9(14, sqrt(14)), e_gC9(0.4, 0.1); // geometrical effciency
	const ExpData nC10_new = nC10 / e_gC10, deltaC10 = nC10_new - nC10;
	const ExpData nC9_new  = nC9  / e_gC9,  deltaC9  = nC9_new  - nC9;
	NR += deltaC10 + deltaC9;
	cout << "deltaC10: "; deltaC10.print(); cout << endl; // DEBUG
	cout << "deltaC9: "; deltaC9.print(); cout << endl; // DEBUG
	cout << "NR: "; NR.print(); cout << endl; // DEBUG
#ifdef CCCS
	const ExpData totalYield(CC);
#else
	const ExpData totalYield(CC + NR/e_dT);
#endif
	const ExpData CS = totalYield / (N0*e_SVT*t5mmC) * 1e27;
	cout << "\033[32;1mTA-in CS: \033[33;1m"; CS.print(); cout << "mb.\033[0m\n";

	const ExpData N0_n(52587, 0);
	const ExpData CC_n(250, sqrt(250)); // charge-changing yield
	ExpData NR_n(273, sqrt(273)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC10_n(35, sqrt(35));
	const ExpData nC9_n(2, sqrt(2)); // geometrical effciency
	const ExpData nC10_new_n = nC10_n / e_gC10, deltaC10_n = nC10_new_n - nC10_n;
	const ExpData nC9_new_n  = nC9_n  / e_gC9,  deltaC9_n  = nC9_new_n  - nC9_n;
	NR_n += deltaC10_n + deltaC9_n;
	cout << "deltaC10: "; deltaC10_n.print(); cout << endl; // DEBUG
	cout << "deltaC9: "; deltaC9_n.print(); cout << endl; // DEBUG
	cout << "NR: "; NR_n.print(); cout << endl; // DEBUG
#ifdef CCCS
	const ExpData totalYield_n(CC_n);
#else
	const ExpData totalYield_n(CC_n + NR_n/e_dT);
#endif
	const ExpData CS_n = totalYield_n / (N0_n*e_SVT*t5mmC) * 1e27;
	cout << "\033[32;1mTA-out CS: \033[33;1m"; CS_n.print(); cout << "mb.\033[0m\n";

	const ExpData CSnet = CS - CS_n;
	cout << "The net cross section: \033[33;1m"; CSnet.print(); cout << "mb.\033[0m\n";

	return 0;
}

#include "/home/asia/pionExp2017/build/PID/errPro/ExpData.C"

