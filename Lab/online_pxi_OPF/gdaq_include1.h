#include <sys/mman.h>
#include <semaphore.h>
#include <mqueue.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define	FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define DEFAULT_INTERFACE_ONLINE_NUM1       	20000
#define DEFAULT_INTERFACE_ONLINE_LEN1       	1000
#define MAX_TMP_STRING_LENGTH	128

typedef struct gdaq_shm_t1{
    //must be initialed when struct be defined
    char 	name[MAX_TMP_STRING_LENGTH];
	size_t	shm_size;	
    //will be updated when gdaq_shm_init and gdaq_shm_link be processed
	void*	ptr;
} gdaq_shm1;



typedef struct online_shm_interface1_t{
    char    name[MAX_TMP_STRING_LENGTH];
    sem_t   mutex, nempty, nstored; 
    int     p_write, p_read;        
    int     section[DEFAULT_INTERFACE_ONLINE_NUM1][DEFAULT_INTERFACE_ONLINE_LEN1];         
} online_shm_interface1;


int gdaq_shm_create1(gdaq_shm1 *p_shm);
int gdaq_shm_open1(gdaq_shm1 *p_shm);
int gdaq_shm_close1(gdaq_shm1 *p_shm);
int gdaq_shm_delete1(gdaq_shm1 *p_shm);

