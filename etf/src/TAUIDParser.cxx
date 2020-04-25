///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAUIDParser.cxx																								     //
//   TAUIDParser.cxx -- source file for class TAUIDParser												     //
//   Introduction: resolve and interpret uid to comprehensible results, used for		 //
// objects identification.																													 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/10/16.																													     //
// Last modified: 2020/04/25, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAPopMsg.h"
#include "TAUIDParser.h"

TAUIDParser::TAUIDParser(){}
TAUIDParser::~TAUIDParser(){}

void TAUIDParser::DNS(int *result, unsigned uid){
	result[0] = uid & 0x3F; // the first 6 bits, detector id
	switch(result[0]){
		case 0: case 1: case 2: // TAT0_0, TAT0_1, TAT0_0UD
			result[1] = (uid>>6) & 0x3; break; // 0: UV; 1: UH; 2:DV; 3:DH
		case 3: case 4: // MWDC Arrays after the big dipole Mag: 3->L, 4->R
		case 6: case 7: // MWDC Arrays around the target: 6->U, 7->D
		case 8: case 9: // MWDC Arrays around the target: 8->PDCU, 9->PDCD
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
			break;
	} // end of switch(result[0])

} // end of member function DNS

unsigned TAUIDParser::UIDGenerator(const int *type){
	if(0 == type[0] || 1 == type[0] || 2 == type[0]){
		if(type[1] <= 0x3) return type[0] + (type[1]<<6); // TAT0_1
	} // end the lengthy if

	if(3 == type[0] || 4 == type[0] // MWDC Arrays after the big dipole Mag: 3->L, 4->R
	   || 6 == type[0] || 7 == type[0] // MWDC Arrays around the target: 6->U, 7->D
	   || 8 == type[0] || 9 == type[0] // MWDC Arrays around the target: 8->PDCArrU, 9->PDCArrD
	   ){
		// check the validity of the type for the MWDC arrays
		if(type[1] < 0 || type[1] > 3)
			TAPopMsg::Error("TAUIDParser",
					"UIDGenerator: Input type out of range: type[1]: %d", type[1]);
		if(type[1] >= 0 && type[1] <= 2){ // MWDCs
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

	TAPopMsg::Error("TAUIDParser", "UIDGenerator: Input type out of range: type[0]: %d", type[0]);
	return 999999999;
} // end of member function UIDGenerator
