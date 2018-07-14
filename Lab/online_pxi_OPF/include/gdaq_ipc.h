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
#ifndef __GDAQ_IPC_H__
#define __GDAQ_IPC_H__
#include "gdaq_general.h"
#include "gdaq_sock.h"
#include <sys/mman.h>
#include <semaphore.h>
#include <mqueue.h>
#include "gdaq_object.h"
#include "list.h"

//#define	GLOBAL_SHARED_INFO_SHM	"/tmp/global_shared_info_shm"
//#define	GLOBAL_SHARED_INFO_SEM	"/tmp/global_shared_info_sem"

#define	FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
//#define     FILE_MODE   0x666
#define DEFAULT_MSG_PRIO 10

#define	QUEUE_NUM_NULL	-1 

#define LN_PAGE_TYPE_NONE           0
#define LN_PAGE_TYPE_DATA     	    1
#define LN_PAGE_TYPE_INTERFACE      2
#define LN_PAGE_TYPE_STATUS      	3
#define LN_PAGE_TYPE_STATUS_INTERFACE 4


//using in IPC and interface definition


typedef struct gdaq_msg_queue{
	char mq_name[MAX_TMP_STRING_LENGTH];
	mqd_t	mqd;
} gdaq_msg_queue;

typedef struct gdaq_shm_t{
    //must be initialed when struct be defined
    char 	name[MAX_TMP_STRING_LENGTH];
	size_t	shm_size;	
    //will be updated when gdaq_shm_init and gdaq_shm_link be processed
	void*	ptr;
} gdaq_shm;

typedef struct gdaq_shm_with_mutex_t{
    //must be initialed when struct be defined
    char 	name[MAX_TMP_STRING_LENGTH];
	size_t	shm_size;	
    //will be updated when gdaq_shm_init and gdaq_shm_link be processed
	void*	ptr;
    sem_t   *mutex;
} gdaq_shm_with_mutex;

typedef struct gdaq_global_buffer_t{
    __u32           page_size;
    __u32           page_num;
    pthread_mutex_t	*p_mutex;
    gdaq_shm        buffer;
    gdaq_shm        free_page_stack;
    gdaq_shm        descriptor;
} gdaq_global_buffer;

typedef struct buf_stack_t{
    __s32       stack_depth;
    __s32       stack_top;				//the stack top of the descriptor memory manage stack 
    int         stack[0];
} buf_stack;

typedef struct shm_page_descriptor_t{
    __s32           page_id;
    __s32           total_user_num;
    __s32           finish_user_num;
	__s32			page_used_length;
    __u8            data_ready;
    pthread_cond_t  data_ready_cond;
    __s32           next;
    __s32           prev;
} shm_page_descriptor;

typedef struct ln_page_entry_t{
    __s32           type;
    __s32           entry;
    __s32           page_used;
    pthread_cond_t  cond;
} ln_page_entry;
typedef struct ln_page_info_t{
    __s32       ln_id;
    __s32       page_type_num;
    ln_page_entry       entry_type[MAX_LN_PAGE_TYPE];
} ln_page_info;

typedef struct buf_descriptor_t{
    __s32     page_used;
    __s32     page_free;
    __s32     ln_num;
    ln_page_info     ln_info[MAX_LN_NUM];    
    shm_page_descriptor page_descriptor[0];   //[DEFAULT_GLOBAL_BUFFER_PAGE_NUM];
} buf_descriptor;


typedef struct shm_interface_t{
    char    name[MAX_TMP_STRING_LENGTH];
    sem_t   mutex, nempty, nstored; 
    int     p_write, p_read;        
    ln_page_addr   section[DEFAULT_INTERFACE_TASK_NUM];         
} shm_interface;

typedef struct online_shm_interface_t{
    char    name[MAX_TMP_STRING_LENGTH];
    sem_t   mutex, nempty, nstored; 
    int     p_write, p_read;        
    int     section[DEFAULT_INTERFACE_TASK_NUM];         
} online_shm_interface;

int gdaq_msg_queue_open_or_create(gdaq_msg_queue* mq);
int gdaq_msg_queue_create(gdaq_msg_queue* mq);
int gdaq_msg_queue_open(gdaq_msg_queue* mq);
int gdaq_msg_queue_close(gdaq_msg_queue* mq);
int gdaq_msg_queue_delete(gdaq_msg_queue* mq);
int gdaq_msg_queue_send(gdaq_msg_queue* mq, gdaq_msg* msg);
int gdaq_msg_queue_recv(gdaq_msg_queue* mq, gdaq_msg* msg);


int gdaq_shm_create(gdaq_shm *p_shm);
int gdaq_shm_open(gdaq_shm *p_shm);
int gdaq_shm_close(gdaq_shm *p_shm);
int gdaq_shm_delete(gdaq_shm *p_shm);

int gdaq_shm_with_mutex_create(gdaq_shm_with_mutex *p_shm);
int gdaq_shm_with_mutex_open(gdaq_shm_with_mutex *p_shm);
int gdaq_shm_with_mutex_close(gdaq_shm_with_mutex *p_shm);
int gdaq_shm_with_mutex_delete(gdaq_shm_with_mutex *p_shm);
int gdaq_shm_with_mutex_lock(gdaq_shm_with_mutex *p_shm);
int	gdaq_shm_with_mutex_unlock(gdaq_shm_with_mutex *p_shm);

int gdaq_global_buf_create(char* buf_name, __u32 page_size, __u32 page_num, gdaq_global_buffer *p_buffer);
int gdaq_global_buf_open(char* buf_name, __u32 page_size, __u32 page_num, gdaq_global_buffer *p_buffer);
int gdaq_global_buf_malloc(gdaq_global_buffer *p_buffer, ln_page_descriptor ln_page);
int gdaq_global_buf_search(gdaq_global_buffer *p_buffer,ln_page_descriptor ln_page);



int gdaq_global_buf_free(gdaq_global_buffer *p_buffer, ln_page_descriptor ln_page);
void gdaq_global_page_user_num_add_one(gdaq_global_buffer *p_buffer, int page);
int gdaq_global_buf_wait_data_ready(gdaq_global_buffer *p_buffer, int page);
int gdaq_global_buf_set_data_ready(gdaq_global_buffer* p_buffer, int page, __u8 data_ready);
int gdaq_global_buf_close(gdaq_global_buffer *p_buffer);
int gdaq_global_buf_delete(gdaq_global_buffer *p_buffer);
char*	gdaq_global_buf_get_addr(gdaq_global_buffer *p_buffer, int page, int offset);



int ln_interface_create(char* shm_name, shm_interface** p_interface);
int ln_interface_open(char* shm_name, shm_interface** p_interface);
int ln_interface_destroy(char* shm_name, shm_interface* p_interface);
int ln_interface_add_task(ln_page_addr task_addr,shm_interface* p_interface);
int ln_interface_get_task(shm_interface* p_interface, ln_page_addr* out_task_addr);

int online_interface_create(char* shm_name, online_shm_interface** p_interface);
int online_interface_open(char* shm_name, online_shm_interface** p_interface);
int online_interface_destroy(char* shm_name, online_shm_interface* p_interface);
int online_interface_add_task(int* task_addr,online_shm_interface* p_interface);
int online_interface_get_task(online_shm_interface* p_interface, int* out_task_addr);
#endif

