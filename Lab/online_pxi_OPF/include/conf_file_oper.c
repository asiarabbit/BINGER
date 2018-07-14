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

#ifndef __CONF_FILE_OPER_C__
#define __CONF_FILE_OPER_C__
#include "conf_file_oper.h"
/*
 trim the appointed char in the right side of a string
 */
char *strtrimr(char *pstr, char ch)
{
    int i;
    i = strlen(pstr) - 1;
    while ((pstr[i] == ch) && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}
/*
trim the appointed char in the left side of a string
 */
char *strtriml(char *pstr, char ch)
{
    int i = 0,j;
    j = strlen(pstr) - 1;
    while ((pstr[i] == ch) && (i <= j))
        i++;
    if (0<i)
    {
        int temp=0;
        for(temp=0;(i+temp)<=j;temp++)
        {
            pstr[temp]=pstr[i+temp];

        }
        pstr[temp]='\0';

    }
    return pstr;
}
/*
 trim the appointed char in the both side of a string
 */
char *strtrim(char *pstr, char ch_l, char ch_r)
{
    char *p;
    p = strtrimr(pstr, ch_r);
    return strtriml(p,ch_l);
}
/*

*/
int  get_item_from_line(char *line, ITEM *item)
{
    char *p_t1 = strtrimr(line,'\n');
	char *p_t2 = strtrimr(p_t1,'\r');
	char *p = strtrim(p_t2, ' ', ' ');
    int len = strlen(p);
    if(len <= 0)
	{
        return ITEM_EMPTY;
    }
    else if(p[0]=='#')
	{
		strcpy((*item).key, p);
		return ITEM_NOTE;
    }
	else if(p[0]=='[')
	{
		char *p2 = strtrim(p,'[',']');
		strcpy(item->key, p2);
		return ITEM_LABEL;
    }
	else
	{
		char *p3 = strchr(p, '=');
        *p3++ = '\0';
		char *p4 = strtrim(p,' ',' ');
		char *p5 = strtrim(p3, ' ', ' ');
		strcpy(item->key,p4);
        strcpy(item->value,p5);
    	return ITEM_NORMAL;
	}
}
#endif 
