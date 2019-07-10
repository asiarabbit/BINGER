/**
	\file TAMWDCTaL.h
	\class TAMWDCTaL
	\brief A class derived from TAMWDCTa. This class is for large size DCs
	downstream of the target.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/3/28 Last revised: 2018/4/3, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCTAL_H_
#define _TAMWDCTAL_H_

#include "TAMWDCTa.h"

class TAMWDCTaL : public TAMWDCTa{
public:
	TAMWDCTaL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCTaL();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const override;
protected:
};

#endif
