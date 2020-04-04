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
	const ExpData e_dT(0.90813, 0.00366526); // detecting efficiency
	// survival rate: exp(-sigma0*t)
	const ExpData sigma0(909.6742, 20.5359); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	cout << "e_SVT: "; e_SVT.print(); cout << endl;
	const ExpData N0(152766, 0);
	const ExpData CC(5485, sqrt(5485)); // charge-changing yield
	ExpData NR(2798, sqrt(2798)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC13(742, sqrt(742)), e_gC13(0.9591, 0.005);
	const ExpData nC12(155, sqrt(155)), e_gC12(0.52, 0.05); // geometrical effciency
	const ExpData nC13_new = nC13 / e_gC13, deltaC13 = nC13_new - nC13;
	const ExpData nC12_new = nC12 / e_gC12, deltaC12 = nC12_new - nC12;
	NR += deltaC13 + deltaC12;
	cout << "deltaC13: "; deltaC13.print(); cout << endl; // DEBUG
	cout << "deltaC12: "; deltaC12.print(); cout << endl; // DEBUG
	cout << "NR: "; NR.print(); cout << endl; // DEBUG
#ifdef CCCS
	const ExpData totalYield(CC);
#else
	const ExpData totalYield(CC + NR/e_dT);
#endif
	const ExpData CS = totalYield / (N0*e_SVT*t5mmC) * 1e27;
	cout << "\033[32;1mTA-in CS: \033[33;1m"; CS.print(); cout << "mb.\033[0m\n";	

	const ExpData N0_n(129523, 0);
	const ExpData CC_n(545, sqrt(545)); // charge-changing yield
	ExpData NR_n(1533, sqrt(1533)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC13_n(352, sqrt(352));
	const ExpData nC12_n(72, sqrt(72)); // geometrical effciency
	const ExpData nC13_new_n = nC13_n / e_gC13, deltaC13_n = nC13_new_n - nC13_n;
	const ExpData nC12_new_n = nC12_n / e_gC12, deltaC12_n = nC12_new_n - nC12_n;
	NR_n += deltaC13_n + deltaC12_n;
	cout << "------------- NOTA --------------" << endl; // DEBUG
	cout << "deltaC13: "; deltaC13_n.print(); cout << endl; // DEBUG
	cout << "deltaC12: "; deltaC12_n.print(); cout << endl; // DEBUG
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

