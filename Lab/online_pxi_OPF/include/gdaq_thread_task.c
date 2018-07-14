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
#ifndef __GDAQ_THREAD_TASK_C__
#define __GDAQ_THREAD_TASK_C__
#include "gdaq_thread_task.h"

int thread_task_queue_init(thread_interface* p_task_queue)
{
    if(sem_init(&p_task_queue->mutex,0,1)==-1)
    {
        printc(MSG_E,"thread_task_queue_init: sem mutex of task queue create error!(%m)\n",errno);
        return -1;
    }
    if(sem_init(&p_task_queue->nempty,0, p_task_queue->task_queue_length)==-1)
    {
        printc(MSG_E,"thread_task_queue_init: sem nempty of task queue create error!(%m)\n", errno);
        return -1;
    }
    if(sem_init(&p_task_queue->nstored,0, 0)==-1)
    {
        printc(MSG_E,"thread_task_queue_init: sem nstored of task queue create error!(%m)\n", errno);
        return -1;
    }
    p_task_queue->p_read=0;
    p_task_queue->p_write=0;
    memset(p_task_queue->task_queue_addr, -1, p_task_queue->task_queue_length*p_task_queue->task_struct_size);	
	return 0;
}

int	thread_add_task(void* task, thread_interface* p_task_queue)
{



	if(sem_wait(&p_task_queue->nempty)!=0)
	{
		printc(MSG_E, "Task queue: sem_wait nempty error!(%m)\n", errno);
		return -1;
	}
	if(sem_wait(&p_task_queue->mutex)!=0)
	{
		printc(MSG_E, "Task queue: sem_wait mutex error!(%m)\n", errno);
		return -1;
	}
	/*critical region*/
	memcpy((void*)(p_task_queue->task_queue_addr+ p_task_queue->p_write*p_task_queue->task_struct_size), task,p_task_queue->task_struct_size);
    p_task_queue->p_write++;
    if(p_task_queue->p_write == p_task_queue->task_queue_length)
		p_task_queue->p_write = 0;

    #ifdef DAQ_DEBUG
//    printd("Thread_add_task: current p_write=%d.\n",p_task_queue->p_write);
    #endif
    /*critical region end*/
	if(sem_post(&p_task_queue->mutex)!=0)
	{
		printc(MSG_E, "Task queue: sem_post mutex error!(%m)\n", errno);
		return -1;
	}
	
	if(sem_post(&p_task_queue->nstored)!=0)
	{
		printc(MSG_E, "Task queue: sem_post nstored error!(%m)\n", errno);
		return -1;
	}
    return 0;

}
int thread_get_task(thread_interface* p_task_queue, void* out_task)
{
    memset(out_task, 0, p_task_queue->task_struct_size);
	if(sem_wait(&p_task_queue->nstored)!=0)
    {
        printc(MSG_E, "Task queue: sem_wait nstored error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_task_queue->mutex)!=0)
    {
        printc(MSG_E, "Task queue: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
	memcpy(out_task, (void*)((unsigned long)p_task_queue->task_queue_addr+ p_task_queue->p_read*p_task_queue->task_struct_size),p_task_queue->task_struct_size);
    memset((void*)((unsigned long)p_task_queue->task_queue_addr+ p_task_queue->p_read*p_task_queue->task_struct_size),0, p_task_queue->task_struct_size);
	p_task_queue->p_read++;
    if(p_task_queue->p_read== p_task_queue->task_queue_length)
        p_task_queue->p_read = 0;
    #ifdef DAQ_DEBUG
//    printd("Thread_get_task: current p_read=%d.\n",p_task_queue->p_read);
    #endif
    /*critical region end*/
    
    if(sem_post(&p_task_queue->mutex)!=0)
    {
        printc(MSG_E, "Task queue: sem_post mutex error!(%m)\n", errno);
		return -1;
    }

    if(sem_post(&p_task_queue->nempty)!=0)
    {
        printc(MSG_E, "Task queue: sem_post nempty error!(%m)\n", errno);
		return -1;
    }
	return 0;
}

#endif

