/**
	\file TASTRCalibDCArrTaU.h
	\class TASTRCalibDCArrTaU
	\brief A tool class to calibrate DC STRs using auto-calibration.
	A derived class of TASTRCalibDCArrTa, specifically for the right DCTas downstream of
	the target. Note that this is for DCs around the target made by P.Ma.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/6/8 Last revised: 2018/6/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASTRCALIBDCARRTAU_H_
#define _TASTRCALIBDCARRTAU_H_

#include "TASTRCalibDCArrTa.h"

class TAMWDCArrayU;

class TASTRCalibDCArrTaU : public TASTRCalibDCArrTa{
public:
	TASTRCalibDCArrTaU(const string &rootfile = "", TAMWDCArrayU *dcArrTaU = nullptr);
	virtual ~TASTRCalibDCArrTaU();
protected:
};

#endif
