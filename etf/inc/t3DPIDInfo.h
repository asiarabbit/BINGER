/**
	\file t3DPIDInfo.h
	\struct t3DPIDInfo
	\brief A struct for storing particle identification results using refined
	three-dimensional straight track information. This struct is initially coined for
	parameter passing of TAEventProcessor::RefinePID(), yet has been seperated alone
	for code extension considerations in the future.	
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/11/23 Last revised: 2018/9/5, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _t3DPIDINFO_H_
#define _t3DPIDINFO_H_

struct t3DPIDInfo{
	double aoz, aozdmin;
	double angTaOut[2], beta2;
	double trkLenT;
	double poz;
	double brho;
	
	t3DPIDInfo();
	void initialize();
};

#endif
