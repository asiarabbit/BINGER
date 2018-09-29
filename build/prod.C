#include <iostream>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;

void errProd(double x1, double dx1, double x2, double dx2);

int main(int argc, char **argv){
    double x1 = atof(argv[1]), dx1 = atof(argv[2]);
    double x2 = atof(argv[3]), dx2 = atof(argv[4]);

	errProd(x1, dx1, x2, dx2);

    return 0;
}

void errProd(double x1, double dx1, double x2, double dx2){
	const double prod = x1*x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = prod * rel;

	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
    cout << "relative error: " << rel << endl;
    cout << prod << "(" << sigma << ")" << endl;
}

