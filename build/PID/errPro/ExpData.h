// ExpData.h -- header file for class ExpData: error propagation

# ifndef EXPDATA_H_
# define EXPDATA_H_

#include <iostream>

class ExpData{
public:
	ExpData(double val, double err = 0.);
	ExpData(const ExpData &data);
	ExpData &operator=(const ExpData &data);

	void print() const;
	friend std::ostream &operator<<(std::ostream &os, const ExpData &data);

	double val() const{ return fVal; }
	double err() const{ return fErr; }
	void SetVal(double val){ fVal = val; }
	void SetErr(double err){ fErr = err; }

	const ExpData &operator+=(const ExpData &);
	const ExpData &operator-=(const ExpData &);
	const ExpData &operator*=(const ExpData &);
	const ExpData &operator/=(const ExpData &);
	ExpData operator+(const ExpData &) const;
	ExpData operator-(const ExpData &) const;
	ExpData operator*(const ExpData &) const;
	ExpData operator/(const ExpData &) const;
private:
	double fVal;
	double fErr;
};

ExpData Sqrt(const ExpData &p);
ExpData operator+(double k, const ExpData &p);
ExpData operator-(double k, const ExpData &p);
ExpData operator*(double k, const ExpData &p);
ExpData operator/(double k, const ExpData &p);
ExpData Exp(const ExpData &p);

# endif
