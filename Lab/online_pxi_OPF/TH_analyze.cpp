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
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
using namespace std;


   struct data_section_head{
          int index;
          int map_index;
          int data_type:8;
          int fragment_num:24;
          }; //cout<<sizeof(data_section_head)<<endl;
   struct data_fragment_head{
          int start_channel;
          int channel_num:30;
          unsigned int channel_full:1;
          unsigned int length_change:1;
          int data_len;
          }; //cout<<sizeof(data_fragment_head)<<endl;
   struct data_validity_table{
          int valid_channel_num;
          int channel_id[0];
          };//cout<<sizeof(data_validity_table)<<endl; //size is 4 bytes.
   struct channel_offset_table{
          int offset[0];
          };//cout<<sizeof(channel_offset_table)<<endl; // size is 0 byte.

         int event_num=0;
         int event_num1=0;
         int fired_ch_per_event=0;     
         int start_num=0;

         int Is_Error_event=0;
         int Is_vacancy_event=0;


         int fragment_num;
         int data_len_per_frag, len_per_event, data_len_per_event;
         int valid_channel_num;

         data_section_head     section_head;
         data_fragment_head    *fragment_head=NULL;
         data_validity_table   *p_data_validity_table=NULL;
         channel_offset_table  *p_channel_offset_table=NULL ;

         int *data_frag = NULL;
         int *data_ch   = NULL;


         const int ch_num_limit=4500;
         const int edge_num_limit=20;

         int data_h_leading[ch_num_limit][edge_num_limit],   edge_num_h_leading[ch_num_limit];
         int data_h_trailing[ch_num_limit][edge_num_limit],  edge_num_h_trailing[ch_num_limit];
         int data_v_leading[ch_num_limit][edge_num_limit],   edge_num_v_leading[ch_num_limit];
         int data_v_trailing[ch_num_limit][edge_num_limit],  edge_num_v_trailing[ch_num_limit];


         float data_h_leading_f[ch_num_limit][edge_num_limit];
         float data_h_trailing_f[ch_num_limit][edge_num_limit];
         float data_v_leading_f[ch_num_limit][edge_num_limit];
         float data_v_trailing_f[ch_num_limit][edge_num_limit];

         int  edge_num_total[ch_num_limit];

         int eventID[100],bunchID[100];

         float timetotrig_h[ch_num_limit],timetotrig_v[ch_num_limit];
          //,timetotrig_v1[ch_num_limit],timetotrig_v2[ch_num_limit];

         int fired_ch_num=0;
         int fired_ch[ch_num_limit];

         float P1_X=-1000., P1_Y=-1000., P2_X=-1000., P2_Y=-1000., P3_X=-1000., P3_Y=-1000.; 
         float Q1_X=-1000., Q1_Y=-1000., Q2_X=-1000., Q2_Y=-1000., Q3_X=-1000., Q3_Y=-1000.; 

         float T_SC0 = -100.; // It is time of flight from SC0 to SC1

         int SC0up_edge_num_total=0;
         int SC0dn_edge_num_total=0;

         int SC0up_edge_num_v_leading=0;
         int SC0dn_edge_num_v_leading=0;

         int SC0up_edge_num_v_trailing=0;
         int SC0dn_edge_num_v_trailing=0;


         int SCup_edge_num_total=0;
         int SCdn_edge_num_total=0;

         int SCup_edge_num_v_leading=0;
         int SCdn_edge_num_v_leading=0;

         int SCup_edge_num_v_trailing=0;
         int SCdn_edge_num_v_trailing=0;



         float SC0up_data_v_leading_f[edge_num_limit];
         float SC0dn_data_v_leading_f[edge_num_limit];

         float SC0up_data_v_trailing_f[edge_num_limit];
         float SC0dn_data_v_trailing_f[edge_num_limit];


         float SCup_data_v_leading_f[edge_num_limit];
         float SCdn_data_v_leading_f[edge_num_limit];

         float SCup_data_v_trailing_f[edge_num_limit];
         float SCdn_data_v_trailing_f[edge_num_limit];




         int TOFWup_fired_ch_num=0, TOFWdn_fired_ch_num=0;
         int MWDC1X1_fired_ch_num=0, MWDC1X2_fired_ch_num=0;
         int MWDC2X1_fired_ch_num=0, MWDC2X2_fired_ch_num=0;
         int MWDC3X1_fired_ch_num=0, MWDC3X2_fired_ch_num=0;
         int PPAC1X1_fired_ch_num=0, PPAC1X2_fired_ch_num=0;
         int PPAC2X1_fired_ch_num=0, PPAC2X2_fired_ch_num=0;
         int PPAC3X1_fired_ch_num=0, PPAC3X2_fired_ch_num=0;
         int PPAC1Y1_fired_ch_num=0, PPAC1Y2_fired_ch_num=0;
         int PPAC2Y1_fired_ch_num=0, PPAC2Y2_fired_ch_num=0;
         int PPAC3Y1_fired_ch_num=0, PPAC3Y2_fired_ch_num=0;
         int QQAC1X1_fired_ch_num=0, QQAC1X2_fired_ch_num=0;
         int QQAC2X1_fired_ch_num=0, QQAC2X2_fired_ch_num=0;
         int QQAC3X1_fired_ch_num=0, QQAC3X2_fired_ch_num=0;
         int QQAC1Y1_fired_ch_num=0, QQAC1Y2_fired_ch_num=0;
         int QQAC2Y1_fired_ch_num=0, QQAC2Y2_fired_ch_num=0;
         int QQAC3Y1_fired_ch_num=0, QQAC3Y2_fired_ch_num=0;



         int TOFWup_fired_ch[31], TOFWdn_fired_ch[31];
         int MWDC1X1_fired_ch[81], MWDC1X2_fired_ch[81];
         int MWDC2X1_fired_ch[81], MWDC2X2_fired_ch[81];
         int MWDC3X1_fired_ch[81], MWDC3X2_fired_ch[81];
         int PPAC1X1_fired_ch[17], PPAC1X2_fired_ch[17];
         int PPAC2X1_fired_ch[17], PPAC2X2_fired_ch[17];
         int PPAC3X1_fired_ch[17], PPAC3X2_fired_ch[17];
         int PPAC1Y1_fired_ch[17], PPAC1Y2_fired_ch[17];
         int PPAC2Y1_fired_ch[17], PPAC2Y2_fired_ch[17];
         int PPAC3Y1_fired_ch[17], PPAC3Y2_fired_ch[17];

         int QQAC1X1_fired_ch[17], QQAC1X2_fired_ch[17];
         int QQAC2X1_fired_ch[17], QQAC2X2_fired_ch[17];
         int QQAC3X1_fired_ch[17], QQAC3X2_fired_ch[17];
         int QQAC1Y1_fired_ch[17], QQAC1Y2_fired_ch[17];
         int QQAC2Y1_fired_ch[17], QQAC2Y2_fired_ch[17];
         int QQAC3Y1_fired_ch[17], QQAC3Y2_fired_ch[17];

         int TOFWup_edge_num_total[31];
         int TOFWup_edge_num_h_leading[31];
         int TOFWup_edge_num_h_trailing[31];
         int TOFWup_edge_num_v_leading[31];
         int TOFWup_edge_num_v_trailing[31];

         int TOFWdn_edge_num_total[31];
         int TOFWdn_edge_num_h_leading[31];
         int TOFWdn_edge_num_h_trailing[31];
         int TOFWdn_edge_num_v_leading[31];
         int TOFWdn_edge_num_v_trailing[31];

         int MWDC1X1_edge_num_total[81];
         int MWDC1X1_edge_num_h_leading[81];
         int MWDC1X1_edge_num_h_trailing[81];
         int MWDC1X2_edge_num_total[81];
         int MWDC1X2_edge_num_h_leading[81];
         int MWDC1X2_edge_num_h_trailing[81];

         int MWDC2X1_edge_num_total[81];
         int MWDC2X1_edge_num_h_leading[81];
         int MWDC2X1_edge_num_h_trailing[81];
         int MWDC2X2_edge_num_total[81];
         int MWDC2X2_edge_num_h_leading[81];
         int MWDC2X2_edge_num_h_trailing[81];

         int MWDC3X1_edge_num_total[81];
         int MWDC3X1_edge_num_h_leading[81];
         int MWDC3X1_edge_num_h_trailing[81];
         int MWDC3X2_edge_num_total[81];
         int MWDC3X2_edge_num_h_leading[81];
         int MWDC3X2_edge_num_h_trailing[81];


         int PPAC1X1_edge_num_total[17];
         int PPAC1X1_edge_num_h_leading[17];
         int PPAC1X1_edge_num_h_trailing[17];
         int PPAC1X2_edge_num_total[17];
         int PPAC1X2_edge_num_h_leading[17];
         int PPAC1X2_edge_num_h_trailing[17];

         int PPAC2X1_edge_num_total[17];
         int PPAC2X1_edge_num_h_leading[17];
         int PPAC2X1_edge_num_h_trailing[17];
         int PPAC2X2_edge_num_total[17];
         int PPAC2X2_edge_num_h_leading[17];
         int PPAC2X2_edge_num_h_trailing[17];

         int PPAC3X1_edge_num_total[17];
         int PPAC3X1_edge_num_h_leading[17];
         int PPAC3X1_edge_num_h_trailing[17];
         int PPAC3X2_edge_num_total[17];
         int PPAC3X2_edge_num_h_leading[17];
         int PPAC3X2_edge_num_h_trailing[17];

         int PPAC1Y1_edge_num_total[17];
         int PPAC1Y1_edge_num_h_leading[17];
         int PPAC1Y1_edge_num_h_trailing[17];
         int PPAC1Y2_edge_num_total[17];
         int PPAC1Y2_edge_num_h_leading[17];
         int PPAC1Y2_edge_num_h_trailing[17];

         int PPAC2Y1_edge_num_total[17];
         int PPAC2Y1_edge_num_h_leading[17];
         int PPAC2Y1_edge_num_h_trailing[17];
         int PPAC2Y2_edge_num_total[17];
         int PPAC2Y2_edge_num_h_leading[17];
         int PPAC2Y2_edge_num_h_trailing[17];

         int PPAC3Y1_edge_num_total[17];
         int PPAC3Y1_edge_num_h_leading[17];
         int PPAC3Y1_edge_num_h_trailing[17];
         int PPAC3Y2_edge_num_total[17];
         int PPAC3Y2_edge_num_h_leading[17];
         int PPAC3Y2_edge_num_h_trailing[17];


         int QQAC1X1_edge_num_total[17];
         int QQAC1X1_edge_num_h_leading[17];
         int QQAC1X1_edge_num_h_trailing[17];
         int QQAC1X2_edge_num_total[17];
         int QQAC1X2_edge_num_h_leading[17];
         int QQAC1X2_edge_num_h_trailing[17];

         int QQAC2X1_edge_num_total[17];
         int QQAC2X1_edge_num_h_leading[17];
         int QQAC2X1_edge_num_h_trailing[17];
         int QQAC2X2_edge_num_total[17];
         int QQAC2X2_edge_num_h_leading[17];
         int QQAC2X2_edge_num_h_trailing[17];

         int QQAC3X1_edge_num_total[17];
         int QQAC3X1_edge_num_h_leading[17];
         int QQAC3X1_edge_num_h_trailing[17];
         int QQAC3X2_edge_num_total[17];
         int QQAC3X2_edge_num_h_leading[17];
         int QQAC3X2_edge_num_h_trailing[17];

         int QQAC1Y1_edge_num_total[17];
         int QQAC1Y1_edge_num_h_leading[17];
         int QQAC1Y1_edge_num_h_trailing[17];
         int QQAC1Y2_edge_num_total[17];
         int QQAC1Y2_edge_num_h_leading[17];
         int QQAC1Y2_edge_num_h_trailing[17];

         int QQAC2Y1_edge_num_total[17];
         int QQAC2Y1_edge_num_h_leading[17];
         int QQAC2Y1_edge_num_h_trailing[17];
         int QQAC2Y2_edge_num_total[17];
         int QQAC2Y2_edge_num_h_leading[17];
         int QQAC2Y2_edge_num_h_trailing[17];

         int QQAC3Y1_edge_num_total[17];
         int QQAC3Y1_edge_num_h_leading[17];
         int QQAC3Y1_edge_num_h_trailing[17];
         int QQAC3Y2_edge_num_total[17];
         int QQAC3Y2_edge_num_h_leading[17];
         int QQAC3Y2_edge_num_h_trailing[17];


         float TOFWup_data_h_leading_f[31][edge_num_limit];
         float TOFWup_data_h_trailing_f[31][edge_num_limit];
         float TOFWdn_data_h_leading_f[31][edge_num_limit];
         float TOFWdn_data_h_trailing_f[31][edge_num_limit];
         float TOFWup_data_v_leading_f[31][edge_num_limit];
         float TOFWup_data_v_trailing_f[31][edge_num_limit];
         float TOFWdn_data_v_leading_f[31][edge_num_limit];
         float TOFWdn_data_v_trailing_f[31][edge_num_limit];

         float MWDC1X1_data_h_leading_f[81][edge_num_limit];
         float MWDC1X1_data_h_trailing_f[81][edge_num_limit];
         float MWDC1X2_data_h_leading_f[81][edge_num_limit];
         float MWDC1X2_data_h_trailing_f[81][edge_num_limit];

         float MWDC2X1_data_h_leading_f[81][edge_num_limit];
         float MWDC2X1_data_h_trailing_f[81][edge_num_limit];
         float MWDC2X2_data_h_leading_f[81][edge_num_limit];
         float MWDC2X2_data_h_trailing_f[81][edge_num_limit];

         float MWDC3X1_data_h_leading_f[81][edge_num_limit];
         float MWDC3X1_data_h_trailing_f[81][edge_num_limit];
         float MWDC3X2_data_h_leading_f[81][edge_num_limit];
         float MWDC3X2_data_h_trailing_f[81][edge_num_limit];

         float PPAC1X1_data_h_leading_f[17][edge_num_limit];
         float PPAC1X1_data_h_trailing_f[17][edge_num_limit];
         float PPAC1X2_data_h_leading_f[17][edge_num_limit];
         float PPAC1X2_data_h_trailing_f[17][edge_num_limit];

         float PPAC2X1_data_h_leading_f[17][edge_num_limit];
         float PPAC2X1_data_h_trailing_f[17][edge_num_limit];
         float PPAC2X2_data_h_leading_f[17][edge_num_limit];
         float PPAC2X2_data_h_trailing_f[17][edge_num_limit];

         float PPAC3X1_data_h_leading_f[17][edge_num_limit];
         float PPAC3X1_data_h_trailing_f[17][edge_num_limit];
         float PPAC3X2_data_h_leading_f[17][edge_num_limit];
         float PPAC3X2_data_h_trailing_f[17][edge_num_limit];

         float PPAC1Y1_data_h_leading_f[17][edge_num_limit];
         float PPAC1Y1_data_h_trailing_f[17][edge_num_limit];
         float PPAC1Y2_data_h_leading_f[17][edge_num_limit];
         float PPAC1Y2_data_h_trailing_f[17][edge_num_limit];

         float PPAC2Y1_data_h_leading_f[17][edge_num_limit];
         float PPAC2Y1_data_h_trailing_f[17][edge_num_limit];
         float PPAC2Y2_data_h_leading_f[17][edge_num_limit];
         float PPAC2Y2_data_h_trailing_f[17][edge_num_limit];

         float PPAC3Y1_data_h_leading_f[17][edge_num_limit];
         float PPAC3Y1_data_h_trailing_f[17][edge_num_limit];
         float PPAC3Y2_data_h_leading_f[17][edge_num_limit];
         float PPAC3Y2_data_h_trailing_f[17][edge_num_limit];


         float QQAC1X1_data_h_leading_f[17][edge_num_limit];
         float QQAC1X1_data_h_trailing_f[17][edge_num_limit];
         float QQAC1X2_data_h_leading_f[17][edge_num_limit];
         float QQAC1X2_data_h_trailing_f[17][edge_num_limit];

         float QQAC2X1_data_h_leading_f[17][edge_num_limit];
         float QQAC2X1_data_h_trailing_f[17][edge_num_limit];
         float QQAC2X2_data_h_leading_f[17][edge_num_limit];
         float QQAC2X2_data_h_trailing_f[17][edge_num_limit];

         float QQAC3X1_data_h_leading_f[17][edge_num_limit];
         float QQAC3X1_data_h_trailing_f[17][edge_num_limit];
         float QQAC3X2_data_h_leading_f[17][edge_num_limit];
         float QQAC3X2_data_h_trailing_f[17][edge_num_limit];

         float QQAC1Y1_data_h_leading_f[17][edge_num_limit];
         float QQAC1Y1_data_h_trailing_f[17][edge_num_limit];
         float QQAC1Y2_data_h_leading_f[17][edge_num_limit];
         float QQAC1Y2_data_h_trailing_f[17][edge_num_limit];

         float QQAC2Y1_data_h_leading_f[17][edge_num_limit];
         float QQAC2Y1_data_h_trailing_f[17][edge_num_limit];
         float QQAC2Y2_data_h_leading_f[17][edge_num_limit];
         float QQAC2Y2_data_h_trailing_f[17][edge_num_limit];

         float QQAC3Y1_data_h_leading_f[17][edge_num_limit];
         float QQAC3Y1_data_h_trailing_f[17][edge_num_limit];
         float QQAC3Y2_data_h_leading_f[17][edge_num_limit];
         float QQAC3Y2_data_h_trailing_f[17][edge_num_limit];


		// Cheng Zehui's Detector - optic fiber: OPF
         int OPFup_fired_ch_num=0, OPFdn_fired_ch_num=0;

         int OPFup_fired_ch[41], OPFdn_fired_ch[41];
         int OPFup_edge_num_total[41];
         int OPFup_edge_num_h_leading[41];
         int OPFup_edge_num_h_trailing[41];
         int OPFup_edge_num_v_leading[41];
         int OPFup_edge_num_v_trailing[41];

         int OPFdn_edge_num_total[41];
         int OPFdn_edge_num_h_leading[41];
         int OPFdn_edge_num_h_trailing[41];
         int OPFdn_edge_num_v_leading[41];
         int OPFdn_edge_num_v_trailing[41];

         float OPFup_data_h_leading_f[41][edge_num_limit];
         float OPFup_data_h_trailing_f[41][edge_num_limit];
         float OPFdn_data_h_leading_f[41][edge_num_limit];
         float OPFdn_data_h_trailing_f[41][edge_num_limit];
         float OPFup_data_v_leading_f[41][edge_num_limit];
         float OPFup_data_v_trailing_f[41][edge_num_limit];
         float OPFdn_data_v_leading_f[41][edge_num_limit];
         float OPFdn_data_v_trailing_f[41][edge_num_limit];
