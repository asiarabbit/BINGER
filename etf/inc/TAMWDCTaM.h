/**
	\file TAMWDCTaM.h
	\class TAMWDCTaM
	\brief A class derived from TAMWDCTa. This class is for medium size DCs
	downstream of the target.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/3/28 Last revised: 2018/4/3, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCTAM_H_
#define _TAMWDCTAM_H_

#include "TAMWDCTa.h"

class TAMWDCTaM : public TAMWDCTa{
public:
	TAMWDCTaM(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCTaM();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const override;
protected:
};

#endif
