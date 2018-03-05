////////////////////////////////////////////////////////////////////////////////////////////////////
// For C++ project DETECTOR written for External Target Facility, CSR, IMP, Lanzhou, China.      //
// Author: Asia SUN(SUN YAZHOU)									//
// Date: Mar. 31, 2017									       //
// MILLEPEDE SOFTWARE ALIGHMENT PACKAGE
////////////////////////////////////////////////////////////////////////////////////////////////////

// TAMatrix.C -- the definition of rectangular matrix class.

#include "TAMatrix.h"
#include "TAMatrixSym.h"

TAMatrix::TAMatrix(){ // the default constructor
	fData = nullptr;
	fNRow = 0; fNColumn = 0;
	fMa.clear();
} // end of the default construtor
TAMatrix::TAMatrix(int nrows, int ncols, const double *data){
	if(!nrows || !ncols){
		TAMatrix();
		return;
	}
	fNRow = nrows; fNColumn = ncols;
	fData = new double[nrows*ncols];
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			if(data) fData[i*fNColumn+j] = data[i*fNColumn+j];
			else fData[i*fNColumn+j] = 0.;
		} // end for over columns
		// so that fMa.at(i) = fData[i*ncols]
		fMa.push_back(&fData[i*fNColumn]); // and fMa[i][j] = fData[i*ncols+j]
	} // end for over rows
} // end of the constructor
TAMatrix::TAMatrix(const TAMatrix &ma){ // copy constructor
	fData = nullptr;
	*this = ma;
}
TAMatrix &TAMatrix::operator=(const TAMatrix &ma){ // assignment function
	if(this == &ma) return *this;
	// ma is empty matrix
	if(!ma.fNRow || !ma.fNColumn || !ma.fData){
		*this = TAMatrix();
		return *this;
	}

	fNRow = ma.fNRow; fNColumn = ma.fNColumn;
	if(fData) delete [] fData; fMa.clear();
	fData = new double[fNRow*fNColumn];
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			fData[i*fNColumn+j] = ma.fData[i*fNColumn+j];
		} // end for over columns
		// so that fMa.at(i) = fData[i*ncols]
		fMa.push_back(&fData[i*fNColumn]); // and fMa[i][j] = fData[i*ncols+j]
	} // end for over rows
	return *this;
} // end assignemnt function
TAMatrix::~TAMatrix(){ // the destructor
	if(fData) delete [] fData;
	fMa.clear();
}

/////// ----------------------------- MATRIX ARITHMETICS ----------------------------------////////
// calculation not in place, new memory would be claimed.
TAMatrix TAMatrix::operator+(const TAMatrix &ma) const{
	if(ma.fNRow != fNRow || ma.fNColumn != fNColumn){
		cout << "\03[31;1mTAMatrix::operator+(...): ";
		cout << "ma.fNRow: " << ma.fNRow << "\tfNRow: " << fNRow << endl;
		cout << "ma.fNColumn: " << ma.fNColumn << "\tfNColumn: " << fNColumn << endl;
		cout << "Matrix size mismatch. Exiting...\033[0m\n";
		getchar();
		exit(1);
	}
	if(!ma.fData){
		cout << "\03[31;1mTAMatrix::operator+(...): ";
		cout << "Input matrix fData is NULL. Exiting...\033[0m\n";
		getchar();
		exit(1);
	}
	TAMatrix ma_t = *this;
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			ma_t.fData[i*fNColumn+j] += ma.fData[i*fNColumn+j];
		} // end for over columns
	} // end for over rows
	return ma_t;
} // end operator+().
TAMatrix TAMatrix::operator-(const TAMatrix &ma) const{
	return *this+ma*(-1.);
}
TAMatrix TAMatrix::operator*(const TAMatrix &ma) const{
	if(fNColumn != ma.fNRow){
		cout << "\03[31;1mTAMatrix::operator*(...): ";
		cout << "fNColumn: " << fNColumn << "\tma.fNRow: " << ma.fNRow << endl;
		cout << "Matrix size mismatch. Exiting...\033[0m\n";
		getchar();
		exit(1);
	}
	if(!ma.fData){
		cout << "\03[31;1mTAMatrix::operator*(...): ";
		cout << "Input matrix fData is NULL. Exiting...\033[0m\n";
		getchar();
		exit(1);
	}
	TAMatrix ma_t = TAMatrix(fNRow, ma.fNColumn);
	double tmp;
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < ma.fNColumn; j++){
			tmp = 0.;
			// row-i, column-j
			for(int k = 0; k < fNColumn; k++){
				tmp += fMa[i][k]*ma.fMa[k][j];
			}
			ma_t.fData[i*ma.fNColumn+j] = tmp;
		} // end for over columns
	} // end for over rows
	return ma_t;
} // end of definition of matrix product
TAMatrix TAMatrix::operator*(double val) const{
	TAMatrix ma_t = *this;
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			ma_t[i][j] *= val;
		} // end for over columns
	} // end for over rows
	return ma_t;
}
TAMatrix TAMatrix::Transpose() const{
	TAMatrix ma_t(fNColumn, fNRow);
	for(int i = 0; i < fNColumn; i++){
		for(int j = 0; j < fNRow; j++){
			ma_t[i][j] = fMa[j][i];
		} // end for over columns
	} // end for over rows
	return ma_t;
} // end function Transpose.


