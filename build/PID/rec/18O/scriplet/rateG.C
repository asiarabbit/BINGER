// geometrical efficiency
#include <iostream>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char **argv){
    const double epsilon = 0.9111, depsilon = 0.0041;
    const double deprel = depsilon / epsilon;
    const double N2 = atoi(argv[1]), N1 = atoi(argv[2]);
    const double rate = N1 / N2 / epsilon;
    const double rel = sqrt(1. / N1 + 1. / N2 + deprel * deprel);
    const double sigma = rate * rel;
    cout << "relative error: " << rel << endl;
    cout << rate << "(" << sigma << ")" << endl;

    return 0;
}
