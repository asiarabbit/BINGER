/**
	\file TAT0CalibDCArrR.h
	\class TAT0CalibDCArrR
	\brief A tool class to calibrate DCT0(delay)s anode by anode by fitting
	drift time distribution. A derived class of TAT0CalibDCArr, specifically
	for the right DC array arm.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2017/11/19, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAT0CALIBDCARRR_H_
#define _TAT0CALIBDCARRR_H_

#include "TAT0CalibDCArr.h"

class TAMWDCArrayR;

class TAT0CalibDCArrR : public TAT0CalibDCArr{
public:
	TAT0CalibDCArrR(const string &rootfile = "", TAMWDCArrayR *dcArrR = nullptr);
	virtual ~TAT0CalibDCArrR();
protected:
};

#endif
