
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
//  "tdc00","tdc01","tdc02","tdc03","tdc04","tdc05","tdc06","tdc07",
//  "tdc08","tdc09","tdc10","tdc11","tdc12","tdc13","tdc14","tdc15",
    "adc00","adc01","adc02","adc03","adc04","adc05","adc06","adc07",
    "adc08","adc09","adc10","adc11","adc12","adc13","adc14","adc15",
    "adc16","adc17","adc18","adc19","adc20","adc21","adc22","adc23",
    "adc24","adc25","adc26","adc27","adc28","adc29","adc30","adc31",

    "qdc00H","qdc00L","qdc01H","qdc01L","qdc02H","qdc02L","qdc03H","qdc03L",
    "qdc04H","qdc04L","qdc05H","qdc05L","qdc06H","qdc06L","qdc07H","qdc07L",
    "qdc08H","qdc08L","qdc09H","qdc09L","qdc10H","qdc10L","qdc11H","qdc11L",
    "qdc12H","qdc12L","qdc13H","qdc13L","qdc14H","qdc14L","qdc15H","qdc15L",

    "mtdc00","mtdc01","mtdc02","mtdc03","mtdc04","mtdc05","mtdc06","mtdc07",
    "mtdc08","mtdc09","mtdc10","mtdc11","mtdc12","mtdc13","mtdc14","mtdc15",
    "m2tdc00","m2tdc01","m2tdc02","m2tdc03","m2tdc04","m2tdc05","m2tdc06","m2tdc07",
    "m2tdc08","m2tdc09","m2tdc10","m2tdc11","m2tdc12","m2tdc13","m2tdc14","m2tdc15",

    "ltdc00","ltdc01","ltdc02","ltdc03","ltdc04","ltdc05","ltdc06","ltdc07",
    "ltdc08","ltdc09","ltdc10","ltdc11","ltdc12","ltdc13","ltdc14","ltdc15",
    "wire00","wire01","wire02","wire03","wire04","wire05","wire06","wire07",
    "wire08","wire09","wire10","wire11","wire12","wire13","wire14","wire15",

    "dsca00","dsca01","dsca02","dsca03","dsca04","dsca05","dsca06","dsca07",
    "dsca08","dsca09","dsca10","dsca11","dsca12","dsca13","dsca14","dsca15",

    "gtdc00","gtdc01","gtdc02","gtdc03","gtdc04","gtdc05","gtdc06","gtdc07",
    "gtdc08","gtdc09","gtdc10","gtdc11","gtdc12","gtdc13","gtdc14","gtdc15",
    "gdrft00","gdrft01","gdrft02","gdrft03","gdrft04","gdrft05","gdrft06","gdrft07",
    "gdrft08","gdrft09","gdrft10","gdrft11","gdrft12","gdrft13","gdrft14","gdrft15"
	};

    HBOOK1(1000, "Req_Rate Monitor (/s)",6000,0.,600.,0);// Trigger rate

    for (int i=0; i<32; i++) {
      HBOOK1(i+1,  hist_name[i]   ,4000,0.,4000.,0); //adc v785 zjw@2018
    }
								//HW
    for (int i=0; i<32; i++) {
      HBOOK1(i+33, hist_name[i+32],4000,0.,4000.,0); //qdc v965 zjw@2018
    }

//start mTDC v1190A 100ps/ch **Plastic part								
    for (int i=0; i<32; i++) {
      HBOOK1(i+65, hist_name[i+64],4000,-60000.,60000.,0);   //mtdc1A & mtdc2A zjw@2018
    }
//end mTDC v1190A 100ps/ch   **Plastic part

//start mTDC v1190A 100ps/ch **MWDC part
    for (int i=0; i<16; i++) {
      HBOOK1(i+97, hist_name[i+96],4000,-20000.,60000.,0);  //Leading edge timing of MWDC plane
      HBOOK1(i+113, hist_name[i+112],8,0.5,8.5,0);     //Wire hit pattern of MWDC plane
    }
