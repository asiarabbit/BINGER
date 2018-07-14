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
#ifndef __GDAQ_SYSINFO_C__
#define __GDAQ_SYSINFO_C__
#include "gdaq_sysinfo.h"


void print_pn_status(pn_status *pn_status_)
{

#ifdef DAQ_DEBUG
    puts("print_pn_status");
    printd("pn_name %s time=%llu mem %llu %llu  cpu %d %d %d %d  net_speed %u %u\n" ,inet_ntoa(pn_status_->pn_name) , (__u64)pn_status_->time ,  pn_status_->mem_total , pn_status_->mem_used,
           pn_status_->cpu_idle , pn_status_->cpu_user , pn_status_->cpu_sys , pn_status_->cpu_wait ,
           pn_status_->net_speed_in , pn_status_->net_speed_out);
#endif
}


int get_pn_status(pn_status *pn_status_)
{


    time_t now;
    /*
    struct tm *timenow;
    char strtemp[12];*/
    time(&now);
//    timenow = localtime(&now);
//    strftime(strtemp,100,"%H:%M:%S",timenow);
//    strcpy(pn_status_->time  , strtemp);
//    //puts(strtemp);
    pn_status_->time = now ;
    // puts("sleep over0");
    if(get_mem(pn_status_)!=0)
    {

        printc(MSG_E,"SYS_INFO : get mem info error");
        return -1;
    }
    // puts("sleep over1");
    if(get_cpu(pn_status_)!=0)
    {
        printc(MSG_E,"SYS_INFO : get cpu info error");
        return -1;
    }

    if(get_net(pn_status_)!=0)
    {
        printc(MSG_E,"SYS_INFO : get net info error");
        return -1;
    }



    return 0;

}

#define LEN_128 128
#define LEN_4096 4096

int get_mem(pn_status *pn_status_)
{


    FILE             *fp;
    char              line[LEN_128];
  //  char              buf[LEN_4096];
    struct stats_mem  st_mem;

    //memset(buf, 0, LEN_4096);
    memset(&st_mem, 0, sizeof(struct stats_mem));
    if ((fp = fopen("/proc/meminfo", "r")) == NULL) {
        return -1;
    }

    while (fgets(line, 128, fp) != NULL) {

        if (!strncmp(line, "MemTotal:", 9)) {
            /* Read the total amount of memory in kB */
            sscanf(line + 9, "%llu", &st_mem.tlmkb);
        }
        else if (!strncmp(line, "MemFree:", 8)) {
            /* Read the amount of free memory in kB */
            sscanf(line + 8, "%llu", &st_mem.frmkb);
        }
        else if (!strncmp(line, "Buffers:", 8)) {
            /* Read the amount of buffered memory in kB */
            sscanf(line + 8, "%llu", &st_mem.bufkb);
        }
        else if (!strncmp(line, "Cached:", 7)) {
            /* Read the amount of cached memory in kB */
            sscanf(line + 7, "%llu", &st_mem.camkb);
        }
        else if (!strncmp(line, "Active:", 7)) {
            /* Read the amount of Active memory in kB */
            sscanf(line + 7, "%llu", &st_mem.acmkb);
        }
        else if (!strncmp(line, "Inactive:", 9)) {
            /* Read the amount of Inactive memory in kB */
            sscanf(line + 9, "%llu", &st_mem.iamkb);
        }
        else if (!strncmp(line, "Slab:", 5)) {
            /* Read the amount of Slab memory in kB */
            sscanf(line + 5, "%llu", &st_mem.slmkb);
        }
        else if (!strncmp(line, "SwapCached:", 11)) {
            /* Read the amount of cached swap in kB */
            sscanf(line + 11, "%llu", &st_mem.caskb);
        }
        else if (!strncmp(line, "SwapTotal:", 10)) {
            /* Read the total amount of swap memory in kB */
            sscanf(line + 10, "%llu", &st_mem.tlskb);
        }
        else if (!strncmp(line, "SwapFree:", 9)) {
            /* Read the amount of free swap memory in kB */
            sscanf(line + 9, "%llu", &st_mem.frskb);
        }
        else if (!strncmp(line, "Committed_AS:", 13)) {
            /* Read the amount of commited memory in kB */
            sscanf(line + 13, "%llu", &st_mem.comkb);
        }
    }



    pn_status_->mem_free = st_mem.frmkb;
    pn_status_->mem_buffer = st_mem.bufkb;
    pn_status_->mem_cache = st_mem.camkb;
    pn_status_->mem_total = st_mem.tlmkb;
    pn_status_->mem_used =  pn_status_->mem_total - pn_status_->mem_free- pn_status_->mem_cache- pn_status_->mem_buffer;


    fclose(fp);
    return 0;

}

