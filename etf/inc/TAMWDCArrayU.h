/**
	\file TAMWDCArrayU.h
	\class TAMWDCArrayU
	\brief The MWDC array upstream of the target. A derived class from
	TAMWDCArray2. Note that there's only two DCs in the DC array.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/3/22 Last revised: 2018/3/22, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCARRAYU_H_
#define _TAMWDCARRAYU_H_

#include "TAMWDCArray2.h"

class TAMWDCArrayU : public TAMWDCArray2{
public:
	TAMWDCArrayU(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayU();

	virtual void Configure() override; ///< create detectors of the MWDC array
protected:
};

#endif

