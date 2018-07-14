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
#ifndef __GDAQ_INTERFACE_C__
#define __GDAQ_INTERFACE_C__
#include "gdaq_interface.h"

#undef  DAQ_DEBUG

/*
name: gdaq_global_buffer_search
function : get the global buffer page of the appointed ln_page_descriptor
// WARNING: 
// page_ready_list MUST be used in one thread, or the list
// may be added more than once in the multi-thread application
// (for quick search and buffer free ?? Zhang Yuanxiang)
*/
int gdaq_global_buffer_search(gdaq_global_buffer *p_buffer,ln_page_descriptor ln_page,struct list_head* p_page_ready_list)
{
    data_ready_page *p_current_page;
    struct list_head *pos, *q;
    //if the page has existed in the page_ready_list, return the address directly from the list
    list_for_each_safe(pos,q,p_page_ready_list)
	{
		p_current_page=list_entry(pos, data_ready_page, node);
        if(memcmp(&p_current_page->ln_page,&ln_page,sizeof(ln_page_descriptor))==0)
        {
            return p_current_page->page;
        } 
    }
 
    //if the page doesn't exist in the page_ready_list, 
    if(!(p_current_page = (data_ready_page*)malloc(sizeof(data_ready_page))))
	{
		printc(MSG_E,"gdaq_global_buffer_search malloc error(%m)!\n",errno);
		return -1;
	}
    memcpy(&p_current_page->ln_page, &ln_page, sizeof(ln_page_descriptor));
    //wait until the page has been malloced and the data is ready, and return the id of page
    if((p_current_page->page= gdaq_global_buf_search(p_buffer,ln_page))==QUEUE_NUM_NULL)
    {
        printc(MSG_E,"gdaq_global_buffer_search search buffer error! \n");
        return -1;
    }
    if(gdaq_global_buf_wait_data_ready(p_buffer,p_current_page->page)!=0)
    {
        printc(MSG_E,"gdaq_global_buffer_search wait_data_ready error! \n");
        return -1;
    }
    list_add_tail(&p_current_page->node, p_page_ready_list);
    return p_current_page->page;
}



/*
name: gdaq_global_buffer_get_addr
function : get the physical address of the appointed ln_page_addr
// WARNING: 
// page_ready_list MUST be used in one thread, or the list
// may be added more than once in the multi-thread application
// (for quick search and buffer free ?? Zhang Yuanxiang)
*/

char* gdaq_global_buffer_get_addr(gdaq_global_buffer *p_buffer,ln_page_addr addr,struct list_head* p_page_ready_list)
{
    char* p_out;
    data_ready_page *p_current_page;
    struct list_head *pos, *q;
     //if the page has existed in the page_ready_list, return the address directly from the list
    list_for_each_safe(pos,q,p_page_ready_list)
	{
		p_current_page=list_entry(pos, data_ready_page, node);
        if(memcmp(&p_current_page->ln_page,&addr.ln_page,sizeof(ln_page_descriptor))==0)
        {
            p_out = gdaq_global_buf_get_addr(p_buffer,p_current_page->page,addr.offset);
            return p_out;
        } 
    }
 
    //if the page doesn't exist in the page_ready_list, 
    if(!(p_current_page = (data_ready_page*)malloc(sizeof(data_ready_page))))
	{
		printc(MSG_E,"gdaq_global_buffer_get_addr malloc error(%m)!\n",errno);
		return NULL;
	}
    memcpy(&p_current_page->ln_page, &addr.ln_page, sizeof(ln_page_descriptor));
    //wait until the page has been malloced and the data is ready, and return the address
    if((p_current_page->page= gdaq_global_buf_search(p_buffer,addr.ln_page))==QUEUE_NUM_NULL)
    {
        printc(MSG_E,"gdaq_global_buffer_get_addr search buffer error! \n");
        return NULL;
    }

    p_out = gdaq_global_buf_get_addr(p_buffer,p_current_page->page,addr.offset);   
    if(gdaq_global_buf_wait_data_ready(p_buffer,p_current_page->page)!=0)
    {
        printc(MSG_E,"gdaq_global_buffer_get_addr wait_data_ready error! \n");
        return NULL;
    }
    list_add_tail(&p_current_page->node, p_page_ready_list);
    return p_out;
}

//name: gdaq_global_buf_search_with_queue
//function : Similar to fuction"gdaq_global_buf_search" in gdaq_ipc.c. But when can't find the ln_page needed currently,
//           this function begin read out task queue until the ln_page appear.
//           It mainly used in "get_ln_page_offset_addr"，to void a potential deadlock.

int gdaq_global_buf_search_with_queue(gdaq_global_buffer *p_buffer,ln_page_descriptor ln_page, thread_interface* p_task_queue, struct list_head* p_wait_list_head)
{
    int i,j;
    int current_ln = QUEUE_NUM_NULL;
    int current_page = QUEUE_NUM_NULL;
    int current_type = QUEUE_NUM_NULL;
    //begin search ln_page in global buffer
    pthread_mutex_lock(p_buffer->p_mutex);
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    for(i=0;i<p_buf_des->ln_num;i++)
    {
        if(p_buf_des->ln_info[i].ln_id==ln_page.ln_id)
        {
            current_ln = i;
            for(j=0;j<p_buf_des->ln_info[i].page_type_num;j++)
            {
                if(ln_page.type==p_buf_des->ln_info[i].entry_type[j].type)
                {
                    current_type = j;
                    break;
                }
            }
            break;
        }
    }
    if((current_ln==QUEUE_NUM_NULL)||(current_type==QUEUE_NUM_NULL))
    {
        printc(MSG_E, "Global Buffer: can't find the appointed type!\n");
        pthread_mutex_unlock(p_buffer->p_mutex);
        return QUEUE_NUM_NULL;                
    }  
    
    while(1)
    {
        current_page = p_buf_des->ln_info[current_ln].entry_type[current_type].entry;
        while(current_page!=QUEUE_NUM_NULL)
        {
            if(p_buf_des->page_descriptor[current_page].page_id == ln_page.page_id)
            {
                pthread_mutex_unlock(p_buffer->p_mutex);
                return current_page;
            }
            else
                current_page=p_buf_des->page_descriptor[current_page].next;
        }                      
    
        pthread_mutex_unlock(p_buffer->p_mutex);//unlock

        //Can't find currently,begin read out task queue.Get new task until get the appointed page.
        thread_task task;
        wait_task* p_new_task;
        wait_task* p_current_task;
        struct list_head *pos,*q;
        struct list_head *insert_pos;
        while(1)
        {
            //get task
            if(thread_get_task(p_task_queue, &task)!=0)
            {
                printc(MSG_E,"thread_get_task_from_id: get new task error!\n");
                return QUEUE_NUM_NULL;
            }
            #ifdef DAQ_DEBUG
            printd("gdaq_global_buf_search_with_queue: get page %d from the task queue!\n", task.page_id);
            #endif
            //adding to wait list
            if(!(p_new_task = (wait_task*)malloc(sizeof(wait_task))))
            {
                printc(MSG_E,"gdaq_global_buf_search_with_queue: malloc error(%m)!\n");
                return QUEUE_NUM_NULL;
            }
            memcpy(&p_new_task->task, &task, sizeof(thread_task));
            list_for_each_safe(pos,q,p_wait_list_head)
            {
                p_current_task = list_entry(pos, wait_task, node);
                if(queue_ahead(task.page_id,p_current_task->task.page_id,MAX_DATA_PAGE_ID))
                {
                    break;
                }
            }
            insert_pos = pos;
            insert_pos->prev->next=&p_new_task->node;
            p_new_task->node.prev = insert_pos->prev;
            p_new_task->node.next = insert_pos;
            insert_pos->prev = &p_new_task->node;
            //check whether it's the appointed page 
            if(task.page_id == ln_page.page_id)
            {
                #ifdef DAQ_DEBUG
                printd("gdaq_global_buf_search_with_queue: find page %d from the task queue!\n",task.page_id);
                #endif
                break;//break from get task while(1)
            }      
        }
        pthread_mutex_lock(p_buffer->p_mutex);//lock,loop back search again,this time the page should be found.
    }
    pthread_mutex_unlock(p_buffer->p_mutex);
    return QUEUE_NUM_NULL;
}

