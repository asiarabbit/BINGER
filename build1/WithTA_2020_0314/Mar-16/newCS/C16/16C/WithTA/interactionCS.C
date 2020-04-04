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

//#define CCCS // calculate charge-changing cross section

int main(){
	const ExpData t5mmC(4.5126e22, 0.0027e22); // cm-2, target nucleus density
	const ExpData e_dT(0.908396, 0.00459323); // detecting efficiency
	// survival rate: exp(-sigma0*t)
	const ExpData sigma0(908.9607, 23.1691); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	cout << "e_SVT: "; e_SVT.print(); cout << endl;
	const ExpData N0(190950, 0);
	// charge-changing: dsca11==0&&A0&&B0&&C0&&dE1<1.5
	const ExpData CC(6832, sqrt(6832)); // charge-changing yield
	// neutron-removal: dsca11==0&&A0&&B0&&C0&&NRC16
	ExpData NR(3402, sqrt(3402)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC14(1336, sqrt(1336)), e_gC14(0.9578, 0.001);
	const ExpData nC13(255, sqrt(255)), e_gC13(0.50, 0.05); // geometrical effciency
	const ExpData nC14_new = nC14 / e_gC14, deltaC14 = nC14_new - nC14;
	const ExpData nC13_new = nC13 / e_gC13, deltaC13 = nC13_new - nC13;
	NR += deltaC14 + deltaC13;
	cout << "deltaC14: "; deltaC14.print(); cout << endl; // DEBUG
	cout << "deltaC13: "; deltaC13.print(); cout << endl; // DEBUG
	cout << "NR: "; NR.print(); cout << endl; // DEBUG
#ifdef CCCS
	const ExpData totalYield(CC);
#else
	const ExpData totalYield(CC + NR/e_dT);
#endif
	const ExpData CS = totalYield / (N0*e_SVT*t5mmC) * 1e27;
	cout << "\033[32;1mTA-in CS: \033[33;1m"; CS.print(); cout << "mb.\033[0m\n";	

	const ExpData N0_n(82493, 0);
	const ExpData CC_n(324, sqrt(324)); // charge-changing yield
	ExpData NR_n(945, sqrt(945)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC14_n(339, sqrt(339));
	const ExpData nC13_n(82, sqrt(82)); // geometrical effciency
	const ExpData nC14_new_n = nC14_n / e_gC14, deltaC14_n = nC14_new_n - nC14_n;
	const ExpData nC13_new_n = nC13_n / e_gC13, deltaC13_n = nC13_new_n - nC13_n;
	NR_n += deltaC14_n + deltaC13_n;
	cout << "------------- NOTA --------------" << endl; // DEBUG
	cout << "deltaC14: "; deltaC14_n.print(); cout << endl; // DEBUG
	cout << "deltaC13: "; deltaC13_n.print(); cout << endl; // DEBUG
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

