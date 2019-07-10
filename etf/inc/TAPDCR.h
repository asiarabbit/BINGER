/**
	\file TAPDCR.h
	\class TAPDCR
	\brief A class derived from TAMWDCTa. This class is for MWDCs from BUAA
	which was R&D-ed by Japan researchers, and would be used around the target. R here
	stands for "Left", out of historical reasons.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/20 Last revised: 2018/4/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPDCR_H_
#define _TAPDCR_H_

#include "TAMWDCTa.h"

class TAPDCR : public TAMWDCTa{
public:
	TAPDCR(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPDCR();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const override;
	virtual void Configure() override;
protected:
};

#endif
