/**
	\file TASTRCalibDCArrL.h
	\class TASTRCalibDCArrL
	\brief A tool class to calibrate DC STRs using auto-calibration.
	A derived class of TASTRCalibDCArr, specifically for the left DC array arm.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2017/12/23, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASTRCALIBDCARRL_H_
#define _TASTRCALIBDCARRL_H_

#include "TASTRCalibDCArr.h"

class TAMWDCArrayL;

class TASTRCalibDCArrL : public TASTRCalibDCArr{
public:
	TASTRCalibDCArrL(const string &rootfile = "", TAMWDCArrayL *dcArrL = nullptr);
	virtual ~TASTRCalibDCArrL();
protected:
};

#endif
