// header.C -- inclusions and global variable definitions

#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>

using namespace std;

const double DEGREE = 3.1415926/180.;
const double offsetT = 25.4; // the laser ball reflection point + the seat
const double offsetBall = 19.00; // the laser ball reflection point + the seat
const double offsetAld = 4.00; // the thickness of the aluminum plate under the seat for certain cases
double offsetGap = 0.7; // the air gap between the seat and the DC wall without aluminum cushion
double dcThickness = 132.8; // 130 + 3.5 ~ 3.5: gap between the 22 plates
double skewAng = 0.5 * DEGREE; // the angle the laser ball seat had caused by gravity
double L(const double *p0, const double *p1); // length between two points
double d(const double *p0, const double *p1, const double *p); // p -> line
double dp(const double *p0, const double *p1, const double *p2, const double *p); // p -> plane
void show(const double *p){ cout << p[0] << "\t" << p[1] << "\t" << p[2] << endl; }
void showAngle(const double *p){
	cout << p[0] / DEGREE << "\t" << p[1] / DEGREE << "\t" << p[2] / DEGREE << endl;
}
void vec(const double *p0, const double *p1); // show vector p0->p1
void vec(const double *p0, const double *p1, double *p); // output vector p0->p1
// opt: 0: horizontal A; 1: horizontal B; 2: vertical; 3: cross
void BEE(int i, int j, int k0, int k1, double err, short opt); // alarm
double phi(const double *p0, const double *p1); // phi angle
double yaw(const double *p0, const double *p1); // yaw angle
double pitch(const double *p0, const double *p1); // pitch angle
double roll(const double *p0, const double *p1); // roll angle

// r_global = R.r_local+r0
// rotation: Y-X-Z; angle0: yaw, angle1: pitch, angle2: roll
void rotate(const double *angIn, const double *pIn, double *pOut);
void globalPos(const double *p0, const double *angIn, const double *pIn, double *pOut);
double chi2(const double *p0, const double *p1);


