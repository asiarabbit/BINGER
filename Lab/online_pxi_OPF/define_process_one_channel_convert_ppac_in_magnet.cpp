
////////////////PPAC1X is from 3301
          if(ch_id>=3401&&ch_id<=3432)
           { int ch_id1=ch_id-3400;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 PPAC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC1X1_fired_ch[PPAC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 PPAC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC1X2_fired_ch[PPAC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }
////////////////PPAC1Y is from 3333
          if(ch_id>=3433&&ch_id<=3464)
           { int ch_id1=ch_id-3432;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 PPAC1Y1_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC1Y1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC1Y1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC1Y1_fired_ch[PPAC1Y1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC1Y1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC1Y1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 PPAC1Y2_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC1Y2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC1Y2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC1Y2_fired_ch[PPAC1Y2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC1Y2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC1Y2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }


////////////////PPAC3X is from 3365
          if(ch_id>=3401+64&&ch_id<=3432+64)
           { int ch_id1=ch_id-3400-64;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 PPAC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC3X1_fired_ch[PPAC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 PPAC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC3X2_fired_ch[PPAC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }
////////////////PPAC3Y is from 3397
          if(ch_id>=3433+64&&ch_id<=3464+64)
           { int ch_id1=ch_id-3432-64;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 PPAC3Y1_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC3Y1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC3Y1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC3Y1_fired_ch[PPAC3Y1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC3Y1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC3Y1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 PPAC3Y2_edge_num_total[ch_id2]=edge_num_total_t;
                 PPAC3Y2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 PPAC3Y2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 PPAC3Y2_fired_ch[PPAC3Y2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 PPAC3Y2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 PPAC3Y2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }


