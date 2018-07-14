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
////////////                        Author:     			                         //////
////////////                        Date:      	                                     //////
////////////                        Version:    V1.0                                 //////
////////////                        CopyRight:  Fast Electronic Laboratory           //////
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef __GDAQ_TIMER_C__
#define __GDAQ_TIMER_C__
#include "gdaq_timer.h"

#undef  DAQ_DEBUG

void *timer_thread(void *arg)
{
	timer_information* p_timer_info=(timer_information*)arg;

	int ret;
	struct timeval now;
    struct timespec stop_time;
	if ((ret = pthread_detach(pthread_self ())) != 0)
	{
		printc(MSG_E,"thread detach error!(%m)\n");
		//pthread_exit ((void*)-1);
		gdaq_exit(-1);
	}
	pthread_mutex_lock(&p_timer_info->mutex);
	while(1)
	{
		while(p_timer_info->run==0)
	    {
			if((ret=pthread_cond_wait(&p_timer_info->wait_start,&p_timer_info->mutex))!=0)
			{
        		printc(MSG_E,"pthread_cond_wait error!\n");
        		pthread_mutex_unlock(&p_timer_info->mutex);
        		gdaq_exit(-1);
        	}
    	}
		if((ret=pthread_cond_broadcast(&p_timer_info->wait_timer_ack))!=0)
    	{
        	printc(MSG_E,"pthread_cond_broadcast error!(%m)\n");
			pthread_mutex_unlock(&p_timer_info->mutex);
        	gdaq_exit(-1);
    	}
		p_timer_info->time_run_out=0;
		p_timer_info->counting=1;
		gettimeofday(&now, NULL);
        stop_time.tv_sec = now.tv_sec + p_timer_info->set_second;
        stop_time.tv_nsec = now.tv_usec*1000;
        ret=pthread_cond_timedwait(&p_timer_info->wait_stop,&p_timer_info->mutex,&stop_time);
        if(ret == ETIMEDOUT)
        {
	        p_timer_info->time_run_out=1;
			p_timer_info->run=0;
			//p_timer_info->counting=0;
			//continue;
        }
		else if(p_timer_info->run!=0)
		{
			printc(MSG_A,"cond be waked up unexpected!\n");
			p_timer_info->time_run_out=1;
			p_timer_info->run=0;
			//p_timer_info->counting=0;
			//continue;
		}
		p_timer_info->counting=0;
		if((ret=pthread_cond_broadcast(&p_timer_info->wait_timer_ack))!=0)
    	{
        	printc(MSG_E,"pthread_cond_broadcast error!(%m)\n");
			pthread_mutex_unlock(&p_timer_info->mutex);
        	gdaq_exit(-1);
    	}
    }
	pthread_mutex_unlock(&p_timer_info->mutex);
}
int init_timer(int second,timer_information* p_timer_info)
{
	int ret;
	p_timer_info->set_second=second;
	p_timer_info->run=0;
	p_timer_info->time_run_out=0;
	p_timer_info->counting=0;
	if((ret=pthread_mutex_init(&p_timer_info->mutex,NULL))!=0)
    {
        printc(MSG_E,"pthread_mutex_init error(%m)\n");
		return -1;
    }
	if((ret=pthread_cond_init(&p_timer_info->wait_timer_ack,NULL))!=0)
    {
        printc(MSG_E,"pthread_cond_init error(%m)\n");
		return -1;
    }  
	if((ret=pthread_cond_init(&p_timer_info->wait_start,NULL))!=0)
    {
        printc(MSG_E,"pthread_cond_init error(%m)\n");
		return -1;
    }  
	if((ret=pthread_cond_init(&p_timer_info->wait_stop,NULL))!=0)
    {
        printc(MSG_E,"pthread_cond_init error(%m)\n");
		return -1;
    }  
	pthread_t timer;
	ret = pthread_create(&timer, NULL, timer_thread, (void*)p_timer_info);
	if(ret!=0)
	{
		printc(MSG_E,"timer thread create error(%m)\n");
		return -1;
	}
	#ifdef DAQ_DEBUG
    printd("init timer done\n");
    #endif
	return 0;
}

int timer_start(timer_information* p_timer_info)
{
	int ret;
	pthread_mutex_lock(&p_timer_info->mutex);
	p_timer_info->run=1;
	if((ret=pthread_cond_broadcast(&p_timer_info->wait_start))!=0)
    {
        printc(MSG_E,"pthread_cond_broadcast error!(%m)\n");
		pthread_mutex_unlock(&p_timer_info->mutex);
        return -1;
    }
	while(p_timer_info->counting==0)
	{
		if((ret=pthread_cond_wait(&p_timer_info->wait_timer_ack,&p_timer_info->mutex))!=0)
		{
        	printc(MSG_E,"pthread_cond_wait error!\n");
        	pthread_mutex_unlock(&p_timer_info->mutex);
        	return -1;
        }
    }
	#ifdef DAQ_DEBUG
	printc(MSG_N,"start timer\n");
	#endif
	pthread_mutex_unlock(&p_timer_info->mutex);
	return 0;
}

int timer_stop(timer_information* p_timer_info)
{
	int ret;
	pthread_mutex_lock(&p_timer_info->mutex);
	if(p_timer_info->time_run_out!=1)
	{
		p_timer_info->run=0;
		if((ret=pthread_cond_broadcast(&p_timer_info->wait_stop))!=0)
		{
			printc(MSG_E,"pthread_cond_broadcast error!(%m)\n");
			pthread_mutex_unlock(&p_timer_info->mutex);
			return -1;
		}
		while(p_timer_info->counting==1)
		{
			if((ret=pthread_cond_wait(&p_timer_info->wait_timer_ack,&p_timer_info->mutex))!=0)
			{
				printc(MSG_E,"pthread_cond_wait error!\n");
				pthread_mutex_unlock(&p_timer_info->mutex);
				return -1;
			}
		}
	}
	p_timer_info->time_run_out=0;
	#ifdef DAQ_DEBUG
	printc(MSG_N,"stop timer\n");
	#endif
	pthread_mutex_unlock(&p_timer_info->mutex);
	return 0;
}
int check_timer_out(timer_information* p_timer_info)
{
	int time_out;
	pthread_mutex_lock(&p_timer_info->mutex);
	if(time_out=p_timer_info->time_run_out)
	p_timer_info->time_run_out=0;
	pthread_mutex_unlock(&p_timer_info->mutex);
	return time_out;
}


#endif

