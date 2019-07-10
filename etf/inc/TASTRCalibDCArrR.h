/**
	\file TASTRCalibDCArrR.h
	\class TASTRCalibDCArrR
	\brief A tool class to calibrate DC STRs using auto-calibration.
	A derived class of TASTRCalibDCArr, specifically for the right DC array arm.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2017/12/23, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASTRCALIBDCARRR_H_
#define _TASTRCALIBDCARRR_H_

#include "TASTRCalibDCArr.h"

class TAMWDCArrayR;

class TASTRCalibDCArrR : public TASTRCalibDCArr{
public:
	TASTRCalibDCArrR(const string &rootfile = "", TAMWDCArrayR *dcArrR = nullptr);
	virtual ~TASTRCalibDCArrR();
protected:
};

#endif