////////////////////////////////////////////////////////////



  void init_variable()
    {
         for(int i=0;i<ch_num_limit;i++) fired_ch[i]=0;

        for(int i=0;i<ch_num_limit;i++)
        for(int j=0;j<edge_num_limit;j++)
        {  data_h_leading[i][j]=-100;data_h_trailing[i][j]=-100;
           data_v_leading[i][j]=-100;data_v_trailing[i][j]=-100;
           data_h_leading_f[i][j]=-100;data_h_trailing_f[i][j]=-100;
           data_v_leading_f[i][j]=-100;data_v_trailing_f[i][j]=-100;
         }

        for(int j=0;j<edge_num_limit;j++)
        {  

          SC0up_data_v_leading_f[j]=-100;
          SC0dn_data_v_leading_f[j]=-100;

          SC0up_data_v_trailing_f[j]=-100;
          SC0dn_data_v_trailing_f[j]=-100;


          SCup_data_v_leading_f[j]=-100;
          SCdn_data_v_leading_f[j]=-100;

          SCup_data_v_trailing_f[j]=-100;
          SCdn_data_v_trailing_f[j]=-100;
         }  




for(int i=0;i<31;i++) {TOFWup_fired_ch[i]=0;TOFWdn_fired_ch[i]=0;}
for(int i=0;i<41;i++) {OPFup_fired_ch[i]=0;OPFdn_fired_ch[i]=0;}
for(int i=0;i<81;i++) {MWDC1X1_fired_ch[i]=0;MWDC1X2_fired_ch[i]=0;}
for(int i=0;i<81;i++) {MWDC2X1_fired_ch[i]=0;MWDC2X2_fired_ch[i]=0;}
for(int i=0;i<81;i++) {MWDC3X1_fired_ch[i]=0;MWDC3X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {PPAC1X1_fired_ch[i]=0;PPAC1X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {PPAC2X1_fired_ch[i]=0;PPAC2X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {PPAC3X1_fired_ch[i]=0;PPAC3X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {PPAC1Y1_fired_ch[i]=0;PPAC1Y2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {PPAC2Y1_fired_ch[i]=0;PPAC2Y2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {PPAC3Y1_fired_ch[i]=0;PPAC3Y2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {QQAC1X1_fired_ch[i]=0;QQAC1X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {QQAC2X1_fired_ch[i]=0;QQAC2X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {QQAC3X1_fired_ch[i]=0;QQAC3X2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {QQAC1Y1_fired_ch[i]=0;QQAC1Y2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {QQAC2Y1_fired_ch[i]=0;QQAC2Y2_fired_ch[i]=0;}
for(int i=0;i<17;i++) {QQAC3Y1_fired_ch[i]=0;QQAC3Y2_fired_ch[i]=0;}

        for(int i=0;i<31;i++)
        {TOFWup_edge_num_h_leading[i]=0;
         TOFWup_edge_num_h_trailing[i]=0;
         TOFWup_edge_num_v_leading[i]=0;
         TOFWup_edge_num_v_trailing[i]=0; 
         TOFWup_edge_num_total[i]=0;

         TOFWdn_edge_num_h_leading[i]=0;
         TOFWdn_edge_num_h_trailing[i]=0;
         TOFWdn_edge_num_v_leading[i]=0;
         TOFWdn_edge_num_v_trailing[i]=0; 
         TOFWdn_edge_num_total[i]=0;

         }
        for(int i=0;i<41;i++)
        {OPFup_edge_num_h_leading[i]=0;
         OPFup_edge_num_h_trailing[i]=0;
         OPFup_edge_num_v_leading[i]=0;
         OPFup_edge_num_v_trailing[i]=0; 
         OPFup_edge_num_total[i]=0;

         OPFdn_edge_num_h_leading[i]=0;
         OPFdn_edge_num_h_trailing[i]=0;
         OPFdn_edge_num_v_leading[i]=0;
         OPFdn_edge_num_v_trailing[i]=0; 
         OPFdn_edge_num_total[i]=0;

         }


        for(int i=0;i<81;i++)
        {MWDC1X1_edge_num_h_leading[i]=0;
         MWDC1X1_edge_num_h_trailing[i]=0;
         MWDC1X1_edge_num_total[i]=0;
         MWDC1X2_edge_num_h_leading[i]=0;
         MWDC1X2_edge_num_h_trailing[i]=0;
         MWDC1X2_edge_num_total[i]=0;

         MWDC2X1_edge_num_h_leading[i]=0;
         MWDC2X1_edge_num_h_trailing[i]=0;
         MWDC2X1_edge_num_total[i]=0;
         MWDC2X2_edge_num_h_leading[i]=0;
         MWDC2X2_edge_num_h_trailing[i]=0;
         MWDC2X2_edge_num_total[i]=0;

         MWDC3X1_edge_num_h_leading[i]=0;
         MWDC3X1_edge_num_h_trailing[i]=0;
         MWDC3X1_edge_num_total[i]=0;
         MWDC3X2_edge_num_h_leading[i]=0;
         MWDC3X2_edge_num_h_trailing[i]=0;
         MWDC3X2_edge_num_total[i]=0;
         }
        for(int i=0;i<17;i++)
        {
         PPAC1X1_edge_num_h_leading[i]=0;
         PPAC1X1_edge_num_h_trailing[i]=0;
         PPAC1X1_edge_num_total[i]=0;
         PPAC1X2_edge_num_h_leading[i]=0;
         PPAC1X2_edge_num_h_trailing[i]=0;
         PPAC1X2_edge_num_total[i]=0;

         PPAC2X1_edge_num_h_leading[i]=0;
         PPAC2X1_edge_num_h_trailing[i]=0;
         PPAC2X1_edge_num_total[i]=0;
         PPAC2X2_edge_num_h_leading[i]=0;
         PPAC2X2_edge_num_h_trailing[i]=0;
         PPAC2X2_edge_num_total[i]=0;

         PPAC3X1_edge_num_h_leading[i]=0;
         PPAC3X1_edge_num_h_trailing[i]=0;
         PPAC3X1_edge_num_total[i]=0;
         PPAC3X2_edge_num_h_leading[i]=0;
         PPAC3X2_edge_num_h_trailing[i]=0;
         PPAC3X2_edge_num_total[i]=0;
       }
        for(int i=0;i<17;i++)
        {
         PPAC1Y1_edge_num_h_leading[i]=0;
         PPAC1Y1_edge_num_h_trailing[i]=0;
         PPAC1Y1_edge_num_total[i]=0;
         PPAC1Y2_edge_num_h_leading[i]=0;
         PPAC1Y2_edge_num_h_trailing[i]=0;
         PPAC1Y2_edge_num_total[i]=0;

         PPAC2Y1_edge_num_h_leading[i]=0;
         PPAC2Y1_edge_num_h_trailing[i]=0;
         PPAC2Y1_edge_num_total[i]=0;
         PPAC2Y2_edge_num_h_leading[i]=0;
         PPAC2Y2_edge_num_h_trailing[i]=0;
         PPAC2Y2_edge_num_total[i]=0;

         PPAC3Y1_edge_num_h_leading[i]=0;
         PPAC3Y1_edge_num_h_trailing[i]=0;
         PPAC3Y1_edge_num_total[i]=0;
         PPAC3Y2_edge_num_h_leading[i]=0;
         PPAC3Y2_edge_num_h_trailing[i]=0;
         PPAC3Y2_edge_num_total[i]=0;
         }
        for(int i=0;i<17;i++)
        {
         QQAC1X1_edge_num_h_leading[i]=0;
         QQAC1X1_edge_num_h_trailing[i]=0;
         QQAC1X1_edge_num_total[i]=0;
         QQAC1X2_edge_num_h_leading[i]=0;
         QQAC1X2_edge_num_h_trailing[i]=0;
         QQAC1X2_edge_num_total[i]=0;

         QQAC2X1_edge_num_h_leading[i]=0;
         QQAC2X1_edge_num_h_trailing[i]=0;
         QQAC2X1_edge_num_total[i]=0;
         QQAC2X2_edge_num_h_leading[i]=0;
         QQAC2X2_edge_num_h_trailing[i]=0;
         QQAC2X2_edge_num_total[i]=0;

         QQAC3X1_edge_num_h_leading[i]=0;
         QQAC3X1_edge_num_h_trailing[i]=0;
         QQAC3X1_edge_num_total[i]=0;
         QQAC3X2_edge_num_h_leading[i]=0;
         QQAC3X2_edge_num_h_trailing[i]=0;
         QQAC3X2_edge_num_total[i]=0;
        }
        for(int i=0;i<17;i++)
        {
         QQAC1Y1_edge_num_h_leading[i]=0;
         QQAC1Y1_edge_num_h_trailing[i]=0;
         QQAC1Y1_edge_num_total[i]=0;
         QQAC1Y2_edge_num_h_leading[i]=0;
         QQAC1Y2_edge_num_h_trailing[i]=0;
         QQAC1Y2_edge_num_total[i]=0;

         QQAC2Y1_edge_num_h_leading[i]=0;
         QQAC2Y1_edge_num_h_trailing[i]=0;
         QQAC2Y1_edge_num_total[i]=0;
         QQAC2Y2_edge_num_h_leading[i]=0;
         QQAC2Y2_edge_num_h_trailing[i]=0;
         QQAC2Y2_edge_num_total[i]=0;

         QQAC3Y1_edge_num_h_leading[i]=0;
         QQAC3Y1_edge_num_h_trailing[i]=0;
         QQAC3Y1_edge_num_total[i]=0;
         QQAC3Y2_edge_num_h_leading[i]=0;
         QQAC3Y2_edge_num_h_trailing[i]=0;
         QQAC3Y2_edge_num_total[i]=0;
         }
        for(int i=0;i<31;i++)for(int j=0;j<edge_num_limit;j++)
        {  
           TOFWup_data_h_leading_f[i][j]=-100;TOFWup_data_h_trailing_f[i][j]=-100;
           TOFWup_data_v_leading_f[i][j]=-100;TOFWup_data_v_trailing_f[i][j]=-100;
           TOFWdn_data_h_leading_f[i][j]=-100;TOFWdn_data_h_trailing_f[i][j]=-100;
           TOFWdn_data_v_leading_f[i][j]=-100;TOFWdn_data_v_trailing_f[i][j]=-100;
         } 
        for(int i=0;i<41;i++)for(int j=0;j<edge_num_limit;j++)
        {  
           OPFup_data_h_leading_f[i][j]=-100;OPFup_data_h_trailing_f[i][j]=-100;
           OPFup_data_v_leading_f[i][j]=-100;OPFup_data_v_trailing_f[i][j]=-100;
           OPFdn_data_h_leading_f[i][j]=-100;OPFdn_data_h_trailing_f[i][j]=-100;
           OPFdn_data_v_leading_f[i][j]=-100;OPFdn_data_v_trailing_f[i][j]=-100;
         } 
        for(int i=0;i<81;i++)for(int j=0;j<edge_num_limit;j++)
        {  
           MWDC1X1_data_h_leading_f[i][j]=-100;MWDC1X1_data_h_trailing_f[i][j]=-100;
           MWDC1X2_data_h_leading_f[i][j]=-100;MWDC1X2_data_h_trailing_f[i][j]=-100;
           MWDC2X1_data_h_leading_f[i][j]=-100;MWDC2X1_data_h_trailing_f[i][j]=-100;
           MWDC2X2_data_h_leading_f[i][j]=-100;MWDC2X2_data_h_trailing_f[i][j]=-100;
           MWDC3X1_data_h_leading_f[i][j]=-100;MWDC3X1_data_h_trailing_f[i][j]=-100;
           MWDC3X2_data_h_leading_f[i][j]=-100;MWDC3X2_data_h_trailing_f[i][j]=-100;
         }          
        for(int i=0;i<17;i++)for(int j=0;j<edge_num_limit;j++)
        {  
           PPAC1X1_data_h_leading_f[i][j]=-100;PPAC1X1_data_h_trailing_f[i][j]=-100;
           PPAC1X2_data_h_leading_f[i][j]=-100;PPAC1X2_data_h_trailing_f[i][j]=-100;
           PPAC2X1_data_h_leading_f[i][j]=-100;PPAC2X1_data_h_trailing_f[i][j]=-100;
           PPAC2X2_data_h_leading_f[i][j]=-100;PPAC2X2_data_h_trailing_f[i][j]=-100;
           PPAC3X1_data_h_leading_f[i][j]=-100;PPAC3X1_data_h_trailing_f[i][j]=-100;
           PPAC3X2_data_h_leading_f[i][j]=-100;PPAC3X2_data_h_trailing_f[i][j]=-100;
         }    
        for(int i=0;i<17;i++)for(int j=0;j<edge_num_limit;j++)
        {  

           PPAC1Y1_data_h_leading_f[i][j]=-100;PPAC1Y1_data_h_trailing_f[i][j]=-100;
           PPAC1Y2_data_h_leading_f[i][j]=-100;PPAC1Y2_data_h_trailing_f[i][j]=-100;
           PPAC2Y1_data_h_leading_f[i][j]=-100;PPAC2Y1_data_h_trailing_f[i][j]=-100;
           PPAC2Y2_data_h_leading_f[i][j]=-100;PPAC2Y2_data_h_trailing_f[i][j]=-100;
           PPAC3Y1_data_h_leading_f[i][j]=-100;PPAC3Y1_data_h_trailing_f[i][j]=-100;
           PPAC3Y2_data_h_leading_f[i][j]=-100;PPAC3Y2_data_h_trailing_f[i][j]=-100;
         }    

        for(int i=0;i<17;i++)for(int j=0;j<edge_num_limit;j++)
        {  
           QQAC1X1_data_h_leading_f[i][j]=-100;QQAC1X1_data_h_trailing_f[i][j]=-100;
           QQAC1X2_data_h_leading_f[i][j]=-100;QQAC1X2_data_h_trailing_f[i][j]=-100;
           QQAC2X1_data_h_leading_f[i][j]=-100;QQAC2X1_data_h_trailing_f[i][j]=-100;
           QQAC2X2_data_h_leading_f[i][j]=-100;QQAC2X2_data_h_trailing_f[i][j]=-100;
           QQAC3X1_data_h_leading_f[i][j]=-100;QQAC3X1_data_h_trailing_f[i][j]=-100;
           QQAC3X2_data_h_leading_f[i][j]=-100;QQAC3X2_data_h_trailing_f[i][j]=-100;
         }    
        for(int i=0;i<17;i++)for(int j=0;j<edge_num_limit;j++)
        {  

           QQAC1Y1_data_h_leading_f[i][j]=-100;QQAC1Y1_data_h_trailing_f[i][j]=-100;
           QQAC1Y2_data_h_leading_f[i][j]=-100;QQAC1Y2_data_h_trailing_f[i][j]=-100;
           QQAC2Y1_data_h_leading_f[i][j]=-100;QQAC2Y1_data_h_trailing_f[i][j]=-100;
           QQAC2Y2_data_h_leading_f[i][j]=-100;QQAC2Y2_data_h_trailing_f[i][j]=-100;
           QQAC3Y1_data_h_leading_f[i][j]=-100;QQAC3Y1_data_h_trailing_f[i][j]=-100;
           QQAC3Y2_data_h_leading_f[i][j]=-100;QQAC3Y2_data_h_trailing_f[i][j]=-100;
         }    



     }    

////////////////////////////////////////////////////////////

void analyze( int* one_event)
{
#define ana
#ifdef ana

    

        T_SC0 = -100.;


        int pos_of_addr=0;

       for(int i=0;i<fired_ch_num;i++)
       {
          edge_num_h_leading[fired_ch[i]]=0;edge_num_h_trailing[fired_ch[i]]=0;
          edge_num_v_leading[fired_ch[i]]=0;edge_num_v_trailing[fired_ch[i]]=0;
          edge_num_total[fired_ch[i]]=0;
          timetotrig_h[fired_ch[i]]=-10000.;timetotrig_v[fired_ch[i]]=-10000.;
       }


        for(int i=0;i<50;i++)
       {
         eventID[i]=-1;bunchID[i]=-1;
 
       }

          SCup_edge_num_total=0;
          SCdn_edge_num_total=0;

          SCup_edge_num_v_leading=0;
          SCdn_edge_num_v_leading=0;

          SCup_edge_num_v_trailing=0;
          SCdn_edge_num_v_trailing=0;

          SC0up_edge_num_total=0;
          SC0dn_edge_num_total=0;

          SC0up_edge_num_v_leading=0;
          SC0dn_edge_num_v_leading=0;

          SC0up_edge_num_v_trailing=0;
          SC0dn_edge_num_v_trailing=0;


        TOFWup_fired_ch_num=0;        TOFWdn_fired_ch_num=0;
        OPFup_fired_ch_num=0;         OPFdn_fired_ch_num=0;
        MWDC1X1_fired_ch_num=0;       MWDC1X2_fired_ch_num=0;
        MWDC2X1_fired_ch_num=0;       MWDC2X2_fired_ch_num=0;
        MWDC3X1_fired_ch_num=0;       MWDC3X2_fired_ch_num=0;

        PPAC1X1_fired_ch_num=0;       PPAC1X2_fired_ch_num=0;
        PPAC2X1_fired_ch_num=0;       PPAC2X2_fired_ch_num=0;
        PPAC3X1_fired_ch_num=0;       PPAC3X2_fired_ch_num=0;

        PPAC1Y1_fired_ch_num=0;       PPAC1Y2_fired_ch_num=0;
        PPAC2Y1_fired_ch_num=0;       PPAC2Y2_fired_ch_num=0;
        PPAC3Y1_fired_ch_num=0;       PPAC3Y2_fired_ch_num=0;

        QQAC1X1_fired_ch_num=0;       QQAC1X2_fired_ch_num=0;
        QQAC2X1_fired_ch_num=0;       QQAC2X2_fired_ch_num=0;
        QQAC3X1_fired_ch_num=0;       QQAC3X2_fired_ch_num=0;

        QQAC1Y1_fired_ch_num=0;       QQAC1Y2_fired_ch_num=0;
        QQAC2Y1_fired_ch_num=0;       QQAC2Y2_fired_ch_num=0;
        QQAC3Y1_fired_ch_num=0;       QQAC3Y2_fired_ch_num=0;

     fired_ch_per_event=0;
     fired_ch_num=0;
     len_per_event=0;
     data_len_per_event=0;

        Is_Error_event=0; 
        Is_vacancy_event=0; 

        P1_X=-1000.; P1_Y=-1000.; P2_X=-1000.; P2_Y=-1000.; P3_X=-1000.; P3_Y=-1000.; 
        Q1_X=-1000.; Q1_Y=-1000.; Q2_X=-1000.; Q2_Y=-1000.; Q3_X=-1000.; Q3_Y=-1000.; 

///////////////////////////////////////////////////

        memcpy(&len_per_event,&one_event[pos_of_addr],sizeof(int));
        pos_of_addr += sizeof(int)/4;

        //if(fread(&section_head,sizeof(data_section_head),1, fp)<=0) {break; }
        memcpy(&section_head,&one_event[pos_of_addr],sizeof(data_section_head));
        pos_of_addr += sizeof(data_section_head)/4;
        fragment_num = section_head.fragment_num;
        event_num++;
        event_num1=section_head.index+1;

         HF1(1,section_head.index%1000000,1.);

        if(fragment_num>0)
        if((fragment_head=(data_fragment_head*)malloc(fragment_num*sizeof(data_fragment_head)))==NULL)
          {cout<<"malloc error!"<<endl;} 

        if(fragment_num>0)
        //if(fread(fragment_head,sizeof(data_fragment_head),fragment_num, fp)<=0) break;
        memcpy(fragment_head,&one_event[pos_of_addr],sizeof(data_fragment_head)*fragment_num);
        pos_of_addr+=sizeof(data_fragment_head)*fragment_num/4;

        if(fragment_num>0)if(fragment_head[0].data_len==0)
       { //cout<<endl<<"This event has no data."<<" Fragment num: "<<fragment_num<<endl;

         Is_vacancy_event=1;
       }

        for(int i=0;i<fragment_num;i++) if(Is_vacancy_event!=1)// i is fragment ID
    {


       // read data_validity_table
       if(fragment_head[i].channel_full==0)   //not full
       {
        //if(fread(&valid_channel_num,sizeof(int),1, fp)<=0) break;
        memcpy(&valid_channel_num,&one_event[pos_of_addr],sizeof(int));
        pos_of_addr+=sizeof(int)/4; //cout<<"atention "<<valid_channel_num<<endl;pause();
        p_data_validity_table = (data_validity_table*)malloc(sizeof(data_validity_table)
                                 + valid_channel_num*sizeof(int)) ;
        p_data_validity_table->valid_channel_num = valid_channel_num;
        for(int j=0;j<valid_channel_num;j++)
        //if(fread(&p_data_validity_table->channel_id[j],sizeof(int),1,fp) <=0)break;
        { memcpy(&p_data_validity_table->channel_id[j],&one_event[pos_of_addr],sizeof(int));
          pos_of_addr+=sizeof(int)/4; 
        }  
       }
      else                                 // it is full
       { cout<<"It is full!"<<endl;
        valid_channel_num=fragment_head[i].channel_num;
        p_data_validity_table = (data_validity_table*)malloc(sizeof(data_validity_table)
                                + valid_channel_num*sizeof(int)) ;
        p_data_validity_table->valid_channel_num = valid_channel_num;
        for(int j=0;j<valid_channel_num;j++)  p_data_validity_table->channel_id[j]=fragment_head[i].start_channel+j;
       }


        // extract data_len_per_frag
      if(fragment_head[i].channel_full==0 && fragment_head[i].length_change==1) // 1,1
       { data_len_per_frag=fragment_head[i].data_len-1*4-valid_channel_num*4*2;}
      else if (fragment_head[i].channel_full==1 && fragment_head[i].length_change==0)// 0,0
       { data_len_per_frag=fragment_head[i].data_len;  }
      else if (fragment_head[i].channel_full==0 && fragment_head[i].length_change==0)//1,0
       { data_len_per_frag=fragment_head[i].data_len-1*4-valid_channel_num*4;}
      else                                                                           //0,1
       { data_len_per_frag=fragment_head[i].data_len-valid_channel_num*4;}
        

        // read channel_offset_table
      if(fragment_head[i].length_change==1) // The length is changed
       {
        p_channel_offset_table =(channel_offset_table*)malloc(sizeof(channel_offset_table) 
                                + valid_channel_num*sizeof(int));
        for(int j=0;j<valid_channel_num;j++) 
        //if(fread(&p_channel_offset_table->offset[j],sizeof(int),1,fp) <=0)break;
        { memcpy(&p_channel_offset_table->offset[j],&one_event[pos_of_addr],sizeof(int));
          pos_of_addr+=sizeof(int)/4;
        }
       }
      else                                 //  no change
       { cout<<"It is not changed!"<<endl;
        p_channel_offset_table =(channel_offset_table*)malloc(sizeof(channel_offset_table)
                               + valid_channel_num*sizeof(int));
        for(int j=0;j<valid_channel_num;j++)p_channel_offset_table->offset[j]=0+data_len_per_frag/valid_channel_num*j;
       }

        // read fragment data --- data_frag
        data_frag=(int*)malloc(sizeof(int)+data_len_per_frag); 
       //if(fread(data_frag,sizeof(int),data_len_per_frag/4,fp) <=0)break;
        memcpy(data_frag,&one_event[pos_of_addr],sizeof(int)*data_len_per_frag/4);
        pos_of_addr+=sizeof(int)*data_len_per_frag/4/4;


     data_len_per_event+=data_len_per_frag/4;

     if(fragment_head[i].channel_num == 129) fired_ch_per_event +=  valid_channel_num-1;
     if(fragment_head[i].channel_num == 33 ) fired_ch_per_event +=  (valid_channel_num-1)/2;


        for(int j=0;j<valid_channel_num;j++) 
        {   
           #include "process_one_channel.cpp"
        } //for(int j=0;j<valid_channel_num;j++)

       int edge_num1=0;
       for(int j=2;j<=32;j=j+2)
       edge_num1+=edge_num_total[j];

         HF1(4,edge_num1,1.);



      free(data_frag);
      free(p_data_validity_table);
      free(p_channel_offset_table);

     }//for(int i=0;i<fragment_num;i++)

     if(fragment_num>0) free(fragment_head);


#endif
	
}


#include "TH_init.cpp"
#include "TH_fill.cpp"
