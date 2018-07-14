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
////////////                        Author:     Yang Junfeng                         //////
////////////                        Date:       2012.11                              //////
////////////                        Version:    V1.0                                 //////
////////////                        CopyRight:  Fast Electronic Laboratory           //////
///////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GDAQ_TYPEDEF_H__
#define __GDAQ_TYPEDEF_H__

#include <stdint.h>

/*
General macro function definition
*/
typedef int8_t	__s8;
typedef	int16_t	__s16;
typedef int32_t	__s32;
typedef int64_t __s64;
//typedef long long __s64;
typedef uint8_t	__u8;
typedef uint16_t 	__u16;
typedef uint32_t 	__u32;
typedef uint64_t    __u64;
//typedef unsigned long    __u64;
//typedef unsigned long long   __u64;


/* older ethtool.h includes want them without the leading underscores */
//typedef unsigned long long u64;
//typedef unsigned int       u32;
//typedef unsigned short     u16;
//typedef unsigned char       u8;



#endif //__GDAQ_TYPEDEF_H__

