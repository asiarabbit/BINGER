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
#ifndef __GDAQ_OBJECT_H__
#define __GDAQ_OBJECT_H__
#include "gdaq_general.h"
#include "list.h"
#include "gdaq_data_format.h"
//#include "gdaq_sock.h"

#pragma pack(4)

/*
DAQ system object structs definition
*/
#define CMC_EXE_NAME    "cmc.exe"


//definition of stream direction
#define NOT_DEFINED 0
#define	IN		1
#define OUT		2
#define INOUT	3

//definition of stream type
#define GATHER		1
#define BROADCAST	2

//definition of the system server flag

#define ALL_SERVER_NOT_RUNNING          ((__u32)0x00000000UL)
#define BROADCAST_SERVER_IS_RUNNING     ((__u32)0x00000001UL)
#define PN_REG_SERVER_IS_RUNNING        ((__u32)0x00000002UL)
#define SYS_CMD_SERVER_IS_RUNNING       ((__u32)0x00000004UL)
#define SYS_DATA_SERVER_IS_RUNNING      ((__u32)0x00000008UL)
#define SYS_STATUS_SERVER_IS_RUNNING      ((__u32)0x00000016UL)
#define SYS_LOG_COLLECT_IS_RUNNING      ((__u32)0x00000032UL)

//using in system configuration


//typedef struct  pn_status_t
//{
//    struct in_addr pn_name ;
//    __u32 status;
//    __u32 length;
//    //void *msg;
//}pn_status;

//-----------------------------------------------

typedef struct card_function_list_t{
__u8 type;
__u32 num;
__s8 function[0][MAX_TMP_STRING_LENGTH];
}card_function_list;


typedef struct card_template_t{
    __u8  type;
    __s8  name[MAX_TMP_STRING_LENGTH];
    __u32 channel_num ;
    __u32 frame_length ;
    __u8  channel_full ;
    __u8  channel_change;
}card_template;

typedef struct card_template_list_t{
    __u32 card_num;
    card_template card_template_[0];
} card_template_list;


typedef struct card_info_t{
__u32 id;
__s8 module_name[MAX_TMP_STRING_LENGTH];
__s8 dev_name[MAX_TMP_STRING_LENGTH];
__s8 type[MAX_TMP_STRING_LENGTH];
__u32 start_channel;
__u32 channel_num;
}card_info;

typedef struct pn_card_info_t{
struct in_addr pn_name;
__s8  endian;
__u32 card_num;
card_info card_info_[0];
} pn_card_info;




typedef struct channel_length_t{
__s8 type;// type =-1
__u32 num; //表示有多少个通道片段
__u32 length[0][3];// //length[0][0] 表示起始通道号,length[0][1]表示结束通道号,length[0][2] 表示这些通道片段的数据帧长度
}channel_length;



typedef struct pn_status_t{

    __u64 time;  //HH:MM:SS
    __u64 mem_free;
    __u64 mem_used;  // M
    __u64 mem_buffer;
    __u64 mem_cache;
    __u64 mem_total;// M
    struct in_addr pn_name;//4
    __u32 net_speed_in;  // KB
    __u32 net_speed_out ; //KB
    __u8  cpu_user; // %
    __u8  cpu_sys;  // %
    __u8  cpu_wait; // %
    __u8  cpu_idle; // %
}pn_status;


//---------------------------------










typedef struct channel_group_info_t{
	__s8			name[MAX_TMP_STRING_LENGTH];		//group name: MRPC, MWDC, etc
	__s8			type[MAX_TMP_STRING_LENGTH];		//group type: PCI, PXI, PCI-E, etc, maybe related to the speed calculation in MCM
	__u8			direction;		//the direction related to the GDAQ processes 
	__u32			channel_num;		//the channel numbers in this group
	__u32			frame_len;		//the frame length in bytes of this channel
	__s8			interface_name[MAX_TMP_STRING_LENGTH];
} channel_group_info;

typedef struct stream_terminal_info_t{
	__s8			name[MAX_TMP_STRING_LENGTH];
	__u8			type;
	__u32			group_num;
	channel_group_info	group_info[0];	
} stream_terminal_info;

typedef struct pn_stream_info_t{
    struct in_addr pn_name;
    __s8        endian;
	__u32		stream_num;	//the number of local stream with terminal
	stream_terminal_info	*p_stream[0];		//the pointer of stream terminal array
} pn_stream_info;


