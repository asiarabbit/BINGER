#include <iostream>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char **argv){
    if(argc < 5){
        cout << "./rateS <x1> <dx1> <x2> <dx2>" << endl;
        exit(EXIT_FAILURE);
    }
    double N1 = atof(argv[1]), dN1 = atof(argv[2]);
    double N2 = atof(argv[3]), dN2 = atof(argv[4]);
    const double rate = N1 / N2;
    const double rel = sqrt(pow(dN1 / N1, 2) + pow(dN2 / N2, 2));
    const double sigma = rate * rel;
    cout << "relative error: " << rel << endl;
    cout << rate << "(" << sigma << ")" << endl;

    return 0;
}
