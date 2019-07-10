/**
	\file TASiPMPlaArray.h
	\class TASiPMPlaArray
	\brief Fast particle tracking detector dedicated especially for high
	counting rates circumstances, consisting of an array of micro-strips of plastic
	scintillators coupled with SiPM readouts, which is analogous to PMT but much smaller.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/10 Last revised: 2017/11/30, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TASIPMPLAARRAY_H_
#define _TASIPMPLAARRAY_H_

#include <vector>
#include "TAStuff.h"
#include "TADetUnion.h"

using std::vector;

class TAChannel;

class TASiPMPlaArray : public TAStuff, public TADetUnion{
public:
	TASiPMPlaArray(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TASiPMPlaArray();

	short GetNStrip(bool isV = true) const; ///< total number of strips
	short GetNFiredStrip(bool isV = true) const; ///< number of fired strips
	/// assign the input array with the list of ids of fired strips
	void GetFiredStripArr(int &multi, int *idLs, double *lTLs = nullptr, bool isV = true) const;
	vector<TAChannel *> &GetChArr(bool isV = true);
	double GetDelay() const;
	double GetX0() const;
	double GetZ0() const;
	double GetWidth() const;
	virtual double GetTime(); ///< get hit time of the fired strip, multi-hit is ignored
	/// get the X position of the fired strip, multi-hit is ignored
	/// the position here is the X position in the global coordinate system
	virtual double GetHitX();
	short GetFiredStripId(); ///< as the name indicates
	double GetStripX(int serialId) const;
	TAChannel *GetUV(int id) const; ///< \return up end of the strip, HPTDC very high resolution mode
	TAChannel *GetUH(int id) const; ///< \return up end of the strip, HPTDC high resolution mode
	double GetStripTime(int stripId, double t0 = -9999., double t1 = -9999., double t2 = -9999.); ///< UV end lv time
	void SetDelay(double delay){ fDelay = delay; }

	virtual void Initialize() override;
	/// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Configure() override; ///< create detector objects
protected:
	double fWidth;
	double fLength;
	double fX0; ///< X position of the strip with the smallest X coordinate
	double fZ0; ///< z position of the SiPM array
	short fNStripV;
	short fNStripH; ///< fNStripH is supposed to be equal to fNStripV
	double fDelay; ///< global delay of the SiPM detector
	double fTime; ///< hit time of the fired strip, multi-hit is ignored.
	double fHitX; ///< the X position of the fired strip, multi-hit is ignored
	short fFiredStripId; ///< as the name indicates
	vector<TAChannel *> fUVArr; ///< Up end - Very high resolution mode channel
	vector<TAChannel *> fUHArr; ///< Up end - High resolution mode
};

#endif