//int get_mem(pn_status *pn_status_)
//{
//    char temp[1024];
//    FILE *fp = popen("free -m" , "r" );

//     while(fgets(temp , sizeof(temp) , fp)!=NULL)
//     {
//        if(strstr(temp , "buffers/cache")!=NULL)
//        {
//            //int used , not_used;
//            sscanf(temp , "%*s %*s %d %d" , &(pn_status_->mem_used),&(pn_status_->mem_not_used));
//            // printf( " used = %dM   not_used = %dM\n" , pn_status_->mem_used , pn_status_->mem_not_used );
//           // puts(temp);
//            pclose(fp);
//            return 0;
//           //
//        }
//     }
//     fclose(fp);
//     return -1;
//}



int get_cpu(pn_status *pn_status_)
{
    char temp[1024];
    int i;
    FILE *fp = popen("iostat -c" , "r" );
    for(i=0 ; i<3 ; i++)
    {
        if(fgets(temp , sizeof(temp) , fp)==NULL)
        {
            puts("get cpu error1");
            pclose(fp);
            return -1;
        }
        //puts(temp);
    }
    if (fgets(temp , sizeof(temp) , fp)==NULL)
    {

        puts("get cpu error2");
        pclose(fp);
        return -1;
    }

   // puts(temp);
    float user , nice , sys , iowait  , idle;
    sscanf(temp , "%f %*f %f %f %*f %f" , &user , &sys , &iowait , &idle);
    pn_status_->cpu_user = (int)user;
    pn_status_->cpu_idle  = (int)idle;
    pn_status_->cpu_wait = (int)iowait;
    pn_status_->cpu_sys = (int)sys;
    //printf( "%f %f %f %f  %f\n" , user, nice , sys , iowait , idle);
    pclose(fp);
    return 0;
}

static int read_traffic_stats( struct stats_traffic  *total_st )
{
    int                   len = 0;
    FILE                 *fp;
    char                 *p = NULL;
    char                  line[LEN_4096] = {0};
   // char                  buf[LEN_4096] = {0};
    struct stats_traffic  cur_st;

    //memset(buf, 0, LEN_4096);
    memset(total_st, 0, sizeof(struct stats_traffic));
    memset(&cur_st, 0, sizeof(struct stats_traffic));

    if ((fp = fopen("/proc/net/dev", "r")) == NULL) {
        return -1;
    }

    memset(total_st, 0, sizeof(cur_st));

    int i=0;
    while (fgets(line, LEN_4096, fp) != NULL) {
        i++;
        if (strncmp(line, "tun",3)!=0 &&strncmp(line, "lo",2)!=0&&i>2) {

            memset(&cur_st, 0, sizeof(cur_st));
            p = strchr(line, ':');

            sscanf(p + 1, "%llu %llu %*u %*u %*u %*u %*u %*u "
                    "%llu %llu %*u %*u %*u %*u %*u %*u",
                    &cur_st.bytein,
                    &cur_st.pktin,
                    &cur_st.byteout,
                    &cur_st.pktout);

            total_st->bytein  += cur_st.bytein;
            total_st->byteout += cur_st.byteout;
            total_st->pktin   += cur_st.pktin;
            total_st->pktout  += cur_st.pktout;
        }
    }


     fclose(fp);
    return 0;
}

int get_net(pn_status *pn_status_)
{
    int i;
    long in1 , in2 , out1 , out2;



     struct stats_traffic  pre_stat , current_stat;

     if(read_traffic_stats(&pre_stat)==-1)
     {
         return -1;
     }


     sleep(1);

     if(read_traffic_stats(&current_stat)==-1)
     {
         return -1;
     }


    pn_status_->net_speed_in = ( current_stat.bytein  - pre_stat.bytein )/1024;
    pn_status_->net_speed_out = ( current_stat.byteout  - pre_stat.byteout )/1024;

  //  puts("sleep over2");
    return 0;

}









//	Get the system resource information
//  Input : none
//	output: the sys_res struct, including memory, disk space and CPU number information
//  return: 0: success; -1: failed

