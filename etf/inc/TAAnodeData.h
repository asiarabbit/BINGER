/**
	\file TAAnodeData.h
	\class TAAnodeData
	\brief Derived from TAAnodeData, embodiment of DC anodes.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/30 Last revised: 2017/10/15, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef _TAANODEDATA_H
#define _TAANODEDATA_H

#include "TAChData.h"

class TAAnodeData : public TAChData{
public:
	TAAnodeData(const string &name = "", const string &title = "", unsigned uid = 999999999)
			: TAChData(name, title, uid){}
	virtual ~TAAnodeData(){}

	virtual void Initialize() override;
	virtual void Show() const override;
	double GetTOF() const;
	void SetTOF(double tof){ fTOF = tof; }
protected:
	double fTOF; //!< time that a particle crossing the TOF array.
};

#endif


