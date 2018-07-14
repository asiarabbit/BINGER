
           int edge_num_total_t=0;
           int edge_num_h_leading_t=0,edge_num_h_trailing_t=0,edge_num_v_leading_t=0,edge_num_v_trailing_t=0; 
           int ch_id=p_data_validity_table->channel_id[j];

           fired_ch[fired_ch_num++]=ch_id; 

           if(ch_id > ch_num_limit-1){cout<<endl<<" channels defined are too small!"<<endl; pause(); }


           // Read channel data ---data_ch
             if(j<valid_channel_num-1)
                {  
                   edge_num_total_t = p_channel_offset_table->offset[j+1]/4-p_channel_offset_table->offset[j]/4;
                   data_ch=(int*)malloc(sizeof(int)+edge_num_total_t*sizeof(int));
                   memcpy(data_ch, &data_frag[p_channel_offset_table->offset[j]/4], 
                  (p_channel_offset_table->offset[j+1]/4-p_channel_offset_table->offset[j]/4)*sizeof(int));
                 }
             else
                {  
                   edge_num_total_t = data_len_per_frag/4-p_channel_offset_table->offset[j]/4;
                   data_ch=(int*)malloc(sizeof(int)+edge_num_total_t*sizeof(int));
                   memcpy(data_ch, &data_frag[p_channel_offset_table->offset[j]/4],
                        (data_len_per_frag/4-p_channel_offset_table->offset[j]/4)*sizeof(int));
                }
         
          if(j==valid_channel_num-1) edge_num_total[ch_id]=0; else   edge_num_total[ch_id]=edge_num_total_t;

          if(edge_num_total_t>edge_num_limit) edge_num_total_t=edge_num_limit;


             // Read every single edge data of one channel
             bool This_is_high = false;
             for(int k=0;k<edge_num_total_t;k++)// if(k<edge_num_limit)
             { 
               if( (data_ch[k]>>31 & 0x1) == 0) //high
                {   This_is_high = true;
                    if( (data_ch[k]>>30 & 0x1) == 0) //leading
                    {  if(fragment_head[i].channel_num== 33)
                       {  data_h_leading[ch_id][edge_num_h_leading_t]= (data_ch[k] & 0x7FFFF); 
                          data_h_leading_f[ch_id][edge_num_h_leading_t]
                         =data_h_leading[ch_id][edge_num_h_leading_t]*0.09765625;
                          edge_num_h_leading_t++;
                       }
                       else   // for mwdc leading and trailing are reversed by USTC.
                       {
                          data_h_trailing[ch_id][edge_num_h_trailing_t]= (data_ch[k] & 0x7FFFF);
                          data_h_trailing_f[ch_id][edge_num_h_trailing_t]
                         =data_h_trailing[ch_id][edge_num_h_trailing_t]*0.09765625;
                         edge_num_h_trailing_t++;
                       }
                    }
                    else    // trailing                         
                    {  if(fragment_head[i].channel_num== 33)
                       {  data_h_trailing[ch_id][edge_num_h_trailing_t]= (data_ch[k] & 0x7FFFF);
                          data_h_trailing_f[ch_id][edge_num_h_trailing_t]
                           =data_h_trailing[ch_id][edge_num_h_trailing_t]*0.09765625;
                          edge_num_h_trailing_t++;
                       }
                       else  // for mwdc leading and trailing are reversed by USTC.
                       {  data_h_leading[ch_id][edge_num_h_leading_t]= (data_ch[k] & 0x7FFFF); 
                          data_h_leading_f[ch_id][edge_num_h_leading_t]
                           =data_h_leading[ch_id][edge_num_h_leading_t]*0.09765625;
                          edge_num_h_leading_t++;
                       }

                    }
                }
               else                      //very high
                {   if( (data_ch[k]>>30 & 0x1) == 0)
                    { data_v_leading[ch_id][edge_num_v_leading_t]= (data_ch[k] & 0x1FFFFF);
                      data_v_leading_f[ch_id][edge_num_h_leading_t]
                       =data_v_leading[ch_id][edge_num_h_leading_t]*0.024414063;
                      edge_num_v_leading_t++;
                    }
                    else                            
                    { data_v_trailing[ch_id][edge_num_v_trailing_t]= (data_ch[k] & 0x1FFFFF);
                      data_v_trailing_f[ch_id][edge_num_h_trailing_t]
                       =data_v_trailing[ch_id][edge_num_h_trailing_t]*0.024414063;
                      edge_num_v_trailing_t++;
                    }
                }
             } //for(int k=0;k<edge_num_total_t;k++)

           if(j!=valid_channel_num-1) 
           edge_num_h_leading[ch_id]=edge_num_h_leading_t;
           edge_num_h_trailing[ch_id]=edge_num_h_trailing_t;
           edge_num_v_leading[ch_id]=edge_num_v_leading_t;
           edge_num_v_trailing[ch_id]=edge_num_v_trailing_t;
          // edge_num_total[ch_id]=edge_num_total_t;
          #include "define_process_one_channel_convert.cpp"

          #include "define_process_one_channel_convert_ppac_in_magnet.cpp"
        //  #include "define_process_one_channel_convert_ppac.cpp"

          #include "define_process_one_channel_convert_qqac.cpp"
          if(j==valid_channel_num-1) 
         {             if(edge_num_total_t>1)
                      { cout<<endl<<"Error data happened! "<<edge_num_total_t
                            <<"  event num: "<<event_num<<endl<<endl; Is_Error_event=1;
                      }
            eventID[i]=(data_ch[0]>>12 & 0xFFF);
            bunchID[i]=(data_ch[0] & 0xFFF);
         }


////////////////////////////////////////////////////// Fill Fill Fill Fill Fill Fill Fill Fill 

         //if(ch_id%2==1)if(ch_id != 3133)if(ch_id != 3233)if(ch_id != 3333)if(ch_id != 3433)if(ch_id != 3533)
         HF1(2,ch_id,1.);



   
            if(j<valid_channel_num-1) // Edge_number_per_ch->Fill(ch_id,edge_num_total_t);
            HF2(7,ch_id,edge_num_total_t,1.);

           if(j<valid_channel_num-1)
           { if(This_is_high)
            HF2(8,ch_id,edge_num_h_leading_t-edge_num_h_trailing_t,1.);
            else
            HF2(8,ch_id,edge_num_v_leading_t-edge_num_v_trailing_t,1.);
           }
        if(edge_num_total_t==2)
        HF2(9,ch_id,(data_h_leading[ch_id][0]*0.09765625-data_h_trailing[ch_id][0]*0.09765625),1.);




         free(data_ch);






