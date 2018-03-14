// test.C -- to do code-relevant test
// Created: 2018/03/14, Lasted modified: 2018/03/14, Author: SUN Yazhou

#include <iostream>
#include <cstdlib>
#include "TAMath.h"

using std::cout;
using std::endl;

int main(){
	const double DEGREE = TAMath::DEGREE();

	// test the rotation matrix in class TAMath
	double pIn[3] = {1., 1., 1.}, pOut[3]{};
	double ang[3] = {0., 0., 30.};
	for(double &x : ang) x *= DEGREE;

	TAMath::rotate(pIn, pOut, ang);

	cout << "Before rotation, pIn:" << endl;
	cout << "(" << pIn[0] << ", " << pIn[1] << ", " << pIn[2] << ")" << endl;
	cout << "\nAfter rotation, pOut:" << endl;
	cout << "(" << pOut[0] << ", " << pOut[1] << ", " << pOut[2] << ")" << endl;
	return 0;
}
