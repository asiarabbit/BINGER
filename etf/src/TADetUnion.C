///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TADetUnion.C														 //
//   TADetUnion.C -- source file for class TADetUnion								 //
//   Introduction: Abstract base class, providing a common interface to detector	 //
// classes that has UID resolution function and is at the first level of the		 //
// detector hierarchy, which also has the right to design UID resolution scheme of	 //
// their own.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/13.															     //
// Last modified: 2018/6/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TADetUnion.h"
#include "TAStuff.h"
#include "TAUIDParser.h"

TADetUnion::TADetUnion(unsigned uid) : fDetId(-1){
	int type[6]{}; TAUIDParser::DNS(type, uid);
	fDetId = type[0];
}
TADetUnion::~TADetUnion(){}
