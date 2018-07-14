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
#ifndef __GDAQ_THREAD_TASK_H__
#define __GDAQ_THREAD_TASK_H__
#include "gdaq_general.h"
#include "gdaq_sock.h"
#include "gdaq_ipc.h"



typedef struct thread_interface_t{
    int     task_queue_length;
    int     task_struct_size;
    sem_t   mutex, nempty, nstored; 
    int     p_write, p_read;
    char*   task_queue_addr;      
} thread_interface;


int thread_task_queue_init(thread_interface* p_task_queue);
int	thread_add_task(void* task, thread_interface* p_task_queue);
int thread_get_task(thread_interface* p_task_queue, void* out_task);


#endif

