/**
	\file TAMWDCTa.h
	\class TAMWDCTa
	\brief A class derived from TAMWDC. TA stands for target. This class is
	for MWDCs around the target.	Note that this class is designed to be an abstract
	base class, to be derived before instantiation.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/3/27 Last revised: 2018/4/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCTA_H_
#define _TAMWDCTA_H_

#include "TAMWDC.h"

class TAMWDCTa : public TAMWDC{
public:
	TAMWDCTa(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCTa() = 0;
	
	virtual void GetAnodeGlobalDirection(int dcType, double *ag) const;
	virtual void GetAnodeGlobalProjection(int dcType, const double *globalCenter, double *globalProjection) const;

	virtual void Configure() override;
protected:
};

#endif
