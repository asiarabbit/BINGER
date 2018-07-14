 // /*
///////////////////// SC Board
           if(ch_id==2805)
           {
                 SC0up_edge_num_total=edge_num_total_t; 
                 SC0up_edge_num_v_leading=edge_num_v_leading_t;
                 SC0up_edge_num_v_trailing=edge_num_v_trailing_t; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 SC0up_data_v_leading_f[k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 SC0up_data_v_trailing_f[k]=data_v_trailing_f[ch_id][k];
           }

           if(ch_id==2805)
           {
                 SC0dn_edge_num_total=edge_num_total_t;
                 SC0dn_edge_num_v_leading=edge_num_v_leading_t;
                 SC0dn_edge_num_v_trailing=edge_num_v_trailing_t;
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 SC0dn_data_v_leading_f[k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 SC0dn_data_v_trailing_f[k]=data_v_trailing_f[ch_id][k];
           }




           if(ch_id==2801)
           {
                 SCup_edge_num_total=edge_num_total_t;
                 SCup_edge_num_v_leading=edge_num_v_leading_t;
                 SCup_edge_num_v_trailing=edge_num_v_trailing_t;
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 SCup_data_v_leading_f[k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 SCup_data_v_trailing_f[k]=data_v_trailing_f[ch_id][k];
           }

           else if(ch_id==2803)
           {
                 SCdn_edge_num_total=edge_num_total_t;
                 SCdn_edge_num_v_leading=edge_num_v_leading_t;
                 SCdn_edge_num_v_trailing=edge_num_v_trailing_t;
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 SCdn_data_v_leading_f[k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 SCdn_data_v_trailing_f[k]=data_v_trailing_f[ch_id][k];
           }
//  */  




///////////////////// TOFW
          if(ch_id>=1&&ch_id<=32)
          // if(ch_id>=1&&ch_id<=16)
           {int ch_id1=ch_id-0;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+1;
                 TOFWup_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWup_fired_ch[TOFWup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+1;
                 TOFWdn_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWdn_fired_ch[TOFWdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
///////////////////// TOFW

          else  if(ch_id>=101&&ch_id<=132)
          // if(ch_id>=101&&ch_id<=116)
           {int ch_id1=ch_id-100;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+1+8;
                 TOFWup_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWup_fired_ch[TOFWup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+1+8;
                 TOFWdn_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWdn_fired_ch[TOFWdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
///////////////////// TOFW
          else  if(ch_id>=201&&ch_id<=232)         //  if(ch_id>=201&&ch_id<=216)
           {int ch_id1=ch_id-200;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+1+8+8;
                 TOFWup_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWup_fired_ch[TOFWup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+1+8+8;
                 TOFWdn_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWdn_fired_ch[TOFWdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
///////////////////// TOFW
          else  if(ch_id>=301&&ch_id<=324) // if(0)    //     if(ch_id>=301&&ch_id<=316)
           {int ch_id1=ch_id-300;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+1+8+8+8;
                 TOFWup_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWup_fired_ch[TOFWup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+1+8+8+8;
                 TOFWdn_edge_num_total[ch_id2]=edge_num_total_t;
                 TOFWdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 TOFWdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 TOFWdn_fired_ch[TOFWdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 TOFWdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 TOFWdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
//////////////////////////////////////////// TOFW ////////////////////
// Cheng's Detector - optic fiber //
          if(ch_id>=3101&&ch_id<=3132)
          // if(ch_id>=1&&ch_id<=16)
           {int ch_id1=ch_id-3100;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+1;
                 OPFup_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFup_fired_ch[OPFup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+1;
                 OPFdn_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFdn_fired_ch[OPFdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
////////////////////////////// OPF
          else if(ch_id>=3201&&ch_id<=3232)
          // if(ch_id>=1&&ch_id<=16)
           {int ch_id1=ch_id-3200;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+8;
                 OPFup_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFup_fired_ch[OPFup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+8;
                 OPFdn_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFdn_fired_ch[OPFdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
////////////////////////////// OPF
          else if(ch_id>=3301&&ch_id<3332)
          // if(ch_id>=1&&ch_id<=16)
           {int ch_id1=ch_id-3300;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+16;
                 OPFup_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFup_fired_ch[OPFup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+16;
                 OPFdn_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFdn_fired_ch[OPFdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
////////////////////////////// OPF
          else if(ch_id>=3401&&ch_id<=3432)
          // if(ch_id>=1&&ch_id<=16)
           {int ch_id1=ch_id-3400;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+24;
                 OPFup_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFup_fired_ch[OPFup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+24;
                 OPFdn_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFdn_fired_ch[OPFdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
////////////////////////////// OPF
          else if(ch_id>=3501&&ch_id<=3532)
          // if(ch_id>=1&&ch_id<=16)
           {int ch_id1=ch_id-3500;
            if(ch_id1%4==1)
              {  int ch_id2=ch_id1/4+32;
                 OPFup_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFup_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFup_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFup_fired_ch[OPFup_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFup_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFup_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }
            else if(ch_id1%4==3)
              {  int ch_id2=ch_id1/4+32;
                 OPFdn_edge_num_total[ch_id2]=edge_num_total_t;
                 OPFdn_edge_num_v_leading[ch_id2]=edge_num_v_leading_t;
                 OPFdn_edge_num_v_trailing[ch_id2]=edge_num_v_trailing_t;
                 OPFdn_fired_ch[OPFdn_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_v_leading_t;k++)
                 OPFdn_data_v_leading_f[ch_id2][k]=data_v_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_v_trailing_t;k++)
                 OPFdn_data_v_trailing_f[ch_id2][k]=data_v_trailing_f[ch_id][k];
              }

           }
////////////////////////////// OPF
////////////////////////////////////////// cheng's optic fiber

////////////////MWDC1X is from 401
          else  if(ch_id>=401&&ch_id<=528)
           { int ch_id1=ch_id-400;
             if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=33-ch_id1;
                 MWDC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X1_fired_ch[MWDC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=49&&ch_id1<=64)
               { int ch_id2=81-ch_id1;
                 MWDC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X1_fired_ch[MWDC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=81&&ch_id1<=96)
               { int ch_id2=129-ch_id1;
                 MWDC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X1_fired_ch[MWDC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=113&&ch_id1<=128)
               { int ch_id2=177-ch_id1;
                 MWDC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X1_fired_ch[MWDC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2=17-ch_id1;
                 MWDC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X2_fired_ch[MWDC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=33&&ch_id1<=48)
               { int ch_id2=65-ch_id1;
                 MWDC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X2_fired_ch[MWDC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=65&&ch_id1<=80)
               { int ch_id2=113-ch_id1;
                 MWDC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X2_fired_ch[MWDC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=97&&ch_id1<=112)
               { int ch_id2=161-ch_id1;
                 MWDC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X2_fired_ch[MWDC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }

//////////// MWDC2X is from 1201
          else  if(ch_id>=1201&&ch_id<=1328)
           { int ch_id1=ch_id-1200;
             if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=33-ch_id1;
                 MWDC2X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X1_fired_ch[MWDC2X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=49&&ch_id1<=64)
               { int ch_id2=81-ch_id1;
                 MWDC2X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X1_fired_ch[MWDC2X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=81&&ch_id1<=96)
               { int ch_id2=129-ch_id1;
                 MWDC2X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X1_fired_ch[MWDC2X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=113&&ch_id1<=128)
               { int ch_id2=177-ch_id1;
                 MWDC2X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X1_fired_ch[MWDC2X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2=17-ch_id1;
                 MWDC2X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X2_fired_ch[MWDC2X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=33&&ch_id1<=48)
               { int ch_id2=65-ch_id1;
                 MWDC2X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X2_fired_ch[MWDC2X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=65&&ch_id1<=80)
               { int ch_id2=113-ch_id1;
                 MWDC2X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X2_fired_ch[MWDC2X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=97&&ch_id1<=112)
               { int ch_id2=161-ch_id1;
                 MWDC2X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X2_fired_ch[MWDC2X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }

////////////MWDC3X is from 1801
          else  if(ch_id>=1801&&ch_id<=1928) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
           { int ch_id1=ch_id-1800;
             if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=33-ch_id1;
                 MWDC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X1_fired_ch[MWDC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=49&&ch_id1<=64)
               { int ch_id2=81-ch_id1;
                 MWDC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X1_fired_ch[MWDC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=81&&ch_id1<=96)
               { int ch_id2=129-ch_id1;
                 MWDC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X1_fired_ch[MWDC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=113&&ch_id1<=128)
               { int ch_id2=177-ch_id1;
                 MWDC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X1_fired_ch[MWDC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2=17-ch_id1;
                 MWDC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X2_fired_ch[MWDC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=33&&ch_id1<=48)
               { int ch_id2=65-ch_id1;
                 MWDC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X2_fired_ch[MWDC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=65&&ch_id1<=80)
               { int ch_id2=113-ch_id1;
                 MWDC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X2_fired_ch[MWDC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=97&&ch_id1<=112)
               { int ch_id2=161-ch_id1;
                 MWDC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X2_fired_ch[MWDC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }

//////////// MWDC1X5 is from 1001
           else if(ch_id>=1001&&ch_id<=1032)
           { int ch_id1=ch_id-1000;

             if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=97-ch_id1;
                 MWDC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X1_fired_ch[MWDC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2=81-ch_id1;
                 MWDC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC1X2_fired_ch[MWDC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }

//////////// MWDC2X5 is from 1601
           else if(ch_id>=1601&&ch_id<=1632) //!!!!!!!!!!!!!!!!!!!!!!!
           { int ch_id1=ch_id-1600;

             if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=97-ch_id1;
                 MWDC2X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X1_fired_ch[MWDC2X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2=81-ch_id1;
                 MWDC2X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC2X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC2X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC2X2_fired_ch[MWDC2X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC2X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC2X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }

           }
      

//////////// MWDC3X5 is from 2401
          else if(ch_id>=2401&&ch_id<=2432) //!!!!!!!!!!!!!!!!!!!!!
           { int ch_id1=ch_id-2400;

             if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=97-ch_id1;
                 MWDC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X1_fired_ch[MWDC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2=81-ch_id1;
                 MWDC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 MWDC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 MWDC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 MWDC3X2_fired_ch[MWDC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 MWDC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 MWDC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }

