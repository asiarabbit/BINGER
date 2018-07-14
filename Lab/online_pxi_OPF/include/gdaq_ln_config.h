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
#ifndef __GDAQ_LN_CONFIG_H__
#define __GDAQ_LN_CONFIG_H__
#include "../include/gdaq_general.h"
#include "../include/gdaq_object.h"
#include "../include/gdaq_sock.h"
#include "../include/conf_file_oper.h"
#include "../include/gdaq_ipc.h"

#include "../include/iniparser.h"
#include "../include/dictionary.h"


#define DEFAULT_LN_CONFIG_FILE_PATH		"./ln_config_file/"
#define INTERFACE_NOT_DEFINE    0
#define INTERFACE_SOCKET        1
#define INTERFACE_SHM           2

typedef struct port_info_t{
    int     direction;  //IN, OUT, INOUT
    int     type;       //INTERFACE_NOT_DEFINE, INTERFACE_SOCKET, INTERFACE_SHM
    char    parameter[MAX_TMP_STRING_LENGTH];//For socket: (ip:port); for shm: (shm_name)
} port_info;
typedef struct node_info_t{
    int     node_id;        
    char    name[MAX_TMP_STRING_LENGTH];
    char    option[MAX_TMP_STRING_LENGTH];
    int     port_num;
    port_info   port[0];
} node_info;

int ln_get_direction_from_string(char *direction);
int ln_get_direction_from_int(int direction_int , char *direction);

int ln_start(char* conf_file_name);
int ln_config_get_cmd(char* conf_file_name, char* cmd);
int ln_config_get_stream_num(char* conf_file_name);
int ln_config_get_ln_num(char* conf_file_name, int stream);
int ln_config_generate_file(char * conf_file_name, int stream, int ln, char* out_file_name);


int ln_config_shm_interface_init(char* conf_file_name);
int ln_config_get_cmd_without_para(char* conf_file_name, char* cmd);


int ln_config_get_ln_port_num(char* conf_file_name);
int ln_config_get_ln_id(char* conf_file_name);
int ln_config_get_ln_cmd_name(char* conf_file_name, char* cmd_name);
int ln_config_get_ln_port_info(char* conf_file_name, int port, port_info *p_port_info);
int ln_config_ln_read_file(char* conf_file_name, node_info **p_node_info);



#endif
