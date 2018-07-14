
////////////////QQAC1X is from 3301
          if(ch_id>=3201&&ch_id<=3232)
           { int ch_id1=ch_id-3200;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 QQAC1X1_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC1X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC1X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC1X1_fired_ch[QQAC1X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC1X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC1X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 QQAC1X2_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC1X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC1X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC1X2_fired_ch[QQAC1X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC1X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC1X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }
////////////////QQAC1Y is from 3333
          if(ch_id>=3233&&ch_id<=3264)
           { int ch_id1=ch_id-3232;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 QQAC1Y1_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC1Y1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC1Y1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC1Y1_fired_ch[QQAC1Y1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC1Y1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC1Y1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 QQAC1Y2_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC1Y2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC1Y2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC1Y2_fired_ch[QQAC1Y2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC1Y2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC1Y2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }


////////////////QQAC3X is from 3365
          if(ch_id>=3201+64&&ch_id<=3232+64)
           { int ch_id1=ch_id-3200-64;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 QQAC3X1_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC3X1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC3X1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC3X1_fired_ch[QQAC3X1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC3X1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC3X1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 QQAC3X2_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC3X2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC3X2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC3X2_fired_ch[QQAC3X2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC3X2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC3X2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }
////////////////QQAC3Y is from 3397
          if(ch_id>=3233+64&&ch_id<=3264+64)
           { int ch_id1=ch_id-3232-64;
             if(ch_id1>=1&&ch_id1<=16)
               { int ch_id2 = ch_id1;
                 QQAC3Y1_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC3Y1_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC3Y1_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC3Y1_fired_ch[QQAC3Y1_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC3Y1_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC3Y1_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
             else if(ch_id1>=17&&ch_id1<=32)
               { int ch_id2=ch_id1-16;
                 QQAC3Y2_edge_num_total[ch_id2]=edge_num_total_t;
                 QQAC3Y2_edge_num_h_leading[ch_id2]=edge_num_h_leading_t;
                 QQAC3Y2_edge_num_h_trailing[ch_id2]=edge_num_h_trailing_t;
                 QQAC3Y2_fired_ch[QQAC3Y2_fired_ch_num++]=ch_id2; 
                 for(int k=0;k<edge_num_h_leading_t;k++)
                 QQAC3Y2_data_h_leading_f[ch_id2][k]=data_h_leading_f[ch_id][k];
                 for(int k=0;k<edge_num_h_trailing_t;k++)
                 QQAC3Y2_data_h_trailing_f[ch_id2][k]=data_h_trailing_f[ch_id][k];
               }
           }


