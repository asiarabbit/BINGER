// TOFWPos.C -- calculate the position and orientation of TOF Wall from the raw measurement
// of laser survey

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

using namespace std;

const double DEGREE = 0.01745329222;
inline void enlonger(double ang, double ofst, const double *pIn, double *pOut);
inline void show(const double *p, const char *s);

int main(){
	////////////////////// MEASUREMENTS ///////////////////////////////////////
    // sampling on the surface of the MWDC
    const double Q0[3] = {1558.82, -718.47, 8250.99}; // Q0
    const double Q3[3] = {2770.18, -711.67, 7926.62}; // Q3
	const double A0x[3] = {1624.24, 476.49, 8414.0};
	const double A3x[3] = {2843.63, 477.83, 8086.1};
	const double B0x[3] = {1625.23, -400.5, 8418.0};
	const double B3x[3] = {2842.76, -378.37, 8092.65};

	// the left edge (small X, close to beam) and the right edge (large X, far from beam)
	const double XcL = (A0x[0] + B0x[0]) / 2., ZcL = (A0x[2] + B0x[2]) / 2.;
	const double XcR = (A3x[0] + B3x[0]) / 2., ZcR = (A3x[2] + B3x[2]) / 2.;
	// the central point of the measured plane
	const double Xc = (XcL + XcR)/2., Yc = 0., Zc = (ZcL + ZcR)/2.;
	// the yaw angle
	const double vec[2] = {XcL - XcR, ZcL - ZcR};
	const double yaw = atan(-vec[1]/vec[0]);

	const double ofst = 25.4;
	double CC[3]{};
	const double pIn[3] = {Xc, Yc, Zc};
	enlonger(yaw, ofst, pIn, CC);
	
	/////////////////////// OUTPUT THE RESULT ///////////////////////////////
	cout << fixed << setprecision(3);
	cout << "\033[33;1mNominal Position: (2275, 0, 8500) at 15 degree.\033[0m\n";
	cout << "yaw angle (Degree): " << yaw / DEGREE << endl;
	show(CC, "CC");

	cout << "-------------- SELF CHECK --------------------" << endl;
	cout << "yaw (Q0->Q3): " << atan(-(Q0[2]-Q3[2])/(Q0[0]-Q3[0]))/DEGREE << endl;

	return 0;
}


// enlonger along a given direction from a point as a line
inline void enlonger(double ang, double ofst, const double *pIn, double *pOut){
	pOut[0] = pIn[0] + ofst * sin(ang);
	pOut[1] = pIn[1];
	pOut[2] = pIn[2] + ofst * cos(ang);
}
// show the result of the calculation
inline void show(const double *p, const char *s){
	cout << s << ": ";
	cout << "(" << p[0] << ", " << p[1] << ", " << p[2] << ")" << endl;
}






