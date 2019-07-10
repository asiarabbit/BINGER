/**
	\file TAPlaStripData.h
	\class TAPlaStripData
	\brief For caching data of an event relating to a TOF Wall plastic strip
	with both-end-readout uisng PMT coupling.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/9 Last revised: 2017/10/29, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPLASTRIPDATA_H_
#define _TAPLASTRIPDATA_H_

#include "TAStuff.h"

class TAPlaStripData : public TAStuff{
public:
	TAPlaStripData(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPlaStripData();

	int GetFiredStatus() const; /// \return fFiredStatus
	double GetTime() const; /// \return fTime
	/// \return fHitPosition
	double GetHitPosition() const; ///< distance from hit point to the down end.
	void Initialize(); ///< initialized to unfired status
	void SetFiredStatus(int sta){ fFiredStatus = sta; }
	void SetTime(double time){ fTime = time; }
	void SetHitPosition(double pos){ pos = fHitPosition; }
	void SetFireStatus(bool sta){ fFiredStatus = sta; }

protected:
	int fFiredStatus;
	double fTime; ///< time when the particle hits the strip, certain for an event
	double fHitPosition; ///< position where the particle hits the strip, certain for an event
};
#endif
