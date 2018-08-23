/////////////////////////////////////////////////////////
// File name: TH_analyze.cpp                           //
// Brief introduction:                                 //
//       Create histograms                             //
//                                                     //
// Version: V1.0                                       //
// Author: Han Jianlong                                //
// Date: Mar. 2012                                     //
// For: RIBLL2                                         //
//                                                     //
// Modified for adding V1190 and V830 for CCCS         //
// Date: Mar. 2018    by zjw                           //
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

#define MONI 1000 //5000

//#define EMESS //zjw@2018

float tdc[32], adc[32], qdc[32];

float mtdc[16][10],mtdc2[16][10];//v1190 Card-A
int hit[16],hit2[16]; 
float ltdc[20][10],wire[20][10]; //leading edge timing
float rtdc[20][10],wirer[20][10];//trialing edge timing
int mhit[20],mhitr[20];

#include "dcmap.inc"

float rate[16], acc_ratio;

unsigned long sca[16], psca[16], dsca[16];
static unsigned long  req, acc;
long sbuff[16];//SCA buffer
float sum,add,sum1;

void clear_sca_buff()
{
  int i;
  for(i=0;i<16;i++){
    sbuff[i] = 0;
  }
}

void analyze( int* event)
{
  int     event_length,i;

  int     slot,header,geo,cnt,channel,nums;
  int     lt,layer;
  int     offset=0,id_v1290=0,id_v830=0; 
  int     data_extr;

  static unsigned long tsca0, tsca1, tsca2, tsca3, tsca4, tsca5, tsca6, tsca7, tsca8;//HE
  static unsigned long tsca9, tsca10, tsca11, tsca12, tsca13, tsca14, tsca15;//HE

  for (int i=0; i<32; i++){
    adc[i] = -9999;
    qdc[i] = -9999;
  }
  
//v1190 Card-A
  for (int i=0; i<16; i++){
    hit[i] = 0; hit2[i] = 0;
    for (int j=0; j<10; j++){
      mtdc[i][j] = -9999;
      mtdc2[i][j] = -9999;
    }   
  }

//v1190 Card-C&D
  for (int i=0; i<20; i++){
    mhit[i] = 0; 
    mhitr[i] = 0; 
    for (int j=0; j<10; j++){
      ltdc[i][j] = -9999;
      rtdc[i][j] = -9999;
      wire[i][j] = -1;      
      wirer[i][j] = -1;
    }
  } 

  event_length = event[offset];
  offset=3;
  
  //printf("event_length:%d\n",event_length);
  for (int k=0; k<event_length-3; k++){
    data_extr=event[offset];
    //printf("DATA:%10x\n",data_extr);
    header=(data_extr >> 24) & 0x7;
    slot = (data_extr >> 27) & 0x1F;
    geo =   data_extr & 0x1F;
    //printf("slot:geo:%d:%d\n",slot,geo);
    if(header == 0&&(slot==5||slot==17)){
      //printf("HIT :%10x,%d\n",data_extr,slot); //for V785&V965
      channel = (data_extr >> 16) & 0x1F;

      if (slot==17) adc[channel] = (data_extr & 0xFFF);   // adc 
      if (slot==5) qdc[channel] = (data_extr & 0xFFF);   // qdc 

      //printf("%f %f\n",channel, adc[channel]);}
    }else if(header == 2&&(slot==5||slot==17)){//header for V785&V965
      cnt  = (data_extr >>  8) & 0x3F;

#ifdef EMESS
      printf("Header  for V7x5:%2d:%2d\n", slot,cnt);
#endif

    }else if(header == 4&&(slot==5||slot==17)){//header for V785&V965

#ifdef EMESS
      printf("Trailer for V785&V965:%2d\n", slot);
#endif

    }else if(slot == 8 && (geo==9||geo==11)){

#ifdef EMESS
      printf("Global Header for V1190:%d\n",geo);
#endif

      id_v1290++;
    }else if(slot== 1&&(id_v1290==1||id_v1290==2)){

#ifdef EMESS
      printf("TDC Header for V1190\n");
#endif

    }else if(slot== 0&&(id_v1290==1||id_v1290==2)){//V1190 TDC measurement
      channel = (data_extr >> 19) & 0x7F;//V1190 channel NO.
      lt = (data_extr >> 26) & 0x01;//V1190 leading (0) or trailing (1) timing
#ifdef EMESS
      printf("Ch:%2d,LR:%1d,TDC:%d:%x\n",channel,lt,data_extr & 0x7FFFF,data_extr);
#endif

/**/
      if(id_v1290==1){
        if(channel<16)//Plastic
          if(lt==0){
            if(hit[channel]<10) //zjw@2018
            mtdc[channel][hit[channel]]=data_extr & 0x7FFFF;
     
            hit[channel]++;
          }
         // printf("mtdc_ch9:%f\n",mtdc[9][0]);
        //printf("Layer:%d, Ch:%d\n",npl1[channel],nch1[channel]);

        if(channel>=64){//Drift chamber
          if(lt==0){
            layer = npl1[channel]-1;
            if(mhit[layer]<10){
              wire[layer][mhit[layer]] = nch1[channel];
              ltdc[layer][mhit[layer]] = data_extr & 0x7FFFF;
            }
            mhit[layer]++;
          }else if(lt==1){
            layer = npl1[channel]-1;
            if(mhitr[layer]<10){
              wirer[layer][mhitr[layer]] = nch1[channel];
              rtdc[layer][mhitr[layer]++] = data_extr & 0x7FFFF;
            }
          }
        } 
      }else if(id_v1290==2){
        if(channel<16)//Plastic
          if(lt==0){
            if(hit2[channel]<10) //zjw@2018
            mtdc2[channel][hit2[channel]]=data_extr & 0x7FFFF;
            
            hit2[channel]++;
          }
         //printf("mtdc2_ch15:%f\n",mtdc2[15][0]);
        //printf("Layer:%d, Ch:%d\n",npl1[channel],nch1[channel]);
       //printf("mtdc_9:%f, mtdc2_9:%f, mtdc2_15:%f\n",mtdc[9][0],mtdc2[9][1],mtdc2[15][0]);
      //if(id_v1290==1&&channel==15&&lt==0)printf("mtdc2[15]:%d\n",data_extr & 0x7FFFF);
        if(channel>=64){//Drift chamber
          if(lt==0){
            layer = npl2[channel]-1;
            if(mhit[layer]<10){
              wire[layer][mhit[layer]] = nch2[channel];
              ltdc[layer][mhit[layer]] = data_extr & 0x7FFFF;
            }
            mhit[layer]++;
          }else if(lt==1){
            layer = npl2[channel]-1;
            if(mhitr[layer]<10){
              wirer[layer][mhitr[layer]] = nch2[channel];
              rtdc[layer][mhitr[layer]++] = data_extr & 0x7FFFF;
            }
          }
        }
      }
/**/
    }else if(slot== 3&&(id_v1290==1||id_v1290==2)){
      cnt  = data_extr & 0xFFF;
#ifdef EMESS
      printf("TDC Ender for V1190:%2d Hit\n",cnt-2);
#endif

    }else if(slot==16&&geo==11){
#ifdef EMESS
      printf("Global Trailer for 2nd V1190\n");
#endif
      id_v1290=0;

    }else if (slot==21){
#ifdef EMESS
      printf("V830 Header\n");
#endif
      nums=0;
      id_v830=1;

    }else if(id_v830==1){
#ifdef EMESS 
      printf("V830 DATA Ch.%2d :%d\n",nums+1,data_extr);
#endif
      psca[nums]=sca[nums]; //The 1st event of SCA must be skiped zjw@2018
      sca[nums]=data_extr & 0xFFFFFFFF;
      dsca[nums]=sca[nums]-psca[nums];
      
      if(nums==15&&sca[15]/MONI!=psca[15]/MONI){//if time is long enough for our clock(1kHz)
        rate[1]=(sca[1]-psca[1])*1000./(sca[15]-psca[15]);//Hz
       }
/*
        for(int i=1; i<16; i++){//ch.0 of v830 is broken
	  rate[i]=(sca[i]-psca[i])*1000./(sca[15]-psca[15]); //counting rate(Hz) zjw@2018
	}

	req = (sca[1]-psca[1]);//increase
	acc = (sca[2]-psca[2]);//increase
	acc_ratio = acc*100./req;//accept ratio(%) of DAQ 
        printf("** Trigger rate information **\n");
	printf("Reqest:%7.2f (Hz), Acc.:%7.2f (Hz), Acc_ratio of DAQ:%5.2f (%), Time:%6ld (sec.), Acc.:%10ld (events)\n"
	       ,rate[1],rate[2],acc_ratio,sca[15]/1000,sca[2]);
        printf("TOF_start:%7.2f(Hz), TOF_stop:%7.2f(Hz), TOF_coin.:%7.2f(Hz), Main_trig.:%7.2f(Hz)\n"//HE
	       ,rate[3],rate[4],rate[5],rate[6]);
        printf("PL2      :%7.2f(Hz), Veto1   :%7.2f(Hz), Veto2    :%7.2f(Hz), Pile_up   :%7.2f(Hz)\n"//HE
	       ,rate[7],rate[8],rate[9],rate[10]);

	fflush(stdout);

        for(int i=1; i<16; i++){//ch.0 of v830 is broken
	  psca[i]=sca[i];       // zjw@2018
	}	

      } 
      //printf("%d:%ld:%ld:%ld\n",nums,sca[nums],psca[nums],dsca[nums]);
*/
      nums++;
      if(nums==16) id_v830=0;//end indicator of V830 data
    }
    //else if ((data_extr >> 24 & 0x7) == 4) event_num = data_extr & 0xFFFFFF;
    offset++; 
  } 
}