//this version use only one (data_ready_page *p_pre_page)(for data scan)
ln_page_addr get_ln_page_offset_addr(gdaq_global_buffer *p_buffer,ln_page_addr start, __s32 offset, data_ready_page *p_pre_page, thread_interface* p_task_queue, struct list_head* p_wait_list_head)
{
	ln_page_descriptor current_ln_page;
	ln_page_addr end;
	int page;
	buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
	memcpy(&current_ln_page,&start.ln_page,sizeof(ln_page_descriptor));
	end.ln_page.ln_id = start.ln_page.ln_id;
	end.ln_page.type = start.ln_page.type;
	if(memcmp(&start.ln_page,&p_pre_page->ln_page,sizeof(ln_page_descriptor))==0)
	{
		page = p_pre_page->page;
	}
	else
	{
		if((page=gdaq_global_buf_search_with_queue(p_buffer,start.ln_page,p_task_queue,p_wait_list_head))==QUEUE_NUM_NULL)
		{
			printc(MSG_E,"get_ln_page_offset_addr search buffer error! \n");
        	end.ln_page.page_id=-1;
        	return end;//error end
		}
		memcpy(&p_pre_page->ln_page, &start.ln_page, sizeof(ln_page_descriptor));
		p_pre_page->page = page;
	}
	if(gdaq_global_buf_wait_data_ready(p_buffer,page)!=0)
    {
        printc(MSG_E,"get_ln_page_offset_addr wait_data_ready error! \n");
	    end.ln_page.page_id=-1;
        return end;//error end
    }
	if((start.offset+offset)<p_buf_des->page_descriptor[page].page_used_length)
	{
		end.ln_page.page_id=start.ln_page.page_id;
		end.offset=start.offset+offset;
		return end;
	}
	else
		offset-=(p_buf_des->page_descriptor[page].page_used_length-start.offset);
	while(1)
	{
		
		current_ln_page.page_id++;
		if(current_ln_page.page_id>=MAX_DATA_PAGE_ID)
			current_ln_page.page_id = 0;
		if((page=gdaq_global_buf_search_with_queue(p_buffer,current_ln_page,p_task_queue,p_wait_list_head))==QUEUE_NUM_NULL)
		{
			printc(MSG_E,"get_ln_page_offset_addr search buffer error! \n");
        	end.ln_page.page_id=-1;
        	return end;//error end
		}
		memcpy(&p_pre_page->ln_page, &current_ln_page, sizeof(ln_page_descriptor));
		p_pre_page->page = page;
	
		if(gdaq_global_buf_wait_data_ready(p_buffer,page)!=0)
        {
            printc(MSG_E,"get_ln_page_offset_addr wait_data_ready error! \n");
			end.ln_page.page_id=-1;
            return end;//error end
        }
		if(p_buf_des->page_descriptor[page].page_used_length>offset)
		{
			end.ln_page.page_id=current_ln_page.page_id;
			end.offset=offset;
			return end;
		}
		offset-=p_buf_des->page_descriptor[page].page_used_length;
	}
	#if 0//old version (page length unchange)
	ln_page_addr end;
	__s64 total_offset = (__s64)start.offset+ (__s64)start.ln_page.page_id*(__s64)DEFAULT_GLOBAL_BUFFER_PAGE_SIZE+(__s64)offset;
	end.ln_page.ln_id = start.ln_page.ln_id;
	end.ln_page.type = start.ln_page.type;
	end.ln_page.page_id = (__s32)(total_offset/(__s64)DEFAULT_GLOBAL_BUFFER_PAGE_SIZE);
    if(end.ln_page.page_id>=MAX_DATA_PAGE_ID)
        end.ln_page.page_id-=MAX_DATA_PAGE_ID;
	end.offset = (__s32)(total_offset%(__s64)DEFAULT_GLOBAL_BUFFER_PAGE_SIZE);
	return end;
	#endif
}

//this version use (struct list_head* p_page_ready_list)
ln_page_addr get_ln_page_offset_address(gdaq_global_buffer *p_buffer,ln_page_addr start, __s32 offset,struct list_head* p_page_ready_list)
{
	ln_page_descriptor current_ln_page;
	ln_page_addr end;
	int page;
	buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
	memcpy(&current_ln_page,&start.ln_page,sizeof(ln_page_descriptor));
	end.ln_page.ln_id = start.ln_page.ln_id;
	end.ln_page.type = start.ln_page.type;
	if((page=gdaq_global_buffer_search(p_buffer,start.ln_page,p_page_ready_list))==QUEUE_NUM_NULL)
	{
		printc(MSG_E,"get_ln_page_offset_address search buffer error! \n");
        end.ln_page.page_id=-1;
        return end;//error end
	}
	if((start.offset+offset)<p_buf_des->page_descriptor[page].page_used_length)
	{
		end.ln_page.page_id=start.ln_page.page_id;
		end.offset=start.offset+offset;
		return end;
	}
	else
		offset-=(p_buf_des->page_descriptor[page].page_used_length-start.offset);
	while(1)
	{
		
		current_ln_page.page_id++;
		if(current_ln_page.page_id>=MAX_DATA_PAGE_ID)
			current_ln_page.page_id = 0;
		if((page=gdaq_global_buffer_search(p_buffer,current_ln_page,p_page_ready_list))==QUEUE_NUM_NULL)
		{
			printc(MSG_E,"get_ln_page_offset_address search buffer error! \n");
        	end.ln_page.page_id=-1;
        	return end;//error end
		}
		if(gdaq_global_buf_wait_data_ready(p_buffer,page)!=0)
        {
            printc(MSG_E,"get_ln_page_offset_address wait_data_ready error! \n");
			end.ln_page.page_id=-1;
            return end;//error end
        }
		if(p_buf_des->page_descriptor[page].page_used_length>offset)
		{
			end.ln_page.page_id=current_ln_page.page_id;
			end.offset=offset;
			return end;
		}
		offset-=p_buf_des->page_descriptor[page].page_used_length;
	}
}


void free_wait_list(struct list_head* p_wait_list_head)
{
	wait_task *p_current_task;
	struct list_head *pos,*q;
	list_for_each_safe(pos,q,p_wait_list_head)
	{
		p_current_task=list_entry(pos, wait_task, node);
		list_del(pos);
		free(p_current_task);
	}
	return;
}


void display_task_info(thread_task* p_task)
{
    #ifdef  DAQ_DEBUG
    printc(MSG_T, "[task info](page_id=%d)\n",p_task->page_id);
    printd("  task_addr=0x%x, current_addr=0x%x\n",(int)p_task->task_addr, (int)p_task->current_addr);
    printd("  page_size=%d, remain_data_num=%d\n",p_task->page_size,p_task->remain_data_num);
    printc(MSG_T, "[task info end](page_id=%d)\n",p_task->page_id);
    #endif
    return;
}

void display_waiting_thread_task(struct list_head* p_wait_list_head)
{
    #ifdef DAQ_DEBUG
    printc(MSG_T, "[waiting thread task info]\n");
    struct list_head *pos,*q;
    wait_task *p_current_task;
    printd("Current waiting task page_id is:\n"); 
    list_for_each_safe(pos,q,p_wait_list_head)
	{
        p_current_task = list_entry(pos, wait_task, node);
        printf("%d ", p_current_task->task.page_id);
	}
    printf("\n");
    printc(MSG_T, "[waiting thread task info end]\n");
    #endif
    return;
}

int thread_get_task_from_id(thread_task* p_task, thread_interface* p_task_queue, struct list_head* p_wait_list_head ,int task_page_id,int offset)
{
    #ifdef DAQ_DEBUG
    printd("thread_get_task_from_id: page %d is wanted!\n", task_page_id);
    #endif
	wait_task* p_new_task;
	wait_task *p_current_task;
	struct list_head *pos,*q;
    struct list_head *insert_pos;

	if((p_task->page_id==task_page_id)&&(p_task->task_addr!=NULL))
	{
	    p_task->current_addr = (char*)(p_task->task_addr+offset);
	    p_task->remain_data_num = p_task->page_size-offset;
	    return 0;
	}
		
		//if page existed in the wait list, return it
	list_for_each_safe(pos,q,p_wait_list_head)
	{
        p_current_task = list_entry(pos, wait_task, node);
		if(p_current_task->task.page_id== task_page_id)
		{
			memcpy(p_task, &p_current_task->task,sizeof(thread_task));
			p_task->current_addr+=offset;
			p_task->remain_data_num -=offset;
			list_del(pos);
			free(p_current_task);
            #ifdef DAQ_DEBUG
            printd("thread_get_task_from_id: get page %d from the waiting list!\n", task_page_id);
            #endif
            display_waiting_thread_task(p_wait_list_head);
			return 0;
		}
		else if(queue_ahead(p_current_task->task.page_id,task_page_id,MAX_DATA_PAGE_ID))
		{
			list_del(pos);
			free(p_current_task);	
		}
        else
        {
            break;
        }
	}
	//if page didn't exist in the wait list, get new task until get the appointed page
	while(1)
	{
		if(thread_get_task(p_task_queue, p_task)!=0)
		{
			printc(MSG_E,"thread_get_task_from_id: get new task error!\n");
			return -1;
		}
        #ifdef DAQ_DEBUG
        printd("thread_get_task_from_id: get page %d from the task queue!\n", p_task->page_id);
        #endif
        if(p_task->page_id == task_page_id)
		{
			p_task->current_addr+=offset;
			p_task->remain_data_num -=offset;
            #ifdef DAQ_DEBUG
            printd("thread_get_task_from_id: find page %d from the task queue!\n", task_page_id);
            #endif
            display_waiting_thread_task(p_wait_list_head);
			return 0;
		}      
		else if(queue_ahead(task_page_id,p_task->page_id,MAX_DATA_PAGE_ID))
		{
			if(!(p_new_task = (wait_task*)malloc(sizeof(wait_task))))
			{
				printc(MSG_E,"thread_get_task_from_id: malloc error(%m)!\n",errno);
				return -1;
			}
			memcpy(&p_new_task->task, p_task, sizeof(thread_task));
            list_for_each_safe(pos,q,p_wait_list_head)
        	{
                p_current_task = list_entry(pos, wait_task, node);
                if(queue_ahead(p_task->page_id,p_current_task->task.page_id,MAX_DATA_PAGE_ID))
        		{
                    break;
        		}
        	}
            insert_pos = pos;
            insert_pos->prev->next=&p_new_task->node;
            p_new_task->node.prev = insert_pos->prev;
            p_new_task->node.next = insert_pos;
            insert_pos->prev = &p_new_task->node;
		}
	}
	return 0;
}