int get_sys_info(sys_res *res)
{
	long page_size;
	long page_num;
	long free_pages;
	struct statfs fs;
		
	res->cpu_num=sysconf(_SC_NPROCESSORS_CONF);
	res->cpu_online=sysconf(_SC_NPROCESSORS_ONLN);
		
	page_size=sysconf(_SC_PAGESIZE);
	page_num=sysconf(_SC_PHYS_PAGES);
	free_pages=sysconf(_SC_AVPHYS_PAGES);
		
	res->total_mem=(long)((long)page_num * (long)page_size)/(long)(1024*1024);
	res->free_mem=(long)((long)free_pages * (long)page_size)/(long)(1024*1024);
	if(statfs("/",&fs)<0)
	{
		printc(MSG_E, "Get disk information failed!(%m)\n", errno);
		return -1;
	}
	res->total_disk_space= (long)((long long)fs.f_blocks*(long long)fs.f_bsize/(long long)(1024*1024));
	res->free_disk_space= (long)((long long)fs.f_bfree*(long long)fs.f_bsize/(long long)(1024*1024)); 
	return 0;
}

//	Get the NIA information appointed by szDevName
//  Input : the name of the NIA
//  Output: the IP, Broad IP, mask and the speed of the NIA stored in the nia_info struct
//	return: 0: success; -1: failed

int get_nia_info(char* szDevName, nia_info *np)
{
	strcpy(np->name, szDevName);
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		printc(MSG_E, "Create socket failed!(%m)\n", errno);
		return -1;
	}
	struct ifreq ifr;
	unsigned long ip, mask, broad_ip;
	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
	{
		ip = 0;
	}
	else
	{
		ip = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
	}
	np->ip = *(struct in_addr*)&ip;

	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFBRDADDR, &ifr) < 0)
	{
		broad_ip = 0;
	}
	else
	{
		broad_ip = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
	}
	np->broad_ip = *(struct in_addr*)&broad_ip;

	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFNETMASK, &ifr) < 0)
	{
		mask = 0;
	}
	else
	{
		mask = *(unsigned long*)&ifr.ifr_netmask.sa_data[2];
	}
	np->mask = *(struct in_addr *)&mask;

	struct ethtool_cmd ep;
	
	strcpy(ifr.ifr_name, szDevName);
	ep.cmd = ETHTOOL_GSET;
	ifr.ifr_data = (caddr_t)&ep;
	if (ioctl(s, SIOCETHTOOL, &ifr) < 0)
	{
		np->speed = 0;
	}
	else
	{
		np->speed = ep.speed;
	}	
	close(s);
	return 0;
}

// Get the NIA (Network Interface Adapt) number
// input: none
// return: the NIA number in this PN

int	get_nia_num()
{
	int count = 0;
	nia_info nia;

	FILE* fp = fopen("/proc/net/dev", "r");
	if (!fp)
	{
		printc(MSG_E, "Open /proc/net/dev failed!(%m)\n", errno);
		return -1;
	}
	char szLine[512];
	char* p=NULL;

	fgets(szLine, sizeof(szLine), fp);
	fgets(szLine, sizeof(szLine), fp);
	while(fgets(szLine, sizeof(szLine), fp))
	{
		char szName[MAX_TMP_STRING_LENGTH] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)continue;
		p = index(szName,':');
		memset(p,0, strlen(p));
		if (strcmp(szName, "lo") == 0)
			continue;
		if(get_nia_info(szName, &nia)!=0)
			continue;
		if((*(unsigned long *)(&nia.ip))!= 0)
		{
			count++;
		}
	}
    fclose(fp);
	return count;
}

// Get the NIA (Network Interface Adapt) information
// input: none
// ouput: the nia information will be saved in the nia_info array
// return: 0:

int	get_nia_all(nia_info p_nia[])
{
	int count = 0;
	nia_info nia;
	
	FILE* fp = fopen("/proc/net/dev", "r");
	if (!fp)
	{
		printc(MSG_E, "Open /proc/net/dev failed!(%m)\n", errno);
		return -1;
	}
	char szLine[512];
	char* p=NULL;
	
	fgets(szLine, sizeof(szLine), fp);
	fgets(szLine, sizeof(szLine), fp);
	while(fgets(szLine, sizeof(szLine), fp))
	{
		char szName[MAX_TMP_STRING_LENGTH] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)continue;
		p = index(szName,':');
		memset(p,0, strlen(p));
		if (strcmp(szName, "lo") == 0)
			continue;
		if(get_nia_info(szName, &nia)!=0)
			continue;
		if((*(unsigned long *)(&nia.ip))!= 0)
		{
			strcpy(p_nia[count].name, nia.name);
			p_nia[count].ip = nia.ip;
			p_nia[count].broad_ip = nia.broad_ip;
			p_nia[count].mask = nia.mask;
			p_nia[count].speed = nia.speed;
			count++;
		}
	}
    fclose(fp);
	return 0;
}

