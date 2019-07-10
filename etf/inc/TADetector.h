/**
	\file TADetector.h
	\class TADetector
	\brief Detector base class for detectors that could move freely in space,
	a container class, including parameters for a detector.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/30 Last revised: 2018/4/4, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TADETECTOR_H_
#define _TADETECTOR_H_

#include "TAStuff.h"

class TADetectorPara;

class TADetector : public TAStuff{
public:
	TADetector(const string &name = "", const string &title = "", int uid = 999999999);
	virtual ~TADetector() = 0;
	TADetectorPara *GetDetPara() const;

	virtual void Initialize() = 0;
	virtual void Configure(); ///< build the class
protected:
	TADetectorPara *fDetPara;
};

#endif
