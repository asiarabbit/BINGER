/**
	\file TAT0CalibDCArrTaU.h
	\class TAT0CalibDCArrTaU
	\brief A tool class to calibrate DC anode T0(delay)s anode by anode by
	fitting drift time distribution. A derived class of TAT0CalibDCArrTa, specifically
	for the DC array upstream of the target and upstream of the magnet.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/3 Last revised: 2018/4/4, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAT0CALIBDCARRTAU_H_
#define _TAT0CALIBDCARRTAU_H_

#include "TAT0CalibDCArrTa.h"

class TAMWDCArrayU ;

class TAT0CalibDCArrTaU : public TAT0CalibDCArrTa{
public:
	TAT0CalibDCArrTaU(const string &rootfile = "", TAMWDCArrayU *dcArrU = nullptr);
	virtual ~TAT0CalibDCArrTaU();
protected:
};

#endif

