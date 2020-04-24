/**
	\file TAT0_0.h
	\class TAT0_0
	\brief Start time detector, located at the middle of RIBLL2 to measure
	particle time of flight. It is actually a plastic scintillator with one-end PMT
	readout.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2020/04/24 Last revised: 2017/10/13, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAT0_0_H_
#define _TAT0_0_H_

#include "TAStrip.h"
#include "TADetUnion.h"

class TAChannel;

class TAT0_0 : public TAPlaStrip, public TADetUnion{
public:
	TAT0_0(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAT0_0();

	virtual void Initialize() override;
	/// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Configure() override; ///< create detector objects
protected:
};

#endif
