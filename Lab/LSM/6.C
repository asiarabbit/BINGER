#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

void vLSM(const int n, double v[2][2], const double *z){
	double zSum = 0., z2Sum = 0.;
	for(int i = n; i--;){
		zSum += z[i];
		z2Sum += z[i]*z[i];
	}
	double deno = n*z2Sum-zSum*zSum;
	v[0][0] = n/deno, v[0][1] = v[1][0] = -zSum/deno, v[1][1] = z2Sum/deno;
} // end function vLSM

inline double norm(double a, double b){
	return sqrt(a*a+b*b);
}

int main(){
	const double ofst = 0;
	double pdc2X1C = -1725.15 + ofst, pdc3X1C = -933.64 + ofst;
	double zPDC[4] = {pdc2X1C, pdc2X1C+10., pdc3X1C, pdc3X1C+10.}; // z-coordiante of of pdc2X1-X2--pdc3X1-X2
	double dc0X1C = norm(473.58,  1961.02);
	double dc1X1C = norm(998.29,  3499.40);
	double dc2X1C = norm(1335.77, 4999.67);
	double zDC[6] = {dc0X1C, dc0X1C+10., dc1X1C, dc1X1C+10., dc2X1C, dc2X1C+10.};
	
	double vPDC[2][2]{}, vDC[2][2]{};
	vLSM(4, vPDC, zPDC);
	vLSM(6, vDC, zDC);

	cout << "PDC error matrix: " << endl;
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			cout << setw(20) << vPDC[i][j];
		} // end for over j
		cout << endl;
	} // end for over i
	cout << "DC error matrix: " << endl;
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			cout << setw(20) << vDC[i][j];
		} // end for over j
		cout << endl;
	} // end for over i

	return 0;
}
