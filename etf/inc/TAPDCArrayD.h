/**
	\file TAPDCArrayD.h
	\class TAPDCArrayD
	\brief The PDC array downstream of the target. A derived class from
	TAMWDCArray2. Note that there's only two DCs in the DC array.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/20 Last revised: 2018/4/21, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPDCARRAYD_H_
#define _TAPDCARRAYD_H_

#include "TAMWDCArray2.h"

class TAPDCArrayD : public TAMWDCArray2{
public:
	TAPDCArrayD(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPDCArrayD();

	virtual void Configure() override; ///< create detectors of the MWDC array
protected:
};

#endif