int copy_from_pages(char* dest, thread_task* p_task, __u32 num, thread_interface* p_task_queue, struct list_head* p_wait_list_head)
{
    int search_page_id;
    if(p_task->remain_data_num >=num)
	{
		memcpy(dest, p_task->current_addr, num);
		p_task->remain_data_num -= num;
		p_task->current_addr+=num;
		return 0;
	}
	else
	{
		char* 	p_tmp = dest;
		int		remain=num;
		if(p_task->remain_data_num>0)
		{
			memcpy(dest, p_task->current_addr,p_task->remain_data_num);
			p_tmp += p_task->remain_data_num;
			remain -= p_task->remain_data_num;
		}
		do
		{
			if(p_task->page_id==MAX_DATA_PAGE_ID-1)
				search_page_id = 0;
			else
				search_page_id=p_task->page_id+1;
			if(thread_get_task_from_id(p_task, p_task_queue, p_wait_list_head,search_page_id,0)!=0)
			{
				printc(MSG_E,"copy_from_pages: thread_get_task_from_id error!\n");
				return -1;
			}
			if(remain > p_task->remain_data_num)
			{
				memcpy(p_tmp, p_task->current_addr, p_task->remain_data_num);
                p_tmp += p_task->remain_data_num;
				remain -= p_task->remain_data_num;				
			}
			else
			{
				memcpy(p_tmp, p_task->current_addr, remain);
				p_task->remain_data_num -= remain;
				p_task->current_addr += remain;
				return 0;
			}
		}
		while(remain != 0);
	}
	return 0;
}

int copy_from_buffer(char* dest, ln_page_addr entry, __u32 num, gdaq_global_buffer* p_buffer, struct list_head* p_page_ready_list)
{
	int				page;
	ln_page_addr 	current_addr;
	int 			page_remain;
	char* 	        p_data;
	char*			p_tmp = dest;
	int  		    remain=num;
	if(entry.ln_page.page_id==-1)//addr null
    {
        printc(MSG_E,"copy_from_buffer:entry addr NUll\n");
	    return -1;
	}
	buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
	memcpy(&current_addr,&entry,sizeof(ln_page_addr));
	if((page=gdaq_global_buffer_search(p_buffer,entry.ln_page,p_page_ready_list))==-1)
	{
		printc(MSG_E,"copy_from_buffer:  gdaq_global_buffer_search error!\n");
        return -1;
	}
	page_remain = p_buf_des->page_descriptor[page].page_used_length-entry.offset;//DEFAULT_GLOBAL_BUFFER_PAGE_SIZE-entry.offset;
	while(1)//(remain != 0)
	{
		if((p_data = gdaq_global_buffer_get_addr(p_buffer, current_addr,p_page_ready_list)) ==NULL)
		{
			printc(MSG_E,"copy_from_buffer:  gdaq_global_buffer_get_addr error!\n");
        	return -1;
		}
		if(page_remain >= remain)
	    {
	        memcpy(p_tmp, p_data, remain);
			return 0;
		}
		else
		{
			memcpy(p_tmp, p_data, page_remain);
			p_tmp += page_remain;
			remain -= page_remain;
			current_addr.ln_page.page_id++;
			if(current_addr.ln_page.page_id==MAX_DATA_PAGE_ID)
				current_addr.ln_page.page_id=0;
			current_addr.offset = 0;
			if((page=gdaq_global_buffer_search(p_buffer,current_addr.ln_page,p_page_ready_list))==-1)
			{
				printc(MSG_E,"copy_from_buffer:  gdaq_global_buffer_search error!\n");
        		return -1;
			}
			page_remain = p_buf_des->page_descriptor[page].page_used_length;
		}
	}
	return -1;
}

int copy_to_buffer(ln_page_addr dest, char* source, __u32 num, gdaq_global_buffer* p_buffer, struct list_head* p_page_ready_list)
{
	int 			page;
	ln_page_addr 	current_addr;
	int 			page_remain;
	char* 	        p_data;
	char*			p_tmp = source;
	int		        remain=num;
	if(dest.ln_page.page_id==-1)//addr null
    {
        printc(MSG_E,"copy_to_buffer:entry addr NUll\n");
	    return -1;
	}
	buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
	memcpy(&current_addr,&dest,sizeof(ln_page_addr));
	if((page=gdaq_global_buffer_search(p_buffer,dest.ln_page,p_page_ready_list))==-1)
	{
		printc(MSG_E,"copy_to_buffer:  gdaq_global_buffer_search error!\n");
        return -1;
	}
	page_remain = p_buf_des->page_descriptor[page].page_used_length-dest.offset;//DEFAULT_GLOBAL_BUFFER_PAGE_SIZE-dest.offset;
	while(1)//(remain != 0)
	{
		if((p_data = gdaq_global_buffer_get_addr(p_buffer, current_addr,p_page_ready_list)) ==NULL)
		{
			printc(MSG_E,"copy_to_buffer:  gdaq_global_buffer_get_addr error!\n");
        	return -1;
		}
		if(page_remain >= remain)
	    {
	        memcpy(p_data, p_tmp, remain);
			return 0;
		}
		else
		{
			memcpy(p_data, p_tmp, page_remain);
			p_tmp += page_remain;
			remain -= page_remain;
			current_addr.ln_page.page_id++;
			if(current_addr.ln_page.page_id==MAX_DATA_PAGE_ID)
				current_addr.ln_page.page_id=0;
			current_addr.offset = 0;
			if((page=gdaq_global_buffer_search(p_buffer,current_addr.ln_page,p_page_ready_list))==-1)
			{
				printc(MSG_E,"copy_to_buffer:  gdaq_global_buffer_search error!\n");
        		return -1;
			}
			page_remain = p_buf_des->page_descriptor[page].page_used_length;
		}
	}
	return -1;
}

//consider merge with data_scan_and_generate_task() in the future
//ln_page_descriptor ln_page_interface = {node_id,LN_PAGE_TYPE_STATUS_INTERFACE,0};
//ln_page_descriptor ln_page_data = {node_id, LN_PAGE_TYPE_STATUS,0};
//ln_page_descriptor null_ln_page = {node_id, LN_PAGE_TYPE_STATUS,-1};


