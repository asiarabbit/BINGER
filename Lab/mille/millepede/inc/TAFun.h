////////////////////////////////////////////////////////////////////////////////////////////////////
// For C++ project DETECTOR written for External Target Facility, CSR, IMP, Lanzhou, China.      //
// Author: Asia SUN(SUN YAZHOU)									//
// Date: Apr. 1, 2017									       //
// MILLEPEDE SOFTWARE ALIGHMENT PACKAGE
////////////////////////////////////////////////////////////////////////////////////////////////////

// TAFun.h -- class for multi-dimensional functions with a parameter array

#ifndef __TAFUN_H__
#define __TAFUN_H__

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

class TAFun{
public:
	TAFun();
	// npar: number of parameters; ndim: number of variables
	TAFun(double (*fcn)(const double *x, const double *p, void *ptr), double ndim, double npar, void *ptr = nullptr);
	~TAFun();

	// first partial derivative of the par_i-th parameter, at variable x and parameter par
	double DerivativePar(int par_i, const double *x, const double *par) const;
	double Eval(const double *x, const double *p) const; // evaluate the function at variable array x.
	
private:
	double (*fFCN)(const double *x, const double *p, void *ptr);
	const int kNdim; // function dimension (number of independent variables)
	const int kNpar; // number of parameters
	void *fPtr; // for spare use
};

#endif
