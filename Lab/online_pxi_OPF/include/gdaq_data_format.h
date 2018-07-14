///////////////////////////////////////////////////////////////////////////////////////////      
//////               GGGGGG             DDDDDDDD            AA           QQQQQQ      //////
//////            GG      GG             DD     DD        AAAA        QQ      QQ     //////
//////         GG                       DD      DD      AA  AA      QQ        QQ     //////
//////       GG                        DD       DD     AA  AA     QQ          QQ     //////
//////      GG                        DD       DD    AA    AA    QQ          QQ      //////
//////     GG                 GDAQ   DD       DD    AA    AA    QQ          QQ       //////
//////    GG           GGGG         DD       DD   AA      AA   QQ          QQ        //////
//////   GG            GG          DD       DD   AAAAAAAAAA   QQ       QQ QQ         //////
//////   GG          GGG          DD      DD   AA        AA   QQ        QQ           //////  
//////    GG      GG GG          DD     DD    AA        AA    QQ      QQ QQ          //////
//////     GGGGGG   GG        DDDDDDDD      AA          AA     QQQQQQ    QQ          //////
///////////////////////////////////////////////////////////////////////////////////////////
////////////                        Author:                                          //////
////////////                        Date:       2014.1                               //////
////////////                        Version:    V1.0                                 //////
////////////                        CopyRight:  Fast Electronic Laboratory           //////
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef __GDAQ_DATA_FORMAT_H__
#define __GDAQ_DATA_FORMAT_H__

#include "./gdaq_typedef.h"   //change when this file is not in directory "include"

//DATA TYPE definition

#define GDAQ_DATA_TYPE_ERROR    -1
#define GDAQ_DATA_TYPE_NONE     0
#define GDAQ_DATA_TYPE_NORMAL   1
#define GDAQ_DATA_TYPE_CANCEL   2
#define GDAQ_DATA_TYPE_HEART_BEAT   3
#define GDAQ_DATA_TYPE_START    0x80
#define GDAQ_DATA_TYPE_STOP     0x40


//CHANNEL FULL definition

#define CHANNEL_FULL    1
#define CHANNEL_NOT_FULL    0

//LENGTH CHANGE definition

#define LENGTH_CHANGE    1
#define LENGTH_NOT_CHANGE    0

typedef struct data_section_head_t{
    __s32       index;
    __s32       map_index;
    __s32       data_type:8;
    __s32       fragment_num:24;  //(in interface page)the fragment number in this page, not the fragment number of the whole data section
} data_section_head;           //using in both data page and interface page

typedef struct data_fragment_head_t{
    __s32       start_channel;
    __s32       channel_num:30;
    __u32       channel_full:1;
    __u32       length_change:1;
    __s32       data_len;
} data_fragment_head;          //using in both data page and interface page

typedef struct data_validity_table_t{
    __s32       valid_channel_num;
    __s32       channel_id[0];
} data_validity_table;

typedef struct channel_offset_table_t{
    __s32       offset[0];
} channel_offset_table;





#endif
