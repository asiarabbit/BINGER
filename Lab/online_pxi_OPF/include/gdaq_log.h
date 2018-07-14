#ifndef LOG_CLIENT_H
#define LOG_CLIENT_H


#include <stdio.h>
#include "gdaq_typedef.h"
#include "gdaq_general.h"
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
//#include "gdaq_object.h"

#define DEFAULT_MSG_TYPE 1
//msg type


#include "gdaq_typedef.h"

#define MAX_LOG_SIZE (1*1024*1024*1024)
#define DEFAULT_MSG_QUEUE_NAME "/log_msg_queue_fads" //must start with "/" and only one "/" can be existed in the name

#define IPC_MSG 1
#define MAX_LOG_MSG_SIZE 512



extern int init_log_client( );
extern int echo_log(__u8 log_level , const char *fmt,...);


typedef struct log_msg_t
{
    time_t time;
    __u8 level;
   __u16 length;
   char pn_name[20];
   char data[MAX_LOG_MSG_SIZE];
}log_msg;




enum log_level
{
    LOG_LEVEL_DEBG = 0,       /* debug-level messages              */
    LOG_LEVEL_INFO = 1,       /* informational                    */
    LOG_LEVEL_NOTC = 2,       /* normal but significant condition  */
    LOG_LEVEL_WARN = 3,       /* warning conditions                */
    LOG_LEVEL_EROR = 4,       /* error conditions                  */
    LOG_LEVEL_CRIT = 5,       /* critical conditions               */
    LOG_LEVEL_ALET = 6,       /* action must be taken immediately  */
    LOG_LEVEL_EMGT = 7,       /* system is unusable                */
    LOG_LEVEL_MAX = LOG_LEVEL_EMGT  //log_level_max =7
};
static char log_level_map[][LOG_LEVEL_MAX+1] =
{
    "[DEBG]",
    "[INFO]",
    "[NOTC]",
    "[WARN]",
    "[EROR]",
    "[CRIT]",
    "[ALET]",
    "[EMGT]"
};


int log_level_to_string(__u8 level ,char *str);

char *basename(char *path);
char *basedir(char *path , char *dir_name);





#define log_output(level , log_fmt, log_arg...) \
    do{ \
        echo_log(level,   "[%s:%d][%s] " log_fmt, basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)


#define log_debug(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_DEBG,   "[%s:%d][%s] " log_fmt "\n",  basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)


#define log_info(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_INFO,   "[%s:%d][%s] " log_fmt "\n", basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)

#define log_nocitce(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_NOTC,   "[%s:%d][%s] " log_fmt "\n", basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)


#define log_warn(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_WARN,   "[%s:%d][%s] " log_fmt "\n",     basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)

#define log_error(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_EROR,   "[%s:%d][%s] " log_fmt, basename(basename(__FILE__)), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)

#define log_critic(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_CRIT,   "[%s:%d][%s] " log_fmt, basename(basename(__FILE__)), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)

#define log_alert(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_ALET,   "[%s:%d][%s] " log_fmt, basename(basename(__FILE__)), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)

#define log_emergent(log_fmt, log_arg...) \
    do{ \
        echo_log(LOG_LEVEL_EMGT,   "[%s:%d][%s] " log_fmt, basename(basename(__FILE__)), __LINE__, __FUNCTION__, ##log_arg); \
    } while (0)



//#define log_notice(log_fmt, log_arg...) \
//    do{ \
//        echo_log(LL_NOTICE,   "[%s:%d][%s] " log_fmt "\n", \
//                     basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
//    } while (0)

//#define log_trace(log_fmt, log_arg...) \
//    do{ \
//        echo_log(LL_TRACE,   "[%s:%d][%s] " log_fmt "\n", \
//                     basename(__FILE__), __LINE__, __FUNCTION__, ##log_arg); \
//    } while (0)



#define INV_HANDLER         -1
/*define the numeric error code*/
#define NOERR     0
#define NOMEM     1
#define INVIO     2
#define NOSPC     3
#define NOSRV     4
#define NOPMT     5
#define SKTMO     6
#define RSTBP     7
#define REJBP     8
#define NOCNF     9
#define LVERR     10
#define FLKFI     11
#define FGTHL     12
#define FLDON     13
#define FITSM     14
#define FMPMM     15
#define FSTSM     16
#define ERRMP     17
#define FQFUL     18
#define FDNRG     19
#define NITCL     20
#define FLOG      21
#define FUDPY     22

typedef struct err_map_t
{
    char* name;
    int   number;
}err_map;
static err_map log_error_map[] =
{
    {"No error",0},
    {"Out of memory",1},
    {"Invalid file handler",2},
    {"No disk space",3},
    {"No server is available",4},
    {"Operation not permitted",5},
    {"Socket time out",6},
    {"Socket reset by the peer",7},
    {"Socket rejected by the peer",8},
    {"No configuration file found",9},
    {"log level error",10},
    {"Failed to lock the lock file",11},
    {"Failed to get the share memory handler",12},
    {"Failed to initialize the client daemon",13},
    {"Failed to initialize the share memory",14},
    {"Failed to map the share memory to the process",15},
    {"Failed to assosiate the sem body with the sem id",16},
    {"Error to release the semphone",17},
    {"Failure,queue is full",18},
    {"Failure,daeon is running",19},
    {"The client was NOT initialized",20},
    {"Finished the logical,return",21},
    {"Failed to update the policy file",22},
    {NULL,-1}
};




int init_msg_queue();
int init_log_client( );
inline int set_current_log_level(__u8 level);
int echo_log(__u8 log_level , const char *fmt,...);

void print_log_msg(log_msg *log_msg_data);
int get_log_msg(log_msg *log_msg_data);

int log_debug_default(const char* fmt,...);

#endif // LOG_CLIENT_H
