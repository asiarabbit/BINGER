/**
	\file TAMUSICL.h
	\class TAMUSICL
	\brief A derived class from TAMUSIC. "L" stands for large. This MUSIC has
	8 channels, i.e., 8 sampling unit.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/22 Last revised: 2018/4/22, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMUSICL_H_
#define _TAMUSICL_H_

#include "TAMUSIC.h"

class TAMUSICL : public TAMUSIC{
public:
	TAMUSICL(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMUSICL();

	virtual void Configure();
	virtual double GetDeltaE();
	virtual double GetZ(); ///< particle charge
protected:
};

#endif

