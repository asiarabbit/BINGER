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
#ifndef __BROADCAST_SERV_C__
#define __BROADCAST_SERV_C__
#include "broadcast_serv.h"

static struct in_addr superior_pn_addr;			// the IP information of the superior PN, net format
static gdaq_shm_with_mutex	global_info;					// the global system information in shared memory

int main(int argc, char *argv[])
{
    init_log_client();
    #ifdef DAQ_DEBUG
	printc(MSG_N, "Broadcast Server: This is Broadcast Server!\n");
    #endif
	int ret=-1;
    int sock_udp;
	struct sockaddr_in server_addr;
	struct sockaddr_in from_addr;
	int from_len=sizeof(struct sockaddr_in);
	int count=-1;
    sprintf(global_info.name, "%s",  GLOBAL_SHARED_INFO);
	global_info.shm_size = sizeof(global_shared_info);
    if(gdaq_shm_with_mutex_open(&global_info)==-1)
    {
        printc(MSG_E,"Broadcast Server: shm open error!\n");
		return -1;
    }
    global_shared_info* p_global_shm;
	p_global_shm = (global_shared_info*)(global_info.ptr);
    
	fd_set readfd;
	char buffer[MAX_FRAME_LEN];
	sock_udp=socket(AF_INET,SOCK_DGRAM,0);
	if(sock_udp<0)
	{
		printc(MSG_E, "Broadcast Server: create sock error!(%m)\n", errno);
		gdaq_shm_with_mutex_close(&global_info);
		return -1;
	}

#ifdef DAQ_DEBUG
    printd("Broadcast server: Socket create OK!\n");
#endif

	memset((void*)&server_addr,0,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(GDAQ_SYS_PN_REG_PORT);


    const int opt=1;
    int nb=0;

#ifdef DAQ_DEBUG
    printc(MSG_N,"Broadcast server: Socket set opt OK!\n");
#endif
//    //nb = setsockopt(sock_udp,SOL_SOCKET,SO_BROADCAST,(char *)&opt,sizeof(opt));
//    if(nb==-1)
//    {
//        printc(MSG_E,"set socketopt error %m\n");
//        return -1;
//    }



	ret=bind(sock_udp,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret<0)
	{
		printc(MSG_E, "Broadcast Server: bind error!(%m)\n", errno);
		gdaq_shm_with_mutex_close(&global_info);
		return -1;
	}
 #ifdef DAQ_DEBUG
    printd("Broadcast server: Socket bind OK!\n");
#endif
    gdaq_shm_with_mutex_lock(&global_info);
    p_global_shm->server_status = p_global_shm->server_status | BROADCAST_SERVER_IS_RUNNING;
    gdaq_shm_with_mutex_unlock(&global_info);
#ifdef DAQ_DEBUG
   printd("Broadcast server: start wait!\n");
#endif
	while(1)
	{
        FD_ZERO(&readfd);
        FD_SET(sock_udp,&readfd);
		ret=select(sock_udp+1,&readfd,NULL,NULL,NULL);
		switch(ret)
		{
			case -1://error
			{
				printc(MSG_E, "Broadcast Server: select error!\n");
				break;
			}
			default:
				if(FD_ISSET(sock_udp,&readfd))
				{
                  //  printc(MSG_E, "Broadcast Server: select error1!\n");
                    gdaq_frame_head fh_rcv;
					count=recvfrom(sock_udp,buffer,MAX_FRAME_LEN,0,(struct sockaddr*)&from_addr,&from_len);
                    if(count<sizeof(gdaq_frame_head))
                    {
                        printc(MSG_E, "Broadcast Server: receive error!\n");
                        continue;                      
                    }
                    
                    memcpy(&fh_rcv, buffer, sizeof(gdaq_frame_head));                    
					if(fh_rcv.frame_type == FRAME_PN_REG)
					{
                        switch(fh_rcv.frame_id)
                        {
                            case PN_REG_BROADCAST:
                            {
                                #ifdef DAQ_DEBUG
                                printd("Broadcast server: gotten a broadcast message!\n");
                                #endif

                                int ena;
                                gdaq_shm_with_mutex_lock(&global_info);    
                                ena = p_global_shm->broadcast_serv_ena;
                                gdaq_shm_with_mutex_unlock(&global_info);

                                if(ena == 1)
                                {
                                    #ifdef DAQ_DEBUG
                                    printd("Broadcast Server: Client is %s:%d\n",inet_ntoa(from_addr.sin_addr),ntohs(from_addr.sin_port));
                                    #endif
                                    gdaq_frame_head fh_send;
                                    fh_send.frame_type = FRAME_PN_REG;
                                    fh_send.frame_id = PN_REG_STOP_BROADCAST;
                                    fh_send.frame_length = sizeof(struct in_addr);
                                    if(gdaq_send_frame(from_addr.sin_addr, GDAQ_SYS_PN_REG_PORT, \
                                       &fh_send ,(void *)(&from_addr.sin_addr))==-1)
                                    {
                                        printc(MSG_E, "Broadcast Server: frame send error!");
                                        continue;                           
                                    } 
                                    #ifdef DAQ_DEBUG
                                    printd("Broadcast server: notice %s to stop broadcast!\n",inet_ntoa(from_addr.sin_addr));
                                    #endif
                                }
                                else
                                {
                                    #ifdef DAQ_DEBUG
                                    printd("Broadcast Server: broadcast message is neglected!\n",inet_ntoa(from_addr.sin_addr),ntohs(from_addr.sin_port));
                                    #endif
                                }
                            } break;
                            case PN_REG_LINK_TEST:
                            {
                                #ifdef DAQ_DEBUG
                                printd("Broadcast Server: Client is %s:%d\n",inet_ntoa(from_addr.sin_addr),ntohs(from_addr.sin_port));
                                #endif
                                //get the superior PN IP address
                                global_shared_info* p_global_shm;
                                p_global_shm = (global_shared_info*)(global_info.ptr);
                                gdaq_shm_with_mutex_lock(&global_info);    
                                superior_pn_addr.s_addr = p_global_shm->ip_table.superior_pn.s_addr;
                                gdaq_shm_with_mutex_unlock(&global_info);
                                //send LINK_TEST frame
                                nia_link new_link;
                                new_link.nia1.s_addr = from_addr.sin_addr.s_addr;
                                memcpy(&new_link.nia2.s_addr, buffer+sizeof(gdaq_frame_head), sizeof(struct in_addr));
                                gdaq_frame_head fh_send;
                                fh_send.frame_type = FRAME_SYS_DATA;
                                fh_send.frame_id = SYS_DATA_LINK_TEST;
                                fh_send.frame_length = sizeof(nia_link);
                                if(gdaq_send_frame(superior_pn_addr, GDAQ_SYS_DATA_PORT, \
                                       &fh_send ,(void *)(&new_link))==-1)
                                {
                                    printc(MSG_E, "Broadcast Server: frame send error!");
                                    continue;                           
                                }                                 
                                
                            } break;
                            default:
                            {
                                printc(MSG_E, "Broadcast Server: package type error!");
						        continue;
                            }
                        }
					}
					else
					{
						printc(MSG_E, "Broadcast Server: package type error!");
						continue;
					}
				}
				break;
		}
	}
    #ifdef DAQ_DEBUG
    printd("Broadcast Server: Broadcast server exit!\n");
    #endif
    gdaq_shm_with_mutex_lock(&global_info);	
    p_global_shm->server_status = p_global_shm->server_status ^ BROADCAST_SERVER_IS_RUNNING;
    gdaq_shm_with_mutex_unlock(&global_info);
	gdaq_shm_with_mutex_close(&global_info);
	return 0;
}

#endif

