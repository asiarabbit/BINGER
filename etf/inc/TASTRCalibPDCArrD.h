/**
	\file TASTRCalibPDCArrD.h
	\class TASTRCalibPDCArrD
	\brief A tool class to calibrate DC STRs using auto-calibration.
	A derived class of TASTRCalibDCArrTa, specifically for the right DCTas downstream of
	the target. Note that this is for DCs around the target made in Japan.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/6/8 Last revised: 2018/6/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASTRCALIBPDCARRD_H_
#define _TASTRCALIBPDCARRD_H_

#include "TASTRCalibDCArrTa.h"

class TAPDCArrayD;

class TASTRCalibPDCArrD : public TASTRCalibDCArrTa{
public:
	TASTRCalibPDCArrD(const string &rootfile = "", TAPDCArrayD *pdcArrD = nullptr);
	virtual ~TASTRCalibPDCArrD();
protected:
};

#endif
