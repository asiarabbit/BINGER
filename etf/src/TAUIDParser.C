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
// Last modified: 2018/3/19, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
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
	if(3 == result[0] || 4 == result[0]){ // MWDC Arrays after the big dipole Mag: 3->L, 4->R
		result[1] = (uid>>6) & 0x7; // the next 3 bits, DC id or TOF Wall [0-4]: [DC0-1-2-TOFW]
		if(result[1] < 3){ // MWDCs
			result[2] = (uid>>9) & 0x3; // the next 2 bits, super layer id [0-1-2]: [X-U-V]
			result[3] = (uid>>11) & 0x7; // the next 3 bits, cable id
			result[4] = (uid>>14) & 0x1; // the next 1 bit, SFE16 id, 0 or 1
			result[5] = (uid>>15) & 0xF; // the next 4 bits, 16 anodes for a certain SFE16 chip
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
	if(6 == result[0] || 7 == result[0]){ // MWDC Arrays around the target: 6->U, 7->D
		result[1] = (uid>>6) & 0x3; // the next 2 bits, DC id or PlaT0 [0-2]: [DC0-1-PlaT0]
		if(result[1] < 2){ // MWDCs
			result[2] = (uid>>8) & 0x1; // the next 1 bit, super layer id [0-1-2]: [X-Y]
			result[3] = (uid>>9) & 0x1; // the next 1 bit, cable id; well actually there's only 1 cab
			result[4] = (uid>>10) & 0x3; // the next 2 bits, SFE16 id, 0 or 1
			result[5] = (uid>>12) & 0xF; // the next 4 bits, 16 anodes for a certain SFE16 chip
		}
		else{ // plastic scintillator strip for beam timing
			result[2] = (uid>>8) & 0x1; // the next 1 bit, strip id; actually there's only 1 strip
			result[3] = (uid>>9) & 0x3; // 0: UV; 1: UH; 2: DV; 3: DH, 2bits
		}
	}
}

unsigned TAUIDParser::UIDGenerator(const int *type){
	if(0 == type[0] && type[1] <= 0x1) return type[0] + (type[1]<<6); // T0_0

	if(1 == type[0] && type[1] <= 0x3) return type[0] + (type[1]<<6); // T0_1

	if(2 == type[0] && type[1] <= 0x1F){ // SiPM Plastic Array
		return type[0] + (type[1]<<6);
	}

	if(3 == type[0] || 4 == type[0]){ // MWDC arrays after the big dipole magnet
		// check the validity of the type for the MWDC arrays
		if(type[1] < 0 || type[1] > 3)
			TAPopMsg::Error("TAUIDParser",
					"UIDGenerator: Input type out of range: type[1]: %d", type[1]);
		if(type[0] >= 0 && type[1] <= 2){ // MWDCs
			if(type[2] > 0x3 || type[2] < 0) // DCSLayer id
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[2]: %d", type[2]);
			if(type[3] > 0x7 || type[3] < 0) // cable id
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[3]: %d", type[3]);
			if(type[4] > 0x1 || type[4] < 0) // SFE16 chip belonging to a cable
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[4]: %d", type[4]);
			if(type[5] > 0xF || type[5] < 0) // anodes belonging to a certain SFE16 chip
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[5]: %d", type[5]);
			// generate the required uid
			unsigned uid = type[0] + (type[1]<<6) + (type[2]<<9)
				+ (type[3]<<11) + (type[4]<<14) + (type[5]<<15);
			return uid;
		}
		else{ // TOF Wall
			if(type[2] > 0x3F || type[2] < 0) // strip Id
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[2]: %d", type[2]);
			if(type[3] > 0x3 || type[3] < 0) // 4 chs for a strip
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[3]: %d", type[3]);
			unsigned uid = type[0] + (type[1]<<6) + (type[2]<<9) + (type[3]<<15);
			return uid;
		}
	}
	if(5 == type[0]){
		return type[0] + (type[1]<<6) + (type[2]<<9) + (type[3]<<15);
	}
	if(6 == type[0] || 7 == type[0]){
		if(type[1] < 0 || type[1] > 2)
			TAPopMsg::Error("TAUIDParser",
				"UIDGenerator: Input type out of range: type[1]: %d", type[1]);
		if(type[1] >= 0 && type[1] <= 1){ // MWDCs
			if(type[2] > 0x1 || type[2] < 0) // DCSLayer id
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[2]: %d", type[2]);
			if(type[3] > 0x1 || type[3] < 0) // cable id
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[3]: %d", type[3]);
			if(type[4] > 0x1 || type[4] < 0) // SFE16 chip belonging to a cable
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[4]: %d", type[4]);
			if(type[5] > 0xF || type[5] < 0) // anodes belonging to a certain SFE16 chip
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[5]: %d", type[5]);
			// generate the required uid
			unsigned uid = type[0] + (type[1]<<6) + (type[2]<<8)
				+ (type[3]<<9) + (type[4]<<10) + (type[5]<<11);
			return uid;
		}
		else{ // fPlaT0
			if(type[2] > 0x1 || type[2] < 0) // strip Id
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[2]: %d", type[2]);
			if(type[3] > 0x2 || type[3] < 0) // 4 chs for a strip
				TAPopMsg::Error("TAUIDParser",
						"UIDGenerator: Input type out of range: type[3]: %d", type[3]);
			unsigned uid = type[0] + (type[1]<<6) + (type[2]<<8) + (type[3]<<9);
			return uid;
		}
	}

	TAPopMsg::Error("TAUIDParser", "UIDGenerator: Input type out of range: type[0]: %d", type[0]);
	return 999999999;
}










