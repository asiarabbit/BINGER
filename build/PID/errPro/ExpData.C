// ExpData.C -- implementation file for class ExpData -- error propagation

#include <iostream>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::endl;

#include "ExpData.h"

ExpData::ExpData(double val, double err): fVal(val), fErr(err){
}
ExpData::ExpData(const ExpData &data){
	*this = data;
}
ExpData &ExpData::operator=(const ExpData &data){
	if(&data == this) return *this;

	fVal = data.fVal;
	fErr = data.fErr;

	return *this;
}

void ExpData::print() const{
	const double rel = fErr / fVal * 100.;
	cout << fVal << "(" << fErr << ", " << rel << "%)";
}
std::ostream &operator<<(std::ostream &os, const ExpData &data){
	os << data.fVal;
	return os;
}
const ExpData &ExpData::operator+=(const ExpData &data){
	fVal += data.fVal;
	fErr = sqrt(fErr*fErr + data.fErr*data.fErr);

	return *this;
}
const ExpData &ExpData::operator-=(const ExpData &data){
	fVal -= data.fVal;
	fErr = sqrt(fErr*fErr + data.fErr*data.fErr);
	
	return *this;
}
const ExpData &ExpData::operator*=(const ExpData &data){
	double prod = fVal*data.fVal;
	fErr = sqrt(pow(fErr*data.fVal, 2) + pow(data.fErr*fVal, 2));
//	fErr = fabs(prod)*sqrt(pow(fErr/fVal, 2) + pow(data.fErr/data.fVal, 2));
	fVal = prod;
	
	return *this;
}
const ExpData &ExpData::operator/=(const ExpData &data){
	double rate = fVal/data.fVal;
	fErr = fabs(rate)*sqrt(pow(fErr/fVal, 2) + pow(data.fErr/data.fVal, 2));
	fVal = rate;
	
	return *this;
}
ExpData ExpData::operator+(const ExpData &data) const{
	return ExpData(*this) += data;
}
ExpData ExpData::operator-(const ExpData &data) const{
	return ExpData(*this) -= data;
}
ExpData ExpData::operator*(const ExpData &data) const{
	return ExpData(*this) *= data;
}
ExpData ExpData::operator/(const ExpData &data) const{
	return ExpData(*this) /= data;
}



