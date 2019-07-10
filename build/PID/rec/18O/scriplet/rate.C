#include <iostream>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char **argv){
    double N2 = atoi(argv[1]), N1 = atoi(argv[2]);
    const double rate = N1 / N2;
    const double rel = sqrt(1. / N1 + 1. / N2);
    const double sigma = rate * rel;
    cout << "relative error: " << rel << endl;
    cout << rate << "(" << sigma << ")" << endl;

    return 0;
}
