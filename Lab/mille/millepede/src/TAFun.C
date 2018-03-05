////////////////////////////////////////////////////////////////////////////////////////////////////
// For C++ project DETECTOR written for External Target Facility, CSR, IMP, Lanzhou, China.      //
// Author: Asia SUN(SUN YAZHOU)									//
// Date: Apr. 1, 2017									       //
// MILLEPEDE SOFTWARE ALIGHMENT PACKAGE
////////////////////////////////////////////////////////////////////////////////////////////////////

// TAFun.C -- the definition of multi-dim function class.

#include "TAFun.h"

TAFun::TAFun() : kNpar(0), kNdim(0){
	fFCN = nullptr;
}
TAFun::TAFun(double (*fcn)(const double *x, const double *p, void *ptr), double ndim, double npar, void *ptr) : kNdim(ndim), kNpar(npar){
	fFCN = fcn; fPtr = ptr;
}
TAFun::~TAFun(){
}

// first partial derivative of the par_i-th parameter, at variable x and parameter par
// if x or par is null, their current values would be used.
double TAFun::DerivativePar(int par_i, const double *x, const double *p) const{
	double xx[kNdim], pp[kNpar];
	for(int i = 0; i < kNdim; i++) xx[i] = x[i];
	for(int i = 0; i < kNpar; i++) pp[i] = p[i];

	double h = 1E-6;
	pp[par_i] += h; // = a+h
	double fhp = Eval(xx, pp);
	pp[par_i] -= 2.*h; // = a+h-2h = a-h
	double fhm = Eval(xx, pp);
	
	pp[par_i] += 3.*h; // = a-h+3h = a+2h
	double f2hp = Eval(xx, pp);
	pp[par_i] -= 4.*h; // = a+2h-4h = a-2h
	double f2hm = Eval(xx, pp);
	
	double Dh = (fhp-fhm)/(2.*h); // Dh = (f(p+h) - f(p-h)) / (2*h)
	double D2h = (f2hp-f2hm)/(4.*h); // D2h = (f(p+2*h) - f(p-2*h)) / (2*2*h)

	double D = (4.*Dh-D2h) / 3.; // Richardson extrapolation ~o(h^2)
	
	return D;
}
double TAFun::Eval(const double *x, const double *p) const{
	if(!fFCN){
		cout << "TAFun::Eval(): member function pointer has not been assigned.\n";
		cout << "Exiting...";
		getchar();
		exit(1);
	}
	return fFCN(x, p, fPtr);
}