//end mTDC v1190A 100ps/ch   **MWDC part

//start SCA v830 **increasement
    for (int i=0; i<16; i++) {
      HBOOK1(i+129, hist_name[i+128],16,-0.5,15.5,0);     //SCA increasement 
    }
//end SCA v830   **increasement

//start good TDC
    for (int i=0; i<16; i++) {
      HBOOK1(i+145, hist_name[i+144],4000,0.,80000.,0);   //good TDC 
    }
//end good TDC

//start good drift length
    for (int i=0; i<16; i++) {
      HBOOK1(i+161, hist_name[i+160],400,0.,2.,0);   //good drift length in cm 
    }
//end good drift time

//start counting rate **trigger part
//  HBOOK1(490, "REQ",3601,0,3600.,0); //Request tirgger
    HBOOK1(490, "ACC",601,0,600.,0);   //Accept  tirgger
    HBOOK1(491, "ACC_m",2000001,0,2000000.,0);
//end counting rate   **trigger part

//TOF
    HBOOK1(500, "TOF_TAC",800,110,150.,0);   //TOF from TAC
    HBOOK1(501, "TOF_TDC",400,110.,150.,0); //TOF from mTDC

//PID MUSIC
    HBOOK2(5101, "PID Z-MUSIC1 vs. aoq",900,1.5,3.,1000,0.,10.,0);      //Z vs. TOF before target with MUSIC1
    HBOOK2(5111, "PID dE-MUSIC1 vs. TOF_TDC",1000,110,160.,4000,0.,4000.,0);   //dE vs. TOF before target with MUSIC1
    HBOOK2(5121, "PID dE-MUSIC1-12 vs. TOF_TDC",1000,110,160.,4000,0.,4000.,0);//dE vs. TOF before target with 1 section of MUSIC1
//PID Si
 //   HBOOK2(5201, "PID dE-Si1 vs. TOF_TDC",1000,110,160.,2500,0.,2500.,0); //dE vs. TOF before target with Si1
//PID Si
    HBOOK2(5201, "PID dE-MUSIC2 vs. TOF_TDC",1000,110,160.,4000,0.,4000.,0); //dE vs. TOF before target with MUSIC2

//PID MUSIC
    HBOOK2(510, "PID Z-MUSIC1 vs. aoq",900,1.5,3.,1000,0.,10.,0);      //Z vs. TOF before target with MUSIC1
    HBOOK2(511, "PID dE-MUSIC1 vs. TOF_TAC",1000,110,160.,2500,0.,2500.,0);   //dE vs. TOF before target with MUSIC1
    HBOOK2(512, "PID dE-MUSIC1-12 vs. TOF_TAC",1000,110,160.,2500,0.,2500.,0);//dE vs. TOF before target with 1 section of MUSIC1

//PID Si
    HBOOK2(520, "PID dE-Si1 vs. TOF",1000,110,160.,2500,0.,2500.,0); //dE vs. TOF before target with Si1
//PID Si
    HBOOK2(521, "PID dE-Si1 vs. TOF",1000,110,160.,2500,0.,2500.,0); //17N counts with pileup rejection

//PID PL
    HBOOK2(530, "PID dE-TOF-start vs. TOF",1000,110,160.,1000,0.,4000.,0); //dE vs. TOF before target with PL0-TOF start
    HBOOK2(531, "PID dE-TOF-stop vs. TOF",1000,110,160.,1000,0.,4000.,0); //dE vs. TOF before target with PL1-TOF stop

