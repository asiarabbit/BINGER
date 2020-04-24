// DCPos.C -- calculate DC position and orientation -> for MWDC0
// using laser survey measurements.
// created: 20180425

// 1. calculate the function of the two measured edges of the MWDC;
// 2. calculate the central Y of the two edges, and the X, Z at the Y, we get point P and Q;
// 3. calculate the coordinate of the mid-point of PQ

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
    // holes in MWDC0; 0: L; 3: R
	const double A0[3] = {153.7586, 394.8057, 2146.556}; 
	const double A3[3] = {837.8053, 401.8507, 1971.8241};
	const double B0[3] = {157.5901, -355.2644, 2135.2797};
	const double B3[3] = {841.9452, -348.8284, 1963.4732};
    // sampling on the surface of the MWDC
	const double A0x[3] = {13.1601, 309.882, 2182.2424};
	const double A3x[3] = {993.2159, 395.7834, 1916.1569};
	const double B0x[3] = {14.7408, -89.4894, 2175.9811};
	const double B3x[3] = {997.807, -268.9078, 1905.3658};
	const bool isFront = true; // if the measured plane is the front plane (downstream)


	////////////////////// the calculation begins ////////////////////////
	// the left edge (small X, close to beam)
	const double YcL = (A0[1] + B0[1]) / 2.; // the central Y coordinate
	const double tL = (YcL - A0x[1]) / (B0x[1] - A0x[1]); // the proportional coefficient
	const double XcL = (B0x[0] - A0x[0]) * tL + A0x[0]; // the central X coordinate
	const double ZcL = (B0x[2] - A0x[2]) * tL + A0x[2]; // the central X coordinate
	// the right edge (large X, far from beam)
	const double YcR = (A3[1] + B3[1]) / 2.; // the central Y coordinate
	const double tR = (YcR - A3x[1]) / (B3x[1] - A3x[1]); // the proportional coefficient
	const double XcR = (B3x[0] - A3x[0]) * tR + A3x[0]; // the central X coordinate
	const double ZcR = (B3x[2] - A3x[2]) * tR + A3x[2]; // the central X coordinate
	// the central point of the measured plane
	const double Xc = (XcL + XcR)/2., Yc = (YcL + YcR)/2., Zc = (ZcL + ZcR)/2.;
	// the yaw angle
	const double vec[2] = {XcL - XcR, ZcL - ZcR};
	const double yaw = atan(-vec[1]/vec[0]);
	// the central point of sense wire layer U1, X1 and V1,
	// together with the geometrical center of the MWDC
	double ofstU1 = -(25.4+110.), ofstX1 = -(25.4+70.), ofstV1 = -(25.4+30.), ofstC = -(25.4+65.);
	if(!isFront){
		ofstU1 = 25.4+20.;
		ofstX1 = 25.4+40.;
		ofstV1 = 25.4+100.;
		ofstC = 25.4+65.;
	}
	double U1C[3]{}, X1C[3]{}, CC[3]{}, V1C[3]{};
	const double pIn[3] = {Xc, Yc, Zc};
	enlonger(yaw, ofstU1, pIn, U1C);
	enlonger(yaw, ofstX1, pIn, X1C);
	enlonger(yaw, ofstC, pIn, CC);
	enlonger(yaw, ofstV1, pIn, V1C);

	/////////////////////// OUTPUT THE RESULT ///////////////////////////////
	cout << fixed << setprecision(3);
	cout << "\033[33;1mNominal Position: (500, 0, 2000) at 15 degree.\033[0m\n";
	cout << "yaw angle (Degree): " << yaw / DEGREE << endl;
	show(U1C, "U1C");
	show(X1C, "X1C");
	show(CC, "CC");
	show(V1C, "V1C");

	cout << "-------------- SELF CHECK --------------------" << endl;
	cout << "YcR-YcL: " << YcR - YcL << endl;
	if(0. != A0[0] && 0. != A3[0])
		cout << "yaw (A0->A3): " << atan(-(A0[2]-A3[2])/(A0[0]-A3[0]))/DEGREE << endl;
	if(0. != B0[0] && 0. != B3[0])
		cout << "yaw (B0->B3): " << atan(-(B0[2]-B3[2])/(B0[0]-B3[0]))/DEGREE << endl;
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






