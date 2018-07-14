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
/*
General variables, structs, and fuctions are defined here.
*/

/*
General micro switch definition
*/
#ifndef __GDAQ_GENERAL_H__
#define __GDAQ_GENERAL_H__

#include "gdaq_log.h"
//#define	DAQ_CHECK_MEM_LEAK		//if check memory leak

//#define DAQ_DISPLAY_LOG		//if display the log file

#define DAQ_SHWMSG				//print,   display the message when running
#define DAQ_DEBUG				//printd,  display the debug message 
#define DAQ_COLOR				//printc,  display the message in color mode
#define	DAQ_DISPLAY_BANNER		//display the banner


/*
General headers definition
*/
#ifdef DAQ_CHECK_MEM_LEAK
#include <mcheck.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/vfs.h>


/*
POSIX IPC is used in the project 
*/
//#include <sys/shm.h>
//#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <mqueue.h>



#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>

#include "./gdaq_typedef.h"


#include "gdaq_log.h"

/*
General constant definition
*/
#define LEN_512 512
#define LEN_256 256
#define LEN_128 128
#define LEN_64  64
#define LEN_32 32



#define USER_DATA_PACK_DISABLE


#define MAX_TMP_STRING_LENGTH	128
#define MAX_NIA_NUM		16		//max nia num in one PN
#define MAX_PN_NUM		1024	//max PN num in system
#define MAX_LN_NUM      128    //max LN num in one PN
#define MAX_LN_PAGE_TYPE    16  //max page type for one LN in the global shared buffer


#define DEFAULT_LN_CONFIG_FILE 	"./ln_config_info"
#define DEFAULT_SAVE_DATA_PATH  "./data_file"
#define DEFAULT_INTERFACE_TASK_NUM       	3
#define DEFAULT_THREAD_TASK_QUEUE_LENGTH	3
#define	MAX_DATA_PAGE_ID			    0xfffff
#define MAX_DATA_INDEX                  0x7fffffff


#define DEFAULT_GLOBAL_BUFFER_PAGE_SIZE         32768
#define DEFAULT_GLOBAL_BUFFER_PAGE_NUM          8192

#define DEFAULT_TCP_BUFFER_LENGTH               (DEFAULT_GLOBAL_BUFFER_PAGE_SIZE+1024)


#define DEFAULT_SOCKET_DATA_SEND_THREAD_NUM   6


#define DEFAULT_GLOBAL_BUFFER_NAME              "global_buffer"

#define	GLOBAL_SHARED_INFO	    "global_shared_info"
#define	GLOBAL_MSG_QUEUE		"/global_msg_queue"     //must start with "/" and only one "/" can be existed in the name 

/*
General macro function definition
*/
#define	POSIX_IPC_PREFIX	"/tmp/"


#ifdef  __DAQ_DISPLAY_LOG__	
#define LOG_FILE_DIR	"../log"
#define	LOG_FILE		"../log/DAQ_log"
#define fprint(FILE_FP, fmt, args...)	fprintf(FILE_FP, fmt, ##args)
#else
#define fprint(FILE_FP, fmt, args...)
#endif

/////////// define normal print
#ifdef DAQ_SHWMSG
 #define print(fmt, args...)	printf("\033[0m"fmt, ##args)
#else
 #define print(fmt, args...)
#endif


////// define debug printf
#ifdef DAQ_DEBUG
 #define printd( fmt, args... ) do{\
    print( fmt, ##args );\
    fflush(stdout);\
}while(0)
#else
 #define printd( fmt, args... ) ///////don't display debug message
#endif


 #define	MSG_N	0
 #define	msg_n	0
 #define	MSG_T	1
 #define	msg_t	1
 #define	MSG_A	2
 #define	msg_a	2
 #define	MSG_E	3
 #define	msg_e	3
 #define	MSG_R   4
 #define	msg_r	4

//if( LEVEL == MSG_N )\
//    printf( "\033[01;32m<NRML> [%s:%d][%s] "fmt"\033[0m",__FILE__,__LINE__,__FUNCTION__ ,##args );\
//else if( LEVEL == MSG_T )\
//    printf( "\033[01;34m<TIPS> [%s:%d][%s] "fmt"\033[0m", __FILE__,__LINE__,__FUNCTION__ ,##args );\
//else if( LEVEL == MSG_A )\
//    printf( "\033[01;35m<ALRT> [%s:%d][%s] "fmt"\033[0m", __FILE__,__LINE__,__FUNCTION__ ,##args );\
//else if( LEVEL == MSG_E )\
//    printf( "\033[01;31m<EROR> [%s:%d][%s] "fmt"\033[0m", __FILE__,__LINE__,__FUNCTION__ ,##args );\
//else if( LEVEL == MSG_R )\
//    printf( "\033[01;31m       [%s:%d][%s] "fmt"\033[0m", __FILE__,__LINE__,__FUNCTION__ ,##args );\

