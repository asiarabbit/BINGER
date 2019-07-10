/**
	\file TADetUnion.h
	\class TADetUnion
	\brief Abstract base class, providing a common interface to detector
	classes that has UID resolution function and is at the first level of the
	detector hierarchy, which also has the right to design UID resolution scheme of
	their own.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/13 Last revised: 2018/6/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TADETUNION_H_
#define _TADETUNION_H_

#include "TAStuff.h"

class TADetUnion{
public:
	TADetUnion(unsigned uid);
	virtual ~TADetUnion();

	virtual void Initialize() = 0;
	/// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const = 0;
	virtual void Configure() = 0; ///< create detector objects
	virtual bool IsDCArr() const{ return false; } ///< whether this is an MWDC array or not
	short GetDetId()const{ return fDetId; } ///< detector Id
private:
	int fDetId; ///< detector Id
};

#endif
