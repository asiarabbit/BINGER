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
#ifndef __GDAQ_SOCK_H__
#define __GDAQ_SOCK_H__

#include "gdaq_general.h"
#include "gdaq_object.h"
/*
IPs and Ports definition
*/

// IPs definition

// #define	GDAQ_CMC_IP				"192.168.195.128"


//Ports definition

#define GDAQ_SYS_PN_REG_PORT	6000
#define	GDAQ_SYS_CMD_PORT		6001
#define GDAQ_SYS_DATA_PORT		6002
#define GDAQ_SYS_STATUS_PORT	6003
#define GDAQ_SYS_LOG_COLLECT	6004


//Frame information
#define	MAX_FRAME_LEN			1400

//Frame type definition
#define	FRAME_EMPTY				0x0
#define FRAME_PN_REG			0x1
#define	FRAME_SYS_CMD			0x2
#define	FRAME_SYS_DATA		    0x3
#define	FRAME_SYS_STATUS		0x4
#define	FRAME_LOG_COLLECT		0x5
#define FRAME_USER		        0x10


//Frame ID definition -- FRAME_PN_REG
#define	PN_REG_NONE				0x0
#define PN_REG_BROADCAST		0x1
#define PN_REG_STOP_BROADCAST	0x2
#define PN_REG_PN_INFO			0x3
#define	PN_REG_START_CONSOLE_REG_SERV  0x4
#define PN_REG_LINK_TEST        0x5



//Frame ID definition -- FRAME_SYS_CMD
#define SYS_CMD_NONE            0x0
#define SYS_CMD_LINK_TEST       0x1
//#define SYS_CMD_GET_STREAM_INFO  0x2
#define SYS_CMD_LN_CONFIG        0x3
#define SYS_CMD_GET_CARD_INFO 0x4
//#define SYS_CMD_SET_CARD_OFFSET 0x5
//#define SYS_CMD_SET_CHANNEL_LENGTH 0x6
#define SYS_CMD_SET_GLOBAL_CHANNEL_LENGTH 0x7
#define SYS_CMD_SET_CARD_FUNCTION 0x8
#define SYS_CMD_SEND_FILE       0x9
#define SYS_CMD_CLOSE_LN        0xA
//filename
//#define SYS_CMD_GET_PN_STATUS 0x9


//Frame ID definition -- FRAME_SYS_DATA
#define SYS_DATA_NONE            0x0
#define SYS_DATA_LINK_TEST       0x1
#define SYS_DATA_STREAM_INFO     0x2
#define SYS_CMD_CONFIG_LN_FREEBACK  0x3
#define SYS_DATA_CARD_INFO 0x4
#define SYS_DATA_PN_STATUS 0x5
#define SYS_DATA_LOG_MSG 0x6



//Frame ID definition -- FRAME_SYS_STATUS
#define SYS_STAUS_NONE            0x0
#define SYS_STATUS_IDLE       0x1
#define SYS_STATUS_MSG            0x2
#define SYS_STATUS_LOG            0x3
//#define SYS_DATA_STATUS_REPORT        0x6



//Frame ID definition -- FRAME_USER
#define USER_NONE            	0x0
#define	USER_DATA_BASE			0x1

/*
macro and struct definition
*/
#define CMD_FILE_PATH_LENGTH 256



/* 
functions definition
*/
int gdaq_send_frame(struct in_addr server_ip, __u16 port, gdaq_frame_head* head, void * frame_content);
int gdaq_send_cmd(struct in_addr server_ip, __u16 port, gdaq_frame_head* head, struct in_addr target, void * cmd_content);
int gdaq_send_stream_info(struct in_addr server_ip, __u16 port, pn_stream_info* p_stream_info);
pn_stream_info* gdaq_get_stream_info_from_frame(char* buffer, int length);
int gdaq_send_cmd_file(struct in_addr server_ip,struct in_addr target_ip, char *file_path ,char *target_file_path);
int gdaq_rcv_frame(int client_sockfd, gdaq_frame_head *p_fh_rcv, char** buffer);


#endif