typedef struct sys_res_t{			//Free resource of the PN
	__u32	cpu_num;				//CPU number
	__u32	cpu_online;				//CPU online number
	__u32	total_mem;				//Total Memory(MByte)
	__u32 	free_mem;				//Free Memory (MByte)
	__u32	total_disk_space;		//Total Disk Space(MByte)
	__u32	free_disk_space;			//Free Disk Space(MByte)
} sys_res;

typedef struct nia_info_t{
	__s8 	name[MAX_TMP_STRING_LENGTH];
	struct in_addr ip;					//all IP information is net format
	struct in_addr broad_ip;
	struct in_addr mask;
	__u32 	speed;
} nia_info;

typedef struct pn_info_t{
	struct in_addr 	reg_ip;
	__s8			pn_endian;
	__s8			name[MAX_TMP_STRING_LENGTH];	//PN name
	sys_res			res;
	__u32 			nia_num;	//the number of NIA
	nia_info		p_nia[0];		//the pointer of NIA array
} pn_info;


typedef struct pn_ip_reg_pair_t{
	struct in_addr ip_to_be_reg;		//ip to be registered, net format
	struct in_addr ip_reg_pos;			//the ip which can be used to access the PN, net format
} pn_ip_reg_pair;

typedef	struct reg_ip_table_t{
    struct in_addr pn_name;
    struct in_addr superior_pn;
    __u32   reg_ip_num;
	pn_ip_reg_pair reg_ip[MAX_PN_NUM];		// registered IP information will be stored here.
}	reg_ip_table;

typedef struct global_shared_info_t{
    __u32   server_status;
    __u8    broadcast_serv_ena;
    reg_ip_table ip_table;
} global_shared_info;

typedef struct reg_ip_list_t{
    __u32   num;
    struct in_addr reg_ip[0];
} reg_ip_list;

typedef struct nia_link_t{
    struct in_addr nia1;
    struct in_addr nia2;
} nia_link;


typedef struct channel_info_t{
    __u32     stream_num;
    __u32     group_num;
    __u32     offset_in_group;
} channel_info;

typedef struct pn_list_t{
    __u32       num;
    __u32       pn_no[0];
} pn_list;

//using in stream and LN configuration
#define MSG_NONE			0
#define MSG_LN_CONFIG		1
#define MSG_LN_CLOSE		2

typedef struct gdaq_msg_t{
	__u8		msg_type;
	char		content[MAX_TMP_STRING_LENGTH];
} gdaq_msg;



//using in system working

typedef struct ln_page_descriptor_t{
    __s32       ln_id;
    __s32       type;
    __s32       page_id;
} ln_page_descriptor;

typedef struct ln_page_addr_t{
	ln_page_descriptor      ln_page;
	int			offset;
} ln_page_addr;

typedef struct shm_fragment_pointer_t{
    ln_page_descriptor  ln_page;
    __s32   			fragment_id;
} shm_fragment_pointer;

typedef struct shm_interface_page_fragment_t{
    data_fragment_head      fragment_head;
    ln_page_addr			fragment_addr;
    shm_fragment_pointer    next;
} shm_interface_page_fragment;


typedef struct shm_interface_page_descriptor_t{
    data_section_head   section_head;
	int					map_factor;
    shm_fragment_pointer  last_fragment_addr;
	pthread_mutex_t		mutex;
    shm_interface_page_fragment    fragment[0];
} shm_interface_page_descriptor;

typedef struct shm_data_head_t{
	data_section_head	section_head;
	data_fragment_head	fragment_head[0];
} shm_data_page_descriptor;




typedef struct gdaq_frame_head_t
{
	__u8 frame_type;
	__u8 frame_id;
    int	 frame_length;	//the length of the frame excluding the fream head
} gdaq_frame_head;


typedef struct gdaq_data_frame_head_t{
	gdaq_frame_head	frame_head;
    __s32			page_id; 
	__s32			page_size;      
} gdaq_data_frame_head;



/*
functions
*/




int is_cmc();
int is_console();


int reg_ip_to_table(reg_ip_table* p_table, struct in_addr client_address, pn_info* p_pn);
void display_reg_ip_table(reg_ip_table* p_table);
struct in_addr find_PN_path(struct in_addr addr, reg_ip_table* p_table);
void pn_info_endian_swap(pn_info * p_pn);
void stream_info_endian_swap(pn_stream_info* p_stream_info);




#endif

