/**
	\file TADetectorPara.h
	\class TADetectorPara
	\brief Store parameter set of Detector level(MWDC, TOFWall, etc.).
	Container class, storing position parameter (x, y, z, phi, theta, psi).
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/24 Last revised: 2017/10/7, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _DETECTORPARA_H_
#define _DETECTORPARA_H_

#include "TAStuff.h"

class TADetectorPara : public TAStuff{
public:
	TADetectorPara(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TADetectorPara();

	double GetX() const;
	double GetY() const;
	double GetZ() const;
	double GetPhi() const;
	double GetTheta() const;
	double GetPsi() const;
	/// r_global = dR.(R.r_local)+r0+dr
	void GetGlobalRotation(const double *p_local, double *p_global) const;
	void GetGlobalPosition(const double *p_local, double *p_global) const;
	bool IsAssigned() const { return fIsAssigned; }
	bool IsOffset() const { return fIsOffset; }
	
	void SetPosition(const double *p);
	void SetPositionOffset(const double *p);
protected:
	/// x, y, z: one point moving with the detector,
	/// usually the center or one corner on the detector, to specify the detector position.
	double fX, fY, fZ;
	/// the orientation of the detector: yaw, pitch, roll
	/// rotation axis: y, z', y''
	double fPhi, fTheta, fPsi;

	double fXOffset, fYOffset, fZOffset;
	double fAlphaOffset, fBetaOffset, fGammaOffset; ///< rotation axis: z, x, y
	bool fIsOffset; ///< if position offset is used.
	bool fIsAssigned; ///< if position is assigned by TAParaManager.
};

#endif
