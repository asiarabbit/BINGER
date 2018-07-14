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

#ifndef __CONF_FILE_OPER_H__
#define __CONF_FILE_OPER_H__

#include <stdio.h>
#include <string.h>
#include "gdaq_general.h"

typedef enum conf_state_t{CONFIG_START, CONFIG_STREAM, CONFIG_GROUP, CONFIG_IDLE, CONFIG_SEL, CONFIG_OTHER} CONFSTATE;


#define	ITEM_EMPTY	0
#define	ITEM_NOTE	1
#define	ITEM_LABEL	2
#define ITEM_NORMAL	3

typedef struct item_t {
    char key[MAX_TMP_STRING_LENGTH];
    char value[MAX_TMP_STRING_LENGTH];
}ITEM;

char *strtrimr(char *pstr, char ch);
char *strtriml(char *pstr, char ch);
char *strtrim(char *pstr, char ch_l, char ch_r);
int  get_item_from_line(char *line, ITEM *item);

#endif 
