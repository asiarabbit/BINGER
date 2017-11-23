///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAUIDParser.C													     //
//   TAUIDParser.C -- source file for class TAUIDParser							     //
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

#include "TAPopMsg.h"
#include "TAUIDParser.h"

TAUIDParser::TAUIDParser(){}
TAUIDParser::~TAUIDParser(){}
	
void TAUIDParser::DNS(int *result, unsigned uid){
	result[0] = uid & 0x3F; // the first 6 bits, detector id
	if(0 == result[0]){ // T0_0
		result[1] = (uid>>6) & 0x1; // 0: UV; 1: UH
	}
	if(1 == result[0]){ // T0_1
		result[1] = (uid>>6) & 0x3; // 0: UV; 1: UH; 2:DV; 3:DH
	}
	if(2 == result[0]){ // SiPM Plastic Array
		result[1] = (uid>>6) & 0x1F; // 0-10: 10 strips, 20chs, 5 bits
	}
	if(3 == result[0] || 4 == result[0]){ // MWDC Array: 3->L, 4->R
		result[1] = (uid>>6) & 0x7; // the next 3 bits, DC id or TOF Wall
		if(result[1] < 3){ // MWDCs
			result[2] = (uid>>9) & 0x3; // the next 2 bits, super layer id [0-1-2]: [X-U-V]
			result[3] = (uid>>11) & 0x7; // the next 3 bits, cable id
			result[4] = (uid>>14) & 0x1; // the next 1 bit, SFE16 id, 0 or 1.
			result[5] = (uid>>15) & 0xF; // the next 4 bits, 16 anodes
		}
		else{ // TOF Wall
			result[2] = (uid>>9) & 0x3F; // the next 6 bits, strip id
			result[3] = (uid>>15) & 0x3; // the next 2 bits, 0: UV; 1: UH; 2: DV; 3: DH
		}
	} // end if(MWDC array)
	if(5 == result[0]){ // SiPM Plastic Barrel
		result[1] = (uid>>6) & 0x7; // plate Id, 3 bits
		result[2] = (uid>>9) & 0x3; // sub-strip id, 2 bits
		result[3] = (uid>>15) & 0x3; // 0: UV; 1: UH; 2: DV; 3: DH, 2bits
	}
}

unsigned TAUIDParser::UIDGenerator(const int *type){
	if(0 == type[0] && type[1] <= 0x1) return type[0] + (type[1]<<6); // T0_0

	if(1 == type[0] && type[1] <= 0x3) return type[0] + (type[1]<<6); // T0_1

	if(2 == type[0] && type[1] <= 0x1F){ // SiPM Plastic Array
		return type[0] + (type[1]<<6);
	}

	if(3 == type[0] || 4 == type[0]){ // MWDC array
		if(type[1] < 0 || type[1] > 3)
			TAPopMsg::Error("TAUIDParser",
					"UIDGenerator: Input type out of range: type[2]: %d", type[1]);
		if(type[0] >= 0 && type[1] < 3){ // MWDCs
			if(type[2] > 0x3 || type[2] < 0)
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[2]: %d", type[2]);
			if(type[3] > 0x7 || type[3] < 0) // DCSLayer
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[3]: %d", type[3]);
			if(type[4] > 0x1 || type[4] < 0)
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[4]: %d", type[4]);
			if(type[5] > 0xF || type[5] < 0)
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[5]: %d", type[5]);
			unsigned uid = type[0] + (type[1]<<6) + (type[2]<<9)
				+ (type[3]<<11) + (type[4]<<14) + (type[5]<<15);
			return uid;
		}
		else{ // TOF Wall
			if(type[2] > 0x3F || type[2] < 0)
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[2]: %d", type[2]);
			if(type[3] > 0x3 || type[3] < 0)
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[3]: %d", type[3]);
			unsigned uid = type[0] + (type[1]<<6) + (type[2]<<9) + (type[3]<<15);
			return uid;
		}
	} // end if(MWDC array)
	if(5 == type[0]){
		return type[0] + (type[1]<<6) + (type[2]<<9) + (type[3]<<15);
	}
	else{
		TAPopMsg::Error("TAUIDParser",
			"UIDGenerator: Input type out of range: type[3]: %d", type[0]);
		return 999999999;
	}
}










