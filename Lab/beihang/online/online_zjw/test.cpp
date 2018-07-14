/////////////////////////////////////////////////////////
// File name: TH_analyze.cpp                           //
// Brief introduction:                                 //
//       Create histograms                             //
//                                                     //
// Version: V1.0                                       //
// Author: Han Jianlong                                //
// Date: Mar. 2012                                     //
// For: RIBLL2                                         //
/////////////////////////////////////////////////////////

#ifndef TH_HBOOK
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "cfortran.h"
#include "hbook.h"
#else
#include <math.h>
#include  "TH_define.h"
#endif
#include <iostream>

float tdc[32],adc[32],qdc[32];

float sca[16];
long sbuff[16];
float sum,add,sum1;

void clear_sca_buff()
{
	int i;
  	for(i=0;i<16;i++)
    sbuff[i] = 0;
}

void analyze(unsigned int* event){
  int     event_length;
  int     slot;
  int     channel;
  int     offset=0; 
  int     data_extr;
  
  for (int i=0; i<32; i++)  {
    tdc[i] = -10000;
    adc[i] = -10000;
  } 
  event_length = event[offset];
  
  offset=3;
  
  for (int k=0; k<event_length; k++) {
    data_extr=event[offset];
    // if ((data_extr>> 24 & 0x7) == 2) crate = (data_extr>>16) & 0xFF;
    if ((data_extr >> 24 & 0x7) == 0){
      slot = (data_extr>>27) & 0x1F;
      channel = (data_extr >> 16) & 0x1F;
      if (slot==10) tdc[channel] = (data_extr & 0xFFF);   // tdc
      if (slot== 4) adc[channel] = (data_extr & 0xFFF);   // adc
      if (slot== 7) qdc[channel] = (data_extr & 0xFFF);   // qdc  
      //cout<<slot<<" "<<channel<<" "<<value[slot][channel]<<endl;
    }
    // else if ((data_extr >> 24 & 0x7) == 4) event_num = data_extr & 0xFFFFFF;
    offset++; 
  } 
}
