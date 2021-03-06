/**
	\file TAMWDCArrayR.h
	\class TAMWDCArrayR
	\brief The right arm of the MWDC arrays. A derived class from TAMWDCArray
	with distinct features of the right arm.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/6 Last revised: 2018/3/22, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCARRAYR_H_
#define _TAMWDCARRAYR_H_

#include "TAMWDCArray.h"

class TAMWDCArrayR : public TAMWDCArray{
public:
	TAMWDCArrayR(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayR();

	virtual void Configure() override; ///< create detectors of the MWDC array
protected:
};

#endif

