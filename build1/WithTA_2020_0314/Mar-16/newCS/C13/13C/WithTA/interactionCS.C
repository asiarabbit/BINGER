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
	const ExpData e_dT(0.93463, 0.00718597); // detecting efficiency
	// survival rate: exp(-sigma0*t)
	const ExpData sigma0(821.8231, 27.3148); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	cout << "e_SVT: "; e_SVT.print(); cout << endl;
	const ExpData N0(59841, 0);
	const ExpData CC(2235, sqrt(2235)); // charge-changing yield (dE1<2.25)
	ExpData NR(602, sqrt(602)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC11(99, sqrt(99)), e_gC11(0.89, 0.01);
	const ExpData nC10(5, sqrt(5)), e_gC10(0.4, 0.05); // geometrical effciency
	const ExpData nC11_new = nC11 / e_gC11, deltaC11 = nC11_new - nC11;
	const ExpData nC10_new = nC10 / e_gC10, deltaC10 = nC10_new - nC10;
	NR += deltaC11 + deltaC10;
	cout << "deltaC11: "; deltaC11.print(); cout << endl; // DEBUG
	cout << "deltaC10: "; deltaC10.print(); cout << endl; // DEBUG
	cout << "NR: "; NR.print(); cout << endl; // DEBUG
#ifdef CCCS
	const ExpData totalYield(CC);
#else
	const ExpData totalYield(CC + NR/e_dT);
#endif
	const ExpData CS = totalYield / (N0*e_SVT*t5mmC) * 1e27;
	cout << "\033[32;1mTA-in CS: \033[33;1m"; CS.print(); cout << "mb.\033[0m\n";

	const ExpData N0_n(35185, 0);
	const ExpData CC_n(177, sqrt(177)); // charge-changing yield (dE1<2.18)
	ExpData NR_n(238, sqrt(238)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC11_n(38, sqrt(38));
	const ExpData nC10_n(7, sqrt(7)); // geometrical effciency
	const ExpData nC11_new_n = nC11_n / e_gC11, deltaC11_n = nC11_new_n - nC11_n;
	const ExpData nC10_new_n = nC10_n / e_gC10, deltaC10_n = nC10_new_n - nC10_n;
	NR_n += deltaC11_n + deltaC10_n;
	cout << "------------- NOTA --------------" << endl; // DEBUG
	cout << "deltaC11: "; deltaC11_n.print(); cout << endl; // DEBUG
	cout << "deltaC10: "; deltaC10_n.print(); cout << endl; // DEBUG
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

