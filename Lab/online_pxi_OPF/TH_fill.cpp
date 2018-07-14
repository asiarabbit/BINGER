
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
// Set Global Parameters for analyze
//- For Histogram ------------------------------------------------------


void hbook_fill()
{

if(Is_vacancy_event!=1) if(Is_Error_event!=1)
{

         HF1(3,len_per_event,1.);

for(int i=0;i<fired_ch_num;i++)
    {

          if(edge_num_h_leading[fired_ch[i]]>=1 && edge_num_h_trailing[fired_ch[i]]>=1)
         //if(edge_num_total[fired_ch[i]]==2)
         HF2(10,fired_ch[i],data_h_trailing[fired_ch[i]][0]*0.09765625-data_h_leading[fired_ch[i]][0]*0.09765625,1.);
          if(edge_num_v_leading[fired_ch[i]]>=1 && edge_num_v_trailing[fired_ch[i]]>=1)
         //if(edge_num_total[fired_ch[i]]==2)
         HF2(11,fired_ch[i],data_v_trailing[fired_ch[i]][0]*0.024414063-data_v_leading[fired_ch[i]][0]*0.024414063,1.);


         if(edge_num_h_leading[fired_ch[i]]>=1)
         { timetotrig_h[fired_ch[i]]=data_h_leading_f[fired_ch[i]][0] -(bunchID[0]- (bunchID[0] & 0x800) )*25.;
           if(timetotrig_h[fired_ch[i]]<0.) timetotrig_h[fired_ch[i]]+=51200.;  
         }
         if(edge_num_v_leading[fired_ch[i]]>=1)
         { timetotrig_v[fired_ch[i]]=data_v_leading_f[fired_ch[i]][0] -(bunchID[0]- (bunchID[0] & 0x800) )*25.;
           if(timetotrig_v[fired_ch[i]]<0.) timetotrig_v[fired_ch[i]]+=51200.;
         }


          if(edge_num_h_leading[fired_ch[i]]>=1)
            HF2(12,fired_ch[i],timetotrig_h[fired_ch[i]],1.);
          if(edge_num_v_leading[fired_ch[i]]>=1)
            HF2(13,fired_ch[i],timetotrig_v[fired_ch[i]],1.);

    } //for(int i=0;i<fired_ch_num;i++)

         HF1(20,fired_ch_per_event,1.);
         HF1(21,data_len_per_event,1.);


if(PPAC1X2_fired_ch_num==1 && PPAC1Y2_fired_ch_num==1)
{
P1_X = 0.5*PPAC1X2_fired_ch[0]-8.5*0.5;
P1_Y = 0.5*PPAC1Y2_fired_ch[0]-8.5*0.5;
//HF2(22,P1_X,P1_Y,1.);
}
if(PPAC2X2_fired_ch_num==1 && PPAC2Y2_fired_ch_num==1)
{
P2_X = 0.5*PPAC2X2_fired_ch[0]-8.5*0.5;
P2_Y = 0.5*PPAC2Y2_fired_ch[0]-8.5*0.5;
//HF2(23,P2_X,P2_Y,1.);
}
if(PPAC3X2_fired_ch_num==1 && PPAC3Y2_fired_ch_num==1)
{
P3_X = 0.5*PPAC3X2_fired_ch[0]-8.5*0.5;
P3_Y = 0.5*PPAC3Y2_fired_ch[0]-8.5*0.5;
//HF2(24,P3_X,P3_Y,1.);
}

if(QQAC1X2_fired_ch_num==1 && QQAC1Y2_fired_ch_num==1)
{
Q1_X = 0.5*QQAC1X2_fired_ch[0]-8.5*0.5;
Q1_Y = 0.5*QQAC1Y2_fired_ch[0]-8.5*0.5;
//HF2(25,Q1_X,Q1_Y,1.);
}
if(QQAC2X2_fired_ch_num==1 && QQAC2Y2_fired_ch_num==1)
{
Q2_X = 0.5*QQAC2X2_fired_ch[0]-8.5*0.5;
Q2_Y = 0.5*QQAC2Y2_fired_ch[0]-8.5*0.5;
//HF2(26,Q2_X,Q2_Y,1.);
}
if(QQAC3X2_fired_ch_num==1 && QQAC3Y2_fired_ch_num==1)
{
Q3_X = 0.5*QQAC3X2_fired_ch[0]-8.5*0.5;
Q3_Y = 0.5*QQAC3Y2_fired_ch[0]-8.5*0.5;
//HF2(27,Q3_X,Q3_Y,1.);
}

    if(SC0up_edge_num_v_leading>=1)
    if(SC0dn_edge_num_v_leading>=1)
   {
     float SC0dn_minus_up=SC0dn_data_v_leading_f[0] -SC0up_data_v_leading_f[0];
     HF1(28,SC0dn_minus_up,1.);
   }

    if(SCup_edge_num_v_leading>=1)
    if(SCdn_edge_num_v_leading>=1)
   {
     float SCdn_minus_up=SCdn_data_v_leading_f[0] -SCup_data_v_leading_f[0];
     HF1(29,SCdn_minus_up,1.);
   }

    if(SC0up_edge_num_v_leading>=1)
    if(SC0dn_edge_num_v_leading>=1)
    if(SCup_edge_num_v_leading>=1)
    if(SCdn_edge_num_v_leading>=1)
   {
     float T_ref_SC0 = 0.5*(SC0dn_data_v_leading_f[0]+SC0up_data_v_leading_f[0]);
     float T_ref_SC = 0.5*(SCdn_data_v_leading_f[0]+SCup_data_v_leading_f[0]);
     T_SC0 = T_ref_SC - T_ref_SC0+130.;
     HF1(30, T_SC0, 1);
   }

    if(SCup_edge_num_v_leading>=1)
    if(SCdn_edge_num_v_leading>=1)
    for(int iii=0;iii<40;iii++){
      if(OPFup_edge_num_v_leading[iii]>=1)
      if(OPFdn_edge_num_v_leading[iii]>=1)
      {
        float T_ref_SC = 0.5*(SCdn_data_v_leading_f[0]+SCup_data_v_leading_f[0]);
        float T_OPF = 0.5*(OPFdn_data_v_leading_f[iii][0]+OPFup_data_v_leading_f[iii][0]);
        HF2(701, iii, T_OPF-T_ref_SC, 1.);
      }
   }
   
	for(int iii=0;iii<40;iii++){
      if(OPFup_edge_num_v_leading[iii]>=1)
      if(OPFdn_edge_num_v_leading[iii]>=1)
      {
		float dt = OPFup_data_v_leading_f[iii][0] - OPFdn_data_v_leading_f[iii][0];
        HF2(702, iii, dt, 1.); // h702->Fill(iii, dt):
      }
   }







for(int i=0;i<TOFWup_fired_ch_num;i++)            HF1(101,TOFWup_fired_ch[i],1.);
for(int i=0;i<TOFWdn_fired_ch_num;i++)            HF1(102,TOFWdn_fired_ch[i],1.);

for(int i=0;i<OPFup_fired_ch_num;i++)             HF1(601,OPFup_fired_ch[i],1.);
for(int i=0;i<OPFdn_fired_ch_num;i++)             HF1(602,OPFdn_fired_ch[i],1.);

for(int i=0;i<MWDC1X1_fired_ch_num;i++)           HF1(103,MWDC1X1_fired_ch[i],1.);
for(int i=0;i<MWDC1X2_fired_ch_num;i++)           HF1(104,MWDC1X2_fired_ch[i],1.);

for(int i=0;i<MWDC2X1_fired_ch_num;i++)           HF1(105,MWDC2X1_fired_ch[i],1.);
for(int i=0;i<MWDC2X2_fired_ch_num;i++)           HF1(106,MWDC2X2_fired_ch[i],1.);

for(int i=0;i<MWDC3X1_fired_ch_num;i++)           HF1(107,MWDC3X1_fired_ch[i],1.);
for(int i=0;i<MWDC3X2_fired_ch_num;i++)           HF1(108,MWDC3X2_fired_ch[i],1.);
/*
for(int i=0;i<PPAC1X1_fired_ch_num;i++)           HF1(113,PPAC1X1_fired_ch[i],1.);
for(int i=0;i<PPAC1X2_fired_ch_num;i++)           HF1(114,PPAC1X2_fired_ch[i],1.);

//for(int i=0;i<PPAC2X1_fired_ch_num;i++)           HF1(115,PPAC2X1_fired_ch[i],1.);
//for(int i=0;i<PPAC2X2_fired_ch_num;i++)           HF1(116,PPAC2X2_fired_ch[i],1.);

for(int i=0;i<PPAC3X1_fired_ch_num;i++)           HF1(117,PPAC3X1_fired_ch[i],1.);
for(int i=0;i<PPAC3X2_fired_ch_num;i++)           HF1(118,PPAC3X2_fired_ch[i],1.);

for(int i=0;i<PPAC1Y1_fired_ch_num;i++)           HF1(123,PPAC1Y1_fired_ch[i],1.);
for(int i=0;i<PPAC1Y2_fired_ch_num;i++)           HF1(124,PPAC1Y2_fired_ch[i],1.);

//for(int i=0;i<PPAC2Y1_fired_ch_num;i++)           HF1(125,PPAC2Y1_fired_ch[i],1.);
//for(int i=0;i<PPAC2Y2_fired_ch_num;i++)           HF1(126,PPAC2Y2_fired_ch[i],1.);

for(int i=0;i<PPAC3Y1_fired_ch_num;i++)           HF1(127,PPAC3Y1_fired_ch[i],1.);
for(int i=0;i<PPAC3Y2_fired_ch_num;i++)           HF1(128,PPAC3Y2_fired_ch[i],1.);


for(int i=0;i<QQAC1X1_fired_ch_num;i++)           HF1(133,QQAC1X1_fired_ch[i],1.);
for(int i=0;i<QQAC1X2_fired_ch_num;i++)           HF1(134,QQAC1X2_fired_ch[i],1.);

//for(int i=0;i<QQAC2X1_fired_ch_num;i++)           HF1(135,QQAC2X1_fired_ch[i],1.);
//for(int i=0;i<QQAC2X2_fired_ch_num;i++)           HF1(136,QQAC2X2_fired_ch[i],1.);

for(int i=0;i<QQAC3X1_fired_ch_num;i++)           HF1(137,QQAC3X1_fired_ch[i],1.);
for(int i=0;i<QQAC3X2_fired_ch_num;i++)           HF1(138,QQAC3X2_fired_ch[i],1.);

for(int i=0;i<QQAC1Y1_fired_ch_num;i++)           HF1(143,QQAC1Y1_fired_ch[i],1.);
for(int i=0;i<QQAC1Y2_fired_ch_num;i++)           HF1(144,QQAC1Y2_fired_ch[i],1.);

//for(int i=0;i<QQAC2Y1_fired_ch_num;i++)           HF1(145,QQAC2Y1_fired_ch[i],1.);
//for(int i=0;i<QQAC2Y2_fired_ch_num;i++)           HF1(146,QQAC2Y2_fired_ch[i],1.);

for(int i=0;i<QQAC3Y1_fired_ch_num;i++)           HF1(147,QQAC3Y1_fired_ch[i],1.);
for(int i=0;i<QQAC3Y2_fired_ch_num;i++)           HF1(148,QQAC3Y2_fired_ch[i],1.);

*/
         HF1(201,TOFWup_fired_ch_num,1.);
         HF1(202,TOFWdn_fired_ch_num,1.);
         HF1(501,OPFup_fired_ch_num,1.);
         HF1(502,OPFdn_fired_ch_num,1.);
         HF1(203,MWDC1X1_fired_ch_num,1.);
         HF1(204,MWDC1X2_fired_ch_num,1.);
         HF1(205,MWDC2X1_fired_ch_num,1.);
         HF1(206,MWDC2X2_fired_ch_num,1.);
         HF1(207,MWDC3X1_fired_ch_num,1.);
         HF1(208,MWDC3X2_fired_ch_num,1.);
/*
         HF1(213,PPAC1X1_fired_ch_num,1.);
         HF1(214,PPAC1X2_fired_ch_num,1.);
      //   HF1(215,PPAC2X1_fired_ch_num,1.);
     //    HF1(216,PPAC2X2_fired_ch_num,1.);
         HF1(217,PPAC3X1_fired_ch_num,1.);
         HF1(218,PPAC3X2_fired_ch_num,1.);

         HF1(223,PPAC1Y1_fired_ch_num,1.);
         HF1(224,PPAC1Y2_fired_ch_num,1.);
      //   HF1(225,PPAC2Y1_fired_ch_num,1.);
      //   HF1(226,PPAC2Y2_fired_ch_num,1.);
         HF1(227,PPAC3Y1_fired_ch_num,1.);
         HF1(228,PPAC3Y2_fired_ch_num,1.);


         HF1(233,QQAC1X1_fired_ch_num,1.);
         HF1(234,QQAC1X2_fired_ch_num,1.);
    //     HF1(235,QQAC2X1_fired_ch_num,1.);
    //     HF1(236,QQAC2X2_fired_ch_num,1.);
         HF1(237,QQAC3X1_fired_ch_num,1.);
         HF1(238,QQAC3X2_fired_ch_num,1.);

         HF1(243,QQAC1Y1_fired_ch_num,1.);
         HF1(244,QQAC1Y2_fired_ch_num,1.);
    //     HF1(245,QQAC2Y1_fired_ch_num,1.);
    //     HF1(246,QQAC2Y2_fired_ch_num,1.);
         HF1(247,QQAC3Y1_fired_ch_num,1.);
         HF1(248,QQAC3Y2_fired_ch_num,1.);
*/
   } //if(Is_vacancy_event!=1) if(Is_Error_event!=1)
}


