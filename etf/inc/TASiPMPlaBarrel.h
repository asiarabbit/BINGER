/**
	\file TASiPMPlaBarrel.h
	\class TASiPMPlaBarrel
	\brief A barrel with a regular octagon cross section, to measure hit
	multiplicity around the target, so as to implicate impact parameter of the nuclei bombardment.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/21 Last revised: 2017/10/21, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASIPMPLABARREL_H_
#define _TASIPMPLABARREL_H_

#include "TATOFWall.h"
#include "TADetUnion.h"

class TASiPMPlaBarrel : public TATOFWall, public TADetUnion{
public:
	TASiPMPlaBarrel(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TASiPMPlaBarrel();

	virtual void AssignStripPosition();
	virtual void GetStripProjection(int serialId, double *p) const;
	using TATOFWall::GetStrip;
	TAPlaStrip *GetStrip(short plateId, short stripId) const;
	virtual void Initialize();
	/// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const;
	virtual void Configure(); ///< create detector objects
};
#endif