int status_scan_and_generate_task(int node_id, gdaq_global_buffer* p_buffer, thread_interface* p_task_queue, shm_interface* p_out_shm_interface)
{
    thread_task current_task = {NULL,-1,0,NULL, 0, QUEUE_NUM_NULL};
    
    LIST_HEAD(wait_list);   /*http://www.ibm.com/developerworks/cn/linux/kernel/l-chain/index.html*/
    wait_task   *task;
    int     new_buf_page;
    
    ln_page_descriptor ln_page_interface = {node_id,LN_PAGE_TYPE_STATUS_INTERFACE,0};
    ln_page_descriptor ln_page_data = {node_id, LN_PAGE_TYPE_STATUS,0};
    ln_page_descriptor null_ln_page = {node_id, LN_PAGE_TYPE_STATUS,-1};
    
    ln_page_addr    null_ln_page_addr = {null_ln_page,0};
    ln_page_addr    section_start = {ln_page_data, 0};
    ln_page_addr    data_start = {ln_page_data, 0};
    ln_page_addr    data_end = {ln_page_data, 0};
    ln_page_addr    ln_addr = {ln_page_data, 0};
    ln_page_addr    interface_page_addr ={ln_page_interface, 0};
        
    data_section_head   section_head;
    shm_interface_page_descriptor *p_interface;
    int     remain_fragment;
    int     fragment_num=0;
    
    data_ready_page pre_page;
    pre_page.ln_page.ln_id=node_id;
    pre_page.ln_page.type=LN_PAGE_TYPE_DATA;
    pre_page.ln_page.page_id=-1;
    pre_page.page=-1;
            
    while(1)
    {
        if(current_task.remain_data_num==0)
        {
            section_start.ln_page.page_id = current_task.page_id+1;
            if(section_start.ln_page.page_id>=MAX_DATA_PAGE_ID)
                section_start.ln_page.page_id = 0;
            section_start.offset = 0;
        }
        else
        {
            section_start.ln_page.page_id = current_task.page_id;
            section_start.offset = current_task.current_addr-current_task.task_addr;
        }
        if(copy_from_pages((char*)&section_head, &current_task, sizeof(data_section_head),p_task_queue, &wait_list)!=0)
        {
            free_wait_list(&wait_list);
            printc(MSG_E,"data_scan_and_generate_task: copy_from_pages error!(page_id=%d)\n",current_task.page_id);
            return -1;
        }
        /*
        #ifdef DAQ_DEBUG
        printd("data_scan_and_generate_task: Get a new section!(index:mindex:type:fragment=%d:%d:%d:%d)\n",
        section_head.index,section_head.map_index, section_head.data_type, section_head.fragment_num);
        #endif
        */     
        remain_fragment = section_head.fragment_num;
        if(current_task.remain_data_num==0)
        {
            ln_addr.ln_page.page_id = current_task.page_id+1;
            if(ln_addr.ln_page.page_id>=MAX_DATA_PAGE_ID)
                ln_addr.ln_page.page_id = 0;
            ln_addr.offset = 0;
        }
        else
        {
            ln_addr.ln_page.page_id = current_task.page_id;
            ln_addr.offset = current_task.current_addr-current_task.task_addr;
        }
        //1byte before real data_start
        data_start = get_ln_page_offset_addr(p_buffer,ln_addr, section_head.fragment_num*sizeof(data_fragment_head)-1,&pre_page,p_task_queue,&wait_list);
       
        int data_offset=0;
        int page_fragment_num;
        int send_task = 1;
        shm_interface_page_descriptor *p_first_page=NULL;
        do
        {
            /*
            while((new_buf_page = gdaq_global_buf_malloc(p_buffer,ln_page_interface))==QUEUE_NUM_NULL)
            {
                usleep(10);
            }
            */
            if((new_buf_page = gdaq_global_buf_malloc(p_buffer,ln_page_interface))==QUEUE_NUM_NULL)
            {
                //(replaced by next line)printc(MSG_A, "Data source(id=%d): No free pages!\n");
                printc(MSG_A,"Data_scan_and_generate_task(id=%d):No free pages!\n", node_id);
                free_wait_list(&wait_list);
                return -1;
            }
            memcpy(&interface_page_addr.ln_page,&ln_page_interface,sizeof(ln_page_descriptor));
                
                
            p_interface = (shm_interface_page_descriptor*)(gdaq_global_buf_get_addr(p_buffer,new_buf_page,0));
            if(p_first_page==NULL)
                p_first_page = p_interface;
            memcpy(&p_interface->section_head,&section_head, sizeof(data_section_head));
            p_interface->map_factor =1;
            int ret;
            if((ret=mutex_process_shared_init(&p_interface->mutex))!=0)
            {
                printc(MSG_E, "data_scan_and_generate_task: init mutex error!(%m)\n",ret);
                free_wait_list(&wait_list);
                return -1;
            } 
    
            if(remain_fragment> MAX_DATA_FRAGMENT_PER_PAGE)
            {
                page_fragment_num = MAX_DATA_FRAGMENT_PER_PAGE;
                remain_fragment -= MAX_DATA_FRAGMENT_PER_PAGE;
            }
            else
            {
                page_fragment_num = remain_fragment;
                remain_fragment = 0;
            }
                
            for(fragment_num =0;fragment_num<page_fragment_num;fragment_num++)
            {
                if(copy_from_pages((char*)&p_interface->fragment[fragment_num].fragment_head,  &current_task,sizeof(data_fragment_head),
                    p_task_queue, &wait_list)!=0)
                {
                    free_wait_list(&wait_list);
                    printc(MSG_E,"data_scan_and_generate_task: copy_from_pages error!(page_id=%d)\n", current_task.page_id);
                    return -1;
                }
                if(p_interface->fragment[fragment_num].fragment_head.data_len!=0)
                    p_interface->fragment[fragment_num].fragment_addr=get_ln_page_offset_addr(p_buffer,data_start,data_offset+1,&pre_page,p_task_queue,&wait_list);
                else
                    p_interface->fragment[fragment_num].fragment_addr=null_ln_page_addr;//NULL
                data_offset+=p_interface->fragment[fragment_num].fragment_head.data_len;
                                              
                #ifdef DAQ_DEBUG
                //printd("data_scan_and_generate_task: Get a new fragment!(start_channel:channel_num:data_length=%d:%d:%d)\n",
                //p_interface->fragment[fragment_num].fragment_head.start_channel,p_interface->fragment[fragment_num].fragment_head.channel_num, p_interface->fragment[fragment_num].fragment_head.data_len);
                #endif
                    
                if((remain_fragment==0)&&(fragment_num == page_fragment_num -1))
                {
                    p_interface->fragment[fragment_num].next.fragment_id = QUEUE_NUM_NULL;
                    p_interface->fragment[fragment_num].next.ln_page.page_id = QUEUE_NUM_NULL;
                    memcpy(&p_first_page->last_fragment_addr.ln_page, &ln_page_interface,sizeof(ln_page_descriptor)); 
                    p_first_page->last_fragment_addr.fragment_id = fragment_num;
                }
                else
                {
                    memcpy(&p_interface->fragment[fragment_num].next.ln_page,&ln_page_interface,sizeof(ln_page_descriptor)); 
                    if(fragment_num == p_interface->section_head.fragment_num -1)
                    {
                        p_interface->fragment[fragment_num].next.fragment_id = 0;
                        p_interface->fragment[fragment_num].next.ln_page.page_id++;
                        if(p_interface->fragment[fragment_num].next.ln_page.page_id==MAX_DATA_PAGE_ID)
                            p_interface->fragment[fragment_num].next.ln_page.page_id=0;
                    }
                    else
                        p_interface->fragment[fragment_num].next.fragment_id = fragment_num+1;
                }
            }
            if(remain_fragment==0)
            {
                if(data_offset==0)
                {//is empty section
                    data_end = data_start;
                }
                else
                {//not empty section
                    data_end = get_ln_page_offset_addr(p_buffer,data_start,data_offset,&pre_page,p_task_queue,&wait_list);
                }
                if(thread_get_task_from_id(&current_task, p_task_queue, &wait_list, data_end.ln_page.page_id,data_end.offset)!=0)
                {
                    free_wait_list(&wait_list);
                    printc(MSG_E,"data_scan_and_generate_task: Move pointer to next task error!\n");
                    return -1;
                }
                current_task.current_addr++;
                current_task.remain_data_num--;
                if(current_task.remain_data_num!=0)
                    gdaq_global_page_user_num_add_one(p_buffer, current_task.page);
                //begin free head in data page
                if(data_offset==0)
                {//is empty section
                    while((section_start.ln_page.page_id==data_start.ln_page.page_id)||(queue_ahead(section_start.ln_page.page_id,data_start.ln_page.page_id,MAX_DATA_PAGE_ID)))
                    {
				        #ifdef DAQ_DEBUG
                        printc(MSG_T,"free head (empty section)\n");
						#endif
                        if(gdaq_global_buf_free(p_buffer,section_start.ln_page)!=0)
                        {
                            printc(MSG_E,"data_scan_and_generate_task: gdaq_global_buf_free error!(free head in date page)(empty section)\n");
                            return -1;
                        }
                        section_start.ln_page.page_id++;
                        if(section_start.ln_page.page_id>=MAX_DATA_PAGE_ID)
                            section_start.ln_page.page_id = 0;
                    }
                }
                else
                {//not empty section
                    //data_start+1 to the real data_start postion
                    data_start = get_ln_page_offset_addr(p_buffer,data_start,1,&pre_page,p_task_queue,&wait_list);
                    while(queue_ahead(section_start.ln_page.page_id,data_start.ln_page.page_id,MAX_DATA_PAGE_ID))
                    {
				        #ifdef DAQ_DEBUG
                        printc(MSG_T,"free head (not empty section)\n");
						#endif
                        if(gdaq_global_buf_free(p_buffer,section_start.ln_page)!=0)
                        {
                            printc(MSG_E,"data_scan_and_generate_task: gdaq_global_buf_free error!(free head in date page)(not empty section)\n");
                            return -1;
                        }
                        section_start.ln_page.page_id++;
                        if(section_start.ln_page.page_id>=MAX_DATA_PAGE_ID)
                            section_start.ln_page.page_id = 0;
                    }
                }   
            }
            if(gdaq_global_buf_set_data_ready(p_buffer,new_buf_page,1)!=0)
            {
                free_wait_list(&wait_list);
                printc(MSG_E,"data_scan_and_generate_task: gdaq_global_buf_set_data_ready error!(page_id=%d)\n", current_task.page_id);
                pthread_exit ((void*)-1);
            }
            if(send_task)
            {
                send_task = 0;
				#ifdef DAQ_DEBUG
                //printd("data_scan_and_generate_task: A task has add to the LN interface.\n");
                //printd("data_scan_and_generate_task: Task LN:type:page=%d:%d:%d.\n",interface_page_addr.ln_page.ln_id, interface_page_addr.ln_page.type,interface_page_addr.ln_page.page_id);
                #endif
                if(ln_interface_add_task(interface_page_addr,p_out_shm_interface)!=0)
                {
                    free_wait_list(&wait_list);
                    printc(MSG_E,"data_scan_and_generate_task: add task to SHM interface error!(page_id=%d)\n", current_task.page_id);
                    pthread_exit ((void*)-1);
                }
            }
            if(ln_page_interface.page_id==MAX_DATA_PAGE_ID-1)
                ln_page_interface.page_id=0;
            else
                ln_page_interface.page_id ++;
        }
        while(remain_fragment!=0);
    }
    free_wait_list(&wait_list);
    return 0;
}    