//if( LEVEL == MSG_N )\
//       printf( "\033[01;32m<NRML> "fmt"\033[0m", ##args );\
//   else if( LEVEL == MSG_T )\
//       printf( "\033[01;34m<TIPS> "fmt"\033[0m", ##args );\
//   else if( LEVEL == MSG_A )\
//       printf( "\033[01;35m<ALRT> "fmt"\033[0m", ##args );\
//   else if( LEVEL == MSG_E )\
//       printf( "\033[01;31m<EROR> "fmt"\033[0m", ##args );\
//   else if( LEVEL == MSG_R )\
//       printf( "\033[01;31m       "fmt"\033[0m", ##args );\
#define LOG_COLLECT 1
    #ifdef LOG_COLLECT
        #define printc( LEVEL, fmt, args... ) do{\
            if( LEVEL == MSG_N )\
                   { log_debug(fmt,##args);}\
               else if( LEVEL == MSG_T )\
                   { log_info(fmt,##args);}\
               else if( LEVEL == MSG_A )\
                   { log_alert(fmt,##args);}\
               else if( LEVEL == MSG_E )\
                   { log_error(fmt,##args);}\
               else if( LEVEL == MSG_R )\
                   { log_emergent(fmt,##args);}\
         }while(0)
    #else
        #define printc( LEVEL, fmt, args... ) do{\
            if( LEVEL == MSG_N )\
                   printf( "\033[01;32m<NRML> "fmt"\033[0m", ##args );\
               else if( LEVEL == MSG_T )\
                   printf( "\033[01;34m<TIPS> "fmt"\033[0m", ##args );\
               else if( LEVEL == MSG_A )\
                   printf( "\033[01;35m<ALRT> "fmt"\033[0m", ##args );\
               else if( LEVEL == MSG_E )\
                   printf( "\033[01;31m<EROR> "fmt"\033[0m", ##args );\
               else if( LEVEL == MSG_R )\
                   printf( "\033[01;31m       "fmt"\033[0m", ##args );\
            fflush(stdout);\
         }while(0)
    #endif



#ifdef DAQ_DISPLAY_BANNER
 #define PRINT_DAQ_BANNER do{                                                                                    \
	printf("\033[0m\n");                                                                                 \
	printf("===============================================================================\n");    \
	printf("\033[01;32m               GGGGGG             DDDDDDDD            AA           QQQQQQ      \033[0m\n");    \
	printf("\033[01;32m            GG      GG             DD     DD        AAAA        QQ      QQ     \033[0m\n");    \
	printf("\033[01;32m         GG                       DD      DD      AA  AA      QQ        QQ     \033[0m\n");    \
	printf("\033[01;32m       GG                        DD       DD     AA  AA     QQ          QQ     \033[0m\n");    \
	printf("\033[01;32m      GG                        DD       DD    AA    AA    QQ          QQ      \033[0m\n");    \
	printf("\033[01;32m     GG                 GDAQ   DD       DD    AA    AA    QQ          QQ       \033[0m\n");    \
	printf("\033[01;32m    GG           GGGG         DD       DD   AA      AA   QQ          QQ        \033[0m\n");    \
	printf("\033[01;32m   GG            GG          DD       DD   AAAAAAAAAA   QQ       QQ QQ         \033[0m\n");    \
	printf("\033[01;32m   GG          GGG          DD      DD   AA        AA   QQ        QQ           \033[0m\n");    \
	printf("\033[01;32m    GG      GG GG          DD     DD    AA        AA    QQ      QQ QQ          \033[0m\n");    \
	printf("\033[01;32m     GGGGGG   GG        DDDDDDDD      AA          AA     QQQQQQ    QQ          \033[0m\n");    \
	printf("================================================================================\n");    \
	printf("Version  :		\033[01;31mV 1.0\033[0m \n");    \
	printf("CopyRight:		\033[01;31mFast Electronic Laboratory\033[0m [\033[01;34mUSTC\033[0m]\n");    \
	printf("================================================================================\n");    \
	printf("\n");                                                                                        \
}while(0)
#else
 #define PRINT_DAQ_BANNER
#endif

#define print_memory(p, num) do{  \
        int i;  \
        char* p_tmp=(char*)p;   \
        printc(MSG_T,"[memory information]\n"); \
        for(i=0;i<num;i++) {   \
            if(i%16==0) \
                printd("%2x",(__u8)p_tmp[i]); \
            else if(i%16==15)   \
                printf(" %2x\n",(__u8)p_tmp[i]);  \
            else    \
                printf(" %2x",(__u8) p_tmp[i]); }   \
        printc(MSG_T,"[memory information end]\n");   \
    } while(0)
#define gdaq_exit(value)    do{ \
    printc(MSG_E,"A Bad Error Happened!!!!!!!!!\n");  \
    exit(value);    \
    } while(0)


#define swap32(x) ((__u32)( \
	 (((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
	 (((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
	 (((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
	 (((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define	swap16(x)	((__u16)( \
	(((__u16)(x) &	(__u16)0x00ff) << 8) | \
	(((__u16)(x) &  (__u16)0xff00) >>8)))


#define min(x,y)	(((x)<(y))?(x):(y))
#define max(x,y)	(((x)>(y))?(x):(y))
#define queue_ahead(x,y,queue_length)     ((((x)>(y))&&(((x)-(y))>(queue_length/2)))||(((x)<(y))&&(((y)-(x))<(queue_length/2))))

/*
General structs definition
*/

/*
General functions definition
*/



#endif //__DAQ_GENERAL_H__