int online_interface_create1(char* shm_name, online_shm_interface1** p_interface);
int online_interface_open1(char* shm_name, online_shm_interface1** p_interface);
int online_interface_destroy1(char* shm_name, online_shm_interface1* p_interface);
int online_interface_add_task1(int* task_addr,online_shm_interface1* p_interface);
int online_interface_get_task1(online_shm_interface1* p_interface, int* out_task_addr);
// /*
int online_interface_create1(char* shm_name, online_shm_interface1** p_interface)
{
    gdaq_shm1 shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(online_shm_interface1);
    if(gdaq_shm_create1(&shm_int)==-1)
    {
       	//printc(MSG_E,"online_interface_create: shm %s create error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    //initialize the shm and sem
	*p_interface = (online_shm_interface1*)(shm_int.ptr);
	
	memset(*p_interface, 0, sizeof(online_shm_interface1));
    strcpy((*p_interface)->name, shm_name);
    if(sem_init(&(*p_interface)->mutex,1,1)==-1)
    {
        //printc(MSG_E,"ln_interface_create: sem mutex of interface %s create error!(%m)\n", shm_name,errno);
        return -1;
    }
    if(sem_init(&(*p_interface)->nempty,1, DEFAULT_INTERFACE_ONLINE_NUM1)==-1)
    {
        //printc(MSG_E,"ln_interface_create: sem nempty of interface %s create error!(%m)\n", shm_name, errno);
        return -1;
    }
    if(sem_init(&(*p_interface)->nstored,1, 0)==-1)
    {
        //printc(MSG_E,"ln_interface_create: sem nstored of interface %s create error!(%m)\n", shm_name, errno);
        return -1;
    }
    (*p_interface)->p_write=0;
    (*p_interface)->p_read=0;

    return 0;
}
// */
int online_interface_open1(char* shm_name, online_shm_interface1** p_interface)
{
    gdaq_shm1    shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(online_shm_interface1);
    if(gdaq_shm_open1(&shm_int)==-1)
    {
       	//printc(MSG_E,"online_interface_open: shm %s open error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    *p_interface = (online_shm_interface1*)shm_int.ptr;
    return 0;    
}
// /*
int online_interface_destroy1(char* shm_name, online_shm_interface1* p_interface)
{
    int return_val = 0;
	gdaq_shm1    shm_int;
    sprintf(shm_int.name, "%s",  shm_name);
	shm_int.shm_size = sizeof(online_shm_interface1);
    if(gdaq_shm_open1(&shm_int)==-1)
    {
       	//printc(MSG_E,"ln_interface_destroy: shm %s open error!\n", shm_name);
        p_interface = NULL;
        return -1;
    }
    p_interface = (online_shm_interface1*)shm_int.ptr;

    if(sem_destroy(&p_interface->mutex)==-1)
    {
        //printc(MSG_E,"ln_interface_destroy: sem mutex of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }
    if(sem_destroy(&p_interface->nempty)==-1)
    {
        //printc(MSG_E,"ln_interface_destroy: sem nempty of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }
    if(sem_destroy(&p_interface->nstored)==-1)
    {
        //printc(MSG_E,"ln_interface_destroy: sem nstored of interface %s destroy error!\n", shm_name,errno);
        return_val = -1;
    }

    if(gdaq_shm_delete1(&shm_int)!=0)
    {
        return_val = -1;
    }
    return return_val;
}
// */
int online_interface_add_task1(int* task_addr,online_shm_interface1* p_interface)
{
    if(sem_wait(&p_interface->nempty)!=0)
    {
        //printc(MSG_E, "ln_interface_add_task: sem_wait nempty error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_interface->mutex)!=0)
    {
        //printc(MSG_E, "ln_interface_add_task: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
    memcpy(&p_interface->section[p_interface->p_write][0], task_addr, sizeof(int)*DEFAULT_INTERFACE_ONLINE_LEN1);
    p_interface->p_write++;
    if(p_interface->p_write == DEFAULT_INTERFACE_ONLINE_NUM1)
        p_interface->p_write = 0;
    #ifdef DAQ_DEBUG
//    printd("%s write: current p_write=%d,p_read=%d.\n",p_interface->name, p_interface->p_write,p_interface->p_read);
    #endif
    /*critical region end*/
    if(sem_post(&p_interface->mutex)!=0)
    {
        //printc(MSG_E, "ln_interface_add_task: sem_post mutex error!(%m)\n", errno);
        return -1;
    }

    if(sem_post(&p_interface->nstored)!=0)
    {
        //printc(MSG_E, "ln_interface_add_task: sem_post nstored error!(%m)\n", errno);
        return -1;
    }
	return 0;
        
}
int online_interface_get_task1(online_shm_interface1* p_interface, int* out_task_addr)
{
    if(sem_wait(&p_interface->nstored)!=0)
    {
        //printc(MSG_E, "ln_interface_get_task: sem_wait nstored error!(%m)\n", errno);
        return -1;
    }
    if(sem_wait(&p_interface->mutex)!=0)
    {
        //printc(MSG_E, "ln_interface_get_task: sem_wait mutex error!(%m)\n", errno);
        return -1;
    }
    /*critical region*/
    memcpy(out_task_addr, &p_interface->section[p_interface->p_read][0], sizeof(int)*DEFAULT_INTERFACE_ONLINE_LEN1);
    p_interface->p_read++;
    if(p_interface->p_read>= DEFAULT_INTERFACE_ONLINE_NUM1)
        p_interface->p_read = 0;
    
    #ifdef DAQ_DEBUG
//    printd("%s read: current p_write=%d,p_read=%d.\n",p_interface->name, p_interface->p_write,p_interface->p_read);
    #endif

    /*critical region end*/
    
    if(sem_post(&p_interface->mutex)!=0)
    {
        //printc(MSG_E, "ln_interface_get_task: sem_post mutex error!(%m)\n", errno);
        return -1;
    }

    if(sem_post(&p_interface->nempty)!=0)
    {
        //printc(MSG_E, "ln_interface_get_task: sem_post nempty error!(%m)\n", errno);
        return -1;
    }
	return 0;
}

// /*
int gdaq_shm_create1(gdaq_shm1 *p_shm)
{
	int fd=-1;
	int	ret=-1;
	shm_unlink(p_shm->name);
	fd = shm_open(p_shm->name,O_RDWR|O_CREAT|O_EXCL, FILE_MODE);
	if(fd == -1)
	{
		//printc(MSG_E, "Shm create error!(%m)\n",errno);
		return -1; 
	}
	ret = ftruncate(fd, p_shm->shm_size);
	if(-1 == ret)
	{
		//printc(MSG_E, "Shm ftruncate error!(%m)\n", errno);
		return -1; 
	}

	p_shm->ptr = mmap(NULL, p_shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(p_shm->ptr == MAP_FAILED)
	{
		//printc(MSG_E, "Shm mmap error!(%m)\n", errno);
		return -1; 
	}
	if(close(fd)==-1)
	{
		//printc(MSG_E, "Shm close error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}
// */
int gdaq_shm_open1(gdaq_shm1 *p_shm)
{
	int fd=-1;
	int ret=-1;

	fd = shm_open(p_shm->name,O_RDWR, FILE_MODE);
	if((ret=fd) == -1)
	{
		//printc(MSG_E, "Shm not found!\n");
		return -1; 
	}
	
	p_shm->ptr = mmap(NULL, p_shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(p_shm->ptr == MAP_FAILED)
	{
		//printc(MSG_E, "Shm mmap error!(%m)\n", errno);
		return -1; 
	}
	if(close(fd)==-1)
	{
		//printc(MSG_E, "Shm close error!(%m)\n", errno);
		return -1; 
	}
	return 0;
}
int gdaq_shm_close1(gdaq_shm1 *p_shm)
{
	if(munmap(p_shm->ptr, p_shm->shm_size)==-1)
	{
		//printc(MSG_E, "Shm unmap error!(%m)\n", errno);
		return -1;
	}
    return 0;
}
// /*
int gdaq_shm_delete1(gdaq_shm1 *p_shm)
{
    if(shm_unlink(p_shm->name)==-1)
	{
		//printc(MSG_E, "Shm unlink error!(%m)\n", errno);
		return -1;
	}
	return 0;
}
// */
