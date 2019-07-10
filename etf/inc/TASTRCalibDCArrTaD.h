/**
	\file TASTRCalibDCArrTaD.h
	\class TASTRCalibDCArrTaD
	\brief A tool class to calibrate DC STRs using auto-calibration.
	A derived class of TASTRCalibDCArrTa, specifically for the right DCTas downstream of
	the target. Note that this is for DCs around the target made by P.Ma.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/6/8 Last revised: 2018/6/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASTRCALIBDCARRTAD_H_
#define _TASTRCALIBDCARRTAD_H_

#include "TASTRCalibDCArrTa.h"

class TAMWDCArrayD;

class TASTRCalibDCArrTaD : public TASTRCalibDCArrTa{
public:
	TASTRCalibDCArrTaD(const string &rootfile = "", TAMWDCArrayD *dcArrTaD = nullptr);
	virtual ~TASTRCalibDCArrTaD();
protected:
};

#endif
