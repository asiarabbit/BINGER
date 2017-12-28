///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/t3DTrkInfo.h																 //
//   t3DTrkInfo.h -- header file for struct t3DTrkInfo								 //
//   Introduction: struct for storing three-dimensional straight track information.	 //
// This struct is initially coined for parameter passing of TAEventProcessor::Refine-//
// Tracks() and TAEventProcessor::RefinePID(), yet has been seperated alone for	code //
// extension considerations in the future.											 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/11/23.															     //
// Last modified: 2017/12/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _t3DTRKINFO_H_
#define _t3DTRKINFO_H_

struct t3DTrkInfo{
	double k1, b1, k2, b2;
	double tof2, taHitX; // tof2: time of flight from target to TOFWall; taHitX: hit pos x in TA
	double TOF_posY, TOF_posY_refine; // hit Y position in TOFW strip, rough and refine (dt or 3Dtrk)
	double chi[18], chi2, Chi;
	bool isDCArrR;
	
	t3DTrkInfo();
	void initialize();
};

#endif