//	print the PN and NIA information
//  input: the pointer of the pn_reg_info struct
//  output: none

void display_pn_info(pn_info* pp)
{
	float mem_free_percent;
	int i,j;
    print("============================   PN	Information   ========================\n");
    print("PN is registed on IP (%s)\n", (pp->reg_ip.s_addr==0)?"Not defined":inet_ntoa(pp->reg_ip));
    print("The endianness of the PN is: %s Endian\n", (pp->pn_endian=='b')?"Big":"Little");
	mem_free_percent=(float)pp->res.free_mem/(float)pp->res.total_mem;
	print("============================   Platform	Information   ========================\n");
	print("                   %d CPU%s installed, %d online\n", pp->res.cpu_num, (pp->res.cpu_num>1)?"s":"", pp->res.cpu_online);
	print("                   %d MB physical memory, %d MB free,	", pp->res.total_mem, pp->res.free_mem);
	if(mem_free_percent<0.1)
		print("\033[01;31m%.1f%%\033[0m Used \033[01;31m\n\t\t\tMust free some procs to get more memories\033[0m\n", (1-mem_free_percent)*100);
	else if(mem_free_percent<0.2)
		print("\033[01;35m%.1f%%\033[0m Used, \033[01;34mfree some procs can get more memories\033[0m\n", (1-mem_free_percent)*100);
	else
		print("\033[01;32m%.1f%%\033[0m Used\n", (1-mem_free_percent)*100);
	print("                   %d MB disk space, %d MB free\n\n", pp->res.total_disk_space, pp->res.free_disk_space);
	print("======================== Network Interface Adapt Information ==================\n");
	print("Physical Node name:\t%s\n", pp->name);
	print("%d Network Interface Adapt%s installed!\n\n",pp->nia_num, (pp->nia_num>1)?"s":"");
	if(pp->nia_num>0)
	{
		print("    |\tName\t\tIP\tBroad IP\tMask\t\tSpeed(Mbps)\n");
		print("-------------------------------------------------------------------------------\n");
	}
	for(i=0;i<pp->nia_num;i++)
	{
		print("%2d  |\t%s", i, pp->p_nia[i].name);
		print("\t%s", inet_ntoa(pp->p_nia[i].ip));
		print("\t%s", inet_ntoa(pp->p_nia[i].broad_ip));
		print("\t%s\t%d\n", inet_ntoa(pp->p_nia[i].mask), pp->p_nia[i].speed);
	}
	print("\n");
}

// Get the number of stream with the teminal in this PN from the configuration file
// input: the configuration file name
// return: the number of stream with the teminal in this PN

int	get_stream_num(char* conf_file_name)
{
	int stream_num=0;
	int line=0;
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
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
			if(strcmp(item.key,"stream")==0)
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

// get the group numbers of the appointed stream in this PN
// input: the configuration file name, the stream number
// ouput: the group number
// return: if >=0: group number
//		   if <0: failed
int	get_group_num(char* conf_file_name, unsigned int stream)
{
	int group_num=0;
	char tmp[MAX_TMP_STRING_LENGTH];	
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	sprintf(tmp,"%s%d","stream",stream);
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
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
			if((state==CONFIG_SEL)&&(strcmp(item.key,"GroupNum")==0))
			{
				group_num = atoi(item.value);
			}
		}
	}
	fclose(fp);
	return group_num;
}
// get the group information of the appointed group in appointed stream in this PN
// input: the configuration file name, the stream number
// ouput: store in the channel_group_info struct pointed by gp
// return: 0: success, -1: failed


