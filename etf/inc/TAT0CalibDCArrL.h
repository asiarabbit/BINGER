/**
	\file TAT0CalibDCArrL.h
	\class TAT0CalibDCArrL
	\brief A tool class to calibrate DCT0(delay)s anode by anode by fitting
	drift time distribution. A derived class of TAT0CalibDCArr, specifically
	for the left DC array arm.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2017/11/19, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAT0CALIBDCARRL_H_
#define _TAT0CALIBDCARRL_H_

#include "TAT0CalibDCArr.h"

class TAMWDCArrayL;

class TAT0CalibDCArrL : public TAT0CalibDCArr{
public:
	TAT0CalibDCArrL(const string &rootfile = "", TAMWDCArrayL *dcArrL = nullptr);
	virtual ~TAT0CalibDCArrL();
protected:
};

#endif