int data_scan_and_generate_task(int node_id, gdaq_global_buffer* p_buffer, thread_interface* p_task_queue, shm_interface* p_out_shm_interface)
{
    thread_task current_task = {NULL,-1,0,NULL, 0, QUEUE_NUM_NULL};

	LIST_HEAD(wait_list);	/*http://www.ibm.com/developerworks/cn/linux/kernel/l-chain/index.html*/
	wait_task	*task;
	int		new_buf_page;

	ln_page_descriptor ln_page_interface = {node_id,LN_PAGE_TYPE_INTERFACE,0};
	ln_page_descriptor ln_page_data = {node_id, LN_PAGE_TYPE_DATA,0};
	ln_page_descriptor null_ln_page = {node_id, LN_PAGE_TYPE_DATA,-1};

	ln_page_addr	null_ln_page_addr = {null_ln_page,0};
	ln_page_addr	section_start = {ln_page_data, 0};
	ln_page_addr	data_start = {ln_page_data, 0};
	ln_page_addr	data_end = {ln_page_data, 0};
	ln_page_addr	ln_addr = {ln_page_data, 0};
	ln_page_addr	interface_page_addr ={ln_page_interface, 0};
	
	data_section_head	section_head;
	shm_interface_page_descriptor *p_interface;
	int		remain_fragment;
	int		fragment_num=0;

	data_ready_page pre_page;
	pre_page.ln_page.ln_id=node_id;
	pre_page.ln_page.type=LN_PAGE_TYPE_DATA;
	pre_page.ln_page.page_id=-1;
	pre_page.page=-1;
		
	while(1)
	{
		if(current_task.remain_data_num==0)
		{
			section_start.ln_page.page_id = current_task.page_id+1;
			if(section_start.ln_page.page_id>=MAX_DATA_PAGE_ID)
            	section_start.ln_page.page_id = 0;
			section_start.offset = 0;
		}
		else
		{
			section_start.ln_page.page_id = current_task.page_id;
			section_start.offset = current_task.current_addr-current_task.task_addr;
		}
        if(copy_from_pages((char*)&section_head, &current_task, sizeof(data_section_head),p_task_queue, &wait_list)!=0)
		{
			free_wait_list(&wait_list);
			printc(MSG_E,"data_scan_and_generate_task: copy_from_pages error!(page_id=%d)\n",current_task.page_id);
			return -1;
		}
/*		#ifdef DAQ_DEBUG
        printd("data_scan_and_generate_task: Get a new section!(index:mindex:type:fragment=%d:%d:%d:%d)\n",
            section_head.index,section_head.map_index, section_head.data_type, section_head.fragment_num);
        #endif
 */     
		remain_fragment = section_head.fragment_num;
		if(current_task.remain_data_num==0)
		{
			ln_addr.ln_page.page_id = current_task.page_id+1;
			if(ln_addr.ln_page.page_id>=MAX_DATA_PAGE_ID)
            	ln_addr.ln_page.page_id = 0;
			ln_addr.offset = 0;
		}
		else
		{
			ln_addr.ln_page.page_id = current_task.page_id;
			ln_addr.offset = current_task.current_addr-current_task.task_addr;
		}
		//1byte before real data_start
		//add result check later
		data_start = get_ln_page_offset_addr(p_buffer,ln_addr, section_head.fragment_num*sizeof(data_fragment_head)-1,&pre_page,p_task_queue,&wait_list);
   
		int data_offset=0;
        int page_fragment_num;
        int send_task = 1;
        shm_interface_page_descriptor *p_first_page=NULL;
		do
		{
            /*
			while((new_buf_page = gdaq_global_buf_malloc(p_buffer,ln_page_interface))==QUEUE_NUM_NULL)
			{
				usleep(10);
			}
			*/
	        if((new_buf_page = gdaq_global_buf_malloc(p_buffer,ln_page_interface))==QUEUE_NUM_NULL)
            {
				//(replaced by next line)printc(MSG_A, "Data source(id=%d): No free pages!\n");
				printc(MSG_A,"Data_scan_and_generate_task(id=%d):No free pages!\n", node_id);
                free_wait_list(&wait_list);
                return -1;
            }
			memcpy(&interface_page_addr.ln_page,&ln_page_interface,sizeof(ln_page_descriptor));
			
			
			p_interface = (shm_interface_page_descriptor*)(gdaq_global_buf_get_addr(p_buffer,new_buf_page,0));
            if(p_first_page==NULL)
                p_first_page = p_interface;
            memcpy(&p_interface->section_head,&section_head, sizeof(data_section_head));
            p_interface->map_factor =1;
            int ret;
			if((ret=mutex_process_shared_init(&p_interface->mutex))!=0)
		    {
		        printc(MSG_E, "data_scan_and_generate_task: init mutex error!(%m)\n",ret);
				free_wait_list(&wait_list);
		        return -1;
		    } 

			if(remain_fragment> MAX_DATA_FRAGMENT_PER_PAGE)
			{
				page_fragment_num = MAX_DATA_FRAGMENT_PER_PAGE;
				remain_fragment -= MAX_DATA_FRAGMENT_PER_PAGE;
			}
			else
			{
				page_fragment_num = remain_fragment;
				remain_fragment = 0;
			}
            
			for(fragment_num =0;fragment_num<page_fragment_num;fragment_num++)
			{
				if(copy_from_pages((char*)&p_interface->fragment[fragment_num].fragment_head,  &current_task,sizeof(data_fragment_head),
					p_task_queue, &wait_list)!=0)
				{
					free_wait_list(&wait_list);
					printc(MSG_E,"data_scan_and_generate_task: copy_from_pages error!(page_id=%d)\n", current_task.page_id);
					return -1;
				}
				if(p_interface->fragment[fragment_num].fragment_head.data_len!=0)
                	p_interface->fragment[fragment_num].fragment_addr=get_ln_page_offset_addr(p_buffer,data_start,data_offset+1,&pre_page,p_task_queue,&wait_list);
				else
					p_interface->fragment[fragment_num].fragment_addr=null_ln_page_addr;//NULL
                data_offset+=p_interface->fragment[fragment_num].fragment_head.data_len;
                           
                
                #ifdef DAQ_DEBUG
     //           printd("data_scan_and_generate_task: Get a new fragment!(start_channel:channel_num:data_length=%d:%d:%d)\n",
     //               p_interface->fragment[fragment_num].fragment_head.start_channel,p_interface->fragment[fragment_num].fragment_head.channel_num, p_interface->fragment[fragment_num].fragment_head.data_len);
                #endif
                
                if((remain_fragment==0)&&(fragment_num == page_fragment_num -1))
                {
                    p_interface->fragment[fragment_num].next.fragment_id = QUEUE_NUM_NULL;
                    p_interface->fragment[fragment_num].next.ln_page.page_id = QUEUE_NUM_NULL;
                    memcpy(&p_first_page->last_fragment_addr.ln_page, &ln_page_interface,sizeof(ln_page_descriptor)); 
                    p_first_page->last_fragment_addr.fragment_id = fragment_num;
                }
                else
                {
                    memcpy(&p_interface->fragment[fragment_num].next.ln_page,&ln_page_interface,sizeof(ln_page_descriptor)); 
                    if(fragment_num == p_interface->section_head.fragment_num -1)
                    {
                        p_interface->fragment[fragment_num].next.fragment_id = 0;
						p_interface->fragment[fragment_num].next.ln_page.page_id++;
						if(p_interface->fragment[fragment_num].next.ln_page.page_id==MAX_DATA_PAGE_ID)
							p_interface->fragment[fragment_num].next.ln_page.page_id=0;
                    }
					else
                        p_interface->fragment[fragment_num].next.fragment_id = fragment_num+1;

                }
			}
			if(remain_fragment==0)
			{
				if(data_offset==0)
            	{//is empty section
                	data_end = data_start;
            	}
            	else
            	{//not empty section
                	data_end = get_ln_page_offset_addr(p_buffer,data_start,data_offset,&pre_page,p_task_queue,&wait_list);
            	}
           		if(thread_get_task_from_id(&current_task, p_task_queue, &wait_list, data_end.ln_page.page_id,data_end.offset)!=0)
            	{
                	free_wait_list(&wait_list);
                	printc(MSG_E,"data_scan_and_generate_task: Move pointer to next task error!\n");
                	return -1;
            	}
            	current_task.current_addr++;
            	current_task.remain_data_num--;
            	if(current_task.remain_data_num!=0)
                	gdaq_global_page_user_num_add_one(p_buffer, current_task.page);
				//begin free head in data page
				if(data_offset==0)
				{//is empty section
					while((section_start.ln_page.page_id==data_start.ln_page.page_id)||(queue_ahead(section_start.ln_page.page_id,data_start.ln_page.page_id,MAX_DATA_PAGE_ID)))
					{
						#ifdef DAQ_DEBUG
						printc(MSG_T,"free head (empty section)\n");
						#endif
						if(gdaq_global_buf_free(p_buffer,section_start.ln_page)!=0)
            			{
                			printc(MSG_E,"data_scan_and_generate_task: gdaq_global_buf_free error!(free head in date page)(empty section)\n");
                			return -1;
            			}
						section_start.ln_page.page_id++;
						if(section_start.ln_page.page_id>=MAX_DATA_PAGE_ID)
            				section_start.ln_page.page_id = 0;
					}
				}
				else
				{//not empty section
					//data_start+1 to the real data_start postion
					data_start = get_ln_page_offset_addr(p_buffer,data_start,1,&pre_page,p_task_queue,&wait_list);
					while(queue_ahead(section_start.ln_page.page_id,data_start.ln_page.page_id,MAX_DATA_PAGE_ID))
					{
						#ifdef DAQ_DEBUG
						printc(MSG_T,"free head (not empty section)\n");
						#endif
						if(gdaq_global_buf_free(p_buffer,section_start.ln_page)!=0)
            			{
                			printc(MSG_E,"data_scan_and_generate_task: gdaq_global_buf_free error!(free head in date page)(not empty section)\n");
                			return -1;
            			}
						section_start.ln_page.page_id++;
						if(section_start.ln_page.page_id>=MAX_DATA_PAGE_ID)
            				section_start.ln_page.page_id = 0;
					}
				}	
			}
            if(gdaq_global_buf_set_data_ready(p_buffer,new_buf_page,1)!=0)
            {
                free_wait_list(&wait_list);
				printc(MSG_E,"data_scan_and_generate_task: gdaq_global_buf_set_data_ready error!(page_id=%d)\n", current_task.page_id);
				pthread_exit ((void*)-1);
            }
            if(send_task)
            {
                send_task = 0;
				#ifdef DAQ_DEBUG
        //        printd("data_scan_and_generate_task: A task has add to the LN interface.\n");
        //        printd("data_scan_and_generate_task: Task LN:type:page=%d:%d:%d.\n",interface_page_addr.ln_page.ln_id, interface_page_addr.ln_page.type,interface_page_addr.ln_page.page_id);
                #endif
			    if(ln_interface_add_task(interface_page_addr,p_out_shm_interface)!=0)
			    {
    				free_wait_list(&wait_list);
    				printc(MSG_E,"data_scan_and_generate_task: add task to SHM interface error!(page_id=%d)\n", current_task.page_id);
    				pthread_exit ((void*)-1);
    			}

            }
            if(ln_page_interface.page_id==MAX_DATA_PAGE_ID-1)
                ln_page_interface.page_id=0;
            else
			    ln_page_interface.page_id ++;
		}
        while(remain_fragment!=0);
	}
	free_wait_list(&wait_list);
    return 0;
}

