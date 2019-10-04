/**
	\file TAPDCArrayTa4.h
	\class TAPDCArrayTa4
	\brief This class is for ameliorated tracking pre- and post-reaction target.
	Note that the tracking is the spline of the two straight tracks, together with
	consistency of target hit positions and the exit positions from the magField,
	which has been particularly added to the tracking around the target zone to
	rectify the tracks and ameliorate the tracking accuracy. And currently this is
	just for the amelioration of Xprojs. This is a singleton class.
	\author SUN Yazhou, asia.rabbit@163.com
	\date Created: 2019/9/24 Last revised: 2018/4/21, SUN Yazhou
	\copyright 2017-2019, SUN Yazhou.
*/

#ifndef _TAMWDCARRAYTA4_H_
#define _TAMWDCARRAYTA4_H_

#include <vector>

#include "TAStuff.h"

class TATrackTa4;
class TAMWDCArray2;
class TAMWDC;
struct tTrack;
class TAEventProcessor;

using std::vector;

class TAPDCArrayTa4 : public TAStuff{
public:
	/// return the unique object
	static TAPDCArrayTa4 *Instance();
	virtual ~TAPDCArrayTa4();
	/// \param: dcArrId: 0-dcArrL, 1-dcArrR
	TAMWDCArray2 *GetMWDCArray(int dcArrId) const; ///< select MWDC array
	virtual void SetPostMagXproj(double k, double b); ///< assign k2 and b2
	/// \retval: the slope of the postMagTrk, i.e. fK2
	double GetK2() const;
	/// \retval: the intercept of the postMagTrk, i.e. fB2
	double GetB2() const;
	// dx2*dx2 + dxTa*dxTa: 10*10 + 20*20, might be too small
	double GetChi2ExtraThre() const{ return fChi2ExtraThre; }
	void SetChi2Extra(double chi2ExtraThre){ fChi2ExtraThre = chi2ExtraThre; }
	vector<TATrackTa4 *> &GetTrackTa4List(){ return fTrackTa4List; }
	/// the Map function, the core track improving function
	virtual bool Map();
	/// update fTrackList with the ameliorated tracks
	/// and the cached tracks in DCs for drawing purposes
	virtual void AssignTracks(vector<tTrack *> &track_ls);
	virtual void Initialize();
	/// whether the current event is ready for splined tracking, to prevent
	/// illegal calling of the Map() function
	bool IsReady() const{ return fIsReady; }
	// no track is detected
	bool ZeroTrack() const{ return fZeroTrack; }

	friend class TAEventProcessor;

protected:
	TAPDCArrayTa4();
	/// compare two tracsk: incompatibility check\n
	/// \retval: 0: the two tracks are different;\n 1: newTrack is defeated by oldTrack\n
	/// 2: newTrack defeats oldTrack
	/// \param dcType: [0-1]: [X-Y]
	int compare(TATrackTa4 *newTrack, TATrackTa4 *oldTrack, bool show = false);
	void SetIsReady(bool isReady = true){ fIsReady = isReady; }

	vector<TATrackTa4 *> fTrackTa4List;
	static TAPDCArrayTa4 *fInstance;
	bool fIsReady; ///< whether is ready for splined tracking
	bool fZeroTrack; ///< if no track is detected

	double fK2, fB2; ///< postMagTrk: x = fK2*z + fB2
	double fTOF; ///< the TOF_stop signal for the trk around Ta zone
	/// threshold for chi2Extra: dx2*dx2 + dxTa*dxTa:
	/// 10*10 + 20*20, might be too small
	static double fChi2ExtraThre;

	TAMWDCArray2 *fDCArr[2]; // PDC arrays upstream of the dipole magnet
	TAMWDC *fDC[2][2]; // [U-D][0-1]
};

#endif
