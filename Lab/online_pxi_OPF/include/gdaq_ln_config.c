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
#ifndef __GDAQ_LN_CONFIG_C__
#define __GDAQ_LN_CONFIG_C__
#include "../include/gdaq_ln_config.h"


int ln_start(char* conf_file_name)
{
    char cmd[MAX_TMP_STRING_LENGTH];
    if(ln_config_get_cmd_without_para(conf_file_name,cmd)!=0)
    {
        printc(MSG_E, "LN Config: Get command error!\n");
        return -1;
    }
    #ifdef DAQ_DEBUG
    printd("LN Config: cmd=%s\n", cmd);
    #endif
    //system(cmd);


    pid_t pid;
    pid=fork();
    if(pid==0)// child
    {
        //execvp
        execl(cmd,cmd,conf_file_name,NULL);
        printc(MSG_E, "LN Config: start error! %s %s\n" , cmd , conf_file_name);
        return -1;
    }


    return 0;    
}

int ln_config_get_cmd_without_para(char* conf_file_name, char* cmd)
{
    int line=0;
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
        p_file_name++;
    else
        p_file_name = conf_file_name;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
        return -1;
    }
    char szLine[512];
    while(fgets(szLine, sizeof(szLine), fp))
    {
        line++;
        ITEM item;
        item_type = get_item_from_line(szLine, &item);
        if(item_type == ITEM_LABEL)
        {
            if(strcmp(item.key,p_file_name)==0)
            {
                state = CONFIG_SEL;
                continue;
            }
            else
            {
                state = CONFIG_OTHER;
                continue;
            }
        }
        else if(item_type == ITEM_NORMAL)
        {
            if((state==CONFIG_SEL)&&(strcmp(item.key,"CmdName")==0))
            {
                //sprintf(cmd,"./%s.exe %s &", item.value, conf_file_name);
                sprintf(cmd,"./%s.exe", item.value);
                break;
            }
        }
    }
    fclose(fp);
    return 0;
}

int ln_config_get_cmd(char* conf_file_name, char* cmd)
{
	int line=0;
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
        p_file_name++;    
    else
        p_file_name = conf_file_name;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(strcmp(item.key,p_file_name)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"CmdName")==0))
			{
                sprintf(cmd,"./%s.exe %s &", item.value, conf_file_name);
                //sprintf(cmd,"./%s.exe", item.value);
                break;
			}
		}
	}
	fclose(fp);
	return 0;
}
int ln_config_get_stream_num(char* conf_file_name)
{
    int stream_num=0;
	int line=0;
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
			if(strcmp(item.key,"PNInfo")==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"StreamNum")==0))
			{
				stream_num = atoi(item.value);
                break;
			}
		}
	}
	fclose(fp);
	return stream_num;
}

int ln_config_get_ln_num(char* conf_file_name, int stream)
{
    int ln_num=0;
	int line=0;
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            char    tmp[MAX_TMP_STRING_LENGTH];
            sprintf(tmp,"stream%d",stream);
            if(strcmp(item.key,tmp)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"LnNum")==0))
			{
				ln_num = atoi(item.value);
                break;
			}
		}
	}
	fclose(fp);
	return ln_num;
}

