///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADeployPara.h												     //
//   TADeployPara.h -- header file for class TADeployPara						     //
//   Introduction: an information class for storing and conveying detector design	 //
// parameters, mainly used to pass specifications when creating detector objects.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/12.															     //
// Last modified: 2018/3/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TADEPLOYPARA_H_
#define _TADEPLOYPARA_H_

// ** unit: mm ** //

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
	// MWDC design parameters
	short fNDCCable[2][3]; // DCArr[L-R], MWDC[0-1-2]	
	// TOF design parameters
	short fNTOFWallStrip;
};

#endif
