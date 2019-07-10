/**
	\file TAOpticFiberArray.h
	\class TAOpticFiberArray
	\brief An optical fiber array to measure position and time of beams with
	high intensity. SiPM is used for the readout.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/6/19 Last revised: 2018/6/19, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAOPTICFIBERARRAY_H_
#define _TAOPTICFIBERARRAY_H_

#include "TATOFWall.h"
#include "TADetUnion.h"

class TAOpticFiberArray : public TATOFWall, public TADetUnion{
public:
	TAOpticFiberArray(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAOpticFiberArray();

	virtual void Initialize();
	/// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const;
	virtual void Configure(); ///< create detector objects
};
#endif
