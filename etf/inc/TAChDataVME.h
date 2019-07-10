/**
	\file TAChDataVME.h
	\class TAChDataVME
	\brief Derived from TAChData. This class is mainly for transformation of
	measured data, by polymorphic implementation of GetLead(Trail)ingTime(...).
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/21 Last revised: 2018/4/21, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef _TACHDATAVME_H_
#define _TACHDATAVME_H_

#include "TAChData.h"

class TAChDataVME : public TAChData{
public:
	TAChDataVME(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAChDataVME();

	virtual double GetLeadingTime(int n = 0) const; /// get the n-th pulse
	virtual double GetTrailingTime(int n = 0) const; /// get the n-th pulse
protected:
};

#endif
