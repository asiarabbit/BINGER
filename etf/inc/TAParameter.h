/**
	\file TAParameter.h
	\class TAParameter
	\brief Base class for unit parameter storage, derived from TAStuff.
	Direct instantiation of this class is supposed to store physical constants.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/24 Last revised: 2018/1/27, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPARAMETER_H_
#define _TAPARAMETER_H_

#include "TAStuff.h"

class TAParameter : public TAStuff{
public:
	TAParameter(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAParameter();
	double GetValue() const;
	void SetValue(double value){ fValue = value; }
protected:
	double fValue;
};

#endif