void free_data_out_list(struct list_head* p_data_out_list_head)
{
	data_out_section *p_data_section;
	struct list_head *pos,*q; 
	list_for_each_safe(pos,q,p_data_out_list_head)
	{
		p_data_section=list_entry(pos, data_out_section, node);
		list_del(pos);
		free(p_data_section);
	}
	return;
}

int __generate_data_list(struct list_head* p_data_out_list, ln_page_addr entry, __u32 length, gdaq_global_buffer* p_buffer, struct list_head* p_page_ready_list, int data_valid)
{
	int 			page;
	data_out_section* p_data_section;
	int page_remain;
	char* p_data;
	buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
	if(p_data_out_list == NULL)
	{
		printc(MSG_N,"__generate_data_list:out_list NULL!\n");
		return -1;
	}
	if(entry.ln_page.page_id==-1)return 0;//addr null
	if((page=gdaq_global_buffer_search(p_buffer,entry.ln_page,p_page_ready_list))==-1)
	{
		printc(MSG_E,"__generate_data_list:  gdaq_global_buffer_search error!\n");
        return -1;
	}
	page_remain = p_buf_des->page_descriptor[page].page_used_length-entry.offset;//DEFAULT_GLOBAL_BUFFER_PAGE_SIZE-entry.offset;
	//if(length==0)gdaq_global_buffer_get_addr(p_buffer, entry,p_page_ready_list);
	while(length!=0)
	{
		if((p_data = gdaq_global_buffer_get_addr(p_buffer,entry,p_page_ready_list)) ==NULL)
		{
			printc(MSG_N,"__generate_data_list:  gdaq_global_buffer_get_addr error!\n");
			return -1;
		}
		if(data_valid)
		{
			if((p_data_section=(data_out_section*)malloc(sizeof(data_out_section)))==NULL)
   			{
            	printc(MSG_E, "__generate_data_list: malloc error!(%m)\n",errno);
            	return -1;
        	}
       		p_data_section->addr = p_data;
        	p_data_section->length = min(page_remain,length);
        	list_add_tail(&p_data_section->node, p_data_out_list);
		}
		length -= min(page_remain,length);
		if(length==0)break;
		entry.ln_page.page_id++;
		if(entry.ln_page.page_id==MAX_DATA_PAGE_ID)
			entry.ln_page.page_id=0;
		entry.offset = 0;
		if((page=gdaq_global_buffer_search(p_buffer,entry.ln_page,p_page_ready_list))==-1)
		{
			printc(MSG_E,"__generate_data_list:  gdaq_global_buffer_search error!\n");
        	return -1;
		}
		page_remain = p_buf_des->page_descriptor[page].page_used_length;
	}
	return 0;
}

/*
name:       generate_data_out_list_splice
function:   generate a list, storing all the data section address information to be data_save
            include fragment splice!
input:        task_entry:  current task descriptor page address
                p_page_ready_list: the head of the data_ready_list
output:     0: success
                -1: failed
                p_data_out_list: the output list head
*/

