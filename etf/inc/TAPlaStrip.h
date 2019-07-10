/**
	\file TAPlaStrip.h
	\class TAPlaStrip
	\brief Designed for both-end-readout plastic scintillator strip.
	Container class, representating all the attributes and function of the strip.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/30 Last revised: 2018/5/3, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAPLASTRIP_H_
#define _TAPLASTRIP_H_

#include "TAStuff.h"

class TAChannel;
class TAPlaStripPara;
class TAPlaStripData;

class TAPlaStrip : public TAStuff{
public:
	TAPlaStrip(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAPlaStrip();

	int GetStripId() const;
	double GetDelay() const;
	double GetHitPosition() const; ///< distance from hit point to the down end.
	/// \retval 0: not fired;\n \retval 1: only upper end fired;\n \retval 2: only down end fired;\n
	/// \retval 3: both end fired, but hit position out of range.\n
	/// \retval 4: both end fired and hit point within ration range.\n
	/// \retval -1: H and V not simultaneously fired.\n \retval -2: Member channels not assigned.
	/// \note For U channels, the returned value is added with 10
	int GetFiredStatus() const; ///< not simply return fFired, but based on the data members
	/// t0, t1 and t2 are set for choosing ch->GetLT over edges
	/// (ch->GetLT-t0) within t1 and t2 is chosen
	/// t0, t1 and t2 using default values, choose the 1st edge
	double GetTime(double t0 = -9999., double t1 = -9999., double t2 = -9999.); ///< get TOF time
	TAChannel *GetUV() const; /// \return up end of the strip, HPTDC very high resolution mode
	TAChannel *GetUH() const; /// \return up end of the strip, HPTDC high resolution mode
	TAChannel *GetDV() const; /// \return down end of the strip, HPTDC very high resolution mode
	TAChannel *GetDH() const; /// \return down end of the strip, HPTDC high resolution mode
	TAPlaStripPara *GetStripPara() const;
	TAPlaStripData *GetStripData() const;
	bool IsSingleEnd() const{ return fIsSingleEnd; } ///< whether to use only one end
	void SetIsSingleEnd(bool opt){ fIsSingleEnd = opt; }
	// Set functions
	void SetStripLength(double length);
	void SetStripId(int id){ fStripId = id; }

	virtual void Initialize();
	virtual void Configure();
protected:
	TAPlaStripPara *fStripPara; ///< strip parameters, including size and calibration constants
	TAPlaStripData *fStripData; ///< strip data, reconstructed data from the four channels
	TAChannel *fUV; ///< Up end - Very high resolution mode channel
	TAChannel *fUH; ///< Up end - High resolution mode
	TAChannel *fDV; ///< Down end - Very high resolution mode channel
	TAChannel *fDH; ///< Down end - High resolution mode
	int fStripId; ///< strip serial id
	double ft0, ft1, ft2; ///< used in GetTime(...) to mark the difference of them between two calls
	bool fIsSingleEnd; ///< whether to use only one end
};
#endif
