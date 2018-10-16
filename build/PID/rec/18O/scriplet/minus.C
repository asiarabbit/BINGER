// minus.C: error propogation - subtraction

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

void minus(double x1, double dx1, double x2, double dx2, double *p);

int main(int argc, char **argv){
	const double x1 = atof(argv[1]), dx1 = atof(argv[2]);
	const double x2 = atof(argv[3]), dx2 = atof(argv[4]);
	
	double p[4];
	minus(x1, dx1, x2, dx2, p);
	
	const double result = p[0], sigma = p[1];
	const double rel = sigma/result*100.;
	
	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
	
	cout << "\033[1mresult: " << result << "(" << sigma << ", " << rel << "%)" << "\033[0m\n";
	
	return 0;
}

void minus(double x1, double dx1, double x2, double dx2, double *p){
	p[0] = x1 - x2;
	p[1] = sqrt(dx1*dx1 + dx2*dx2);
}