int generate_data_out_list_splice(gdaq_global_buffer* p_buffer, ln_page_addr task_entry, struct list_head* p_data_out_list, int* p_total_data_len, struct list_head* p_page_ready_list)
{
	shm_interface_page_descriptor* p_interface; 
	shm_interface_page_fragment *p_fragment=NULL, *p_prev_fragment=NULL;
	char* 			p_data;
	data_out_section *p_data_section,*p_data_section2;

	shm_fragment_pointer current_fragment;      //point to the current fragment to be processed 
	ln_page_addr 	current_addr,offset_table_addr;
	__s32           valid_channel_num,pre_valid_channel_num;
	__s32			offset_table_channel_num;
	channel_offset_table* p_offset_table_tmp;
	int				i;  //used in for(;;)
	int 			data_length,pre_data_length=-1;
	int				be_spliced=0;
    int             data_valid=1;
	
	LIST_HEAD(head_section_list);
	LIST_HEAD(validity_table);
	LIST_HEAD(offset_table);
	LIST_HEAD(validity_list);
	LIST_HEAD(offset_list);
	LIST_HEAD(data_list);
	#define FREE_LIST 	free_data_out_list(&head_section_list);\
						free_data_out_list(&validity_table);\
						free_data_out_list(&offset_table);\
						free_data_out_list(&validity_list);\
						free_data_out_list(&offset_list);\
						free_data_out_list(&data_list);
	//put data section head into list
    if((p_interface = (shm_interface_page_descriptor*)gdaq_global_buffer_get_addr(p_buffer, task_entry,p_page_ready_list)) ==NULL)
    {
        printc(MSG_N,"generate_data_out_list:  gdaq_global_buffer_get_addr error!\n");
		return -1;
    }
    
    if((p_data_section=(data_out_section*)malloc(sizeof(data_out_section)))==NULL)
    {
        printc(MSG_E, "generate_data_out_list:  malloc error!(%m)\n",errno);
        return -1;
    }
    /*removed realize data type later
    if((p_interface->section_head.data_type == GDAQ_DATA_TYPE_CANCEL)||
        (p_interface->section_head.data_type == GDAQ_DATA_TYPE_ERROR))
    {
        p_interface->section_head.fragment_num = 0;//!!!!!!!!!
        data_valid = 0;
    }
    */
    p_data_section->addr = (char*)p_interface;
    p_data_section->length = sizeof(data_section_head);
    list_add_tail(&p_data_section->node, &head_section_list);
 
    memcpy(&current_fragment.ln_page, &task_entry.ln_page, sizeof(ln_page_descriptor));
    current_fragment.fragment_id = 0;
    do //go through relevant page no matter the data is valid or not,it will be recorded in page_ready_list and used when free buffer. zyx
    {
        //put the fragment head into the list
    	memcpy(&current_addr.ln_page,&current_fragment.ln_page,sizeof(ln_page_descriptor));
		current_addr.offset = (unsigned long)&p_interface->fragment[current_fragment.fragment_id]-(unsigned long)p_interface;
		if((p_fragment = (shm_interface_page_fragment*)gdaq_global_buffer_get_addr(p_buffer, current_addr,p_page_ready_list)) ==NULL)
		{
			printc(MSG_N," generate_data_out_list:  gdaq_global_buffer_get_addr error!\n");
            FREE_LIST
			return -1;
		}
		valid_channel_num = offset_table_channel_num = -1;
		if(data_valid)
        {
        	if(p_fragment->fragment_head.data_len != 0)
        	{
        		//get validity table
        		if(p_fragment->fragment_head.channel_full == CHANNEL_NOT_FULL)
            	{
            		if(copy_from_buffer((char*)&valid_channel_num, p_fragment->fragment_addr, sizeof(valid_channel_num),p_buffer,p_page_ready_list) != 0)
            		{
            			printc(MSG_N,"generate_data_out_list:  copy from buffer error!\n");
						FREE_LIST
						return -1;    
                	}
					if(__generate_data_list(&validity_table,p_fragment->fragment_addr,sizeof(data_validity_table)+valid_channel_num*sizeof(__s32),p_buffer,p_page_ready_list,data_valid))
    				{
       					printc(MSG_E, "generate_data_out_list:  get validity table error!\n");
						FREE_LIST
        				return -1;
    				}
            	}
				//get offset table
				if(p_fragment->fragment_head.length_change == LENGTH_CHANGE)
				{
					if(valid_channel_num == -1)
					{
						offset_table_channel_num = p_fragment->fragment_head.channel_num;
						memcpy(&offset_table_addr,&p_fragment->fragment_addr,sizeof(ln_page_addr));
					}
					else
					{
						offset_table_channel_num = valid_channel_num;
						offset_table_addr= get_ln_page_offset_address(p_buffer,p_fragment->fragment_addr, sizeof(data_validity_table)+valid_channel_num*sizeof(__s32),p_page_ready_list);
					}
					if(__generate_data_list(&offset_table,offset_table_addr,offset_table_channel_num*sizeof(__s32),p_buffer,p_page_ready_list,data_valid))
    				{
       					printc(MSG_E, "generate_data_out_list:  get offset table error!\n");
						FREE_LIST
        				return -1;
    				}			
				}
        	}
			//pure data length
			data_length = p_fragment->fragment_head.data_len;
        	if(valid_channel_num != -1)data_length -= (sizeof(data_validity_table)+valid_channel_num*sizeof(__s32));
        	if(offset_table_channel_num != -1)data_length -= offset_table_channel_num*sizeof(__s32);
       
			//splice the adjacent channel blocks
			be_spliced = 0;
			if((p_prev_fragment!=NULL)&&((p_prev_fragment->fragment_head.start_channel+p_prev_fragment->fragment_head.channel_num)==p_fragment->fragment_head.start_channel))
			{
				if(pre_data_length == 0)
				{
					if((data_length==0)||(p_fragment->fragment_head.channel_full==CHANNEL_NOT_FULL))
					{
						pre_data_length += data_length;
						pre_valid_channel_num = valid_channel_num;
						p_prev_fragment->fragment_head.channel_num+=p_fragment->fragment_head.channel_num;
               			p_prev_fragment->fragment_head.data_len+=p_fragment->fragment_head.data_len;
						p_prev_fragment->fragment_head.channel_full = p_fragment->fragment_head.channel_full;
						p_prev_fragment->fragment_head.length_change = p_fragment->fragment_head.length_change;
						p_prev_fragment->fragment_addr = p_fragment->fragment_addr;
						p_interface->section_head.fragment_num--;     
                		p_fragment->fragment_head.start_channel=p_prev_fragment->fragment_head.start_channel;
                		p_fragment->fragment_head.channel_num=0;
						if(!list_empty(&validity_table))
						{
							__list_splice(&validity_table,validity_list.prev,&validity_list);
							INIT_LIST_HEAD(&validity_table);
						}
						if(!list_empty(&offset_table))
						{
							__list_splice(&offset_table,offset_list.prev,&offset_list);
							INIT_LIST_HEAD(&offset_table);
						}
						be_spliced = 1;
					}
				}
				else if((data_length==0)&&(p_prev_fragment->fragment_head.channel_full == CHANNEL_NOT_FULL))
				{
					p_prev_fragment->fragment_head.channel_num+=p_fragment->fragment_head.channel_num;
					p_interface->section_head.fragment_num--;     
                	p_fragment->fragment_head.start_channel=p_prev_fragment->fragment_head.start_channel;
                	p_fragment->fragment_head.channel_num=0;
					be_spliced = 1;
				}
				else if((p_prev_fragment->fragment_head.length_change==p_fragment->fragment_head.length_change)&&(p_prev_fragment->fragment_head.channel_full==p_fragment->fragment_head.channel_full))
				{
					if(valid_channel_num != -1)
					{
					    //-----updata valid_channel_num----
						pre_valid_channel_num +=valid_channel_num;
						copy_to_buffer(p_prev_fragment->fragment_addr,(char*)&pre_valid_channel_num,sizeof(pre_valid_channel_num),p_buffer,p_page_ready_list);
                        //----remove&free second fragment's valid_channel_num-------
                        __s32 length_left_to_be_freed;
                        length_left_to_be_freed = sizeof(valid_channel_num);
                        p_data_section=list_entry(validity_table.next, data_out_section, node);
                        while(length_left_to_be_freed>=p_data_section->length)
                        {
                            length_left_to_be_freed -= p_data_section->length;
                            p_data_section2=list_entry(p_data_section->node.next, data_out_section, node);
                            list_del(&(p_data_section->node));
						    free(p_data_section);
						    p_data_section=p_data_section2;
                        }
                        p_data_section->addr += length_left_to_be_freed;
                        p_data_section->length -= length_left_to_be_freed;                       
                        //------splice two validity table----
						__list_splice(&validity_table,validity_list.prev,&validity_list);
						INIT_LIST_HEAD(&validity_table);
					}
					if(offset_table_channel_num != -1)
					{
						if((p_offset_table_tmp=(channel_offset_table*)malloc(offset_table_channel_num*sizeof(__s32)))==NULL)
						{
							printc(MSG_E, "generate_data_out_list: malloc error!(%m)\n", errno);
                    		FREE_LIST
                			return -1;
						}
						copy_from_buffer((char*)p_offset_table_tmp,offset_table_addr,offset_table_channel_num*sizeof(__s32),p_buffer,p_page_ready_list);
						for(i=0;i<offset_table_channel_num;i++)p_offset_table_tmp->offset[i]+=pre_data_length;
						copy_to_buffer(offset_table_addr,(char*)p_offset_table_tmp,offset_table_channel_num*sizeof(__s32),p_buffer,p_page_ready_list);
						free(p_offset_table_tmp);
						__list_splice(&offset_table,offset_list.prev,&offset_list);
						INIT_LIST_HEAD(&offset_table);
					}
					pre_data_length += data_length;
					p_prev_fragment->fragment_head.channel_num+=p_fragment->fragment_head.channel_num;
                	p_prev_fragment->fragment_head.data_len+=p_fragment->fragment_head.data_len;
					if(valid_channel_num != -1)p_prev_fragment->fragment_head.data_len -=sizeof(valid_channel_num);
                	p_interface->section_head.fragment_num--;     
                	p_fragment->fragment_head.start_channel=p_prev_fragment->fragment_head.start_channel;
                	p_fragment->fragment_head.channel_num=0;		
					be_spliced = 1;
				}
			}
			if(be_spliced == 0)
			{
			    //------delete empty fragment that can't be spliced or not
				if(pre_data_length==0)
				{
					p_data_section=list_entry(head_section_list.prev, data_out_section, node);
					list_del(head_section_list.prev);
            		free(p_data_section);
					p_interface->section_head.fragment_num--; 
				}
				if((data_length==0)&&(p_fragment->next.fragment_id==QUEUE_NUM_NULL)&&(p_interface->section_head.fragment_num>1))
				{
					p_interface->section_head.fragment_num--; 
				}
				else
				{
					if((p_data_section=(data_out_section*)malloc(sizeof(data_out_section)))==NULL)
            		{
                		printc(MSG_E, "generate_data_out_list: malloc error!(%m)\n", errno);
                    	FREE_LIST
                		return -1;
            		}
            		p_data_section->addr = (char*)&p_fragment->fragment_head;
            		p_data_section->length = sizeof(data_fragment_head);
            		list_add_tail(&p_data_section->node, &head_section_list); 
                	p_prev_fragment = p_fragment;
					pre_valid_channel_num = valid_channel_num;
					pre_data_length = data_length;
					if(!list_empty(&validity_list))
					{
						__list_splice(&validity_list,p_data_out_list->prev,p_data_out_list);
						INIT_LIST_HEAD(&validity_list);
					}
					if(!list_empty(&offset_list))
					{
						__list_splice(&offset_list,p_data_out_list->prev,p_data_out_list);
						INIT_LIST_HEAD(&offset_list);
					}
					if(!list_empty(&data_list))
					{
						__list_splice(&data_list,p_data_out_list->prev,p_data_out_list);
						INIT_LIST_HEAD(&data_list);
					}
					//----------------
					if(!list_empty(&validity_table))
					{
						__list_splice(&validity_table,validity_list.prev,&validity_list);
						INIT_LIST_HEAD(&validity_table);
					}
					if(!list_empty(&offset_table))
					{
						__list_splice(&offset_table,offset_list.prev,&offset_list);
						INIT_LIST_HEAD(&offset_table);
					}
				}
			}		       
        }
        //put the fragment data into the list
        memcpy(&current_addr,&p_fragment->fragment_addr,sizeof(ln_page_addr));
        if(data_valid)
        {
        	if(valid_channel_num != -1)current_addr = get_ln_page_offset_address(p_buffer,current_addr,sizeof(data_validity_table)+valid_channel_num*sizeof(__s32),p_page_ready_list);
			if(offset_table_channel_num != -1)current_addr = get_ln_page_offset_address(p_buffer,current_addr,offset_table_channel_num*sizeof(__s32),p_page_ready_list);	
        }
		else
		{
			data_length = p_fragment->fragment_head.data_len;
		}
		__generate_data_list(&data_list,current_addr,data_length,p_buffer,p_page_ready_list,data_valid);
        memcpy(&current_fragment,&p_fragment->next,sizeof(shm_fragment_pointer));
    }
	while(p_fragment->next.fragment_id!=QUEUE_NUM_NULL);
	if(!list_empty(&validity_list))
		__list_splice(&validity_list,p_data_out_list->prev,p_data_out_list);
	if(!list_empty(&offset_list))
		__list_splice(&offset_list,p_data_out_list->prev,p_data_out_list);
	if(!list_empty(&data_list))
		__list_splice(&data_list,p_data_out_list->prev,p_data_out_list);
    //splice the head list and the data list
	list_splice(&head_section_list,p_data_out_list);
    struct list_head    *pos, *q;
    data_out_section* p_prev_data_section=list_entry(p_data_out_list->next,data_out_section,node);
    //splice the adjacent data block
    *p_total_data_len=0;
    list_for_each_safe(pos,q, p_data_out_list)
	{
		p_data_section=list_entry(pos, data_out_section, node);
        (*p_total_data_len)+= p_data_section->length;
        if(p_prev_data_section->addr+p_prev_data_section->length==p_data_section->addr)
        {
            p_prev_data_section->length+=p_data_section->length;
            p_prev_data_section->node.next = p_data_section->node.next;
            list_del(pos);
            free(p_data_section);
        }
        else
        {
           p_prev_data_section = p_data_section;
        }
    }
    return 0;
}

