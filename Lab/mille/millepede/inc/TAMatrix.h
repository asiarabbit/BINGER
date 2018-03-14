////////////////////////////////////////////////////////////////////////////////////////////////////
// For C++ project DETECTOR written for External Target Facility, CSR, IMP, Lanzhou, China.      //
// Author: Asia SUN(SUN YAZHOU)									//
// Date: Mar. 31, 2017									       //
// MILLEPEDE SOFTWARE ALIGHMENT PACKAGE
////////////////////////////////////////////////////////////////////////////////////////////////////

// TAMatrix.h -- rectangular matrix class. Basic matrix arithmetics are defined.

#ifndef __TAMATRIX_H__
#define __TAMATRIX_H__

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;
class TAMatrixSym; // class forward declaration

class TAMatrix{
public:
	TAMatrix();
	TAMatrix(int nrows, int ncols, const double *data = nullptr); // data[i,j] = data[i*ncols+j]
	TAMatrix(const TAMatrix &ma); // copy costructor
	virtual ~TAMatrix();
	TAMatrix &operator=(const TAMatrix &ma); // assignment function

	// calculation not in place, new memory would be claimed.
	TAMatrix operator+(const TAMatrix &ma) const;
	TAMatrix operator-(const TAMatrix &ma) const;
	TAMatrix operator*(const TAMatrix &ma) const;
	TAMatrix operator*(double val) const;
	TAMatrix Transpose() const;
	// calculation in place
	TAMatrix &operator+=(const TAMatrix &ma);
	TAMatrix &operator-=(const TAMatrix &ma);
	TAMatrix &operator*=(const TAMatrix &ma);
	TAMatrix &operator*=(double val);
	double *operator[](int row) const;
	// conversion functions
	operator TAMatrixSym(); // type conversion from TAMatrix objects to TAMatrixSym ones
	
	friend TAMatrix operator*(double val, const TAMatrix &ma);
	
	vector<double *> &GetDataVec(){ return fMa; }
	int GetNRow() const{ return fNRow; }
	int GetNColumn() const{ return fNColumn; }
	void GetData(double *data) const; // output matrix data
	// n: number of elements to be assigned. Defautl is n=0 corresponding to all elements.
	void AppendData(double *data, int n = 0) const; // output matrix data as increments
	void SetData(const double *data); // substitute fData with input array.
	void Initialize(){ for(double *x:fMa) for(int i = 0; i < fNColumn; i++) x[i] = 0.; } // set all data to zero
	
	void Show(); // display the matrix in matrix form
private:
	double *fData;
	int fNRow;
	int fNColumn;
	vector<double *> fMa; // [row#][column#]
};

#endif