int	get_group_info(char* conf_file_name, unsigned int stream, unsigned int group, channel_group_info *gp)
{
	char tmp[MAX_TMP_STRING_LENGTH];	
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	int line=0;

	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	char szLine[512];

	while(fgets(szLine, sizeof(szLine), fp))
	{
		ITEM item;
		line++;
		item_type = get_item_from_line(szLine, &item);

		if(item_type == ITEM_LABEL)
		{
			sprintf(tmp,"%s%d","stream",stream);
			if(strcmp(item.key,tmp)==0) 
			{
				state = CONFIG_STREAM;
				continue;
			}
			if(state == CONFIG_STREAM)
			{
				sprintf(tmp,"%s%d","group",group);
				if(strcmp(item.key,tmp)==0)
				{
					state = CONFIG_SEL;
					continue;
				}
			}
			else if(state == CONFIG_SEL)
			{
				state = CONFIG_OTHER;
			}
		}
		else if((item_type == ITEM_NORMAL)&&(state==CONFIG_SEL))
		{
			if(strcmp(item.key,"name")==0)
			{
				strcpy(gp->name,item.value);
				continue;
			}
			else if(strcmp(item.key,"type")==0)
			{
				strcpy(gp->type,item.value);
				continue;
			}
			else if(strcmp(item.key,"direction")==0)
			{
				if(strcmp(item.value,"in")==0)
					gp->direction = IN;
				else if(strcmp(item.value,"out")==0)
					gp->direction = OUT;
				else if(strcmp(item.value,"inout")==0)
					gp->direction = INOUT;
				else
				{
					printc(MSG_E, "Group type is not supported in file( %s ) line %d!\n", conf_file_name,line);
					fclose(fp);
					return -1;
				}
				continue;
			}
			else if(strcmp(item.key,"interface")==0)
			{
				strcpy(gp->interface_name,item.value);
				continue;
			}
			else if(strcmp(item.key,"ChannelNum")==0)
			{
				gp->channel_num=atoi(item.value);
				continue;
			}
			else if(strcmp(item.key,"FrameLen")==0)
			{
				gp->frame_len=atoi(item.value);
				continue;
			}
			else
			{
				printc(MSG_E, "The key is not supported in File( %s ) line %d!\n", conf_file_name,line);
				fclose(fp);
				return -1;
			}
		}
	}
	fclose(fp);
	return 0;
}

// get the stream information of the appointed stream in this PN
// input: the configuration file name, the stream number
// ouput: the group information array gp[]
// return: 0: success, -1: failed

