///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/t3DPIDInfo.h																 //
//   t3DPIDInfo.h -- header file for struct t3DPIDInfo								 //
//   Introduction: struct for storing particle identification results using refined	 //
// three-dimensional straight track information. This struct is initially coined for //
// parameter passing of TAEventProcessor::RefinePID(), yet has been seperated alone	 //
// for code extension considerations in the future.									 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/11/23.															     //
// Last modified: 2017/12/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _t3DPIDINFO_H_
#define _t3DPIDINFO_H_

struct t3DPIDInfo{
	double aoz, aozdmin;
	double angTaOut[2], beta2;
	double trkLenT;
	double poz;
	
	t3DPIDInfo();
	void initialize();
};

#endif
