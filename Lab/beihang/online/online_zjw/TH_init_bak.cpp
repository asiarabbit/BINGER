
#ifndef TH_HBOOK
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "cfortran.h"
#include "hbook.h"
#else
#include  "TH_define.h"
#endif



void hbook_init()
{
  
  char hist_name[][10]={
    "tdc00","tdc01","tdc02","tdc03","tdc04","tdc05","tdc06","tdc07",
    "tdc08","tdc09","tdc10","tdc11","tdc12","tdc13","tdc14","tdc15",
    "tdc16","tdc17","tdc18","tdc19","tdc20","tdc21","tdc22","tdc23",
    "tdc24","tdc25","tdc26","tdc27","tdc28","tdc29","tdc30","tdc31",
    "adc00","adc01","adc02","adc03","adc04","adc05","adc06","adc07",
    "adc08","adc09","adc10","adc11","adc12","adc13","adc14","adc15",
    "adc16","adc17","adc18","adc19","adc20","adc21","adc22","adc23",
    "adc24","adc25","adc26","adc27","adc28","adc29","adc30","adc31",     
    "qdc00","qdc01","qdc02","qdc03","qdc04","qdc05","qdc06","qdc07",
    "qdc08","qdc09","qdc10","qdc11","qdc12","qdc13","qdc14","qdc15",
    "qdc16","qdc17","qdc18","qdc19","qdc20","qdc21","qdc22","qdc23",
    "qdc24","qdc25","qdc26","qdc27","qdc28","qdc29","qdc30","qdc31",
    //};
    "mtdc00","mtdc01","mtdc02","mtdc03","mtdc04","mtdc05","mtdc06","mtdc07",
    "mtdc08","mtdc09","mtdc10","mtdc11","mtdc12","mtdc13","mtdc14","mtdc15",
    "mtdc16","mtdc17","mtdc18","mtdc19","mtdc20","mtdc21","mtdc22","mtdc23",
    "mtdc24","mtdc25","mtdc26","mtdc27","mtdc28","mtdc29","mtdc30","mtdc31",
    "mhit00","mhit01","mhit02","mhit03","mhit04","mhit05","mhit06","mhit07",
    "mhit08","mhit09","mhit10","mhit11","mhit12","mhit13","mhit14","mhit15",
    "mhit16","mhit17","mhit18","mhit19","mhit20","mhit21","mhit22","mhit23",
    //"mhit24","mhit25","mhit26","mhit27","mhit28","mhit29","mhit30","mhit31"};
    "mhit24","mhit25","mhit26","mhit27","mhit28","mhit29","mhit30","mhit31",
    "sca00"};
  
    for (int i=0; i<32; i++) {
      HBOOK1(i+1,  hist_name[i]   ,4000,0.,4000.,0);
    }
    for (int i=0; i<32; i++) {
      HBOOK1(i+33, hist_name[i+32],4000,0.,4000.,0);
    }
    for (int i=0; i<32; i++) {
      HBOOK1(i+65, hist_name[i+64],4000,0.,4000.,0);
    }
    for (int i=0; i<32; i++) {//MTDC 25ps/ch 
      HBOOK1(i+97, hist_name[i+96],4000,0.,200000.,0);
    }
    for (int i=0; i<32; i++) {//MTDC
      HBOOK1(i+129, hist_name[i+128],10,-0.5,9.5,0);
    }
    HBOOK1(161, hist_name[160],16,-0.5,15.5,0);

    HBOOK1(500, "MWPC0x1-x2",4000,-2000.,2000,0);
    HBOOK1(501, "MWPC0y1-y2",4000, 2000.,2000,0);
    HBOOK2(502, "MWPC0x1a x2a",400,000.,4000,400,000.,4000,0);
    HBOOK2(503, "MWPC0y1a y2a",400,000.,4000,400,000.,4000,0);
    HBOOK2(504, "MWPC0x12 y12",400,-2000.,2000,400,-2000.,2000,0);
    //wang
    HBOOK1(505, "MWPC1x1-x2",4000,-2000.,2000,0);
    HBOOK1(506, "MWPC1y1-y2",4000, 2000.,2000,0); 
    HBOOK1(507, "MWPC2x1-x2",4000,-2000.,2000,0);
    HBOOK1(508, "MWPC2y1-y2",4000, 2000.,2000,0);  
    HBOOK1(509, "MWPC3x1-x2",4000,-2000.,2000,0);
    HBOOK1(510, "MWPC3y1-y2",4000, 2000.,2000,0); 

 }
