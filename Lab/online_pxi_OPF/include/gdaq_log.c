#include "gdaq_log.h"

#include "gdaq_ipc.h"

typedef struct msg_queue_t{
    char mq_name[MAX_TMP_STRING_LENGTH];
    mqd_t	mqd;
}msg_queue;

#define DEFAULT_MSG_TYPE 1
//typedef struct msg_queue_data_t{
//    __u8		msg_type;
//    char		content[MAX_LOG_MSG_SIZE];
//} msg_queue_data;

typedef struct msg_queue_data_t{
    __u8		msg_type;
   log_msg      log_msg_data;
} msg_queue_data;

void print_log_msg_local(log_msg *log_msg_data)
{
    if(log_msg_data->level ==LOG_LEVEL_EMGT)
    {
        printf("\033[01;31mlog_local__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else if(log_msg_data->level ==LOG_LEVEL_EROR)
    {
        printf("\033[01;31mlog_local__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else if(log_msg_data->level ==LOG_LEVEL_ALET)
    {
        printf("\033[01;35mlog_local__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else if(log_msg_data->level ==LOG_LEVEL_INFO)
    {
        printf("\033[01;34mlog_local__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else
    {
        printf("\033[01;32mlog_local__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    fflush(stdout);
}

//int msg_queue_open_or_create(msg_queue* mq)
//{
//    mq_unlink(mq->mq_name);
//    mq->mqd = mq_open(mq->mq_name,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR,NULL);
//    if(mq->mqd == -1)
//    {
//       // printc(MSG_E, "Message queue create error!(%m)\n", errno);
//        return INV_HANDLER;
//    }
//    return NOERR;
//}


int msg_queue_create(msg_queue* mq)
{
    mq_unlink(mq->mq_name);
    mq->mqd = mq_open(mq->mq_name,O_RDWR|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,NULL);
    if(mq->mqd == -1)
    {
       // printc(MSG_E, "Message queue create error!(%m)\n", errno);
        return INV_HANDLER;
    }
    return NOERR;
}

int msg_queue_open(msg_queue* mq)
{
    mq->mqd = mq_open(mq->mq_name,O_RDWR,S_IRUSR|S_IWUSR,NULL);
    if(mq->mqd == -1)
    {
       // printc(MSG_E, "Message queue open error!(%m)\n", errno);
        return INV_HANDLER;
    }
    return NOERR;
}

int msg_queue_delete(msg_queue* mq)
{
    if(mq_unlink(mq->mq_name)!=0)
    {
       // printc(MSG_E, "Message queue delete error!(%m)\n", errno);
        return INV_HANDLER;
    }
    return NOERR;
}

int msg_queue_close(msg_queue* mq)
{
    if(mq_close(mq->mqd)==-1)
    {
       // printc(MSG_E, "Message queue open error!(%m)\n", errno);
        return INV_HANDLER;
    }
    return NOERR;
}

int msg_queue_send(msg_queue* mq, msg_queue_data* msg)
{
    if(mq_send(mq->mqd, (char*)msg, sizeof(msg_queue_data), DEFAULT_MSG_PRIO)!=0)
    {
       // printc(MSG_E, "Message queue send error!(%m)\n", errno);
        return INV_HANDLER;
    }
    return NOERR;
}
int msg_queue_recv(msg_queue* mq, msg_queue_data* msg)
{
    unsigned int prio;
    struct mq_attr  attr;
    char*   buffer;

    if(mq_getattr(mq->mqd,&attr)!=0)
    {
       // printc(MSG_E, "Get message queue attribution error!(%m)\n", errno);
        return INV_HANDLER;
    }
   // puts("eeeeeeeeeeeeeee");
   // printf("!!!!!!!!!ss%d\n" ,(int)attr.mq_msgsize );
    if((buffer=(char *)malloc(attr.mq_msgsize))==NULL)
    {
       // printc(MSG_E, "Malloc message queue receive buffer error!(%m)\n", errno);
        return INV_HANDLER;
    }
    int n = mq_receive(mq->mqd, buffer, attr.mq_msgsize, &prio);
    if(n==-1)
    {
       // printc(MSG_E, "Message queue receive error!(%m)\n", errno);
        free(buffer);
        return INV_HANDLER;
    }

    if(n!=sizeof(msg_queue_data))
    {
       // printc(MSG_E, "Message queue receive data type is not matched!\n");
        free(buffer);
        return INV_HANDLER;
    }

    memcpy(msg, buffer, sizeof(msg_queue_data));
    free(buffer);
    return NOERR;
}



int current_log_level;

static msg_queue  log_msg_queue;
char pn_name[32];
int is_init=-1;
int is_create=-1;
int init_msg_queue()
{
    sprintf(log_msg_queue.mq_name , DEFAULT_MSG_QUEUE_NAME);
    if(msg_queue_create(&log_msg_queue)!=NOERR)
    {
        printf("create msg queue error %m\n");
        return INVIO;
    }
    is_create=1;
}


void init_pn_name()
{
    int num=0;
    log_debug_default("init pn name\n");
    if((num=get_nia_num())<=0)
    {
        strcpy(pn_name , "localhost");

    }
    else
    {
        nia_info *nia_info_list =(nia_info*) malloc(num*sizeof(nia_info));
        get_nia_all(nia_info_list);
        strcpy(pn_name , inet_ntoa(nia_info_list[0].ip));
        free(nia_info_list);
    }

}

int init_log_client()
{


    init_pn_name();


    current_log_level = LOG_LEVEL_WARN;
    printf("initial log debug %d\n" , LOG_LEVEL_MAX);
    sprintf(log_msg_queue.mq_name , DEFAULT_MSG_QUEUE_NAME);
    if(msg_queue_open(&log_msg_queue)!=NOERR)
    {
        printf("open msg queue error %m\n");
        return INVIO;
    }
    is_init=1;
    return NOERR;
}

inline int set_current_log_level(__u8 level)
{
    if(level<0||level>LOG_LEVEL_MAX)
    {
        return INVIO;
    }
   current_log_level = level;
   return NOERR;
}


char *basedir(char *path , char *dir_name)
{
    memset(dir_name , 0, sizeof(dir_name));
    if(path==NULL)
    {
        return dir_name;
    }
    char *s = strrchr(path, '/');
    if (!s)
    {
        strcpy(dir_name ,".");
        return dir_name;
    }
    else
    {
        strncpy(dir_name ,path , s-path+1);
        dir_name[s-path+1]='\0';
        return dir_name;
    }
}

char *basename(char *path)
{
    if(path==NULL)
    {
        return NULL;
    }
    char *s = strrchr(path, '/');
    if (!s)
    {
        return path;
    }
    else
    {
        if(strlen(s)<=1)
        {
            return path;
        }
        else
        {
            return  s+1;
        }
    }
}

/* return the total length of the log*/
int format_log_msg(__u8 log_level,log_msg *log_msg_data,const char* fmt,va_list args)
{
    strncpy(log_msg_data->pn_name , pn_name , sizeof(log_msg_data->pn_name));
    log_msg_data->level = log_level;
    log_msg_data->time = time(NULL);
    //int tmp = sprintf(log_msg_data->data , "[%s:%d][%s]",basename(__FILE__),__LINE__,__FUNCTION__);
    //int log_msg_datasize= sizeof(log_msg_head);
    int iret= vsnprintf((log_msg_data->data),(MAX_LOG_MSG_SIZE-2),fmt,args);
    //add the \n
    if(log_msg_data->data[iret-1] != '\n')
    {
        log_msg_data->data[iret]   = '\n';
        log_msg_data->data[iret+1] = '\0';
        iret = iret + 2;
    }
    else
    {
        log_msg_data->data[iret] = '\0';
        iret = iret + 1;    /*create the daemon thread*/

    }
    log_msg_data->length = iret;


    //memcpy(buff , &log_msg_data , log_msg_datasize);
    return NOERR;
}

int echo_log(__u8 log_level , const char *fmt,...)
{

    //if(log_msg_queue.)
    msg_queue_data msg_queue_data_;
    msg_queue_data_.msg_type = DEFAULT_MSG_TYPE;
    va_list args;
    va_start( args, fmt );
    format_log_msg(log_level, &msg_queue_data_.log_msg_data , fmt, args);
    va_end( args );

    if(is_init!=1)//msq_queue not initial
    {

       print_log_msg_local(&msg_queue_data_.log_msg_data);
       return NOERR;
    }

    if(msg_queue_send(&log_msg_queue , &msg_queue_data_)!=NOERR)
    {
        printf("send msg error , %m\n");
        return INVIO;
    }
    //printf("send msg success dd\n");
    //print_log_msg(&msg_queue_data_.log_msg_data);
    return NOERR;
}
void print_log_msg(log_msg *log_msg_data)
{
    if(log_msg_data->level ==LOG_LEVEL_EMGT)
    {
        printf("\033[01;31mlog__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else if(log_msg_data->level ==LOG_LEVEL_EROR)
    {
        printf("\033[01;31mlog__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else if(log_msg_data->level ==LOG_LEVEL_ALET)
    {
        printf("\033[01;35mlog__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else if(log_msg_data->level ==LOG_LEVEL_INFO)
    {
        printf("\033[01;34mlog__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    else
    {
        printf("\033[01;32mlog__pn:%s:level(%d),time(%ld),length(%d),data:%s\n\033[0m" , log_msg_data->pn_name , log_msg_data->level ,log_msg_data->time,log_msg_data->length ,log_msg_data->data);
    }
    fflush(stdout);
}





int get_log_msg(log_msg *log_msg_data)
{

    if(log_msg_data==NULL)
    {
        return -1;
    }
    msg_queue_data msg_queue_data_;

    if(msg_queue_recv(&log_msg_queue , &msg_queue_data_)==NOERR)
    {
        memcpy(log_msg_data,&msg_queue_data_.log_msg_data , sizeof(log_msg));
    }
    else
    {
        return -1;
    }


}


/**
* @brief log_debug_default
* @param fmt
* @return
*/
int log_debug_default(const char* fmt,...)
{
    int iret = 0;
   char log_doc_buff[MAX_LOG_MSG_SIZE];
//    if(log_debug_fd == INV_HANDLER)
//        return INVIO;
//    /*format the string*/
       va_list args;
        va_start( args, fmt );
        iret = vsnprintf(log_doc_buff,MAX_LOG_MSG_SIZE,fmt,args);
        va_end( args );

//    if(debug_level & DEBUG_PRINT)
//    {
        printf("log debug : %s",log_doc_buff);
//    }
//    if(debug_level & DEBUG_LOCAL)
//    {
//        if(log_debug_fd == INV_HANDLER)
//            return INVIO;
//        write(log_debug_fd,log_doc_buff,iret);
//    }
    return NOERR;
}
