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
#ifndef __GDAQ_INTERFACE_H__
#define __GDAQ_INTERFACE_H__
#include "gdaq_general.h"
#include "gdaq_sock.h"
#include "gdaq_ipc.h"
#include "gdaq_thread_task.h"
#include "list.h"

#define MAX_VALID_INTERFACE_PAGE_SIZE	 	DEFAULT_GLOBAL_BUFFER_PAGE_SIZE
#define MAX_VALID_DATA_PAGE_SIZE			DEFAULT_GLOBAL_BUFFER_PAGE_SIZE
#define MAX_DATA_FRAGMENT_PER_PAGE	((MAX_VALID_INTERFACE_PAGE_SIZE-sizeof(shm_interface_page_descriptor))/sizeof(shm_interface_page_fragment))

typedef struct thread_task_t{
	char* 	task_addr;      //the address of task data  in memory
	__s32	page_id;        //the page id for current LN ID and current type
	__s32	page_size;      
	char*	current_addr;
	__s32	remain_data_num;
    __s32   page;           //the page number in global buffer
} thread_task;

typedef struct wait_task_t{
	thread_task 		task;
	struct list_head 	node;
} wait_task;


typedef struct data_out_section_t{
    char*               addr;
    int                 length;
    struct list_head    node;
} data_out_section;


typedef struct data_ready_page_t{                                           
    ln_page_descriptor  ln_page;                                            
    int                 page;                                               
	struct list_head 	node;                                               
} data_ready_page;                                                          
                                                                            

int gdaq_global_buffer_search(gdaq_global_buffer *p_buffer,ln_page_descriptor ln_page,struct list_head* p_page_ready_list);
char* gdaq_global_buffer_get_addr(gdaq_global_buffer *p_buffer,ln_page_addr addr,struct list_head* p_page_ready_list);
int gdaq_global_buf_search_with_queue(gdaq_global_buffer *p_buffer,ln_page_descriptor ln_page, thread_interface* p_task_queue, struct list_head* p_wait_list_head);
ln_page_addr get_ln_page_offset_addr(gdaq_global_buffer *p_buffer,ln_page_addr start, __s32 offset, data_ready_page *p_pre_page, thread_interface* p_task_queue, struct list_head* p_wait_list_head);
ln_page_addr get_ln_page_offset_address(gdaq_global_buffer *p_buffer,ln_page_addr start, __s32 offset,struct list_head* p_page_ready_list);


/*may be used in the receive and the input_interface module*/
void free_wait_list(struct list_head* p_wait_list_head);
void display_task_info(thread_task* p_task);
int thread_get_task_from_id(thread_task* p_task, thread_interface* p_task_queue, struct list_head* p_wait_list_head ,int task_page_id,int offset);
int copy_from_pages(char* dest, thread_task* p_task, __u32 num, thread_interface* p_task_queue, struct list_head* p_wait_list_head);
int status_scan_and_generate_task(int node_id, gdaq_global_buffer* p_buffer, thread_interface* p_task_queue, shm_interface* p_out_shm_interface);
int data_scan_and_generate_task(int node_id,gdaq_global_buffer* p_buffer, thread_interface* p_task_queue, shm_interface* p_out_shm_interface);
/*may be used in the save, display_ln_task and transmit module*/
int copy_from_buffer(char* dest, ln_page_addr entry, __u32 num, gdaq_global_buffer* p_buffer, struct list_head* p_page_ready_list);
int copy_to_buffer(ln_page_addr dest, char* source, __u32 num, gdaq_global_buffer* p_buffer, struct list_head* p_page_ready_list);
int __generate_data_list(struct list_head* p_data_out_list, ln_page_addr entry, __u32 length, gdaq_global_buffer* p_buffer, struct list_head* p_page_ready_list, int data_valid);
void free_data_out_list(struct list_head* p_data_out_list_head);
int generate_data_out_list_splice(gdaq_global_buffer* p_buffer, ln_page_addr task_entry, struct list_head* p_data_out_list, int* p_total_data_len, struct list_head* p_page_ready_list);
int generate_data_out_list(gdaq_global_buffer* p_buffer, ln_page_addr task_entry, struct list_head* p_data_out_list, int* p_total_data_len, struct list_head* p_page_ready_list);
int free_page_ready_list(gdaq_global_buffer *p_buffer, struct list_head* p_page_ready_list, int if_free_page);



#endif

