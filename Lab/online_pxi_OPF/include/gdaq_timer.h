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
////////////                        Date:                                            //////
////////////                        Version:    V1.0                                 //////
////////////                        CopyRight:  Fast Electronic Laboratory           //////
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef __GDAQ_TIMER_H__
#define __GDAQ_TIMER_H__
#include "../include/gdaq_general.h"
#include "../include/gdaq_object.h"

typedef struct timer_information_t{
	pthread_mutex_t 	mutex;
	int run;
	int time_run_out;
	int counting;
	pthread_cond_t wait_timer_ack;
	pthread_cond_t wait_start;
	pthread_cond_t wait_stop;
	int set_second;
}timer_information;

int init_timer(int second,timer_information* p_timer_info);
int timer_start(timer_information* p_timer_info);
int timer_stop(timer_information* p_timer_info);
int check_timer_out(timer_information* p_timer_info);


#endif

