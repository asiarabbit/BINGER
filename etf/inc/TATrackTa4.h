/**
	\file TATrackTa4.h
	\class TATrackTa4
	\brief This class is to store the splined track pre- and post-target, so as to
	make use of the target hit position and the exit magFiled position, to achieve
	tracking amelioration around the target zone.
	\author SUN Yazhou, asia.rabbit@163.com
	\date Created: 2019-09-27 Last revised: 2019-10-3, SUN Yazhou.
	\copyright 2017-2019, SUN Yazhou.
*/

#ifndef _TATRACKTA4_H_
#define _TATRACKTA4_H_

#include "TAMath.h"
#include "TATrack2.h"

class TATrackTa4 : public TAStuff{
public:
	TATrackTa4(const string &name = "", const string &title = "");
	TATrackTa4(const TATrackTa4 &track); ///< the copy constructor
	TATrackTa4(TATrackTa4 &&track); ///< move constructor
	TATrackTa4 &operator=(const TATrackTa4 &track); // the assignment constructor
	TATrackTa4 &operator=(TATrackTa4 &&track); // the move assignment constructor
	virtual ~TATrackTa4();
	
	/// get the track upstream of the target
	TATrack2 *GetTrackPreTa() const;
	/// get the track downstream of the target
	TATrack2 *GetTrackPostTa() const;
	/// \retval: the total chi including dx2 and dxTa
	virtual double GetChi2();
	/// \retval: chi2 / ndf
	virtual double GetChi();
	/// assign nu array for both tracks
	void GetNu(int *nu) const{
		GetTrackPreTa()->GetNu(nu);
		GetTrackPostTa()->GetNu(nu+6);
	}
	// the sum of NFiredAnodeLayers
	int GetNFiredAnodeLayer(){
		return GetTrackPreTa()->GetNFiredAnodeLayer() +
			GetTrackPostTa()->GetNFiredAnodeLayer();
	}

	/// x2 - x2p
	virtual double GetDX2();
	/// DxTa0(PreTa) - DxTa1(PostTa)
	virtual double GetDxTa();
	/// \retval: dx2*dx2 + dxTa*dxTa
	virtual double GetChi2Extra(){
		return TAMath::Sum2(GetDX2(), GetDxTa());
	}
	/// \retval: fK2
	virtual double GetK2();
	/// \retval: fB2
	virtual double GetB2();
	bool IsFitted() const{ return fIsFitted; }
	virtual int GetNFiredAnodeLayer() const;

	virtual void SetK2(double k2);
	virtual void SetB2(double b2);
	/// set trk info postMagField
	virtual void SetPostMagXproj(double k, double b){ fK2 = k; fB2 = b; }

	// set name, together with the sub-trk's name
	virtual void SetName(const string &name);
	// set title, together with the sub-trk's title
	virtual void SetTitle(const string &title);

	virtual void Initialize();
	/// the core fitting method, fChi2, fDX2, and fDxTa would be assigned here
	/// \note for fit method, BFGS is mandatory, for 4-D minimization is too slow
	virtual void Fit();
	virtual void Show() const;

protected:
	/// minimization using BFGS method
	/// \param k2: slope of postMagTrk; \param b2: intercept of postMagTrk
	/// \param z2: +z border of the equivalent magField
	virtual void BFGSFit(TATrack2 *trkPreTa, TATrack2 *trkPostTa, double k2, double b2);

	double fK2, fB2; // fK2*z + fB2: trk from post-MagField DCs
	double fDX2; // x2 - x2p: x2 calculated from postMagTrk - x2 from arc in the mag
	double fDxTa; ///< the difference of TaHitX between the results of pre- and post-TA track: DxTa0(PreTa) - DxTa1(PostTa)
	double fChi2; // chi^2, the sum of the square of fitting residues
	bool fIsFitted; ///< whether Fit() had been called
	/// NOTE that the class only reference the two TATrack2 objects,
	/// so don't bother releasing their memory, or constructing them
	TATrack2 *fPreTaTrk;
	TATrack2 *fPostTaTrk;
};

#endif

