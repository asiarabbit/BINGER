/**
	\file tTrack2.h
	\struct tTrack2
	\brief Struct for holding track information. The struct is mainly for
	assigning TTree objects and output of track parameters. Different from tTrack,
	this struct serves for DCs around the target. To ease the coding process and to
	increase uniformity and stability, tTrack2 is set as derived from tTrack.
	
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/3/22 Last revised: 2018/4/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _tTRACK2_H_
#define _tTRACK2_H_

#include "tTrack.h"

struct tTrack2 : public tTrack{ // struct of tracks
	tTrack2(); ///< the default constructor
	virtual ~tTrack2(){}; ///< the default destructor
	virtual void show() const;
	virtual void initialize();
};

#endif
