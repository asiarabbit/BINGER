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
#ifndef __GDAQ_SOCK_C__
#define __GDAQ_SOCK_C__
#include "gdaq_sock.h"
int readn(int fd, void *vptr, size_t n)
{
    size_t  nleft=n;
    int     nread;
    char*   ptr= vptr;
    while(nleft>0)
    {
        if((nread=read(fd,ptr,nleft))<0)
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if(nread==0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return(n-nleft);
}

int writen(int fd, const void *vptr, size_t n)
{
    size_t  nleft = n;
    int     nwritten;
    const char* ptr = vptr;
    while(nleft>0)
    {
        if((nwritten = write(fd, ptr, nleft))<=0)
        {
            if(nwritten <0 && (errno == EINTR))
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

int gdaq_send_frame(struct in_addr server_ip, __u16 port, gdaq_frame_head* head, void * frame_content)
{
    int sock_tcp;
    int ret;
    struct sockaddr_in server_addr;
    gdaq_frame_head fh_send;

    sock_tcp=socket(AF_INET,SOCK_STREAM,0);
	if(sock_tcp<0)
	{
		printc(MSG_E, "Send frame: create sock error!(%m)\n", errno);
		return -1;
	}
	int result;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = server_ip.s_addr;
	server_addr.sin_port = htons(port);
	result = connect(sock_tcp,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
	if(result==-1)
	{
        printc(MSG_E, "Send frame: connect error!(%m) ip:%s port:%d \n", inet_ntoa(server_ip) , port);
		close(sock_tcp);
		return -1;
	}
	fh_send.frame_type = head->frame_type;
	fh_send.frame_id = head->frame_id;
	fh_send.frame_length = htonl(head->frame_length);
	ret =writen(sock_tcp,(void*)&fh_send,sizeof(fh_send));	
	if(ret == -1)
	{
		printc(MSG_E, "Send frame: send frame head error!(%m)\n", errno);
        close(sock_tcp);
		return -1;
	}
    if((fh_send.frame_length!=0)&&(frame_content!=NULL))
    {
	    ret=writen(sock_tcp,frame_content,head->frame_length);	
	    if(ret == -1)
	    {
		    printc(MSG_E, "Send frame: send frame content error!\n");
            close(sock_tcp);
		    return -1;
	    }
    }
	close(sock_tcp);
    return 0;
}

int gdaq_rcv_frame(int client_sockfd, gdaq_frame_head *p_fh_rcv, char** buffer)
{
    int ret;
    ret = readn(client_sockfd, (char*)p_fh_rcv, sizeof(gdaq_frame_head));
	if(ret!=sizeof(gdaq_frame_head))
	{
		printc(MSG_E,"Receive Frame: frame format error!");
        return -1;
	}

    int frame_length = ntohl(p_fh_rcv->frame_length);
    p_fh_rcv->frame_length = frame_length;
    if(frame_length>0)
    {   
        *buffer=(char*)malloc(frame_length);

        if(!(*buffer))
        {
            printc(MSG_E, "Receive Frame: Malloc error!(%m)\n", errno);
            return -1;
        }
        memset(*buffer,0,frame_length);
        ret = readn(client_sockfd, (char*)(*buffer), frame_length);
        if(ret != frame_length)
        {
            printc(MSG_E,"Receive Frame: receive error!(%d/%d)\n", ret, frame_length);
            return -1;
        }
    }
    return 0;
}

int gdaq_send_cmd_file(struct in_addr server_ip,struct in_addr target_ip, char *file_path ,char *target_file_path)
{


    int fd= open(file_path, O_RDONLY);
    if (fd==-1)
    {
        printc(MSG_E, " File %s open error!(%m)\n", file_path );
        return -1;
    }




    struct stat buf;
    if(stat(file_path,&buf)!=0)
    {
        printc(MSG_E, "CMC LN_CONF: File %s get file error!(%m)\n", file_path );
        return -1;
    }
    int file_size = buf.st_size;
    #ifdef DAQ_DEBUG
    printc(MSG_N ,"The size of file %s is: %d\n", file_path, file_size);
    #endif

    char* buffer = (char* )malloc(file_size+CMD_FILE_PATH_LENGTH);
    char *tmp_buffer = buffer;
    if(buffer==NULL)
    {
        printc(MSG_E, "CMC LN_CONF: File %s malloc error!(%m)\n", file_path );
        close(fd);
        return -1;
    }
    sprintf(buffer , "%s" , target_file_path);
    //puts(buffer);
    buffer+=CMD_FILE_PATH_LENGTH;
    if(readn(fd, buffer, file_size)!= file_size)
    {
        printc(MSG_E, "CMC LN_CONF: File %s read error!(%m)\n", file_path );
        close(fd);
        free(tmp_buffer);
        return -1;
    }

    gdaq_frame_head head;
    head.frame_type = FRAME_SYS_CMD;
    head.frame_id = SYS_CMD_SEND_FILE;
    head.frame_length = sizeof(struct in_addr)+ file_size +CMD_FILE_PATH_LENGTH;

   // puts("fsad");
    if(gdaq_send_cmd(server_ip, GDAQ_SYS_CMD_PORT,&head,target_ip,tmp_buffer)!= 0)
    {
        printc(MSG_E, "CMC LN_CONF: File %s send error!(%m)\n", file_path );
        close(fd);
        free(tmp_buffer);
        return -1;
    }
   // puts("fsad");
    close(fd);
    free(tmp_buffer);
}

int gdaq_send_cmd(struct in_addr server_ip, __u16 port, gdaq_frame_head* head, struct in_addr target, void * cmd_content)
{
    int sock_tcp;
    int ret;
    struct sockaddr_in server_addr;
    gdaq_frame_head fh_send;
    struct in_addr target_addr;
    target_addr.s_addr = target.s_addr;
    int frame_length = head->frame_length;

    if(frame_length < sizeof(struct in_addr))
    {
        printc(MSG_E, "Send cmd: frame length invalid!\n");
        return -1;
    }
    
    sock_tcp=socket(AF_INET,SOCK_STREAM,0);
	if(sock_tcp<0)
	{
		printc(MSG_E, "Send cmd: create sock error!(%m)\n", errno);
		return -1;
	}
	int result;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = server_ip.s_addr;
	server_addr.sin_port = htons(port);
    result = connect(sock_tcp,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
	if(result==-1)
	{
		printc(MSG_E, "Send cmd: connect error!(%m)\n", errno);
		close(sock_tcp);
		return -1;
	}
	fh_send.frame_type = head->frame_type;
	fh_send.frame_id = head->frame_id;
	fh_send.frame_length = htonl(frame_length);
    #ifdef DAQ_DEBUG
    printd("Send cmd: TYPE:ID:LENGTH = %d:%d:%d:%d:%d\n", fh_send.frame_type, fh_send.frame_id, head->frame_length, sizeof(gdaq_frame_head) ,sizeof(struct in_addr));
    #endif
    
	ret=writen(sock_tcp,(void*)&fh_send,sizeof(fh_send));	
	if(ret== -1)
	{
		printc(MSG_E, "Send cmd: send command head error!(%m)\n", errno);
        close(sock_tcp);
		return -1;
	}
    ret=writen(sock_tcp,(void*)&target_addr,sizeof(struct in_addr));	
	if(ret== -1)
	{
		printc(MSG_E, "Send cmd: send command target error!(%m)\n", errno);
        close(sock_tcp);
		return -1;
	}
    int cmd_length = frame_length-sizeof(struct in_addr);
    if((cmd_length>0)&&(cmd_content!=NULL))
    {
        
        ret=writen(sock_tcp,cmd_content,cmd_length);	
	    if(ret==-1)
	    {
		    printc(MSG_E, "Send cmd: send command content error!\n");
            close(sock_tcp);
		    return -1;
	    }
    }
	close(sock_tcp);
    return 0;
}

int gdaq_send_stream_info(struct in_addr server_ip, __u16 port, pn_stream_info* p_stream_info)
{
    int sock_tcp;
    int ret;
    struct sockaddr_in server_addr;
    gdaq_frame_head fh_send;
    
    sock_tcp=socket(AF_INET,SOCK_STREAM,0);
	if(sock_tcp<0)
	{
		printc(MSG_E, "Send stream information: create sock error!(%m)\n", errno);
		return -1;
	}
	int result;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = server_ip.s_addr;
	server_addr.sin_port = htons(port);
	result = connect(sock_tcp,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
	if(result==-1)
	{
		printc(MSG_E, "Send stream information: connect error!(%m)\n", errno);
		close(sock_tcp);
		return -1;
	}
    
	fh_send.frame_type = FRAME_SYS_DATA;
	fh_send.frame_id = SYS_DATA_STREAM_INFO;
    int i;
    int frame_length = sizeof(pn_stream_info);

    for(i=0;i<p_stream_info->stream_num;i++)
    {
        frame_length += sizeof(stream_terminal_info)+ p_stream_info->p_stream[i]->group_num*sizeof(channel_group_info);
    }

    fh_send.frame_length = htonl(frame_length);
	ret=writen(sock_tcp,(void*)&fh_send,sizeof(fh_send));	
	if(ret == -1)
	{
		printc(MSG_E, "Send stream information: send frame head error!(%m)\n", errno);
        close(sock_tcp);
		return -1;
	}
        
    ret=writen(sock_tcp,(void*)p_stream_info,sizeof(pn_stream_info)); 
    if(ret==-1)
    {
        printc(MSG_E, "Send stream information: send stream head error!(%m)\n", errno);
        close(sock_tcp);
        return -1;
    }

    for(i=0;i<p_stream_info->stream_num;i++)
    {
        int length;
        length = sizeof(stream_terminal_info)+ p_stream_info->p_stream[i]->group_num*sizeof(channel_group_info);
        ret=writen(sock_tcp,(void*)p_stream_info->p_stream[i],length);	
	    if(ret==-1)
	    {
		    printc(MSG_E, "Send stream information: send stream content error!(%m)\n", errno);
            close(sock_tcp);
		    return -1;
	    }
    }
	close(sock_tcp);
    return 0;
}

pn_stream_info* gdaq_get_stream_info_from_frame(char* buffer, int length)
{
    if(length<sizeof(pn_stream_info))
    {
        printc(MSG_E, "Send stream information: stream information length error!\n");
        return NULL;
    }
    pn_stream_info *p_stream_info=NULL;
    pn_stream_info stream_info_head;
    int count;
    memcpy((char*)&stream_info_head,buffer,sizeof(pn_stream_info));
    p_stream_info = (pn_stream_info*)malloc(sizeof(pn_stream_info)+stream_info_head.stream_num*sizeof(stream_terminal_info*));
    if(!p_stream_info)
    {
        printc(MSG_E, "Send stream information: malloc error!(%m)\n", errno);
        return NULL;
    }
    p_stream_info->stream_num = stream_info_head.stream_num;
    p_stream_info->endian = stream_info_head.endian;
    p_stream_info->pn_name.s_addr = stream_info_head.pn_name.s_addr;
    
    int i=0;
    int offset = sizeof(pn_stream_info);
    for(i=0;i<p_stream_info->stream_num;i++)
    {
        p_stream_info->p_stream[i]=(stream_terminal_info*)(buffer+offset);
        offset += sizeof(stream_terminal_info)+p_stream_info->p_stream[i]->group_num*sizeof(channel_group_info);
    }    
    return p_stream_info;
}


#endif