int ln_config_generate_file(char * conf_file_name, int stream, int ln, char* out_file_name)
{
	int line=0;
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
    char    file_name[MAX_TMP_STRING_LENGTH];
    sprintf(file_name,"Stream%dLn%d",stream,ln);
    sprintf(out_file_name,"%s%s",DEFAULT_LN_CONFIG_FILE_PATH,file_name);
    FILE* outfp=fopen(out_file_name,"w");
    if (!outfp)
	{
		printc(MSG_E, "LN Config: Open output file:%s failed!(%m)\n", out_file_name, errno);
        fclose(fp);
		return -1;
	}
    
	char szLine[512];
    char tmp_line[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
        strcpy(tmp_line, szLine);
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{

            if(strstr(item.key,file_name)!=NULL)
			{
				state = CONFIG_SEL;
                fputs(tmp_line,outfp);
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if(state==CONFIG_SEL)
			{
				fputs(tmp_line,outfp);
			}
		}
	}
	fclose(fp);
    fclose(outfp);
	return 0;
}


int ln_config_shm_interface_remove_all(char* conf_file_name)
{
    int line=0;
    int ln_id;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
	int interface_type = INTERFACE_NOT_DEFINE;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(interface_type==INTERFACE_SHM)
            {
                printc(MSG_E, "LN Config: Can't find SHM name in file:%s line %d", conf_file_name, line);
                return -1;
            }
            if(strstr(item.key,"Port")!=NULL)
			{
                int tmp1, tmp2;
                sscanf(item.key,"Stream%dLn%dPort%d", &tmp1, &ln_id, &tmp2); 
                state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"type")==0))
			{
				if(strcmp(item.value,"shm")==0)
				{
                    interface_type = INTERFACE_SHM;
				}
                else if(strcmp(item.value,"socket")==0)
                {
                    interface_type = INTERFACE_SOCKET;
                }
                else
                {
                    interface_type = INTERFACE_NOT_DEFINE;
                }
                continue;
			}
            if((state==CONFIG_SEL)&&(strcmp(item.key,"parameter")==0))
			{
                if(interface_type!= INTERFACE_SHM)
                {
                    interface_type = INTERFACE_NOT_DEFINE;
                    state=CONFIG_IDLE;
                    ln_id = -1;
                    continue;
                }
                else
                {
                    int ln1=0, ln2=0;
                    sscanf(item.value,"shm_%d_%d", &ln1, &ln2); 
                    if(ln_id==ln1)
                    {
                        shm_interface* p_interface;
                        if(ln_interface_open(item.value,&p_interface)!=0)
                        {
                            printc(MSG_E, "LN Config: Open interface %s error in file:%s line %d", item.value, conf_file_name, line);
                            return -1;
                        }
                        #ifdef DAQ_DEBUG
                        printd("LN Config: Starting destroy interface %s!\n", item.value);
                        #endif
                        if(ln_interface_destroy(item.value,p_interface)!=0)
                        {
                            printc(MSG_E, "LN Config: Delete interface %s error in file:%s line %d", item.value, conf_file_name, line);
                            return -1;
                        }
                        #ifdef DAQ_DEBUG
                        printd("LN Config: Destroy interface %s success!\n", item.value);
                        #endif
                        interface_type = INTERFACE_NOT_DEFINE;
                        state=CONFIG_IDLE;
                        ln_id = -1;
                        continue;
                    }
                    else
                    {
                        #ifdef DAQ_DEBUG
                            printd("LN Config: The shm %s has been created, omit it!\n", item.value);
                        #endif
                        interface_type = INTERFACE_NOT_DEFINE;
                        state=CONFIG_IDLE;
                        ln_id = -1;
                        continue;
                    }
                }
			}
		}
	}
	fclose(fp);
	return 0;
}


int ln_config_shm_interface_init(char* conf_file_name)
{
    int line=0;
    int ln_id = -1;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
	int interface_type = INTERFACE_NOT_DEFINE;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(interface_type==INTERFACE_SHM)
            {
                printc(MSG_E, "LN Config: Can't find SHM name in file:%s line %d", conf_file_name, line);
                return -1;
            }
            if(strstr(item.key,"Port")!=NULL)
			{
                int tmp1, tmp2;
                sscanf(item.key,"Stream%dLn%dPort%d", &tmp1, &ln_id, &tmp2); 
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"type")==0))
			{
				if(strcmp(item.value,"shm")==0)
				{
                    interface_type = INTERFACE_SHM;
				}
                else if(strcmp(item.value,"socket")==0)
                {
                    interface_type = INTERFACE_SOCKET;
                }
                else
                {
                    interface_type = INTERFACE_NOT_DEFINE;
                }
                continue;
			}
            if((state==CONFIG_SEL)&&(strcmp(item.key,"parameter")==0))
			{
                if(interface_type!= INTERFACE_SHM)
                {
                    interface_type = INTERFACE_NOT_DEFINE;
                    state=CONFIG_IDLE;
                    continue;
                }
                else
                {
                    int ln1=0, ln2=0;
                    sscanf(item.value,"shm_%d_%d", &ln1, &ln2); 
                    if(ln_id==ln1)
                    {
                        shm_interface   *p_interface;
                        #ifdef DAQ_DEBUG
                        printd("LN Config: Starting create interface %s!\n", item.value);
                        #endif
                        if(ln_interface_create(item.value, &p_interface)!=0)
                        {
                            printc(MSG_E, "LN Config: Create interface %s error in file:%s line %d\n", item.value, conf_file_name, line);
                            return -1;
                        }
                        #ifdef DAQ_DEBUG
                        printd("LN Config: Create interface %s success!\n", item.value);
                        #endif
                        interface_type = INTERFACE_NOT_DEFINE;
                        state=CONFIG_IDLE;
                        ln_id = -1;
                        continue;
                    }
                    else
                    {
                        #ifdef DAQ_DEBUG
                            printd("LN Config: The shm %s has been created, omit it!\n", item.value);
                        #endif
                        interface_type = INTERFACE_NOT_DEFINE;
                        state=CONFIG_IDLE;
                        ln_id = -1;
                        continue;
                    }
                }
			}
		}
	}
	fclose(fp);
	return 0;
}