/*
name:       generate_data_out_list
function:   generate a list, storing all the data section address information to be data_save
            simply version no fragment splice
input:        task_entry:  current task descriptor page address
                p_page_ready_list: the head of the data_ready_list
output:     0: success
                -1: failed
                p_data_out_list: the output list head
*/
int generate_data_out_list(gdaq_global_buffer* p_buffer, ln_page_addr task_entry, struct list_head* p_data_out_list, int* p_total_data_len, struct list_head* p_page_ready_list)
{
	shm_interface_page_descriptor* p_interface; 
	shm_interface_page_fragment *p_fragment=NULL, *p_prev_fragment=NULL;
	char* 			p_data;
	data_out_section *p_data_section,*p_data_section2;

	shm_fragment_pointer current_fragment;      //point to the current fragment to be processed 
	ln_page_addr 	current_addr;
	int				i;  //used in for(;;)
	int 			data_length,pre_data_length=-1;
    int             data_valid=1;
	
	LIST_HEAD(head_section_list);
	//put data section head into list
    if((p_interface = (shm_interface_page_descriptor*)gdaq_global_buffer_get_addr(p_buffer, task_entry,p_page_ready_list)) ==NULL)
    {
        printc(MSG_N,"generate_data_out_list:  gdaq_global_buffer_get_addr error!\n");
		return -1;
    }
    
    if((p_data_section=(data_out_section*)malloc(sizeof(data_out_section)))==NULL)
    {
        printc(MSG_E, "generate_data_out_list:  malloc error!(%m)\n",errno);
        return -1;
    }
    /*removed realize data type later
    if((p_interface->section_head.data_type == GDAQ_DATA_TYPE_CANCEL)||
        (p_interface->section_head.data_type == GDAQ_DATA_TYPE_ERROR))
    {
        p_interface->section_head.fragment_num = 0;//!!!!!!!!!
        data_valid = 0;
    }
    */
    p_data_section->addr = (char*)p_interface;
    p_data_section->length = sizeof(data_section_head);
    list_add_tail(&p_data_section->node, &head_section_list);
 
    memcpy(&current_fragment.ln_page, &task_entry.ln_page, sizeof(ln_page_descriptor));
    current_fragment.fragment_id = 0;
    do //go through relevant page no matter the data is valid or not,it will be recorded in page_ready_list and used when free buffer. zyx
    {
        //put the fragment head into the list
    	memcpy(&current_addr.ln_page,&current_fragment.ln_page,sizeof(ln_page_descriptor));
		current_addr.offset = (unsigned long)&p_interface->fragment[current_fragment.fragment_id]-(unsigned long)p_interface;
		if((p_fragment = (shm_interface_page_fragment*)gdaq_global_buffer_get_addr(p_buffer, current_addr,p_page_ready_list)) ==NULL)
		{
			printc(MSG_N," generate_data_out_list:  gdaq_global_buffer_get_addr error!\n");
            free_data_out_list(&head_section_list);
			return -1;
		}
	    if(data_valid)
	    {
	        if((p_data_section=(data_out_section*)malloc(sizeof(data_out_section)))==NULL)
            {
                printc(MSG_E, "generate_data_out_list: malloc error!(%m)\n", errno);
                free_data_out_list(&head_section_list);
                return -1;
            }
            p_data_section->addr = (char*)&p_fragment->fragment_head;
            p_data_section->length = sizeof(data_fragment_head);
            list_add_tail(&p_data_section->node, &head_section_list); 
	    }
        //put the fragment data into the list
        memcpy(&current_addr,&p_fragment->fragment_addr,sizeof(ln_page_addr));
		__generate_data_list(p_data_out_list,current_addr,p_fragment->fragment_head.data_len,p_buffer,p_page_ready_list,data_valid);
        memcpy(&current_fragment,&p_fragment->next,sizeof(shm_fragment_pointer));
    }
	while(p_fragment->next.fragment_id!=QUEUE_NUM_NULL);
	
    //splice the head list and the data list
	list_splice(&head_section_list,p_data_out_list);
    struct list_head    *pos, *q;
    data_out_section* p_prev_data_section=list_entry(p_data_out_list->next,data_out_section,node);
    //splice the adjacent data block
    *p_total_data_len=0;
    list_for_each_safe(pos,q, p_data_out_list)
	{
		p_data_section=list_entry(pos, data_out_section, node);
        (*p_total_data_len)+= p_data_section->length;
        if(p_prev_data_section->addr+p_prev_data_section->length==p_data_section->addr)
        {
            p_prev_data_section->length+=p_data_section->length;
            p_prev_data_section->node.next = p_data_section->node.next;
            list_del(pos);
            free(p_data_section);
        }
        else
        {
           p_prev_data_section = p_data_section;
        }
    }
    return 0;
}


int free_page_ready_list(gdaq_global_buffer *p_buffer, struct list_head* p_page_ready_list, int if_free_page)
{    
	data_ready_page *p_current_page;
	struct list_head *pos,*q;
    list_for_each_safe(pos,q,p_page_ready_list)
    {
		p_current_page=list_entry(pos, data_ready_page, node);
        if(if_free_page)
        {
            if(gdaq_global_buf_free(p_buffer, p_current_page->ln_page)!=0)
            {
                printc(MSG_E,"free_page_ready_list: gdaq_global_buf_free error! \n");
                return -1;
            }
        }
		list_del(pos);
		free(p_current_page);
	}
	return 0;
}


#endif

