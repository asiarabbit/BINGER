/**
	\file TAMWDCArrayL.h
	\class TAMWDCArrayL
	\brief The left arm of the MWDC arrays. A derived class from TAMWDCArray
	with distinct features of the left arm.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/6 Last revised: 2018/3/18, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCARRAYL_H_
#define _TAMWDCARRAYL_H_

#include "TAMWDCArray.h"

class TAMWDCArrayL : public TAMWDCArray{
public:
	TAMWDCArrayL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayL();

	virtual void Configure() override; ///< create detectors of the MWDC array
protected:
};

#endif

