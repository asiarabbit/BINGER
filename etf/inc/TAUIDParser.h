///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAUIDParser.h													     //
//   TAUIDParser.h -- header file for class TAUIDParser							     //
//   Introduction: resolve and interpret uid to comprehensible results, used for	 //
// objects identification.															 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/16.															     //
// Last modified: 2017/10/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAUIDPARSER_H_
#define _TAUIDPARSER_H_

class TAUIDParser{
public:
	TAUIDParser();
	virtual ~TAUIDParser();
	
	static void DNS(int *result, unsigned uid);
	static unsigned UIDGenerator(const int *type);
};

#endif
