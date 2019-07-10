// CS: calculate cross section

#include <iostream>
#include <cstdlib>
#include <cmath>

using std::endl;
using std::cout;

// error propagation
// pp: assign the final result
void rate(double N1, double N2, double *pp = nullptr);
void errDiv(double x1, double dx1, double x2, double dx2, double *pp = nullptr);
void errProd(double x1, double dx1, double x2, double dx2, double *pp = nullptr);
void CS(double N0, double deltaN, double eff, double deff);

int main(int argc, char **argv){
	if(argc < 5){
		cout << "./cs <N0> <eff(100%)> <deff(100%)> <dN>" << endl;
		exit(1);
	} // end if

	const double N0 = atof(argv[1]), dN = atof(argv[4]);
	const double eff = atof(argv[2])/100., deff = atof(argv[3])/100.;

	CS(N0, dN, eff, deff);
	return 0;
}

void CS(double N0, double deltaN, double eff, double deff){
	// target information: 5mmC
	const double t = 4.5126e22, dt = 0.0027e22; // cm-2, target density

	double p[4]{}; // an intermediate memory //
	errDiv(deltaN, sqrt(deltaN), eff, deff, p);
	errDiv(p[0], p[1], N0, sqrt(N0), &p[2]);
	errDiv(p[2], p[3], t, dt, p);
	const double cs = p[0]*1e27, dcs = p[1]*1e27; // unit: mb
	const double errRel = dcs/cs*100.; // relative error

//	cout << endl;
	cout << "\033[32;1mThe final cross section is: ";
	cout << "\033[33;1m" << cs << "(" << dcs << ", " << errRel << "%) mb." << endl;
	cout << "\033[0m";
//	cout << "\033[34;1mDONE\033[0m" << endl;
}




void rate(double N1, double N2, double *pp){ // pp: store result
	const double rate = N2 / N1;
	const double rel = sqrt(1. / N1 + 1. / N2);
	const double sigma = rate * rel;

//	cout << "N1: " << N1 << "\tN2: " << N2 << endl;
//	cout << "relative error: " << rel << endl;
//	cout << rate << "(" << sigma << ")" << endl;

	pp[0] = rate; pp[1] = sigma;
}

void errDiv(double x1, double dx1, double x2, double dx2, double *pp){
	const double rate = x1/x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = rate * rel;

//	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
//	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
//	cout << "relative error: " << rel << endl;
//	cout << rate << "(" << sigma << ")" << endl;

	pp[0] = rate; pp[1] = sigma;
}

void errProd(double x1, double dx1, double x2, double dx2, double *pp){
	const double prod = x1*x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = prod * rel;

//	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
//	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
//	cout << "relative error: " << rel << endl;
//	cout << prod << "(" << sigma << ")" << endl;

	pp[0] = prod; pp[1] = sigma;

}