int	get_stream_terminal_info(char* conf_file_name, unsigned int stream, stream_terminal_info *sp)
{
	char tmp[MAX_TMP_STRING_LENGTH];	
	CONFSTATE state=CONFIG_IDLE;
	int item_type=0;
	int line=0;
	FILE* fp = fopen(conf_file_name, "r");
	if (!fp)
	{
		printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
		return -1;
	}
	sprintf(tmp,"%s%d","stream",stream);
	char szLine[512];
	while(fgets(szLine, sizeof(szLine), fp))
	{
		line++;
		ITEM item;
		item_type = get_item_from_line(szLine, &item);
		if(item_type == ITEM_LABEL)
		{
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
		else if((state==CONFIG_SEL)&&(item_type == ITEM_NORMAL))
		{
			if(strcmp(item.key,"name")==0)
			{
				strcpy(sp->name,item.value);
				continue;
			}
			else if(strcmp(item.key,"type")==0)
			{
				if(strcmp(item.value, "gather")==0)
					sp->type = GATHER;
				else if(strcmp(item.value, "broadcast")==0)
					sp->type = BROADCAST;
				else
				{
					printc(MSG_E, "Stream type is not supported in file( %s ) line %d!\n", conf_file_name,line);
					fclose(fp);
					return -1;
				}
				continue;
			}
			else if(strcmp(item.key,"GroupNum")==0)
			{
				sp->group_num = atoi(item.value);
			}
			else
			{
				printc(MSG_E, "The key is not supported in File( %s ) line %d!\n", conf_file_name,line);
				fclose(fp);
				return -1;
			}
		}
	}
	fclose(fp);
	int group;
	for(group=0;group<sp->group_num;group++)
	{
		if(get_group_info(conf_file_name,stream,group,&(sp->group_info[group]))!=0)
			return -1;
	}
	return 0;
}
//	print the stream terminal information
//  input: the pointer of the stream_reg_info struct
//  output: none


void display_stream_info(pn_stream_info* srp)
{
	int i,j;
	print("======================== Physical Node Terminal Information ===================\n");
	print("%d stream%s found in %s!\n",srp->stream_num, (srp->stream_num>1)?"s":"", inet_ntoa(srp->pn_name));
		for(i=0;i<srp->stream_num;i++)
		{
			stream_terminal_info *sp=srp->p_stream[i];
			print("Stream %d:  %s(%s), %d group%s\n",i,sp->name, 
				(sp->type==GATHER)?"gather":((sp->type==BROADCAST)?"broadcast":"Not defined"),
				sp->group_num, (sp->group_num>0)?"s":"");
			for(j=0;j<sp->group_num;j++)
			{
				print("    Group %d: \t%s(%s), %s",
					j,sp->group_info[j].name,sp->group_info[j].type,
					(sp->group_info[j].direction==IN)?"in":((sp->group_info[j].direction==OUT)?"out":((sp->group_info[j].direction==INOUT)?"inout":"N/A")));
				if(sp->group_info[j].direction != OUT)
				{
					print(", %d channels, %d Byte/Frame",sp->group_info[j].channel_num,sp->group_info[j].frame_len);
				}
				print("\n    \tinterface name: %s\n", sp->group_info[j].interface_name);
			} 
		}
		return;
}

pn_info* get_pn_info()
{
    union {char c[4]; unsigned long mylong;} endian_test={{'l','?','?','b'}};
	pn_info *pn_p;
	int	nia_num=0;

	nia_num = get_nia_num();
	pn_p = (pn_info	*)malloc(sizeof(pn_info)+nia_num*sizeof(nia_info));
	if(!pn_p)
	{
		printc(MSG_E, "Malloc error!(%m)\n", errno);
		return NULL;
	}
	pn_p->nia_num = nia_num;
  	pn_p->pn_endian = (__s8)endian_test.mylong;
	if(gethostname(pn_p->name, sizeof(pn_p->name))!=0)
	{
		printc(MSG_E, "Get host name failed!(%m)\n", errno);
		free(pn_p);
		return NULL;
	}
	if(get_sys_info(&pn_p->res)!=0)
	{
		printc(MSG_E, "Get system information failed!(%m)\n", errno);
		free(pn_p);
		return NULL;
	}	
	if(get_nia_all(pn_p->p_nia)!=0)
	{
		printc(MSG_E, "Get NIA information failed!(%m)\n", errno);
		return NULL;
	}
	return pn_p;
}
pn_stream_info* get_pn_stream_info(char* conf_file, struct in_addr pn_name)
{
	
	// Get the information of the stream with terminal in this PN
    union {char c[4]; unsigned long mylong;} endian_test={{'l','?','?','b'}};
    int stream_num;
	pn_stream_info* srp;
	stream_num = get_stream_num(conf_file);
	srp = (pn_stream_info *)malloc(sizeof(pn_stream_info)+stream_num*sizeof(stream_terminal_info*));
	if(!srp)
	{
		printc(MSG_E, "Malloc error!(%m)\n", errno);
		free_stream_pointer(srp);
		return NULL;
	}
    srp->pn_name.s_addr = pn_name.s_addr;
    srp->endian = (__s8)endian_test.mylong;
	srp->stream_num=stream_num;
	int stream=0,group_num=0;
	for(stream=0;stream<stream_num;stream++)
	{
		srp->p_stream[stream]=NULL;
	}
	for(stream=0;stream<stream_num;stream++)
	{
		group_num = get_group_num(conf_file,stream);
		srp->p_stream[stream]=(stream_terminal_info*)malloc(sizeof(stream_terminal_info)+group_num*sizeof(channel_group_info)); 	
		if(!srp->p_stream[stream])
		{
			printc(MSG_E, "Malloc error!(%m)\n", errno);
			free_stream_pointer(srp);
			return NULL;
		}
		if(get_stream_terminal_info(conf_file,stream,srp->p_stream[stream])!=0)
		{
			printc(MSG_E, "Get stream terminal information error!\n");
			free_stream_pointer(srp);
			return NULL;
		}
	}
    return srp;
}
void	free_stream_pointer(pn_stream_info* srp)
{
	int i;
	if(!srp)
	{
		for(i=0;i<=srp->stream_num;i++)
		{
			free(srp->p_stream[i]);
		}
		free(srp);
	}
} 
//----------------------------------------------------------------------------------------------------




int get_card_channel_num(char *conf_file_name)
{
    int num=0;
    int line=0;
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
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
            if(strcmp(item.key,"channel_length")==0)
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
            if((state==CONFIG_SEL)&&(strcmp(item.key,"num")==0))
            {
                num = atoi(item.value);
                break;
            }
        }
    }
    fclose(fp);
    return num;
}

