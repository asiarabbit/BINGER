/////////////////////////////////////////////////////////
// File name: TH_fill.cpp                              //
// Brief introduction:                                 //
//       Fill histograms                               //
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
#include <stdio.h>
#include <math.h>
#include  "TH_define.h"
#endif

// Set Global Parameters for analyze
//- For Histogram ------------------------------------------------------

extern float  tdc[32],adc[32],qdc[32],mtdc[32][10]; 
extern int    hit[32];
extern unsigned long sca[16], psca[16], dsca[16];

void hbook_fill()
{

   for (int i=0; i<32; i++) {
       HF1(i+1,tdc[i],1.);
       HF1(i+33,adc[i],1.);
       HF1(i+65,qdc[i],1.);
       HF1(i+129,hit[i],1.);
       for (int j=0; j<hit[i]; j++) HF1(i+97,mtdc[i][j],1.);
       if(i<16&&dsca[i]>0){
	 //printf("%2d,:%f:%ld\n",i,weight,dsca[i]);
	 //HF1(161,i,dsca[i]);
	 if(dsca[i]>10){
	   HF1(161,i,10.);
	 }else{
	   HF1(161,i,dsca[i]);
	 }
       }
   }
   //HF1(500,(tdc[1]+tdc[2])/2,1.);
   //HF1(501,(tdc[3]+tdc[4])/2,1.);
   if(tdc[10]>0&&tdc[11]>0)
     HF1(500,(tdc[10]-tdc[11]),1.);
   if(tdc[12]>0&&tdc[13]>0)
     HF1(501,(tdc[12]-tdc[13]),1.);
   HF2(502,tdc[10]-tdc[9],tdc[11]-tdc[9],1.);
   HF2(503,tdc[12]-tdc[9],tdc[13]-tdc[9],1.);
   if(tdc[10]>0&&tdc[11]>0&&tdc[12]>0&&tdc[13]>0)
     HF2(504,tdc[10]-tdc[11],tdc[12]-tdc[13],1.);
   //wang
   if(tdc[17]>0&&tdc[18]>0)
     HF1(505,(tdc[17]-tdc[18]),1.);
   if(tdc[19]>0&&tdc[20]>0)
     HF1(506,(tdc[19]-tdc[20]),1.);
   if(tdc[22]>0&&tdc[23]>0)
     HF1(507,(tdc[22]-tdc[23]),1.);
   if(tdc[24]>0&&tdc[25]>0)
     HF1(508,(tdc[24]-tdc[25]),1.);
   if(tdc[27]>0&&tdc[28]>0)
     HF1(509,(tdc[27]-tdc[28]),1.);
   if(tdc[29]>0&&tdc[30]>0)
     HF1(510,(tdc[29]-tdc[30]),1.);
   if(tdc[0]>0&&tdc[1]>0&&tdc[2]>0&&tdc[3]>0&&adc[4]>0&&adc[5]>0)
     HF2(511,adc[4]/2.+adc[5]/2.,(tdc[1]+tdc[2])/2.-tdc[0],1.);
}