//dE-dE
    HBOOK2(540, "Si1 vs. MUSIC1",1000,0.,4000.,1000,0.,4000.,0); //dE vs. dE before target with Si1, MUSIC1
    HBOOK2(541, "MUSIC2 34 vs. 12",1000,0.,4000.,1000,0.,4000.,0); //dE vs. dE after target with 2 sections of MUSIC2

    HBOOK1(5420, "dE TOF-start",600,0.,600.,0);//dE TOF-start
    HBOOK1(5421, "dE TOF-stop",1000,0.,1000.,0);//dE TOF-stop
    HBOOK1(5430, "dE MUSIC1",4000,0.,4000.,0);//dE MUSIC1
    HBOOK1(5431, "dE MUSIC2",4000,0.,4000.,0);//dE MUSIC2
    HBOOK1(5432, "dE MUSIC3",4000,0.,4000.,0);//dE MUSIC3
    HBOOK1(5441, "dE Si1",4000,0.,4000.,0);//dE Si1
    HBOOK1(5442, "dE Si21",4000,0.,4000.,0);//dE Si21
    HBOOK1(5443, "dE Si22",4000,0.,4000.,0);//dE Si22


    HBOOK2(5411, "Zin vs. Xt (mm)",560,-70.,70.,1000,0.,10.,0);
    HBOOK2(5412, "Zin vs. Yt (mm) ",560,-70.,70.,1000,0.,10.,0);
    HBOOK2(54111, "dE-MUSIC1 vs. Xtb (mm)",560,-70.,70.,4000,0.,4000.,0);
    HBOOK2(54121, "dE-MUSIC1 vs. Ytb (mm) ",560,-70.,70.,4000,0.,4000.,0);
    HBOOK2(55111, "dE-Si1 vs. Xtb (mm)",560,-70.,70.,4000,0.,4000.,0);
    HBOOK2(55121, "dE-Si1 vs. Ytb (mm) ",560,-70.,70.,4000,0.,4000.,0);

    HBOOK2(5413, "Zin-MUSIC1 vs. Time(s)",360,0.,360.,1000,0.,10.,0);

//Target Angle & Position with MWDC12
    HBOOK1(571, "th.(rad)",200,-0.1,0.1,0);
    HBOOK1(572, "ph.(rad)",200,-0.1,0.1,0);
    HBOOK2(573, "ph.(rad) vs. th.(rad)",200,-0.1,0.1,200,-0.1,0.1,0);

//Target Angle & Position with MWDC34
    HBOOK1(5711, "th.34(rad)",200,-0.1,0.1,0);
    HBOOK1(5721, "ph.34(rad)",200,-0.1,0.1,0);
    HBOOK2(5731, "ph.34(rad) vs. th.34(rad)",200,-0.1,0.1,200,-0.1,0.1,0);

//Target Angle & Position with MWDC12
    HBOOK2(581, "th.(rad) TargetX (mm)",560,-70.,70.,200,-0.1,0.1,0);
    HBOOK2(582, "ph.(rad) TargetY (mm)",560,-70.,70.,200,-0.1,0.1,0);

//Target Angle & Position with MWDC34
    HBOOK2(5811, "th.34(rad) TargetX34 (mm)",560,-70.,70.,200,-0.1,0.1,0);
    HBOOK2(5821, "ph.34(rad) TargetY34 (mm)",560,-70.,70.,200,-0.1,0.1,0);

//Target Position
    HBOOK1(591, "Pos.TargetX (mm)",560,-70.,70.,0);
    HBOOK1(592, "Pos.TargetY (mm)",560,-70.,70.,0);
    HBOOK2(593, "Pos.TargetYX (mm)",560,-70.,70.,560,-70.,70.,0);
    HBOOK1(5911, "Pos34.TargetX (mm)",560,-70.,70.,0);
    HBOOK1(5921, "Pos34.TargetY (mm)",560,-70.,70.,0);
    HBOOK2(5931, "Pos34.TargetYX (mm)",560,-70.,70.,560,-70.,70.,0);

//MWDC1 HE
    HBOOK1(601, "17N TargetX (mm)",560,-70.,70.,0);
    HBOOK1(602, "17N TargetY (mm)",560,-70.,70.,0);
    HBOOK2(603, "17N TargetY vs. X (mm)",560,-70.,70.,560,-70.,70.,0);

