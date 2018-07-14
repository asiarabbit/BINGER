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
#ifndef __GDAQ_OBJECT_C__
#define __GDAQ_OBJECT_C__
#include "gdaq_object.h"

int reg_ip_to_table(reg_ip_table* p_table, struct in_addr client_address, pn_info* p_pn)
{
    int i=0;
    for(i=0;i<p_table->reg_ip_num;i++)
    {
        if(p_table->reg_ip[i].ip_to_be_reg.s_addr == p_pn->reg_ip.s_addr)
        {
            printc(MSG_A, "The PN has been registered before, Refresh the register ip!\n");
            p_table->reg_ip[p_table->reg_ip_num].ip_reg_pos.s_addr = client_address.s_addr;
            return 1;
        }
    }
    if(p_table->reg_ip_num<MAX_PN_NUM)
    {
        p_table->reg_ip[p_table->reg_ip_num].ip_to_be_reg.s_addr = p_pn->reg_ip.s_addr;
        p_table->reg_ip[p_table->reg_ip_num].ip_reg_pos.s_addr = client_address.s_addr;
        p_table->reg_ip_num++;
    }
    else
    {
        printc(MSG_E,"Registed IP table full!\n");
        return -1;
    }
	return 0;
}

void display_reg_ip_table(reg_ip_table* p_table)
{
    #ifdef DAQ_DEBUG
    if(p_table->reg_ip_num==0)
    {
        printd("The registered IP table is empty!\n");
    }
    else
    {
        printd("Current registered IP table is:\n");
        printd("\t PN ip -> Registered Position:\n");
        int i;
        for(i=0;i<p_table->reg_ip_num;i++)
        {
            
            printd("\t%s -> ",inet_ntoa(p_table->reg_ip[i].ip_to_be_reg));
            printf("%s\n", inet_ntoa(p_table->reg_ip[i].ip_reg_pos));
        }
    }
    #endif
}

struct in_addr find_PN_path(struct in_addr addr, reg_ip_table* p_table)
{
    struct in_addr ret_addr;
    ret_addr.s_addr = 0;
    int i=0;
    for(i=0;i<p_table->reg_ip_num;i++)
    {
        if(p_table->reg_ip[i].ip_to_be_reg.s_addr == addr.s_addr)
        {
            ret_addr.s_addr = p_table->reg_ip[i].ip_reg_pos.s_addr;
            break;
        }
    }
    return ret_addr;
}


int is_process_exist(char* process_name)
{
    int flag=0;
    char szLine[512];
	char* p=NULL;
    char  temp_file_name[MAX_TMP_STRING_LENGTH];
    char  temp_cmd[MAX_TMP_STRING_LENGTH];
    memset(temp_file_name,0,sizeof(temp_file_name));
    memset(temp_cmd,0,sizeof(temp_cmd));
    sprintf(temp_file_name, "/tmp/is_process_exist_tmp_%lu",pthread_self());
    sprintf(temp_cmd, "ps -A >%s",temp_file_name);
    system(temp_cmd);
    FILE* fp = fopen(temp_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "is_process_exist: Open %s failed!(%m)\n", temp_file_name, errno);
        sprintf(temp_cmd, "rm -f %s",temp_file_name);
        system(temp_cmd);
		return -1;
	}
	
	while(fgets(szLine, sizeof(szLine), fp))
	{
        if(strstr(szLine, process_name)!= NULL)
        {
            #ifdef DAQ_DEBUG
            printd("is_process_exist: find a matched process!\n");
            printd("is_process_exist: line = %s", szLine);
            #endif
            flag = 1;
            break;
        }
    }
    fclose(fp);
    sprintf(temp_cmd, "rm -f %s",temp_file_name);
    system(temp_cmd);
    return flag;
}

int is_cmc()
{
    int flag;
    flag = is_process_exist(CMC_EXE_NAME);
    return flag;
}

void pn_info_endian_swap(pn_info * p_pn)
{
    union {char c[4]; unsigned long mylong;} endian_test={{'l','?','?','b'}};

    if(p_pn->pn_endian !=  (__s8)endian_test.mylong)
    {
        p_pn->res.cpu_num = swap32(p_pn->res.cpu_num);
        p_pn->res.cpu_online = swap32(p_pn->res.cpu_online);
        p_pn->res.total_mem = swap32(p_pn->res.total_mem);
        p_pn->res.free_mem = swap32(p_pn->res.free_mem);
        p_pn->res.total_disk_space = swap32(p_pn->res.total_disk_space);
        p_pn->res.free_disk_space = swap32(p_pn->res.free_disk_space);
        p_pn->nia_num = swap32(p_pn->nia_num);
        return ;
    }
    return;
}
void stream_info_endian_swap(pn_stream_info* p_stream_info)
{
    union {char c[4]; unsigned long mylong;} endian_test={{'l','?','?','b'}};

    if(p_stream_info->endian !=  (__s8)endian_test.mylong)
    {
        p_stream_info->stream_num = swap32(p_stream_info->stream_num);
        int i,j;
        for(i=0;i<p_stream_info->stream_num;i++)
        {
            p_stream_info->p_stream[i]->group_num = swap32(p_stream_info->p_stream[i]->group_num);
            for(j=0;j<p_stream_info->p_stream[i]->group_num;j++)
            {
                p_stream_info->p_stream[i]->group_info[j].channel_num = swap32(p_stream_info->p_stream[i]->group_info[j].channel_num);
                p_stream_info->p_stream[i]->group_info[j].frame_len= swap32(p_stream_info->p_stream[i]->group_info[j].frame_len);
                
            }
        }
    }
    return;
}

#endif