void display_channel_length(channel_length *sp)
{

    int index;
    print("======================== Card Channel Length ===================\n");
    print("type = %d , card channel length found!\n",sp->type );
    print("\tstart\tend\tlength\t\n");

    for(index=0;index<sp->num;index++)
    {
        print("\t%d\t%d\t\t%d\t\n" ,sp->length[index][0],sp->length[index][1],sp->length[index][2]);
    }
    return;
}
int get_card_channel_length(char *conf_file_name ,channel_length *channel_length_)
{
    char tmp[MAX_TMP_STRING_LENGTH];
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    int line=0;
    int index =0;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
        return -1;
    }
    sprintf(tmp,"%s%d","length",index);
    char szLine[512];
    while(fgets(szLine, sizeof(szLine), fp))
    {
        line++;
        ITEM item;
        item_type = get_item_from_line(szLine, &item);
        if(item_type == ITEM_LABEL)
        {


            if(strcmp(item.key,tmp)==0)
            {
                state = CONFIG_SEL;
                index ++;
                memset(tmp , 0 ,sizeof(tmp));
                sprintf(tmp,"%s%d","length",index);
                continue;
            }

            else if(strcmp(item.key , "channel_length")==0)
            {
                state = CONFIG_SEL;
               // puts(item.key);

                continue;
            }
            else
            {
               // puts(item.key);

                state = CONFIG_OTHER;
                continue;
            }
        }
        else if((state==CONFIG_SEL)&&(item_type == ITEM_NORMAL))
        {
            if(strcmp(item.key,"type")==0)
            {
                channel_length_->type = atoi(item.value);
                //puts("name-----");
                 //strcpy(card_template_list_info->card_template_[index].name,item.value);
            //    puts("value");
             //   puts(item.value);
                continue;
            }
            else if(strcmp(item.key,"start")==0)
            {
                channel_length_->length[index-1][0] = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"end")==0)
            {
                channel_length_->length[index-1][1] = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"length")==0)
            {
                channel_length_->length[index-1][2] = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"num")==0)
            {
                channel_length_->num = atoi(item.value);
                continue;
            }

            else
            {
                printc(MSG_E, "The key is not supported in File( %s ) line %d!\n", conf_file_name,line);
                fclose(fp);
                return -1;
            }
           // break;
        }
    }
    fclose(fp);
    return 0;
}




void display_pn_card_info(pn_card_info *sp)
{
    int index;
    print("======================== Pn Card Information ===================\n");
    print("%d card template found in %s!\n",sp->card_num , inet_ntoa(sp->pn_name));
    print("\tindex\tid\ttype\t\n");

    for(index=0;index<sp->card_num;index++)
    {
        print("\t%d\t%d\t%d\t%s\t%s\t\n" ,index , sp->card_info_[index].id, sp->card_info_[index].channel_num,sp->card_info_[index].type,sp->card_info_[index].module_name );
    }
    return;
}

void display_card_template_list(card_template_list *sp)
{

    int index;
    print("======================== Card Template Information ===================\n");
    print("%d card template found!\n",sp->card_num);
    print("\tindex\tname\ttype\tframe_length\tchannel_num\tchannel_change\tchannel_full\t\n");

    for(index=0;index<sp->card_num;index++)
    {
        print("\t%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\n" ,index , sp->card_template_[index].name,sp->card_template_[index].type,sp->card_template_[index].frame_length,sp->card_template_[index].channel_num,sp->card_template_[index].channel_change , sp->card_template_[index].channel_full );
       // print("\t%d\t%s\n" ,index , sp->card_template_[index].name);
    }
    return;
}

int get_card_template_num(char *conf_file_name)
{
    int card_template_num=0;
    int line=0;
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
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
            if(strcmp(item.key,"card")==0)
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

            if((state==CONFIG_SEL)&&(strcmp(item.key,"num")==0))
            {
                card_template_num = atoi(item.value);
                break;
            }
        }
    }
    fclose(fp);
    return card_template_num;
}






int get_card_function_num(char *conf_file_name)
{
    int card_function_num=0;
    int line=0;
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
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
            if(strcmp(item.key,"card_function")==0)
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

            if((state==CONFIG_SEL)&&(strcmp(item.key,"num")==0))
            {
                card_function_num = atoi(item.value);
                break;
            }
        }
    }
    fclose(fp);
    return card_function_num;
}


