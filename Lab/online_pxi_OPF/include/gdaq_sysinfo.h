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
#ifndef __GDAQ_SYSINFO_H__
#define __GDAQ_SYSINFO_H__
#include "../include/gdaq_general.h"
#include "../include/gdaq_object.h"
#include "../include/gdaq_sock.h"
#include "../include/conf_file_oper.h"

//PE NIA information
#define ETHTOOL_GSET 0x00000001    /* Get settings. */
#define ETHTOOL_SSET 0x00000002    /* Set settings. */

#ifndef SIOCETHTOOL
#define SIOCETHTOOL		0x8946
#endif



struct stats_mem {
    __u64 frmkb;
    __u64 bufkb;
    __u64 camkb;
    __u64 tlmkb;
    __u64 acmkb;
    __u64 iamkb;
    __u64 slmkb;
    __u64 frskb;
    __u64 tlskb;
    __u64 caskb;
    __u64 comkb;
};

struct stats_cpu {
    __u64  cpu_user;
    __u64 cpu_nice;
    __u64 cpu_sys;
    __u64 cpu_idle;
    __u64 cpu_iowait;
    __u64 cpu_steal;
    __u64 cpu_hardirq;
    __u64 cpu_softirq;
    __u64 cpu_guest;
    __u64 cpu_number;
};

struct stats_load {
    __u64 nr_running;
    __u32  load_avg_1;
    __u32  load_avg_5;
    __u32  load_avg_15;
    __u32  nr_threads;
};


struct stats_traffic {
    __u64 bytein;
    __u64 byteout;
    __u64 pktin;
    __u64 pktout;
} ;




struct ethtool_cmd{
	__u32	cmd;
	__u32	supported;		/* Features this interface supports*/
	__u32	advertising;	/* Features this interface advertises*/
	__u16	speed;			/* The forced speed, 10Mb, 100Mb, gigabit*/
	__u8	duplex;			/* Duplex, half or full */
	__u8	port;			/* Which connector port */
	__u8	phy_address;
	__u8	transceiver;	/* Which transceiver to use */
	__u8	autoneg;		/* Enable or disable autonegotiation */
	__u32	maxtxpkt;		/* Tx pkts before generating tx int */
	__u32	maxrxpkt;		/* Rx pkts before generating rx int */
	__u32	reserved[4];
};




/*
	function definitions
*/
void print_pn_status(pn_status *pn_status);
int get_pn_status(pn_status *pn_status_);
int 	get_sys_info(sys_res *res);
int		get_nia_num();
int 	get_nia_info(char* szDevName, nia_info *np);
int		get_nia_all(nia_info p_nia[]);
int		get_stream_num(char* conf_file_name);
int		get_group_num(char* conf_file_name, __u32 stream);
int		get_group_info(char* conf_file_name, unsigned int stream, unsigned int group, channel_group_info *gp);
int		get_stream_terminal_info(char* conf_file_name, unsigned int stream, stream_terminal_info *sp);

pn_info* get_pn_info();
pn_stream_info* get_pn_stream_info(char* conf_file, struct in_addr pn_name);

void	display_pn_info(pn_info* pp);
void	display_stream_info(pn_stream_info* srp);

void	free_stream_pointer(pn_stream_info* srp);


//--------------------------------

int get_card_function_list(char *conf_file_name ,card_function_list* card_function_list_info);
int get_card_function_num(char *conf_file_name);

int get_card_channel_length(char *conf_file_name ,channel_length *channel_length_);
void display_channel_length(channel_length *sp);
int get_card_channel_num(char *conf_file_name);

void display_pn_card_info(pn_card_info *sp);

void display_card_template_list(card_template_list *sp);
int get_card_template_num(char *conf_file_name);
int get_card_template(char *conf_file_name ,card_template_list* card_template_list_info, unsigned int index );
card_template_list* get_card_template_list(char *conf_file);



#endif