// calculation done in place -- operation implemented on object itself, saving memtory and time. //
TAMatrix &TAMatrix::operator+=(const TAMatrix &ma){
	if(ma.fNRow != fNRow || ma.fNColumn != fNColumn){
		cout << "\03[31;1mTAMatrix::operator+=(...): ";
		cout << "ma.fNRow: " << ma.fNRow << "\tfNRow: " << fNRow << endl;
		cout << "ma.fNColumn: " << ma.fNColumn << "\tfNColumn: " << fNColumn << endl;
		cout << "Matrix size mismatch. Exiting...\033[0m\n";
		getchar();
		exit(1);
	}
	if(!ma.fData){
		cout << "\03[31;1mTAMatrix::operator+=(...): ";
		cout << "Input matrix fData is NULL. Exiting...\033[0m\n";
		getchar();
		exit(1);
	}
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			fData[i*fNColumn+j] += ma.fData[i*fNColumn+j];
		} // end for over columns
	} // end for over rows
	return *this;
} // end of function operator-.
TAMatrix &TAMatrix::operator-=(const TAMatrix &ma){
	return *this += ma*(-1.);
}
TAMatrix &TAMatrix::operator*=(const TAMatrix &ma){
	return *this = *this * ma;
} // end of matrix product, in place
TAMatrix &TAMatrix::operator*=(double val){
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			fMa[i][j] *= val;
		} // end for over columns
	} // end for over rows
	return *this;
}
double *TAMatrix::operator[](int row) const{
	if(row >= fNRow){
		cout << "\033[33;1mTAMatrix::operator[](): ";
		cout << "Required row number out of range.\033[0m\n";
		cout << "Press ENTER to exit...";
		getchar();
		exit(1);
	}
	return fMa[row]; 
} // end function operator[]

TAMatrix operator*(double val, const TAMatrix &ma){
	return ma*val;
}

// type conversion from TAMatrix objects to TAMatrixSym ones
TAMatrix::operator TAMatrixSym(){
//	cout << "\033[33;1mTAMatrix::operator TAMatrixSym() is called.\033[0m\n"; // DEBUG
	if(fNRow != fNColumn){
		cout << "\033[33;1mTAMatrix::operator TAMatrixSym(): ";
		cout << "Row number isn't equal to column number.\033[0m\n";
		cout << "Press ENTER to exit...";
		getchar();
		exit(1);
	}
	return TAMatrixSym(fNRow, fData);
} // end of function operator TAMatrixSym.

void TAMatrix::GetData(double *data) const{
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			data[i*fNColumn+j] = fMa[i][j];
		} // end for over columns
	} // end for over rows
} // end function GetData
// n: number of elements to be assigned. Defautl is n=0 corresponding to all elements.
void TAMatrix::AppendData(double *data, int n) const{
	if(n == 0) n = fNRow*fNColumn;
	int cnt = 0;
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			if(cnt++ >= n) return;
			data[i*fNColumn+j] += fMa[i][j];
		} // end for over columns
	} // end for over rows
} // end function AppendData
void TAMatrix::SetData(const double *data){
	if(!data){
		cout << "\03[33;1mTAMatrix::SetData(): ";
		cout << "Input data array is empty.\033[0m\n";
		getchar();
		return;
	}
	for(int i = 0; i < fNRow; i++){
		for(int j = 0; j < fNColumn; j++){
			fMa[i][j] = data[i*fNColumn+j];
		} // end for over columns
	} // end for over rows
} // end of function SetData

void TAMatrix::Show(){
	if(!fNRow || !fNColumn || !fData){
		cout << "\03[33;1mTAMatrix::Show(): ";
		cout << "Matrix is empty.\033[0m\n";
		getchar();
		return;
	}
	ios_base::fmtflags initial;
	initial = cout.setf(ios_base::fixed, ios_base::floatfield);
	cout.precision(3); // 15 3
	cout.unsetf(ios_base::fixed);
	cout << setw(6) << "column";
	for(int i = 0; i < fNColumn; i++)
		cout << setw(11) << i;
	cout << endl;
	for(int i = 0; i < fNRow; i++){
		cout << setw(3) << "row " << setw(3) << i; // << row << i
		for(int j = 0; j < fNColumn; j++){
			cout << "\033[32;1m" << setw(10) << fMa[i][j] << "," << "\033[0m";
		} // end for over columns
		cout << endl;
//		cout << "},\n{";
	} // end for over rows
	cout.setf(initial); // restore initial formatting state
} // end show function
/////// ----------------------------- MATRIX ARITHMETICS ----------------------------------////////