//MWDC1 HE
    HBOOK1(611, "MWDC1X (mm)",560,-70.,70.,0);
    HBOOK1(612, "MWDC1Y (mm)",560,-70.,70.,0);
    HBOOK2(613, "MWDC1Y vs. X (mm)",560,-70.,70.,560,-70.,70.,0);

//MWDC2 HE
    HBOOK1(621, "MWDC2X (mm)",560,-70.,70.,0);
    HBOOK1(622, "MWDC2Y (mm)",560,-70.,70.,0);
    HBOOK2(623, "MWDC2Y vs. X (mm)",560,-70.,70.,560,-70.,70.,0);

//MWDC3 HE
    HBOOK1(631, "MWDC3X (mm)",560,-70.,70.,0);
    HBOOK1(632, "MWDC3Y (mm)",560,-70.,70.,0);
    HBOOK2(633, "MWDC3Y vs. X (mm)",560,-70.,70.,560,-70.,70.,0);

//MWDC4 HE
    HBOOK1(641, "MWDC4X (mm)",560,-70.,70.,0);
    HBOOK1(642, "MWDC4Y (mm)",560,-70.,70.,0);
    HBOOK2(643, "MWDC4Y vs. X (mm)",560,-70.,70.,560,-70.,70.,0);

//MWDC1 residual for the first layer
    HBOOK1(711, "MWDC1X (mm)",500,-0.5,0.5,0);
    HBOOK1(712, "MWDC1Y (mm)",500,-0.5,0.5,0);
//MWDC2 residual for the first layer
    HBOOK1(721, "MWDC2X (mm)",500,-0.5,0.5,0);
    HBOOK1(722, "MWDC2Y (mm)",500,-0.5,0.5,0);
//MWDC3 residual for the first layer
    HBOOK1(731, "MWDC3X (mm)",500,-0.5,0.5,0);
    HBOOK1(732, "MWDC3Y (mm)",500,-0.5,0.5,0);
//MWDC4 residual for the first layer
    HBOOK1(741, "MWDC4X (mm)",500,-0.5,0.5,0);
    HBOOK1(742, "MWDC4Y (mm)",500,-0.5,0.5,0);

/*
    HBOOK2(607,"MWPC2x12 y12 with veto",100,-50.,50.,100,-50.,50.,0);//PID 
    HBOOK2(608,"MWPC2 x angle",100,-50.,50.,100,-0.05,0.05,0);//PID
    HBOOK2(609,"MWPC2 y angle",100,-50.,50.,100,-0.05,0.05,0);//PID
    HBOOK2(610,"PID2 BeforTarget;TOF;dE_{PL1}",300,120.,142.,1000,0.,4000.,0);//PID
    HBOOK2(625,"PID22 BeforTarget;TOF;dE_{PL2}",300,120.,142.,1000,0.,4000.,0);//PID

    HBOOK2(611,"PID3 AfterTarget in Si;dE1;dE2",800,0,4000,800,0.,4000.,0);//PID

    HBOOK2(614,"PID0 BeforTarget With PID cut;TOF;dE_{MUSIC}",150,120,140,400,0.,4000.,0);//PID
    HBOOK2(615,"PID1 BeforTarget With PID cut;TOF;dE_{}",150,120,140,400,0.,4000.,0);//PID
    HBOOK2(616,"PID2 BeforTarget With PID cut;TOF;dE_{MUSIC}",150,120,140,400,0.,4000.,0);//PID

    HBOOK2(617, "MWPC1 x12 y12 with PID cut",400,-50.,50.,400,-50.,50.,0);//PID 
    HBOOK2(618, "MWPC2 x12 y12 with PID cut",100,-50.,50.,100,-50.,50.,0);//PID 
    HBOOK2(619, "MWPC3 x12 y12 with PID cut",100,-50.,50.,100,-50.,50.,0);//PID 

    HBOOK2(620,"MWPC2 x angle with PID cut",100,-50.,50.,100,-0.05,0.05,0);//PID
    HBOOK2(621,"MWPC2 y angle with PID cut",100,-50.,50.,100,-0.05,0.05,0);//PID


*/

 }
