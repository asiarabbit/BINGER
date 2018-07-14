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
#ifndef __GDAQ_IPC_C__
#define __GDAQ_IPC_C__
#include "gdaq_ipc.h"


int gdaq_msg_queue_open_or_create(gdaq_msg_queue* mq)
{
    mq_unlink(mq->mq_name);
    mq->mqd = mq_open(mq->mq_name,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR,NULL);
    if(mq->mqd == -1)
    {
        printc(MSG_E, "Message queue create error!(%m)\n", errno);
        return -1;
    }
    return 0;
}


int gdaq_msg_queue_create(gdaq_msg_queue* mq)
{
	mq_unlink(mq->mq_name);
	mq->mqd = mq_open(mq->mq_name,O_RDWR|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,NULL);
	if(mq->mqd == -1)
	{
		printc(MSG_E, "Message queue create error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}

int gdaq_msg_queue_open(gdaq_msg_queue* mq)
{
	mq->mqd = mq_open(mq->mq_name,O_RDWR,S_IRUSR|S_IWUSR,NULL);
	if(mq->mqd == -1)
	{
		printc(MSG_E, "Message queue open error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}

int gdaq_msg_queue_delete(gdaq_msg_queue* mq)
{
	if(mq_unlink(mq->mq_name)!=0)
	{
		printc(MSG_E, "Message queue delete error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}

int gdaq_msg_queue_close(gdaq_msg_queue* mq)
{
	if(mq_close(mq->mqd)==-1)
	{
		printc(MSG_E, "Message queue open error!(%m)\n", errno);
		return -1; 
	}
	return 0;	
}

int gdaq_msg_queue_send(gdaq_msg_queue* mq, gdaq_msg* msg)
{
	if(mq_send(mq->mqd, (char*)msg, sizeof(gdaq_msg), DEFAULT_MSG_PRIO)!=0)
	{
		printc(MSG_E, "Message queue send error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}
int gdaq_msg_queue_recv(gdaq_msg_queue* mq, gdaq_msg* msg)
{
	unsigned int prio;
    struct mq_attr  attr;
    char*   buffer;

    if(mq_getattr(mq->mqd,&attr)!=0)
    {
        printc(MSG_E, "Get message queue attribution error!(%m)\n", errno);
		return -1;
    }
    if((buffer=malloc(attr.mq_msgsize))==NULL)
    {
        printc(MSG_E, "Malloc message queue receive buffer error!(%m)\n", errno);
		return -1; 
    }
	int n = mq_receive(mq->mqd, buffer, attr.mq_msgsize, &prio);
	if(n==-1)
	{
		printc(MSG_E, "Message queue receive error!(%m)\n", errno);
        free(buffer);
		return -1; 
	}

    if(n!=sizeof(gdaq_msg))
    {
        printc(MSG_E, "Message queue receive data type is not matched!\n");
        free(buffer);
        return -1;
    }

    memcpy(msg, buffer, sizeof(gdaq_msg));
    free(buffer);
	return 0;
}

int mutex_process_shared_init(pthread_mutex_t *mptr)
{
    int ret;
    pthread_mutexattr_t mattr;
    if((ret=pthread_mutexattr_init(&mattr))!=0)
    {
        printc(MSG_E, "Mutex attr init error!(%m)\n", ret);
        return -1;
    }
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    ret = pthread_mutex_init(mptr,&mattr);
    pthread_mutexattr_destroy(&mattr);
    if(ret!=0)
    {
        printc(MSG_E, "Mutex init error!(%m)\n", ret);
        return -1;
    }
    return 0;
}

int cond_process_shared_init(pthread_cond_t *cptr)
{
    int ret;          
    pthread_condattr_t cattr;
    if((ret=pthread_condattr_init(&cattr))!=0)
    {
        printc(MSG_E, "Global Buffer: Cond attr init error!(%m)\n",ret);
        return -1;
    }
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    ret = pthread_cond_init(cptr,&cattr);
    pthread_condattr_destroy(&cattr);
    if(ret!=0)
    {
        printc(MSG_E, "Cond init error!(%m)\n",ret);
        return -1;
    }
    return 0;
}

int gdaq_shm_create(gdaq_shm *p_shm)
{
	int fd=-1;
	int	ret=-1;
	shm_unlink(p_shm->name);
	fd = shm_open(p_shm->name,O_RDWR|O_CREAT|O_EXCL, FILE_MODE);
	if(fd == -1)
	{
		printc(MSG_E, "Shm create error!(%m)\n",errno);
		return -1; 
	}
	ret = ftruncate(fd, p_shm->shm_size);
	if(-1 == ret)
	{
		printc(MSG_E, "Shm ftruncate error!(%m)\n", errno);
		return -1; 
	}

	p_shm->ptr = mmap(NULL, p_shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(p_shm->ptr == MAP_FAILED)
	{
		printc(MSG_E, "Shm mmap error!(%m)\n", errno);
		return -1; 
	}
	if(close(fd)==-1)
	{
		printc(MSG_E, "Shm close error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}

int gdaq_shm_open(gdaq_shm *p_shm)
{
	int fd=-1;
	int ret=-1;

	fd = shm_open(p_shm->name,O_RDWR, FILE_MODE);
	if((ret=fd) == -1)
	{
		printc(MSG_E, "Shm not found!\n");
		return -1; 
	}
	
	p_shm->ptr = mmap(NULL, p_shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(p_shm->ptr == MAP_FAILED)
	{
		printc(MSG_E, "Shm mmap error!(%m)\n", errno);
		return -1; 
	}
	if(close(fd)==-1)
	{
		printc(MSG_E, "Shm close error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}
int gdaq_shm_close(gdaq_shm *p_shm)
{
	if(munmap(p_shm->ptr, p_shm->shm_size)==-1)
	{
		printc(MSG_E, "Shm unmap error!(%m)\n", errno);
		return -1;
	}
    return 0;
}
int gdaq_shm_delete(gdaq_shm *p_shm)
{
    if(shm_unlink(p_shm->name)==-1)
	{
		printc(MSG_E, "Shm unlink error!(%m)\n", errno);
		return -1;
	}
	return 0;
}

int gdaq_shm_with_mutex_create(gdaq_shm_with_mutex *p_shm)
{
	int fd=-1;
	int	ret=-1;
    char    shm_name[MAX_TMP_STRING_LENGTH];
    char    sem_name[MAX_TMP_STRING_LENGTH];
    sprintf(shm_name,"%s_shm", p_shm->name);
    sprintf(sem_name,"%s_sem", p_shm->name);
    
	shm_unlink(shm_name);
	fd = shm_open(shm_name,O_RDWR|O_CREAT|O_EXCL, FILE_MODE);
	if(fd == -1)
	{
		printc(MSG_E, "Shm create error!(%m)\n",errno);
		return -1; 
	}
	ret = ftruncate(fd, p_shm->shm_size);
	if(-1 == ret)
	{
		printc(MSG_E, "Shm ftruncate error!(%m)\n", errno);
		return -1; 
	}

	p_shm->ptr = mmap(NULL, p_shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(p_shm->ptr == MAP_FAILED)
	{
		printc(MSG_E, "Shm mmap error!(%m)\n", errno);
		return -1; 
	}
	if(close(fd)==-1)
	{
		printc(MSG_E, "Shm close error!(%m)\n", errno);
		return -1; 
	}

    sem_unlink(sem_name);
	if((p_shm->mutex = sem_open(sem_name,O_CREAT|O_EXCL, FILE_MODE,1))==SEM_FAILED)
	{
		printc(MSG_E, "Sem create error!(%m)\n",errno);
		return -1; 
	}
    if(sem_close(p_shm->mutex)==-1)
    {
        printc(MSG_E, "Sem close error!(%m)\n", errno);
		return -1;
    }
	return 0;
}

int gdaq_shm_with_mutex_open(gdaq_shm_with_mutex *p_shm)
{
	int fd=-1;
	int ret=-1;

    char    shm_name[MAX_TMP_STRING_LENGTH];
    char    sem_name[MAX_TMP_STRING_LENGTH];
    sprintf(shm_name,"%s_shm", p_shm->name);
    sprintf(sem_name,"%s_sem", p_shm->name);

	fd = shm_open(shm_name,O_RDWR, FILE_MODE);
	if((ret=fd) == -1)
	{
		printc(MSG_E, "Shm not found!\n");
		return -1; 
	}
	
	p_shm->ptr = mmap(NULL, p_shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(p_shm->ptr == MAP_FAILED)
	{
		printc(MSG_E, "Shm mmap error!(%m)\n", errno);
		return -1; 
	}
	if(close(fd)==-1)
	{
		printc(MSG_E, "Shm close error!(%m)\n", errno);
		return -1; 
	}

	if((p_shm->mutex = sem_open(sem_name,0))==SEM_FAILED)
	{
		printc(MSG_E, "Sem open error!(%m)\n",errno);
		return -1; 
	}
	return 0;
}
int gdaq_shm_with_mutex_close(gdaq_shm_with_mutex *p_shm)
{
	if(munmap(p_shm->ptr, p_shm->shm_size)==-1)
	{
		printc(MSG_E, "Shm unmap error!(%m)\n", errno);
		return -1;
	}
    if(sem_close(p_shm->mutex)==-1)
    {
        printc(MSG_E, "Sem close error!(%m)\n", errno);
		return -1;
    }
    return 0;
}
int gdaq_shm_with_mutex_delete(gdaq_shm_with_mutex *p_shm)
{
    char    shm_name[MAX_TMP_STRING_LENGTH];
    char    sem_name[MAX_TMP_STRING_LENGTH];
    sprintf(shm_name,"%s_shm", p_shm->name);
    sprintf(sem_name,"%s_sem", p_shm->name);
    if(shm_unlink(shm_name)==-1)
	{
		printc(MSG_E, "Shm unlink error!(%m)\n", errno);
		return -1;
	}
    if(sem_unlink(sem_name)==-1)
    {
        printc(MSG_E, "Sem close error!(%m)\n", errno);
		return -1;
    }
	return 0;
}


int gdaq_shm_with_mutex_lock(gdaq_shm_with_mutex *p_shm)
{
    int ret;
    if((ret = sem_wait(p_shm->mutex))!=0)
    {
		printc(MSG_E, "Mutex lock error!(%m)\n", ret);
		return -1;
	}
	return 0;
}
int	gdaq_shm_with_mutex_unlock(gdaq_shm_with_mutex *p_shm)
{
    int ret;
    if((ret = sem_post(p_shm->mutex))!=0)
    {
        printc(MSG_E, "Mutex unlock error!(%m)\n", ret);
        return -1;
    }
    return 0;
}

int gdaq_global_buf_create(char* buf_name, __u32 page_size, __u32 page_num, gdaq_global_buffer *p_buffer)
{
    int i,j;
    gdaq_shm    global_buffer_mutex_shm;
    sprintf(global_buffer_mutex_shm.name,"%s_mutex",buf_name);
    global_buffer_mutex_shm.shm_size = sizeof(pthread_mutex_t);
    if(gdaq_shm_create(&global_buffer_mutex_shm)!=0)
    {
        printc(MSG_E, "Global Buffer: Create global buffer mutex shm error!\n");
        return -1;
    }
    p_buffer->p_mutex = (pthread_mutex_t*)global_buffer_mutex_shm.ptr;
    if(mutex_process_shared_init(p_buffer->p_mutex)!=0)
    {
        printc(MSG_E, "Global Buffer: Initial global buffer mutex error!\n");
        return -1;
    }

    p_buffer->page_num = page_num;
    p_buffer->page_size = page_size;
    sprintf(p_buffer->buffer.name,"%s",buf_name);
    p_buffer->buffer.shm_size = page_size*page_num;


    pthread_mutex_lock(p_buffer->p_mutex);
    if(gdaq_shm_create(&p_buffer->buffer)!=0)
    {
        printc(MSG_E, "Global Buffer: Create global buffer error!\n");
        pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;
    }
    memset(p_buffer->buffer.ptr,0,p_buffer->buffer.shm_size);
    
    sprintf(p_buffer->free_page_stack.name,"%s_stack",p_buffer->buffer.name);
    p_buffer->free_page_stack.shm_size = sizeof(buf_stack)+page_num*sizeof(int);

    if(gdaq_shm_create(&p_buffer->free_page_stack)!=0)
    {
        gdaq_shm_delete(&p_buffer->buffer);
        printc(MSG_E, "Global Buffer: Create global buffer stack error!\n");
        pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;
    }
    
    memset(p_buffer->free_page_stack.ptr,0,p_buffer->free_page_stack.shm_size);
    buf_stack * p_stack = (buf_stack *)p_buffer->free_page_stack.ptr;

    p_stack->stack_depth = page_num;
    p_stack->stack_top = page_num-1;
    for(i=0;i<page_num;i++)
    {
        p_stack->stack[i]= page_num-i-1;
    }
    
    sprintf(p_buffer->descriptor.name,"%s_des",p_buffer->buffer.name);
    p_buffer->descriptor.shm_size = sizeof(buf_descriptor)+page_num*sizeof(shm_page_descriptor);
    
    if(gdaq_shm_create(&p_buffer->descriptor)!=0)
    {
        gdaq_shm_delete(&p_buffer->buffer);
        gdaq_shm_delete(&p_buffer->free_page_stack);
        printc(MSG_E, "Global Buffer: Create global buffer descriptor error!\n");
        pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;
    }
    
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    p_buf_des->page_used = 0;
    p_buf_des->page_free = page_num;
    p_buf_des->ln_num = 0;
        
    for(i=0;i<MAX_LN_NUM;i++)
    {
        p_buf_des->ln_info[i].ln_id = QUEUE_NUM_NULL;
        p_buf_des->ln_info[i].page_type_num = 0;
        for(j=0;j<MAX_LN_PAGE_TYPE;j++)
        {
            p_buf_des->ln_info[i].entry_type[j].type= QUEUE_NUM_NULL;
            p_buf_des->ln_info[i].entry_type[j].entry= QUEUE_NUM_NULL;
            p_buf_des->ln_info[i].entry_type[j].page_used = 0;
            if(cond_process_shared_init(&p_buf_des->ln_info[i].entry_type[j].cond)!=0)
            {
                pthread_mutex_unlock(p_buffer->p_mutex);
                gdaq_shm_delete(&p_buffer->buffer);
                gdaq_shm_delete(&p_buffer->free_page_stack);
                gdaq_shm_delete(&p_buffer->descriptor);
                pthread_mutex_unlock(p_buffer->p_mutex);
                return -1;
            } 
        }
    }
    for(i=0;i<page_num;i++)
    {
        p_buf_des->page_descriptor[i].page_id = 0;
        p_buf_des->page_descriptor[i].total_user_num= 0;
        p_buf_des->page_descriptor[i].finish_user_num = 0;
		p_buf_des->page_descriptor[i].page_used_length = page_size;
        p_buf_des->page_descriptor[i].data_ready = 0;
        p_buf_des->page_descriptor[i].next = QUEUE_NUM_NULL;
        p_buf_des->page_descriptor[i].prev = QUEUE_NUM_NULL;
        if(cond_process_shared_init(&p_buf_des->page_descriptor[i].data_ready_cond)!=0)
        {
            pthread_mutex_unlock(p_buffer->p_mutex);
            gdaq_shm_delete(&p_buffer->buffer);
            gdaq_shm_delete(&p_buffer->free_page_stack);
            gdaq_shm_delete(&p_buffer->descriptor);
            pthread_mutex_unlock(p_buffer->p_mutex);
            return -1;
        } 
    }
    pthread_mutex_unlock(p_buffer->p_mutex);   
    return 0;    
}
int gdaq_global_buf_open(char* buf_name, __u32 page_size, __u32 page_num, gdaq_global_buffer *p_buffer)
{
    gdaq_shm    global_buffer_mutex_shm;
    sprintf(global_buffer_mutex_shm.name,"%s_mutex",buf_name);
    global_buffer_mutex_shm.shm_size = sizeof(pthread_mutex_t);
    if(gdaq_shm_open(&global_buffer_mutex_shm)!=0)
    {
        printc(MSG_E, "Global Buffer: Create global buffer mutex shm error!\n");
        return -1;
    }
    p_buffer->p_mutex = (pthread_mutex_t*)global_buffer_mutex_shm.ptr;
    
    p_buffer->page_num = page_num;
    p_buffer->page_size = page_size;
    sprintf(p_buffer->buffer.name,"%s",buf_name);
    p_buffer->buffer.shm_size = page_size*page_num;

    if(gdaq_shm_open(&p_buffer->buffer)!=0)
    {
        printc(MSG_E, "Global Buffer: Open global buffer error!\n");
        return -1;
    }
    sprintf(p_buffer->free_page_stack.name,"%s_stack",p_buffer->buffer.name);
    p_buffer->free_page_stack.shm_size = sizeof(buf_stack)+page_num*sizeof(int);

    if(gdaq_shm_open(&p_buffer->free_page_stack)!=0)
    {
        gdaq_shm_close(&p_buffer->buffer);
        printc(MSG_E, "Global Buffer: Open global buffer stack error!\n");
        return -1;
    }

    sprintf(p_buffer->descriptor.name,"%s_des",p_buffer->buffer.name);
    p_buffer->descriptor.shm_size = sizeof(buf_descriptor)+page_num*sizeof(shm_page_descriptor);
    
    if(gdaq_shm_open(&p_buffer->descriptor)!=0)
    {
        gdaq_shm_close(&p_buffer->buffer);
        gdaq_shm_close(&p_buffer->free_page_stack);
        printc(MSG_E, "Global Buffer: open global buffer descriptor error!\n");
        return -1;
    }
    return 0;
}

void gdaq_global_buf_print_descriptor(gdaq_global_buffer *p_buffer)
{
    #if 0
    int i,j, current_page;
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    printc(MSG_T, "[Global buffer page descriptor information]\n");
    printc(MSG_N, "  used_page=%d, free_page=%d, LN_number=%d\n", p_buf_des->page_used, p_buf_des->page_free, p_buf_des->ln_num);
    for(i=0;i<p_buf_des->ln_num;i++)
    {
        printc(MSG_N, "  [LN%d]: ID=%d, page_type=%d\n", i, p_buf_des->ln_info[i].ln_id, p_buf_des->ln_info[i].page_type_num);
        for(j=0;j<p_buf_des->ln_info[i].page_type_num;j++)
        {
            printc(MSG_N, "    [LN%d_type%d]type=%d, used_page=%d\n",i,j,p_buf_des->ln_info[i].entry_type[j].type,p_buf_des->ln_info[i].entry_type[j].page_used);
            current_page = p_buf_des->ln_info[i].entry_type[j].entry;
            if(current_page!=QUEUE_NUM_NULL)
            {
                printc(MSG_N, "      page=%d", current_page);
                while(p_buf_des->page_descriptor[current_page].next!=QUEUE_NUM_NULL)
                {
                    current_page = p_buf_des->page_descriptor[current_page].next;
                    printf(", %d", current_page);
                }
                printf("\n");
            }
        }
    }
    printc(MSG_T,"[Global buffer page descriptor information end]\n");
    #endif
    return;
}
int gdaq_global_buf_malloc(gdaq_global_buffer *p_buffer, ln_page_descriptor ln_page)
{
//    printc(MSG_N, "Global Buffer: page (LN:type:page=%d:%d:%d) malloc start!\n",ln_page.ln_id,ln_page.type, ln_page.page_id);
    int out_page = 0;
    int i, j;
    int current_page=QUEUE_NUM_NULL;
    int current_type=QUEUE_NUM_NULL;
    int current_ln = QUEUE_NUM_NULL;
  
    pthread_mutex_lock(p_buffer->p_mutex);

    buf_stack * p_stack = (buf_stack *)p_buffer->free_page_stack.ptr;

    if(stack_pop(p_stack,&out_page)!=0)
    {
        printc(MSG_E, "Global Buffer: stack pop error!\n");
        pthread_mutex_unlock(p_buffer->p_mutex);
        return QUEUE_NUM_NULL;
    }
    
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    p_buf_des->page_used++;
    p_buf_des->page_free--;
    p_buf_des->page_descriptor[out_page].page_id = ln_page.page_id;
    p_buf_des->page_descriptor[out_page].total_user_num= 1;
    p_buf_des->page_descriptor[out_page].finish_user_num = 0;
	p_buf_des->page_descriptor[out_page].page_used_length = p_buffer->page_size;
    p_buf_des->page_descriptor[out_page].data_ready = 0;

    int ret;
    for(i=0;i<p_buf_des->ln_num;i++)
    {
        if(ln_page.ln_id==p_buf_des->ln_info[i].ln_id)
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
    if(current_ln == QUEUE_NUM_NULL)
    {
        current_ln=p_buf_des->ln_num;
        p_buf_des->ln_num++;
        p_buf_des->ln_info[current_ln].ln_id = ln_page.ln_id;
        p_buf_des->ln_info[current_ln].page_type_num++;
		current_type = 0;
        p_buf_des->ln_info[current_ln].entry_type[current_type].type= ln_page.type;
        p_buf_des->ln_info[current_ln].entry_type[current_type].entry= out_page;
        p_buf_des->ln_info[current_ln].entry_type[current_type].page_used++;

        p_buf_des->page_descriptor[out_page].prev = QUEUE_NUM_NULL;
        p_buf_des->page_descriptor[out_page].next = QUEUE_NUM_NULL;
    }
    else if(current_type==QUEUE_NUM_NULL)
    {
        current_type = p_buf_des->ln_info[current_ln].page_type_num;
        p_buf_des->ln_info[current_ln].page_type_num++;
        p_buf_des->ln_info[current_ln].entry_type[current_type].type = ln_page.type;
        p_buf_des->ln_info[current_ln].entry_type[current_type].entry= out_page;
        p_buf_des->ln_info[current_ln].entry_type[current_type].page_used++;

        p_buf_des->page_descriptor[out_page].prev = QUEUE_NUM_NULL;
        p_buf_des->page_descriptor[out_page].next = QUEUE_NUM_NULL;
    }
    else
    {
        current_page = p_buf_des->ln_info[current_ln].entry_type[current_type].entry;           
        if(current_page == QUEUE_NUM_NULL)
        {           
            p_buf_des->ln_info[current_ln].entry_type[current_type].entry= out_page;
            p_buf_des->ln_info[current_ln].entry_type[current_type].page_used++;
            p_buf_des->page_descriptor[out_page].prev = QUEUE_NUM_NULL;
            p_buf_des->page_descriptor[out_page].next = QUEUE_NUM_NULL;
        }
        else
        {
            while(p_buf_des->page_descriptor[current_page].next!=QUEUE_NUM_NULL)
            {
                current_page = p_buf_des->page_descriptor[current_page].next;
            }

            p_buf_des->ln_info[current_ln].entry_type[current_type].page_used++;                
            p_buf_des->page_descriptor[out_page].prev = current_page;
            p_buf_des->page_descriptor[out_page].next = QUEUE_NUM_NULL;
            p_buf_des->page_descriptor[current_page].next = out_page;                  
        }
    }
    #ifdef DAQ_DEBUG
//    gdaq_global_buf_print_descriptor(p_buffer);
    #endif
    if((ret=pthread_cond_broadcast(&p_buf_des->ln_info[i].entry_type[current_type].cond))!=0)
    {
        printc(MSG_E, "Global Buffer: cond broadcast error!(%m)",ret);
        pthread_mutex_unlock(p_buffer->p_mutex);
        return QUEUE_NUM_NULL;
    }
	pthread_mutex_unlock(p_buffer->p_mutex);
    #ifdef DAQ_DEBUG
//    printd("Global Buffer: page %d (LN:type:page=%d:%d:%d) malloc success!\n",out_page, ln_page.ln_id,ln_page.type, ln_page.page_id);
    #endif
    
    return out_page;
}

int gdaq_global_buf_free(gdaq_global_buffer *p_buffer, ln_page_descriptor ln_page)
{
//    printc(MSG_N, "Global Buffer: page (LN:type:page=%d:%d:%d) free start!\n",ln_page.ln_id,ln_page.type, ln_page.page_id);
    int i,j;
    int current_ln = QUEUE_NUM_NULL;
    int current_page = QUEUE_NUM_NULL;
    int current_type = QUEUE_NUM_NULL;
    int prev_page, next_page;
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
        printc(MSG_E, "Global Buffer: Can't find ln or type in free shared buffer!,ln:type:page_id=(%d:%d:%d)\n",ln_page.ln_id,ln_page.type,ln_page.page_id);
        pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;                
    }
    current_page = p_buf_des->ln_info[current_ln].entry_type[current_type].entry;
    if(current_page==QUEUE_NUM_NULL)
    {
        printc(MSG_E, "Global Buffer: Can't find page in free shared buffer!,ln:type:page_id=(%d:%d:%d)\n",ln_page.ln_id,ln_page.type,ln_page.page_id);
        pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;
    }
    do
    {    
        if(p_buf_des->page_descriptor[current_page].page_id==ln_page.page_id)
            break;
        else
            current_page=p_buf_des->page_descriptor[current_page].next;
    }
    while(current_page!=QUEUE_NUM_NULL);
    if(current_page==QUEUE_NUM_NULL)
    {
        printc(MSG_E, "Global Buffer: Can't find page in free shared buffer!,ln:type:page_id=(%d:%d:%d)\n",ln_page.ln_id,ln_page.type,ln_page.page_id);
        pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;
    }

    p_buf_des->page_descriptor[current_page].finish_user_num ++;
    if(p_buf_des->page_descriptor[current_page].finish_user_num==p_buf_des->page_descriptor[current_page].total_user_num)
    {
        prev_page = p_buf_des->page_descriptor[current_page].prev;
        next_page = p_buf_des->page_descriptor[current_page].next;
        if(prev_page!=QUEUE_NUM_NULL)
        {
            p_buf_des->page_descriptor[prev_page].next = next_page;
        }
        else
        {
            p_buf_des->ln_info[i].entry_type[current_type].entry = next_page;
        }
        if(next_page!=QUEUE_NUM_NULL)
            p_buf_des->page_descriptor[next_page].prev = prev_page;
        p_buf_des->page_descriptor[current_page].page_id = QUEUE_NUM_NULL;
        p_buf_des->page_descriptor[current_page].total_user_num= 0;
        p_buf_des->page_descriptor[current_page].finish_user_num = 0;
		p_buf_des->page_descriptor[current_page].page_used_length = p_buffer->page_size;
        p_buf_des->page_descriptor[current_page].data_ready = 0;
        p_buf_des->page_descriptor[current_page].next = QUEUE_NUM_NULL;
        p_buf_des->page_descriptor[current_page].prev = QUEUE_NUM_NULL;
        p_buf_des->ln_info[current_ln].entry_type[current_type].page_used--;
        p_buf_des->page_free++;
        p_buf_des->page_used--;
        memset((void *)(p_buffer->buffer.ptr+current_page*p_buffer->page_size),0,p_buffer->page_size);
        buf_stack * p_stack = (buf_stack *)p_buffer->free_page_stack.ptr;
        int page;
        if(stack_push(p_stack,current_page)!=0)
        {
            printc(MSG_E, "Global Buffer: stack push error!\n");
            pthread_mutex_unlock(p_buffer->p_mutex);
            return -1;
        }
        #ifdef DAQ_DEBUG
        gdaq_global_buf_print_descriptor(p_buffer);
        #endif
        pthread_mutex_unlock(p_buffer->p_mutex);
        #ifdef DAQ_DEBUG
        //printd("Global Buffer: page %d (LN:type:page=%d:%d:%d) free success!\n",current_page, ln_page.ln_id, ln_page.type, ln_page.page_id);
        #endif
        return 0;   
    }  
    else
    {
        #ifdef DAQ_DEBUG
       // printc(MSG_T, "Global Buffer: page %d (LN:type:page=%d:%d:%d) not all user finished, will be freed later!\n",current_page, ln_page.ln_id, ln_page.type, ln_page.page_id);
        #endif
        gdaq_global_buf_print_descriptor(p_buffer);
        pthread_mutex_unlock(p_buffer->p_mutex);
        return 0; 
    }
}

	
int gdaq_global_buf_search(gdaq_global_buffer *p_buffer,ln_page_descriptor ln_page)
{
    int i,j,ret;
    int current_ln = QUEUE_NUM_NULL;
    int current_page = QUEUE_NUM_NULL;
    int current_type = QUEUE_NUM_NULL;
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
        if((ret=pthread_cond_wait(&p_buf_des->ln_info[i].entry_type[current_type].cond,p_buffer->p_mutex))!=0)
        {
            printc(MSG_E, "Global Buffer: pthread_cond_wait error(%m)!\n",ret);
            pthread_mutex_unlock(p_buffer->p_mutex);
            return QUEUE_NUM_NULL;  
        }
    }
            
    pthread_mutex_unlock(p_buffer->p_mutex);
    return QUEUE_NUM_NULL;
}

void gdaq_global_page_user_num_add_one(gdaq_global_buffer *p_buffer, int page)
{
    pthread_mutex_lock(p_buffer->p_mutex);
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    p_buf_des->page_descriptor[page].total_user_num++;
    pthread_mutex_unlock(p_buffer->p_mutex);
    return;    
}


int gdaq_global_buf_wait_data_ready(gdaq_global_buffer *p_buffer, int page)
{
    int ret;
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    pthread_mutex_lock(p_buffer->p_mutex);
    while(p_buf_des->page_descriptor[page].data_ready!=1)
    {
        if((ret=pthread_cond_wait(&p_buf_des->page_descriptor[page].data_ready_cond,p_buffer->p_mutex))!=0)
        {
            printc(MSG_E, "Global Buffer: pthread_cond_wait error(%m)!\n",ret);
            pthread_mutex_unlock(p_buffer->p_mutex);
            return -1;  
        }
    }
    pthread_mutex_unlock(p_buffer->p_mutex);
    return 0;
}

int gdaq_global_buf_set_data_ready(gdaq_global_buffer* p_buffer, int page, __u8 data_ready)
{
    int ret;
    buf_descriptor *p_buf_des = (buf_descriptor*)p_buffer->descriptor.ptr;
    pthread_mutex_lock(p_buffer->p_mutex);
    p_buf_des->page_descriptor[page].data_ready = data_ready;
    if((ret=pthread_cond_broadcast(&p_buf_des->page_descriptor[page].data_ready_cond))!=0)
    {
        printc(MSG_E, "Global Buffer: cond broadcast error!(%m)",ret);
		pthread_mutex_unlock(p_buffer->p_mutex);
        return -1;
    }
    pthread_mutex_unlock(p_buffer->p_mutex);
    return 0;
}


int stack_push(buf_stack* p_stack, int page)
{
    if(p_stack->stack_top<p_stack->stack_depth-1)
    {
        p_stack->stack[++p_stack->stack_top]= page;
        return 0;
    }
    else
    {
        printc(MSG_E, "Global Buffer: stack full!\n");
        return -1;
    }
}

int stack_pop(buf_stack* p_stack, int* page)
{
    if(p_stack->stack_top>=0)
    {
        *page = p_stack->stack[p_stack->stack_top--];
        return 0;
    }
    else
    {
        printc(MSG_E, "Global Buffer: stack empty!\n");
        return -1;
    }
}

int gdaq_global_buf_close(gdaq_global_buffer *p_buffer)
{
    int ret=0;
    if(gdaq_shm_close(&p_buffer->buffer)!=0)
    {
        printc(MSG_E, "Global Buffer: buffer close error!\n");
        ret = -1;
    }
    if(gdaq_shm_close(&p_buffer->free_page_stack)!=0)
    {
        printc(MSG_E, "Global Buffer: buffer stack close error!\n");
        ret = -1;
    }
    if(gdaq_shm_close(&p_buffer->descriptor)!=0)
    {
        printc(MSG_E, "Global Buffer: buffer descriptor close error!\n");
        ret = -1;
    }
    return ret;
}
int gdaq_global_buf_delete(gdaq_global_buffer *p_buffer)
{
    int ret=0;
    if(pthread_mutex_destroy(p_buffer->p_mutex)!=0)
    {
        printc(MSG_E, "Global Buffer: Destroy global buffer mutex error!\n");
        return -1;
    }
    gdaq_shm    global_buffer_mutex_shm;
    sprintf(global_buffer_mutex_shm.name,"%s_mutex",p_buffer->buffer.name);

    if(gdaq_shm_delete(&global_buffer_mutex_shm)!=0)
    {
        printc(MSG_E, "Global Buffer: Delete global buffer mutex shm error!\n");
        return -1;
    }
    if(gdaq_shm_delete(&p_buffer->buffer)!=0)
    {
        printc(MSG_E, "Global Buffer: buffer delete error!\n");
        ret = -1;
    }
    if(gdaq_shm_delete(&p_buffer->free_page_stack)!=0)
    {
        printc(MSG_E, "Global Buffer: buffer stack delete error!\n");
        ret = -1;
    }
    if(gdaq_shm_delete(&p_buffer->descriptor)!=0)
    {
        printc(MSG_E, "Global Buffer: buffer descriptor delete error!\n");
        ret = -1;
    }

    return ret;
}

char*	gdaq_global_buf_get_addr(gdaq_global_buffer *p_buffer, int page, int offset)
{
	return (char*)((unsigned long)(p_buffer->buffer.ptr)+page*p_buffer->page_size+offset);
}


int ln_interface_create(char* shm_name, shm_interface** p_interface)
{
    gdaq_shm shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(shm_interface);
    if(gdaq_shm_create(&shm_int)==-1)
    {
       	printc(MSG_E,"ln_interface_create: shm %s create error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    //initialize the shm and sem
	*p_interface = (shm_interface*)(shm_int.ptr);
	
	memset(*p_interface, 0, sizeof(shm_interface));
    strcpy((*p_interface)->name, shm_name);
    if(sem_init(&(*p_interface)->mutex,1,1)==-1)
    {
        printc(MSG_E,"ln_interface_create: sem mutex of interface %s create error!(%m)\n", shm_name,errno);
        return -1;
    }
    if(sem_init(&(*p_interface)->nempty,1, DEFAULT_INTERFACE_TASK_NUM)==-1)
    {
        printc(MSG_E,"ln_interface_create: sem nempty of interface %s create error!(%m)\n", shm_name, errno);
        return -1;
    }
    if(sem_init(&(*p_interface)->nstored,1, 0)==-1)
    {
        printc(MSG_E,"ln_interface_create: sem nstored of interface %s create error!(%m)\n", shm_name, errno);
        return -1;
    }
    (*p_interface)->p_write=0;
    (*p_interface)->p_read=0;

    return 0;
}

int ln_interface_open(char* shm_name, shm_interface** p_interface)
{
    gdaq_shm    shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(shm_interface);
    if(gdaq_shm_open(&shm_int)==-1)
    {
       	printc(MSG_E,"ln_interface_open: shm %s open error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    *p_interface = (shm_interface*)shm_int.ptr;
    return 0;    
}



int ln_interface_destroy(char* shm_name, shm_interface* p_interface)
{
    int return_val = 0;
	gdaq_shm    shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(shm_interface);
    if(gdaq_shm_open(&shm_int)==-1)
    {
       	printc(MSG_E,"ln_interface_destroy: shm %s open error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    p_interface = (shm_interface*)shm_int.ptr;

    if(sem_destroy(&p_interface->mutex)==-1)
    {
        printc(MSG_E,"ln_interface_destroy: sem mutex of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }
    if(sem_destroy(&p_interface->nempty)==-1)
    {
        printc(MSG_E,"ln_interface_destroy: sem nempty of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }
    if(sem_destroy(&p_interface->nstored)==-1)
    {
        printc(MSG_E,"ln_interface_destroy: sem nstored of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }

    if(gdaq_shm_delete(&shm_int)!=0)
    {
        return_val = -1;
    }
    return return_val;
}

int ln_interface_add_task(ln_page_addr task_addr,shm_interface* p_interface)
{
    if(sem_wait(&p_interface->nempty)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_wait nempty error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
    memcpy(&p_interface->section[p_interface->p_write], &task_addr, sizeof(ln_page_addr));
    p_interface->p_write++;
    if(p_interface->p_write == DEFAULT_INTERFACE_TASK_NUM)
        p_interface->p_write = 0;
    #ifdef DAQ_DEBUG
//    printd("%s write: current p_write=%d,p_read=%d.\n",p_interface->name, p_interface->p_write,p_interface->p_read);
    #endif
    /*critical region end*/
    if(sem_post(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_post mutex error!(%m)\n", errno);
        return -1;
    }

    if(sem_post(&p_interface->nstored)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_post nstored error!(%m)\n", errno);
        return -1;
    }
	return 0;
        
}
int ln_interface_get_task(shm_interface* p_interface, ln_page_addr* out_task_addr)
{
    if(sem_wait(&p_interface->nstored)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_wait nstored error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
    memcpy(out_task_addr, &p_interface->section[p_interface->p_read], sizeof(ln_page_addr));
    p_interface->p_read++;
    if(p_interface->p_read>= DEFAULT_INTERFACE_TASK_NUM)
        p_interface->p_read = 0;
    
    #ifdef DAQ_DEBUG
//    printd("%s read: current p_write=%d,p_read=%d.\n",p_interface->name, p_interface->p_write,p_interface->p_read);
    #endif

    /*critical region end*/
    
    if(sem_post(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_post mutex error!(%m)\n", errno);
        return -1;
    }

    if(sem_post(&p_interface->nempty)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_post nempty error!(%m)\n", errno);
        return -1;
    }
	return 0;
}
 

int online_interface_create(char* shm_name, online_shm_interface** p_interface)
{
    gdaq_shm shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(online_shm_interface);
    if(gdaq_shm_create(&shm_int)==-1)
    {
       	printc(MSG_E,"online_interface_create: shm %s create error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    //initialize the shm and sem
	*p_interface = (online_shm_interface*)(shm_int.ptr);
	
	memset(*p_interface, 0, sizeof(online_shm_interface));
    strcpy((*p_interface)->name, shm_name);
    if(sem_init(&(*p_interface)->mutex,1,1)==-1)
    {
        printc(MSG_E,"ln_interface_create: sem mutex of interface %s create error!(%m)\n", shm_name,errno);
        return -1;
    }
    if(sem_init(&(*p_interface)->nempty,1, DEFAULT_INTERFACE_TASK_NUM)==-1)
    {
        printc(MSG_E,"ln_interface_create: sem nempty of interface %s create error!(%m)\n", shm_name, errno);
        return -1;
    }
    if(sem_init(&(*p_interface)->nstored,1, 0)==-1)
    {
        printc(MSG_E,"ln_interface_create: sem nstored of interface %s create error!(%m)\n", shm_name, errno);
        return -1;
    }
    (*p_interface)->p_write=0;
    (*p_interface)->p_read=0;

    return 0;
}

int online_interface_open(char* shm_name, online_shm_interface** p_interface)
{
    gdaq_shm    shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(online_shm_interface);
    if(gdaq_shm_open(&shm_int)==-1)
    {
       	printc(MSG_E,"online_interface_open: shm %s open error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    *p_interface = (online_shm_interface*)shm_int.ptr;
    return 0;    
}

int online_interface_destroy(char* shm_name, online_shm_interface* p_interface)
{
    int return_val = 0;
	gdaq_shm    shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(online_shm_interface);
    if(gdaq_shm_open(&shm_int)==-1)
    {
       	printc(MSG_E,"ln_interface_destroy: shm %s open error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    p_interface = (online_shm_interface*)shm_int.ptr;

    if(sem_destroy(&p_interface->mutex)==-1)
    {
        printc(MSG_E,"ln_interface_destroy: sem mutex of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }
    if(sem_destroy(&p_interface->nempty)==-1)
    {
        printc(MSG_E,"ln_interface_destroy: sem nempty of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }
    if(sem_destroy(&p_interface->nstored)==-1)
    {
        printc(MSG_E,"ln_interface_destroy: sem nstored of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }

    if(gdaq_shm_delete(&shm_int)!=0)
    {
        return_val = -1;
    }
    return return_val;
}

int online_interface_add_task(int* task_addr,online_shm_interface* p_interface)
{
    if(sem_wait(&p_interface->nempty)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_wait nempty error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
    memcpy(&p_interface->section[p_interface->p_write], task_addr, sizeof(int));
    p_interface->p_write++;
    if(p_interface->p_write == DEFAULT_INTERFACE_TASK_NUM)
        p_interface->p_write = 0;
    #ifdef DAQ_DEBUG
//    printd("%s write: current p_write=%d,p_read=%d.\n",p_interface->name, p_interface->p_write,p_interface->p_read);
    #endif
    /*critical region end*/
    if(sem_post(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_post mutex error!(%m)\n", errno);
        return -1;
    }

    if(sem_post(&p_interface->nstored)!=0)
    {
        printc(MSG_E, "ln_interface_add_task: sem_post nstored error!(%m)\n", errno);
        return -1;
    }
	return 0;
        
}
int online_interface_get_task(online_shm_interface* p_interface, int* out_task_addr)
{
    if(sem_wait(&p_interface->nstored)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_wait nstored error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
    memcpy(out_task_addr, &p_interface->section[p_interface->p_read], sizeof(int));
    p_interface->p_read++;
    if(p_interface->p_read>= DEFAULT_INTERFACE_TASK_NUM)
        p_interface->p_read = 0;
    
    #ifdef DAQ_DEBUG
//    printd("%s read: current p_write=%d,p_read=%d.\n",p_interface->name, p_interface->p_write,p_interface->p_read);
    #endif

    /*critical region end*/
    
    if(sem_post(&p_interface->mutex)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_post mutex error!(%m)\n", errno);
        return -1;
    }

    if(sem_post(&p_interface->nempty)!=0)
    {
        printc(MSG_E, "ln_interface_get_task: sem_post nempty error!(%m)\n", errno);
        return -1;
    }
	return 0;
}
#endif

