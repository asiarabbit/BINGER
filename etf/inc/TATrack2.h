/**
	\file TATrack2.h
	\class TATrack2
	\brief A derived class from TATrack, specifically designed for DCs
	around the target. This class is mainly for track fitting of those DCs.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/3/21 Last revised: 2018/4/3, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TATRACK2_H_
#define _TATRACK2_H_

#include "TATrack.h"

class TATrack2 : public TATrack{
public:
	TATrack2(const string &name = "", const string &title = "", unsigned uid = 999999999);
	TATrack2(const TATrack2 &track); ///< the copy constructor
	TATrack2 &operator=(const TATrack2 &track); ///< the assignment constructor
	virtual ~TATrack2();

	// since that TATrack2 class processes 4-d arrays, some of the methods
	// involving in TATrack would be re-written
	virtual void GetNu(int *nu) const; ///< output array fNu
	virtual void GetLAYER(int *LAYER) const; ///< output array fLAYER
	virtual void GetZ(double *z) const; ///< output array fZ
	virtual void GetX(double *x) const; ///< output array fX
	virtual void GetDriftTime(double *t) const; ///< output array fT
	virtual void GetDriftDistance(double *r) const; ///< output array fR
	virtual void GetChi(double *chi); ///< output array fChi
	using TATrack::GetChi;
	virtual void SetData(const double *x, const double *z, const double *t, const double *r, double kL, double bL, double dsquare, int gGOOD, const int *nu, const int *LAYER, const double *weight = nullptr);
	virtual void SetDriftTime(const double *t, const double *w = nullptr); ///< \param w: weight
	virtual void SetDriftDistance(const double *r); ///< as the name indicates
	virtual void AssignTrack(tTrack *ptrack); ///< assign tTrack
	/// show information about the track
	virtual void Show();
protected:
};

#endif

