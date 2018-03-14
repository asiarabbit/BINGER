////////////////////////////////////////////////////////////////////////////////////////////////////
// For C++ project DETECTOR written for External Target Facility, CSR, IMP, Lanzhou, China.      //
// Author: Asia SUN(SUN YAZHOU)									//
// Date: Mar. 31, 2017									       //
// MILLEPEDE SOFTWARE ALIGHMENT PACKAGE
////////////////////////////////////////////////////////////////////////////////////////////////////

// TAMatrixSym.h -- symmetic square matrix class, derived from TAMatrix class, inversion defined.
#ifndef __TAMatrixSym_H__
#define __TAMatrixSym_H__

#include "TAMatrix.h"

class TAMatrixSym : public TAMatrix{
public:
	TAMatrixSym();
	TAMatrixSym(int ndim, const double *data = nullptr);
	~TAMatrixSym();

	// shadow the father class methods
	TAMatrixSym operator+(const TAMatrixSym &ma) const{ return TAMatrix::operator+(ma); }
	TAMatrixSym operator-(const TAMatrixSym &ma) const{ return TAMatrix::operator-(ma); }
	TAMatrixSym &operator+=(const TAMatrixSym &ma){ return *this = TAMatrix::operator+=(ma); }
	TAMatrixSym &operator-=(const TAMatrixSym &ma){ return *this = TAMatrix::operator-=(ma); }

	TAMatrixSym Invert() const; // inversion implemented in the copy of *this
	TAMatrixSym &InvertSelf(); // inversion implemented in place
	static TAMatrixSym &SPMINV(TAMatrixSym &ma); // SPMINV algorithm, inversion implemented in place
};

#endif
