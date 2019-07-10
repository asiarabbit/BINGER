/**
	\file TAMWDCArray.h
	\class TAMWDCArray
	\brief MWDC array class, implementing pattern recognition and 3-D
	tracking through track projection matching.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/6 Last revised: 2018/4/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMWDCARRAY_H_
#define _TAMWDCARRAY_H_

#include <vector>
#include "TAStuff.h"
#include "TADetUnion.h"

using std::vector;

// ROOT include
class TGraph;

class TAMWDC;
class TATOFWall;
class TAPlaStrip;
class TATrack;
class TASimulation;
struct tTrack;

class TAMWDCArray : public TAStuff, public TADetUnion{
public:
	TAMWDCArray(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDCArray() = 0; ///< abstract base class

	vector<TATrack *> &GetTrackList(int dcType);
	TAMWDC *GetMWDC(int dcId) const; ///< get MWDC #dcId
	TATOFWall *GetTOFWall() const;
	TAPlaStrip *GetPlaT0() const;
	void SetPlaT0(TAPlaStrip *t0);
	double GetPhiAvrg();
	double GetDsquareThresholdPerDot() const{ return fDsquareThresholdPerDot; }
	void SetDsquareThresholdPerDot(double d2Thre);
	/// Map function: have the fired wires filtered, sorted, and stored in TATrack objects
	/// for further fittings
	virtual void Map();
	/// subordinate function of void Map()
	virtual bool Map(TAMWDC **MWDC, vector<TATrack *> &track, int dcType);
	virtual void AssignTracks(vector<tTrack *> &track_ls); // assign tracks to track list
	void TrackMerger(); // assembly projections to 3-D tracks
	/// drift time correction: time of flight and signal propagation time
	/// \param a[2]: [0-1]: [layer0~layer18, nu]
	/// \param p[4]: [0-3]: [k1, k2, b1, b2];
	/// \param beta: particle speed; kl: track slope
	void DriftTimeCorrection(double &t, double &r, const double *a, const double *p, int firedStripId, double beta);
	/// a method dedicated for TAVisual::Fill()
	void FillTrack(TGraph *gTrack, TGraph *gTrack_R) const;
	int GetN3DTrack() const{ return fN3DTrk; }

	virtual void Initialize() override;
	/// get the channel that belongs to this with uid
	virtual TAStuff *GetChannel(unsigned uid) const override;
	virtual void Info() const; ///< print MWDC array information
	virtual bool IsDCArr() const override { return true; }

	friend class TASimulation;
protected:
	/// copmare two tracks\n
	/// \retval: 0: the two tracks are different;\n 1: newTrack is defeated by oldTrack\n
	/// 2: newTrack defeats oldTrack
	/// \param dcType: [0-1]: [X-Y]
	static int compare(TATrack *newTrack, TATrack *oldTATrack, char type = 'X', bool show = false);
	static void cleanUp(vector<TATrack *> &tr, const int n);
	/// to see if track projections resolved from UV data is compatible with that from X data.
	bool Compatible(double k, double b, double ku, double bu, double kv, double bv, double &k2, double &b2, double *xMiss3D);
	/// tool functions for drift time correction
	/// \param j: X-U-V;
	/// \param k: DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
	double GetWirePropagationTime(const double *b, const double *B, int nu, int j, int k);
	/// time of flight for particles from the MWDC to TOF wall
	double GetTimeOfFlight(const double *b, const double *B, int nu, int j, int k, int firedStripId, double beta);
	double GetWirePropagationLength(const double *b, const double *B, int nu, int j, int k);
	double GetDistanceOfFlight(const double *b, const double *B, int nu, int j, int k, int firedStripId);

	TAMWDC *fMWDC[3]; ///< three MWDCs in the MWDC array
	TATOFWall *fTOFWall; ///< the TOF wall
	TAPlaStrip *fPlaT0; ///< the plastic scintillator placed around the target for beam timing

	/// projection of 3-D track to normal planes of [X-U-V] sense wire layers
	vector<TATrack *> fTrackList[3];
	int fN3DTrk; ///< number of 3D tracks
	double fPhiAvrg; ///< average of phi-s of the three MWDCs
	double fDsquareThresholdPerDot; ///< as the name indicates
	static const double kPropagationSpeed; ///< mm/ns electric signal travel speed along wires
};

#endif