int ln_config_get_ln_port_num(char* conf_file_name)
{
    int line=0;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
    int port_num=-1;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
    {
        p_file_name++;
    }
    else
        p_file_name = conf_file_name;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(strcmp(item.key,p_file_name)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"PortNum")==0))
			{
				port_num = atoi(item.value);
                break;
			}

		}
	}
	fclose(fp);
	return port_num;
}

int ln_config_get_ln_id(char* conf_file_name)
{
    int line=0;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
    int ln_id=-1;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
        p_file_name++;    
    else
        p_file_name = conf_file_name;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(strcmp(item.key,p_file_name)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"id")==0))
			{
				ln_id = atoi(item.value);
                break;
			}

		}
	}
	fclose(fp);
	return ln_id;
}

int ln_config_get_ln_cmd_name(char* conf_file_name, char* cmd_name)
{
    int line=0;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
        p_file_name++;    
    else
        p_file_name = conf_file_name;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(strcmp(item.key,p_file_name)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"CmdName")==0))
			{
				strcpy(cmd_name, item.value);
                break;
			}

		}
	}
	fclose(fp);
	return 0;
}
int ln_config_get_ln_cmd_option(char* conf_file_name, char* option)
{
    int line=0;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
        p_file_name++;    
    else
        p_file_name = conf_file_name;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            if(strcmp(item.key,p_file_name)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if((state==CONFIG_SEL)&&(strcmp(item.key,"option")==0))
			{
				strcpy(option, item.value);
                break;
			}

		}
	}
	fclose(fp);
	return 0;
}

int ln_config_get_ln_port_info(char* conf_file_name, int port, port_info *p_port_info)
{
    int line=0;
	CONFSTATE state=CONFIG_IDLE;
    int item_type;
    char    *p_file_name;
    if((p_file_name = strrchr(conf_file_name, '/'))!=NULL)
        p_file_name++;    
    else
        p_file_name = conf_file_name;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "LN Config: Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
            char tmp_key[MAX_TMP_STRING_LENGTH];
            sprintf(tmp_key, "%sPort%d", p_file_name, port);
            if(strcmp(item.key,tmp_key)==0)
			{
				state = CONFIG_SEL;
				continue;
			}
			else
			{
				state = CONFIG_OTHER;
				continue;
			}
		}
		else if(item_type == ITEM_NORMAL)
		{
			if(state==CONFIG_SEL)
         	{
                if(strcmp(item.key,"direction")==0)
				{
                    if(strcmp(item.value,"input")==0)
                    {
                        p_port_info->direction = IN;
                        continue;
                    }
                    else if(strcmp(item.value,"output")==0)
                    {
                        p_port_info->direction = OUT;
                        continue;
                    }
                    else if(strcmp(item.value,"inout")==0)
                    {
                        p_port_info->direction = INOUT;
                        continue;
                    }
                    else
                    {
                        #ifdef DAQ_DEBUG
                        printd("LN Config: Key not defined in Line %d of file %s!\n", line,conf_file_name);
                        #endif
                        p_port_info->direction = NOT_DEFINED;
                        fclose(fp);
	                    return -1;
                    }
                }
                else if(strcmp(item.key,"type")==0)
                {
                    if(strcmp(item.value,"socket")==0)
                    {
                        p_port_info->type = INTERFACE_SOCKET;
                        continue;
                    }
                    else if(strcmp(item.value,"shm")==0)
                    {
                        p_port_info->type = INTERFACE_SHM;
                        continue;
                    }
                    else
                    {
                        #ifdef DAQ_DEBUG
                        printd("LN Config: Key not defined in Line %d of file %s!\n", line,conf_file_name);
                        #endif
                        p_port_info->type = NOT_DEFINED;
                        fclose(fp);
	                    return -1;
                    }
                }
                else if(strcmp(item.key,"parameter")==0)
                {
                    if((p_port_info->type == INTERFACE_SHM)||(p_port_info->type == INTERFACE_SOCKET))
                    {
                        strcpy(p_port_info->parameter, item.value);
                    }
                    else
                    {
                        fclose(fp);
	                    return -1;
                    }
                }
			}
		}
	}
	fclose(fp);
	return 0;
}

