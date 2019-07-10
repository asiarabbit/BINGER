/**
	\file TASTRCalibPDCArrU.h
	\class TASTRCalibPDCArrU
	\brief A tool class to calibrate DC STRs using auto-calibration.
	A derived class of TASTRCalibDCArrTa, specifically for the right DCTas upstream of
	the target. Note that this is for DCs around the target made in Japan.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/6/8 Last revised: 2018/6/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASTRCALIBPDCARRU_H_
#define _TASTRCALIBPDCARRU_H_

#include "TASTRCalibDCArrTa.h"

class TAPDCArrayU;

class TASTRCalibPDCArrU : public TASTRCalibDCArrTa{
public:
	TASTRCalibPDCArrU(const string &rootfile = "", TAPDCArrayU *pdcArrU = nullptr);
	virtual ~TASTRCalibPDCArrU();
protected:
};

#endif