int get_card_function_list(char *conf_file_name ,card_function_list* card_function_list_info)
{
    char tmp[MAX_TMP_STRING_LENGTH];
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    int line=0;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
        return -1;
    }
    char szLine[512];
    int index = 0;
    sprintf(tmp,"%s%d","func",index);
    while(fgets(szLine, sizeof(szLine), fp))
    {
        line++;
        ITEM item;
        item_type = get_item_from_line(szLine, &item);
        if(item_type == ITEM_LABEL)
        {
            if(strcmp(item.key,"card_function")==0)
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
        else if((state==CONFIG_SEL)&&(item_type == ITEM_NORMAL))
        {
            if(strcmp(item.key,"num")==0)
            {
                card_function_list_info->num = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"type")==0)
            {
                card_function_list_info->type = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,tmp)==0)
            {
                //if(index>)
                if(card_function_list_info->num < (index+1))
                {
                    return -1; //check num
                }

                strcpy(card_function_list_info->function[index] , item.value);
                index++;
                memset(tmp , 0 , sizeof(tmp));
                sprintf(tmp,"%s%d","func",index);
                continue;
            }
            else
            {
                printc(MSG_E, "The key is not supported in File( %s ) line %d!\n", conf_file_name,line);
                fclose(fp);
                return -1;
            }
           // break;
        }
    }
    if(card_function_list_info->num != index)
    {
        return -1;
    }
    fclose(fp);
    return 0;
}



int get_card_template(char *conf_file_name ,card_template_list* card_template_list_info, unsigned int index )
{
    char tmp[MAX_TMP_STRING_LENGTH];
    CONFSTATE state=CONFIG_IDLE;
    int item_type=0;
    int line=0;
    FILE* fp = fopen(conf_file_name, "r");
    if (!fp)
    {
        printc(MSG_E, "Open configuration file:%s failed!(%m)\n", conf_file_name, errno);
        return -1;
    }
    sprintf(tmp,"%s%d","card",index);
    char szLine[512];
    while(fgets(szLine, sizeof(szLine), fp))
    {
        line++;
        ITEM item;
        item_type = get_item_from_line(szLine, &item);
        if(item_type == ITEM_LABEL)
        {
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
        else if((state==CONFIG_SEL)&&(item_type == ITEM_NORMAL))
        {
            if(strcmp(item.key,"device_name")==0)
            {
                //puts("name-----");
                strcpy(card_template_list_info->card_template_[index].name,item.value);
                //puts("value");
                //puts(item.value);
                continue;
            }
            else if(strcmp(item.key,"type")==0)
            {
                card_template_list_info->card_template_[index].type = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"channel_num")==0)
            {
                card_template_list_info->card_template_[index].channel_num = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"channel_change")==0)
            {
                card_template_list_info->card_template_[index].channel_change = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"channel_full")==0)
            {
                card_template_list_info->card_template_[index].channel_full = atoi(item.value);
                continue;
            }
            else if(strcmp(item.key,"frame_length")==0)
            {
                card_template_list_info->card_template_[index].frame_length = atoi(item.value);
                continue;
            }
            else
            {
                printc(MSG_E, "The key is not supported in File( %s ) line %d!\n", conf_file_name,line);
                fclose(fp);
                return -1;
            }
           // break;
        }
    }
    fclose(fp);
    return 0;
}


card_template_list* get_card_template_list(char *conf_file)
{
    int card_template_num=0;
    card_template_list *card_template_list_info;
    card_template_num = get_card_template_num(conf_file);
    if(card_template_num<0)
    {
        return NULL;
    }

  //  printf("fdsfsd-----%d\n" ,card_template_num);
    card_template_list_info = (card_template_list*)malloc(sizeof(card_template_list) + card_template_num * sizeof (card_template));

    if(!card_template_list_info)
    {
        printc(MSG_E , "Malloc error!(%m)\n" , errno);

        free(card_template_list_info);
        return NULL;
    }

    card_template_list_info->card_num = card_template_num;

    int index = 0;
    for(index =0 ; index < card_template_num ; index ++)
    {

        if (get_card_template(conf_file , card_template_list_info , index)!=0)
        {
            printc(MSG_E, "Get card template information error!\n");
            free(card_template_list_info);
            return NULL;
        }
    }

    return card_template_list_info;
}




#endif
