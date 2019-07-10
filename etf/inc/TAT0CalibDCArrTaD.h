/**
	\file TAT0CalibDCArrTaD.h
	\class TAT0CalibDCArrTaD
	\brief A tool class to calibrate DC anode T0(delay)s anode by anode by
	fitting drift time distribution. A derived class of TAT0CalibDCArrTa, specifically
	for the DC array downstream of the target and upstream of the magnet.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/3 Last revised: 2018/4/4, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAT0CALIBDCARRTAD_H_
#define _TAT0CALIBDCARRTAD_H_

#include "TAT0CalibDCArrTa.h"

class TAMWDCArrayD;

class TAT0CalibDCArrTaD : public TAT0CalibDCArrTa{
public:
	TAT0CalibDCArrTaD(const string &rootfile = "", TAMWDCArrayD *dcArrD = nullptr);
	virtual ~TAT0CalibDCArrTaD();
protected:
};

#endif

