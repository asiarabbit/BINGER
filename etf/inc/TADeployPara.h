/**
	\file TADeployPara.h
	\class TADeployPara
	\brief An information class for storing and conveying detector design
	parameters, mainly used to pass specifications when creating detector objects.
	accommodates TADCCable objects.
	** unit: mm **
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/12 Last revised: 2018/3/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TADEPLOYPARA_H_
#define _TADEPLOYPARA_H_


class TADeployPara{
public:
	static TADeployPara *Instance();
	virtual ~TADeployPara();
	
	short GetNDCCable(unsigned uid) const;
	short GetNTOFWallStrip(unsigned uid) const;
	double GetTOFWallStripWidth(unsigned uid) const;
	double GetTOFWallStripLength(unsigned uid) const;
	double GetTOFWallStripDelay(unsigned uid) const;
	double GetTOFWallStripVeff(unsigned uid) const;
	double GetTOFWallDelayAvrg(unsigned uid) const;
	double GetMWDCDelay(unsigned uid) const;
	double GetSiPMArrayZ0() const{ return -885.75; }
	double GetTargetZ0() const;
protected:
	TADeployPara();

	static TADeployPara *fInstance;
	/// MWDC design parameters
	short fNDCCable[2][3]; // DCArr[L-R], MWDC[0-1-2]
	/// TOF design parameters
	short fNTOFWallStrip;
};

#endif
