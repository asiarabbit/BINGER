/**
	\file TAMUSICM.h
	\class TAMUSICM
	\brief A derived class from TAMUSIC. "M" stands for Medium. This MUSIC has
	4 channels, i.e., 4 sampling unit.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/22 Last revised: 2018/4/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMUSICM_H_
#define _TAMUSICM_H_

#include "TAMUSIC.h"

class TAMUSICM : public TAMUSIC{
public:
	TAMUSICM(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMUSICM();
	
	virtual void Configure();
	virtual double GetDeltaE();
	virtual double GetZ(); ///< particle charge
protected:
};

#endif

