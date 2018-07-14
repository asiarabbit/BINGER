/////////////////////////////////////////////////////////
// File name: TH_fill.cpp                              //
// Brief introduction:                                 //
//       Fill histograms                               //
//                                                     //
// Version: V1.0                                       //
// Author: Han Jianlong                                //
// Date: Mar. 2012                                     //
// For: RIBLL2                                         //
//                                                     //
// Modified for adding V1190 and V830 for CCCS         //
// Date: Mar. 2018     by zjw                          //
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

#include <TCutG.h>

#include <TMatrixD.h>
//#include "analysis.inc"

// Set Global Parameters for analyze
//- For Histogram ------------------------------------------------------

extern float tdc[32],adc[32],qdc[32]; 

extern float mtdc[16][10],mtdc2[16][10];
extern int hit[16],hit2[16];
extern float ltdc[20][10],rtdc[20][10]; 
extern float wire[20][10],wirer[20][10];
extern int mhit[20],mhitr[20];

extern int layer;
extern unsigned long sca[16], dsca[16];
extern float  rate[16];

unsigned long event=0;


void hbook_fill()
{
  float clock;
  static int cnum;

//parameters for MWDC 
  double gtdc[16], gdrft[16], drft[16], xx[4];
  double par[5]={900.0, 3.85e-4, 0.0, 0.0, 0.0};
  double chimax, chi, tch, dxx[16];
  double x0, th0, x0b, th0b;
  float xp[2], yp[2], th[2], ph[2];
  float xt, yt, xb[10], yb[10]; 

  int cid=15;//trigger Ch.
  float tcom1=0.,tcom2=0.;//trigger timing 

  float tpar[5]={1200.0,3.85e-4,0.0,0.0,0.0};
//relative offset for all plane in 'ch'
  float toff[16]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
//Z-information in cm
float leng[16]={-77.2,-76.2,-75.2,-74.2,-23.,-22.,-21.,-20.
		,+6.8,+7.8,+8.8,+9.8,+86.,+87.,+88.,+89.};//ETF 4 MWDC X&Y
//length of detectors Veto1,PL1,YX1,MU1,YX2,Veto2,tgt@6,XY3,MU2,XY4,PL2,MU-1,MU-2,--,--; in cm
float zz[15]={-95.5,-92.,-75.7,-48.,-21.5,-11.0,0.0,+8.3,+48.8,+87.5,+94.,+253.,+295.,0.0,0.0};
 
  float off_mwx[4] ={0.,4.64206,0.81633,2.282};//mm
  float coe_mwx[4] ={1.,0.0355,0.03369,0.03944};//ch to mm
  float off_mwy[4] ={0.,5.2768,4.121027,4.48838};//mms
  float coe_mwy[4] ={1.,-0.0375,-0.0342,-0.03679};//ch to mm
  float off_tof=159.3;//ns original

  float coe_tof[4] ={-0.010217,-0.0104695,0.0104408,1.};//ch to ns 201805
  float off_tof_tdc=0.;//ns
  float coe_tof_tdc[3] ={0.07065,0.07149,0.07186};//ch to ns

  float brho=6.0168;//Tm after F1 :26Na setting 11160020

//float coe_aoq[2]={0.0303,1.0476};// calibrated by 16O 11141619
  float coe_aoq[2]={0.3303,1.0476};// offset for 18O setting

  float tof=(coe_tof[0]*adc[0]+coe_tof[1]*adc[1])/2+off_tof;
  float beta=25.88/(0.3*tof);
  float gamma=1/sqrt(1-beta*beta);

  float dE_music=(adc[21]+adc[22]+adc[23]+adc[24])/4;
  float zin=sqrt(dE_music)*pow(beta,0.8)/2-0.2;//original for 10mm Be
  float aoq=coe_aoq[0]+coe_aoq[1]*(brho*0.3/(beta*gamma));
  float time=sca[15]/1000;

//adc qdc  
  for (int i=0; i<32; i++) {
    HF1(i+1,adc[i],1.);
    HF1(i+33,qdc[i],1.);
  }

//mtdc card-A **Plastic part
  for (int i=0; i<16; i++) {
    for(int j=0;j<hit[i];j++) HF1(i+65,mtdc[i][j],1.);    //mtdc1A zjw@2018
    for(int j=0;j<hit2[i];j++) HF1(i+81,mtdc2[i][j],1.);   //mtdc2A zjw@2018
  }

//mtdc card-C&D **MWDC part
  for (int i=0; i<16; i++) {
    for(int j=0;j<mhit[i];j++) HF1(i+97,ltdc[i][j],1.);    //mtdc leading edge zjw@2018
    for(int j=0;j<mhit[i];j++) HF1(i+113,wire[i][j],1.);    //mtdc wire hit zjw@2018

//    for(int j=0;j<mhitr[j];j++) HF1(i+273,rtdc[i][j],1.);   //mtdc trailing edge zjw@2018
  }

//SCA increasement 
  for (int i=0; i<16; i++) {
    HF1(i+128,dsca[i],1.);
  }


//start deduced online monitor **MWDC part
   //v1x90 offset subtraction
  if(hit[cid]>0)  tcom1=mtdc[cid][0];
  if(hit2[cid]>0)  tcom2=mtdc2[cid][0];

  for(int ii=0;ii<16;ii++)
     if(hit[ii]<10)//10 hits is max.
     for(int j=0;j<hit[ii];j++) mtdc[ii][j] -= tcom1;

  for(int ii=0;ii<8;ii++)
     if(mhit[ii]<10)//10 hits is max.
     for(int j=0;j<mhit[ii];j++){
	ltdc[ii][j] -= tcom1;
	rtdc[ii][j] -= tcom1;
	}

  for(int ii=0;ii<16;ii++)
     if(hit2[ii]<10)//10 hits is max.
     for(int j=0;j<hit2[ii];j++) mtdc2[ii][j] -= tcom2;

  for(int ii=8;ii<16;ii++)
     if(mhit[ii]<10)//10 hits is max.
     for(int j=0;j<mhit[ii];j++){
	ltdc[ii][j] -= tcom2;
	rtdc[ii][j] -= tcom2;
	}

//MWDC by zjw@2018
  int mmhit=10;//10 hits is max.
  for(int id=0;id<2;id++){//track before/after Target: DC12=0 or DC34=1
  for(int yx=0;yx<2;yx++){//plane: X=1 or Y=0
    chimax=999999.;
    x0b=-9999.;
    th0b=-9999.; 
    int off=yx*4+id*8;//offset for yx and DC_id
    if(mhit[0+off]<mmhit&&mhit[1+off]<mmhit&&mhit[2+off]<mmhit&&mhit[3+off]<mmhit){
    for(int i1=0;i1<mhit[0+off];i1++){
      for(int i2=0;i2<mhit[1+off];i2++){
        for(int i3=0;i3<mhit[2+off];i3++){
          for(int i4=0;i4<mhit[3+off];i4++){

          tch=ltdc[0+off][i1]+par[0];
          drft[0]=par[1]*tch+par[2]*tch*tch+par[3]*tch*tch*tch+par[4]*tch*tch*tch*tch;
          //printf("drft[0]:%f\n",drft[0]);

          tch=ltdc[1+off][i2]+par[0];
          drft[1]=par[1]*tch+par[2]*tch*tch+par[3]*tch*tch*tch+par[4]*tch*tch*tch*tch;
          //printf("drft[1]:%f\n",drft[1]);

          tch=ltdc[2+off][i3]+par[0];
          drft[2]=par[1]*tch+par[2]*tch*tch+par[3]*tch*tch*tch+par[4]*tch*tch*tch*tch;
          //printf("drft[2]:%f\n",drft[2]);

          tch=ltdc[3+off][i4]+par[0];
          drft[3]=par[1]*tch+par[2]*tch*tch+par[3]*tch*tch*tch+par[4]*tch*tch*tch*tch;
          //printf("drft[3]:%f\n",drft[3]);

          for(int i0=0;i0<16;i0++){
//start** for PDC-LX  !! LY-ASDcard side is -X direction, Wire_ID is reversed, so -1* is need
            if(yx==1&&id==0){
            xx[0]=-1.*(2.0*(wire[0][i1]-4.25)+((i0>>0&0x1)*2-1)*drft[0]);//LX1 :plane1 for X direction
            xx[1]=-1.*(2.0*(wire[1][i1]-4.75)-((i0>>1&0x1)*2-1)*drft[1]);//LX1':plane2 for X direction
            xx[2]=-1.*(2.0*(wire[4][i1]-4.25)+((i0>>2&0x1)*2-1)*drft[2]);//LX2 :plane3 for X direction
            xx[3]=-1.*(2.0*(wire[5][i1]-4.75)-((i0>>3&0x1)*2-1)*drft[3]);//LX2':plane4 for X direction
            }
//ender** for PDC-LX 
//start** for PDC-LY !! LX-ASDcard side is +Y direction
            if(yx==0&&id==0){
            xx[0]=2.0*(wire[0+off][i1]-4.25)+((i0>>0&0x1)*2-1)*drft[0];//LY1 :plane1 for Y direction
            xx[1]=2.0*(wire[1+off][i1]-4.75)-((i0>>1&0x1)*2-1)*drft[1];//LY1':plane2 for Y direction
            xx[2]=2.0*(wire[4+off][i1]-4.25)+((i0>>2&0x1)*2-1)*drft[2];//LY2 :plane3 for Y direction
            xx[3]=2.0*(wire[5+off][i1]-4.75)-((i0>>3&0x1)*2-1)*drft[3];//LY2':plane4 for Y direction
            }
//ender** for PDC-LY 

//start** for PDC-RX  !! RY-ASDcard side is -X direction
            if(yx==0&&id==1){
            xx[0]=2.0*(wire[0+off][i1]-4.75)+((i0>>0&0x1)*2-1)*drft[0];//LR2 :plane1 for X direction
            xx[1]=2.0*(wire[1+off][i1]-4.25)-((i0>>1&0x1)*2-1)*drft[1];//LR2':plane2 for X direction
            xx[2]=2.0*(wire[4+off][i1]-4.75)+((i0>>2&0x1)*2-1)*drft[2];//LR1 :plane3 for X direction
            xx[3]=2.0*(wire[5+off][i1]-4.25)-((i0>>3&0x1)*2-1)*drft[3];//LR1':plane4 for X direction
            }
//ender** for PDC-RX 
//start** for PDC-RY !! RX-ASDcard side is +Y direction
            if(yx==1&&id==1){
            xx[0]=-1.*(2.0*(wire[0+off][i1]-4.75)+((i0>>0&0x1)*2-1)*drft[0]);//RY2 :plane1 for Y direction
            xx[1]=-1.*(2.0*(wire[1+off][i1]-4.25)-((i0>>1&0x1)*2-1)*drft[1]);//RY2':plane2 for Y direction
            xx[2]=-1.*(2.0*(wire[4+off][i1]-4.75)+((i0>>2&0x1)*2-1)*drft[2]);//RY1 :plane3 for Y direction
            xx[3]=-1.*(2.0*(wire[5+off][i1]-4.25)-((i0>>3&0x1)*2-1)*drft[3]);//RY1':plane4 for Y direction
            }
//ender** for PDC-RY 
//solve matrix
	    TMatrixD xvec(2,1); xvec.Zero();
	    TMatrixD xmat(2,2); xmat.Zero();
	    for(Int_t ii=0;ii<4;ii++){
	      xmat(0,0) ++ ;
	      xmat(0,1) += leng[ii+off] ;
	      xmat(1,0) = xmat(0,1);
	      xmat(1,1) += leng[ii+off]*leng[ii+off] ;
	      xvec(0,0) += xx[ii];
	      xvec(1,0) += leng[ii+off]*xx[ii];
	    }
            Double_t det = xmat(0,0)*xmat(1,1)-xmat(0,1)*xmat(1,0);
	    if(fabs(det)>1e-5){
	      TMatrixD rxvec = xmat.Invert() * xvec;
	      x0 = rxvec(0,0);
	      th0 = rxvec(1,0);
	      chi = 0.0;
	      for(Int_t ii=0;ii<4;ii++)
		chi += (x0+th0*leng[ii+off]-xx[ii])
			*(x0+th0*leng[ii+off]-xx[ii]);
	      //cout<<"Best:"<<chimax<<":"<<chi<<endl;
	      if(chi<chimax&&chi>1.e-10){
		chimax=chi;
		x0b=x0;
		th0b=th0;
		//cout<<"Best:"<<xy<<":"<<chimax<<":"<<x0<<":"<<th0*180./3.1415<<endl;
                dxx[0+off]=x0b+th0b*leng[0+off]-xx[0+off];//residual
                dxx[1+off]=x0b+th0b*leng[1+off]-xx[1+off];//residual
                dxx[4+off]=x0b+th0b*leng[4+off]-xx[4+off];//residual
                dxx[5+off]=x0b+th0b*leng[5+off]-xx[5+off];//residual
		gdrft[0+off]=((i0>>0&0x1)*2-1)*drft[0];
		gdrft[1+off]=((i0>>1&0x1)*2-1)*drft[1];
		gdrft[4+off]=((i0>>2&0x1)*2-1)*drft[2];
		gdrft[5+off]=((i0>>2&0x1)*2-1)*drft[3];
		gtdc[0+off]=ltdc[0+off][i1]+tpar[0]+toff[0+off];
		gtdc[1+off]=ltdc[1+off][i2]+tpar[0]+toff[1+off];
		gtdc[4+off]=ltdc[4+off][i3]+tpar[0]+toff[4+off];
		gtdc[5+off]=ltdc[5+off][i4]+tpar[0]+toff[5+off];
              } //if(chi<chimax&&chi>1.e-10)
            }//if(fabs(det)>1e-5) 
          }//for(int i0=0;i0<16;i0++)

         }
       }
     }
   }
    if(yx==0){
       yp[id]=x0b; ph[id]=th0b;}//for Y direction before and after Target
      else{
       xp[id]=x0b; th[id]=th0b;}//for X direction before and after Target
 }//end of 10 hits max. condition
  } //end of plane X or Y 
  }//end of track before or after Target

//Beam tracking using DC1&2 before target
  for(int j=0;j<10;j++){
      xb[j]=xp[0]+zz[j]*th[0];
      yb[j]=yp[0]+zz[j]*ph[0];
  }

//Beam tracking using DC3&4 after target
  for(int j=7;j<10;j++){
      xb[j]=xp[1]+zz[j]*th[1];
      yb[j]=yp[1]+zz[j]*ph[1];
  }

//mtdc ** MWDC drift time and length
  for (int i=0; i<16; i++) {
    HF1(i+145,gtdc[i],1.);    //mtdc good TDC  zjw@2018
    HF1(i+161,gdrft[i],1.);    //mtdc drift length in cm zjw@2018
  }

  xt=xb[6]; yt=yb[6];

  HF2(581,xt*10.,th[0],1.);
  HF2(582,yt*10.,ph[0],1.);

  HF1(591,xt*10.,1.);
  HF1(592,yt*10.,1.);
  HF2(593,xt*10.,yt*10.,1.);

  HF1(601,xb[2]*10.,1.);
  HF1(602,yb[2]*10.,1.);
  HF2(607,xb[2]*10.,yb[2]*10.,1.);

  HF1(611,xb[4]*10.,1.);
  HF1(612,yb[4]*10.,1.);
  HF2(617,xb[4]*10.,yb[4]*10.,1.);

  HF1(621,xb[7]*10.,1.);
  HF1(622,yb[7]*10.,1.);
  HF2(627,xb[7]*10.,yb[7]*10.,1.);

  HF1(631,xb[9]*10.,1.);
  HF1(632,yb[9]*10.,1.);
  HF2(637,xb[9]*10.,yb[9]*10.,1.);
 
//end deduced online monitor **MWDC part



//start deduced online monitor **SCA part

  if(cnum<sca[15]/600000)//10 min
    HRESET(1000);
  cnum=sca[15]/600000;
  clock = sca[15] - cnum*600000;
  HF1(1000,clock/1000,rate[1]/100);//sec, Hz
/*
  //wang//HE MWPC1
  if(tdc[16]>0&&tdc[17]>0&&tdc[16]<4096&&tdc[17]<4096){
    HF1(601,coe_mwx[1]*(tdc[17]-tdc[16])+off_mwx[1],1.);
    HF2(605,tdc[16]-tdc[20],tdc[17]-tdc[20],1.);
  }
  if(tdc[18]>0&&tdc[19]>0&&tdc[18]<4096&&tdc[19]<4096){
    HF1(602,coe_mwy[1]*(tdc[19]-tdc[18])+off_mwy[1],1.);
    HF2(606,tdc[18]-tdc[21],tdc[19]-tdc[21],1.);
  }
  if(tdc[16]>0&&tdc[17]>0&&tdc[16]<4096&&tdc[17]<4096&&tdc[18]>0&&tdc[19]>0&&tdc[18]<4096&&tdc[19]<4096){
    HF2(607,coe_mwx[1]*(tdc[17]-tdc[16])+off_mwx[1],coe_mwy[1]*(tdc[19]-tdc[18])+off_mwy[1],1.);
  }
  if(tdc[16]>0&&tdc[17]>0&&tdc[16]<4096&&tdc[17]<4096)
    HF1(603, tdc[17]+tdc[16]-2*tdc[20],1.);
  if(tdc[19]>0&&tdc[18]>0&&tdc[19]<4096&&tdc[18]<4096)
    HF1(604, tdc[19]+tdc[18]-2*tdc[21],1.);

//HE Target
  if(tdc[16]>0&&tdc[17]>0&&tdc[16]<4096&&tdc[17]<4096&&tdc[18]>0&&tdc[19]>0&&tdc[18]<4096&&tdc[19]<4096&&tdc[22]>0&&tdc[23]>0&&tdc[22]<4096&&tdc[23]<4096&&tdc[25]>0&&tdc[24]>0&&tdc[24]<4096&&tdc[25]<4096){
    HF1(591,(-200.8*(coe_mwx[1]*(tdc[17]-tdc[16])+off_mwx[1])+709.1*(coe_mwx[2]*(tdc[23]-tdc[22])+off_mwx[2]))/508.3,1.);
    HF1(592,(-200.8*(coe_mwy[1]*(tdc[19]-tdc[18])+off_mwy[1])+709.1*(coe_mwy[2]*(tdc[25]-tdc[24])+off_mwy[2]))/508.3,1.);
    HF2(593,(-200.8*(coe_mwx[1]*(tdc[17]-tdc[16])+off_mwx[1])+709.1*(coe_mwx[2]*(tdc[23]-tdc[22])+off_mwx[2]))/508.3,(-200.8*(coe_mwy[1]*(tdc[19]-tdc[18])+off_mwy[1])+709.1*(coe_mwy[2]*(tdc[25]-tdc[24])+off_mwy[2]))/508.3,1.);
  }

//Target Anger
  if(tdc[16]>0&&tdc[17]>0&&tdc[16]<4096&&tdc[17]<4096&&tdc[18]>0&&tdc[19]>0&&tdc[18]<4096&&tdc[19]<4096&&tdc[22]>0&&tdc[23]>0&&tdc[22]<4096&&tdc[23]<4096&&tdc[25]>0&&tdc[24]>0&&tdc[24]<4096&&tdc[25]<4096){
    HF2(581,(-200.8*(coe_mwx[1]*(tdc[17]-tdc[16])+off_mwx[1])+709.1*(coe_mwx[2]*(tdc[23]-tdc[22])+off_mwx[2]))/508.3,(coe_mwx[2]*(tdc[23]-tdc[22])+off_mwx[2]-(coe_mwx[1]*(tdc[17]-tdc[16])+off_mwx[1]))*180/(540.39*3.1415926),1.);
    HF2(582,(-200.8*(coe_mwy[1]*(tdc[19]-tdc[18])+off_mwy[1])+709.1*(coe_mwy[2]*(tdc[25]-tdc[24])+off_mwy[2]))/508.3,(coe_mwy[2]*(tdc[25]-tdc[24])+off_mwy[2]-(coe_mwy[1]*(tdc[19]-tdc[18])+off_mwy[1]))*180/(540.39*3.1415926),1.);
  }

//HE TOF
  if(adc[0]>500&&adc[2]>500&&adc[0]<4096&&adc[2]<4096){
    HF1(500,tof,1.);}
//    HF1(500,(coe_tof[0]*adc[0]+coe_tof[1]*adc[2])/2+off_tof,1.);}
//  if(tdc[6]>0&&tdc[7]>0&&tdc[8]>0&&tdc[6]<4096&&adc[7]<4096&&adc[8]<4096){
//    HF1(501,(coe_tof_tdc[1]*tdc[7]+coe_tof_tdc[2]*tdc[8])/2-coe_tof_tdc[0]*tdc[6]+off_tof_tdc,1.);}
*/
//HE PID MUSIC
  if(adc[0]>500&&adc[2]>500&&adc[0]<4096&&adc[2]<4096&&adc[6]>0&&adc[7]>0&&adc[8]>0&&adc[9]>0&&adc[3]<500){
    HF2(510,tof,zin,1.);

    HF2(511,tof,(adc[6]+adc[7]+adc[8]+adc[9])/4,1.);}

  if(adc[0]>500&&adc[2]>500&&adc[0]<4096&&adc[2]<4096&&adc[6]>0&&adc[3]<500){
    HF2(512,(coe_tof[0]*adc[0]+coe_tof[1]*adc[2])/2+off_tof,adc[6],1.);}

//HE PID Si
  if(adc[0]>500&&adc[2]>500&&adc[0]<4096&&adc[2]<4096&&adc[4]>0&&adc[4]<4096&&adc[3]<500){
    HF2(520,tof,adc[4],1.);}
/*
//HE PID PL
  if(adc[0]>500&&adc[2]>500&&adc[0]<4096&&adc[2]<4096&&qdc[4]>0&&qdc[6]>0){
    HF2(530,tof,sqrt(qdc[4]*qdc[6]),1.);}
  if(adc[0]>500&&adc[2]>500&&adc[4]>0&&adc[0]<4096&&adc[2]<4096&&adc[4]<4096&&qdc[5]>0&&qdc[7]>0){
    HF2(531,tof,sqrt(qdc[5]*qdc[7]),1.);}
*/
//HE dE-dE
  if(adc[6]>0&&adc[6]<4096&&adc[7]>0&&adc[7]<4096&&adc[8]>0&&adc[8]<4096&&adc[9]>0&&adc[9]<4096&&adc[4]>0&&adc[4]<4096&&adc[31]<500){
    HF2(540,adc[4],(adc[6]+adc[7]+adc[8]+adc[9])/4,1.);}
  if(adc[10]>0&&adc[10]<4096&&adc[11]>0&&adc[11]<4096&&adc[31]<500){
    HF2(541,adc[10],adc[11],1.);}

//HE dE PL2
  if(adc[31]<500){
    HF1(5430,(adc[6]+adc[7]+adc[8]+adc[9])/4,1.);
    HF1(5431,(adc[10]+adc[11]+adc[12]+adc[13]+adc[14]+adc[15])/6,1.);
 
    //HF1(5461,sqrt(qdc[4]*qdc[6]),1.);
    //HF1(5462,sqrt(qdc[16]*qdc[18]),1.);
    }

//HE RATE
    
    HF1(490,2*time/26,1);
    HF1(491,sca[15],1);

//time dE
  if(adc[31]<500){
    HF2(5413,time/26,(adc[21]+adc[22]+adc[23]+adc[24])/4,1);
    HF2(5414,time/26,(adc[25]+adc[26]+adc[27]+adc[28]+adc[29]+adc[30])/6,1);
}

//  if(tdc[22]>0&&tdc[23]>0&&tdc[24]>0&&tdc[25]>0)
//    HF2(601, coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1]),coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1]),1.);
//  if(tdc[22]>0&&tdc[23]>0&&tdc[24]>0&&tdc[25]>0&&(tdc[5]<0 && tdc[6]<0))
//    HF2(607, coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1]),coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1]),1.);
//  if(tdc[27]>0&&tdc[28]>0)
//    HF1(509, coe_mwx[2]*(tdc[28]-tdc[27]-off_mwx[2]),1.);
//  if(tdc[29]>0&&tdc[30]>0)
//    HF1(510, coe_mwy[2]*(tdc[30]-tdc[29]-off_mwy[2]),1.);
/*
  if(tdc[27]>0&&tdc[28]>0&&tdc[29]>0&&tdc[30]>0)
    HF2(602, coe_mwx[2]*(tdc[27]-tdc[28]-off_mwx[2]),coe_mwy[2]*(tdc[29]-tdc[30]-off_mwy[2]),1.);
  // PID fill
  //if(tdc[0]>0&&tdc[1]>0&&tdc[2]>0&&tdc[3]>0&&adc[4]>103&&adc[5]>95)
    HF2(603,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,adc[5],1.);
    HF2(604,adc[4],adc[5],1.);
    HF2(605,adc[14],adc[0],1.);
  // if(adc[0]>100)//only use Si 1
    HF2(606,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,adc[14],1.);
    HF2(610,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,sqrt(qdc[1]*qdc[7]),1.);
    HF2(625,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,sqrt(qdc[5]*qdc[21]),1.);

if(tdc[17]>0&&tdc[18]>0&&tdc[22]>0&&tdc[23]>0)
    HF2(608,coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1]),(coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1])-coe_mwx[0]*(tdc[17]-tdc[18]-off_mwx[0]))/497.5,1.);
if(tdc[19]>0&&tdc[20]>0&&tdc[24]>0&&tdc[25]>0)
    HF2(609,coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1]),(coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1])-coe_mwy[0]*(tdc[19]-tdc[20]-off_mwy[0]))/497.5,1.);


    HF2(611,adc[13],adc[14],1.);
    HF2(612,sqrt(adc[16]*adc[17]),sqrt(adc[18]*adc[19]),1.);
    HF2(613,sqrt(adc[16]*adc[17]),adc[0],1.);
*/
//end** deduced online monitor
/*
   TCutG *cutg01 = new TCutG("CUTG01",9); // PID cut in TOF-dE_PL1
   cutg01->SetVarX("PID befor");
   cutg01->SetVarY("");
   cutg01->SetTitle("Graph");
   cutg01->SetFillColor(1);
   cutg01->SetPoint(0,126.0776,1556.962);
   cutg01->SetPoint(1,126.1674,1672.996);
   cutg01->SetPoint(2,126.5086,1757.384);
   cutg01->SetPoint(3,126.8678,1715.19);
   cutg01->SetPoint(4,127.0115,1630.802);
   cutg01->SetPoint(5,126.9397,1525.316);
   cutg01->SetPoint(6,126.5266,1419.831);
   cutg01->SetPoint(7,126.0776,1504.219);
   cutg01->SetPoint(8,126.0776,1556.962);

   TCutG *cutg02 = new TCutG("CUTG02",10);
   cutg02->SetVarX("PID2 BeforTarget With PID cut");
   cutg02->SetVarY("");
   cutg02->SetTitle("Graph");
   cutg02->SetFillColor(1);
   cutg02->SetMarkerColor(2);
   cutg02->SetMarkerStyle(4);
   cutg02->SetPoint(0,125.9578,1512.917);
   cutg02->SetPoint(1,125.5844,1464.478);
   cutg02->SetPoint(2,125.4696,1362.217);
   cutg02->SetPoint(3,125.6132,1254.575);
   cutg02->SetPoint(4,126.0727,1184.607);
   cutg02->SetPoint(5,126.4173,1195.371);
   cutg02->SetPoint(6,126.7045,1313.778);
   cutg02->SetPoint(7,126.604,1448.332);
   cutg02->SetPoint(8,126.2737,1512.917);
   cutg02->SetPoint(9,125.9578,1512.917);

   TCutG *cutg03 = new TCutG("CUTG03",9);
   cutg03->SetVarX("PID2 BeforTarget");
   cutg03->SetVarY("");
   cutg03->SetTitle("Graph");
   cutg03->SetFillColor(1);
   cutg03->SetPoint(0,125.6132,1422.443);
   cutg03->SetPoint(1,125.9003,1550.643);
   cutg03->SetPoint(2,126.4029,1621.131);
   cutg03->SetPoint(3,127.0635,1571.051);
   cutg03->SetPoint(4,127.3219,1433.646);
   cutg03->SetPoint(5,127.0204,1248.107);
   cutg03->SetPoint(6,126.0439,1212.717);
   cutg03->SetPoint(7,125.5988,1322.015);
   cutg03->SetPoint(8,125.6132,1422.443);


   TCutG *cutg04 = new TCutG("CUTG04",11);
   cutg04->SetVarX("PID2 BeforTarget");
   cutg04->SetVarY("");
   cutg04->SetTitle("Graph");
   cutg04->SetFillColor(1);
   cutg04->SetPoint(0,125.5126,1400.498);
   cutg04->SetPoint(1,125.8285,1549.751);
   cutg04->SetPoint(2,126.3599,1619.403);
   cutg04->SetPoint(3,126.8624,1529.851);
   cutg04->SetPoint(4,127.0348,1370.647);
   cutg04->SetPoint(5,126.6901,1231.343);
   cutg04->SetPoint(6,125.8429,1171.642);
   cutg04->SetPoint(7,125.4983,1178.608);
   cutg04->SetPoint(8,125.3547,1301.433);
   cutg04->SetPoint(9,125.369,1362.845);
   cutg04->SetPoint(10,125.5126,1400.498);

   TCutG *cutg05 = new TCutG("CUTG05",10);
   cutg05->SetVarX("PID2 BeforTarget");
   cutg05->SetVarY("");
   cutg05->SetTitle("Graph");
   cutg05->SetFillColor(1);
   cutg05->SetPoint(0,125.1249,1629.347);
   cutg05->SetPoint(1,125.2685,1752.65);
   cutg05->SetPoint(2,125.6993,1819.376);
   cutg05->SetPoint(3,126.2881,1790.473);
   cutg05->SetPoint(4,126.5465,1670.443);
   cutg05->SetPoint(5,126.4173,1528.254);
   cutg05->SetPoint(6,125.9291,1425.802);
   cutg05->SetPoint(7,125.2685,1433.433);
   cutg05->SetPoint(8,125.0675,1506.654);
   cutg05->SetPoint(9,125.1249,1629.347);

   TCutG *cutg06 = new TCutG("CUTG06",9);
   cutg06->SetVarX("PID2 BeforTarget");
   cutg06->SetVarY("");
   cutg06->SetTitle("Graph");
   cutg06->SetFillColor(1);
   cutg06->SetPoint(0,125.8142,1662.892);
   cutg06->SetPoint(1,126.2019,1762.17);
   cutg06->SetPoint(2,126.7188,1769.807);
   cutg06->SetPoint(3,126.9342,1670.529);
   cutg06->SetPoint(4,126.7763,1517.792);
   cutg06->SetPoint(5,126.1875,1468.153);
   cutg06->SetPoint(6,125.7711,1544.521);
   cutg06->SetPoint(7,125.7998,1597.979);
   cutg06->SetPoint(8,125.8142,1662.892);


   if(tdc[5]<0 && tdc[6]<0 && cutg06->IsInside(((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,sqrt(qdc[1]*qdc[7]))){
//
//   if(tdc[5]<0 && tdc[6]<0 ){

     //Before target
     HF2(614,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,adc[5],1.);
     HF2(615,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,adc[14],1.);
     HF2(616,((tdc[1]+tdc[2])/2.-tdc[0]+400.)*0.0672+113.67,sqrt(qdc[1]*qdc[7]),1.);

     if(tdc[17]>0&&tdc[18]>0&&tdc[19]>0&&tdc[20]>0)
         HF2(617, coe_mwx[0]*(tdc[17]-tdc[18]-off_mwx[0]),coe_mwy[0]*(tdc[19]-tdc[20]-off_mwy[0]),1.);
     if(tdc[22]>0&&tdc[23]>0&&tdc[24]>0&&tdc[25]>0)
         HF2(618, coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1]),coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1]),1.);
     if(tdc[27]>0&&tdc[28]>0&&tdc[29]>0&&tdc[30]>0)
         HF2(619, coe_mwx[2]*(tdc[27]-tdc[28]-off_mwx[2]),coe_mwy[2]*(tdc[29]-tdc[30]-off_mwy[2]),1.);

     if(tdc[17]>0&&tdc[18]>0&&tdc[22]>0&&tdc[23]>0)
       HF2(620,coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1]),(coe_mwx[1]*(tdc[22]-tdc[23]-off_mwx[1])-coe_mwx[0]*(tdc[17]-tdc[18]-off_mwx[0]))/497.5,1.);
     if(tdc[19]>0&&tdc[20]>0&&tdc[24]>0&&tdc[25]>0)
       HF2(621,coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1]),(coe_mwy[1]*(tdc[24]-tdc[25]-off_mwy[1])-coe_mwy[0]*(tdc[19]-tdc[20]-off_mwy[0]))/497.5,1.);

     // After target
     HF2(622,adc[0],adc[1],1.);
     HF2(623,sqrt(adc[16]*adc[17]),sqrt(adc[18]*adc[19]),1.);
     HF2(624,sqrt(adc[16]*adc[17]),adc[0],1.);

     


   }*/
  //  HF2(610,qdc[7],qdc[9],1.);

  //  if(adc[8]>123&&adc[9]>113&&adc[10]>122&&adc[11]>103&&adc[12]>113&&adc[13]>96&&adc[0]>112&&adc[1]>112&&adc[2]>103&&adc[3]>123)
  //    HF2(512,(adc[0]+adc[1]+adc[2]+adc[3])/4.,(adc[8]+adc[9]+adc[10]+adc[11]+adc[12]+adc[13])/6.,1.);
  //  if(adc[8]>123&&adc[9]>113&&adc[10]>122&&adc[11]>103&&adc[12]>113&&adc[13]>96&&adc[16]>149&&adc[17]>122&&adc[18]>115&&adc[19]>117)
  //    HF2(513,(adc[16]+adc[17]+adc[18]+adc[19])/4.,(adc[8]+adc[9]+adc[10]+adc[11]+adc[12]+adc[13])/6.,1.);
  //  if(adc[8]>123&&adc[9]>113&&adc[10]>122&&adc[11]>103&&adc[12]>113&&adc[13]>96)
  //   {
  //   for(int i=0;i<6;i++)
  //     {
  //     if(adc[i+20]>160){
  //    HF2(514,adc[i+20],(adc[8]+adc[9]+adc[10]+adc[11]+adc[12]+adc[13])/6.,1.);
  //      break;
  //    }
  //     }
  //   }
}
