/**
	\file TAPDCArrayU.h
	\class TAPDCArrayU
	\brief The PDC array upstream of the target. A derived class from
	TAMWDCArray2. Note that there's only two DCs in the DC array.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/20 Last revised: 2018/4/21, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPDCARRAYU_H_
#define _TAPDCARRAYU_H_

#include "TAMWDCArray2.h"

class TAPDCArrayU : public TAMWDCArray2{
public:
	TAPDCArrayU(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPDCArrayU();

	virtual void Configure() override; ///< create detectors of the MWDC array
protected:
};

#endif