int ln_config_ln_read_file(char* conf_file_name, node_info **p_node_info)
{
    int i;
    int port_num;
    if((port_num = ln_config_get_ln_port_num(conf_file_name))==-1)
    {
        printc(MSG_E, "LN Config: LN port number get error!\n");
        return -1;
    }
    *p_node_info = (node_info*)malloc(sizeof(node_info)+port_num*sizeof(port_info));
    if(*p_node_info == NULL)
    {
        printc(MSG_E, "LN Config: Malloc error(%m)!\n",errno);
        return -1;
    }
    memset(*p_node_info,0,sizeof(node_info)+port_num*sizeof(port_info));
    (*p_node_info)->port_num = port_num;
    if(((*p_node_info)->node_id = ln_config_get_ln_id(conf_file_name))==-1)
    {
        printc(MSG_E, "LN Config: LN id get error!\n");
        return -1;
    }
    if(ln_config_get_ln_cmd_name(conf_file_name, (*p_node_info)->name))
    {
        printc(MSG_E, "LN Config: LN command name get error!\n");
        return -1;
    }
    
    if(ln_config_get_ln_cmd_option(conf_file_name,(*p_node_info)->option))
    {
        printc(MSG_E, "LN Config: LN command option get error!\n");
        return -1;
    }
    
    for(i=0; i<port_num;i++)
    {
        if(ln_config_get_ln_port_info(conf_file_name,i,&(*p_node_info)->port[i])!=0)
        {
            printc(MSG_E, "LN Config: LN port information get error!\n");
            return -1; 
        }
    }
	return 0;
}

int ln_get_type_from_string(char *type)
{

    if(strcmp(type,"socket")==0)
    {
        return INTERFACE_SOCKET;
    }
    else if(strcmp(type,"shm")==0)
    {
        return INTERFACE_SHM;
    }
    else
    {
        return -1;
       // return  NOT_DEFINED;
    }

    return 0;
}

int ln_get_type_from_int(int type_int , char *type)
{
    if(type ==NULL)
    {
        return -1;
    }
    memset(type , 0 , sizeof(type));
    if(type_int == INTERFACE_SOCKET)
    {
        sprintf(type,"%s" , "socket");
    }
    else if(type_int == INTERFACE_SHM)
    {
        sprintf(type,"%s" , "shm");
    }
    else
    {
        return -1;
    }
    return 0;
}

int ln_get_direction_from_string(char *direction)
{
    if(strcmp(direction,"input")==0)
    {
        return IN;
    }
    else if(strcmp(direction,"output")==0)
    {
        return OUT;
    }
    else if(strcmp(direction,"inout")==0)
    {
        return  INOUT;
    }

    return -1;
}

int ln_get_direction_from_int(int direction_int , char *direction)
{
    if(direction ==NULL)
    {
        return -1;
    }
    memset(direction , 0 , sizeof(direction));
    if(direction_int == IN)
    {
        sprintf(direction,"%s" , "input");
    }
    else if(direction_int == OUT)
    {
        sprintf(direction,"%s" , "output");
    }
    else if(direction_int == IN)
    {
        sprintf(direction,"%s" , "inout");
    }
    else
    {
        return -1;
    }
    return 0;
}

#endif
