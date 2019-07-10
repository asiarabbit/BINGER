/**
	\file t3DTrkInfo.h
	\struct t3DTrkInfo
	\brief A struct for storing three-dimensional straight track information.
	This struct is initially coined for parameter passing of TAEventProcessor::RefineTracks()
	and TAEventProcessor::RefinePID(), yet has been seperated alone for code extension
	considerations in the future.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/11/23 Last revised: 2018/9/7, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _t3DTRKINFO_H_
#define _t3DTRKINFO_H_

struct t3DTrkInfo{
	double k1, b1, k2, b2; //!< track information downstream the dipole magnet
	double tof2, taHitX; //!< tof2: time of flight from target to TOFWall; taHitX: hit pos x in TA
	int firedStripId;
	double TOF_posY, TOF_posY_refine; //!< hit Y position in TOFW strip, rough and refine (dt or 3Dtrk)
	double chi[18], chi2, Chi;
	/// time over threshold of DC signals, indicator of dE/dx
	double dcTOTAvrg; //!< averaged over all hit anode layers for a 3D track (maximum: 18)
	int dcTOTcnt; //!< number of valid dcTOTs
	bool isDCArrR; //!< false: L; true: R;     false: U; true: D;

	t3DTrkInfo();
	void initialize();
};

#endif
