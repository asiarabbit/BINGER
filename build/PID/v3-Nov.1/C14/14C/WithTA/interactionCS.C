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
	const ExpData e_dT(0.912436, 0.00263738); // detecting efficiency
	// survival rate: exp(-sigma0*t)
	const ExpData sigma0(822.2614, 11.9900); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	cout << "e_SVT: "; e_SVT.print(); cout << endl;
	const ExpData N0(334315, 0);
	const ExpData CC(11677, sqrt(11677)); // charge-changing yield
	ExpData NR(4189, sqrt(4189)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC12(819, sqrt(819)), e_gC12(0.8415, 0.005);
	const ExpData nC11(80, sqrt(80)), e_gC11(0.4, 0.05); // geometrical effciency
	const ExpData nC12_new = nC12 / e_gC12, deltaC12 = nC12_new - nC12;
	const ExpData nC11_new = nC11 / e_gC11, deltaC11 = nC11_new - nC11;
	NR += deltaC12 + deltaC11;
	cout << "deltaC12: "; deltaC12.print(); cout << endl; // DEBUG
	cout << "deltaC11: "; deltaC11.print(); cout << endl; // DEBUG
	cout << "NR: "; NR.print(); cout << endl; // DEBUG
#ifdef CCCS
	const ExpData totalYield(CC);
#else
	const ExpData totalYield(CC + NR/e_dT);
#endif
	const ExpData CS = totalYield / (N0*e_SVT*t5mmC) * 1e27;
	cout << "\033[32;1mTA-in CS: \033[33;1m"; CS.print(); cout << "mb.\033[0m\n";	

	const ExpData N0_n(251927, 0);
	const ExpData CC_n(1059, sqrt(1059)); // charge-changing yield
	ExpData NR_n(2068, sqrt(2068)); // neutron-removal yield
	// make up for geometrical efficiency //
	const ExpData nC12_n(405, sqrt(405));
	const ExpData nC11_n(42, sqrt(42)); // geometrical effciency
	const ExpData nC12_new_n = nC12_n / e_gC12, deltaC12_n = nC12_new_n - nC12_n;
	const ExpData nC11_new_n = nC11_n / e_gC11, deltaC11_n = nC11_new_n - nC11_n;
	NR_n += deltaC12_n + deltaC11_n;
	cout << "------------- NOTA --------------" << endl; // DEBUG
	cout << "deltaC12: "; deltaC12_n.print(); cout << endl; // DEBUG
	cout << "deltaC11: "; deltaC11_n.print(); cout << endl; // DEBUG
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

