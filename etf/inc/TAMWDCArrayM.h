/**
	\file TAMWDCArrayM.h
	\class TAMWDCArrayM
	\brief An MWDC array composed of three medium sized DCs. A derived class
	from TAMWDCArray with distinct features of the medium DCs.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/13 Last revised: 2018/4/13, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCARRAYM_H_
#define _TAMWDCARRAYM_H_

#include "TAMWDCArrayR.h"

class TAMWDCArrayM : public TAMWDCArrayR{
public:
	TAMWDCArrayM(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArrayM();

	using TAMWDCArray::Map;
	virtual void Map() override; ///< Map for XY-DCs
	virtual void Configure() override; ///< create detectors of the MWDC array
protected:
};

#endif
