// calculate dxTa's uncertainty

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

int main(){
    const int n = 4;
    double z[n] = {-1725.15, -1715.15, -933.64, -923.64};
    double zm = -560.5;

    double sumz = 0., sumz2 = 0., sumz_2 = 0.;
    for(double x : z) { sumz += x; sumz2 += x*x; }
    sumz_2 = sumz * sumz;

    double deno = n*sumz2 - sumz_2;
    double ff = n*zm*zm + sumz2 - 2.*zm*sumz;

    double v_x2 = ff / deno;
    double v_dx2 = 2. * v_x2;
    double sigma_dx2 = sqrt(v_dx2);

    cout << "v(x2): " << v_x2 << endl;
    cout << "v(dx2): " << v_dx2 << endl;
    cout << "sigma(dx2): " << sigma_dx2 << endl;

    return 0;
}
