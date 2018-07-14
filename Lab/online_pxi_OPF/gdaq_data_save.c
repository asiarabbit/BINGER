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
#ifndef __GDAQ_DATA_SAVE_C__
#define __GDAQ_DATA_SAVE_C__
#include "gdaq_data_save.h"

static int node_id=-1;      //node ID

static shm_interface* p_input_shm_interface;    // input interface 
static online_shm_interface* p_output_online_shm_interface;    // input interface 
static gdaq_global_buffer global_buffer;        // global buffer

int main(int argc, char *argv[])
{
    init_log_client();
	#ifdef DAQ_DEBUG
	printc(MSG_N, "Data save: This is Data save Node!\n");
    #endif
    if(argc!=2)
    {
        printc(MSG_E, "Data save: the number of argc is not matching!\n");
        gdaq_exit(-1);
    }
    char	conf_file[MAX_TMP_STRING_LENGTH];
	strcpy(conf_file, argv[1]);
    node_info *p_node=NULL;
    if(ln_config_ln_read_file(conf_file,&p_node)!=0)
    {
        printc(MSG_E, "Data save: Read configuration file error!\n");
        free(p_node);
        gdaq_exit(-1);
    }
    node_id = p_node->node_id;
    if((strcmp(p_node->name, "data_save")!=0)||(p_node->port_num!=1))
    {
        printc(MSG_E, "Data save(id=%d): the port information is not matching!\n", node_id);
        free(p_node);
        gdaq_exit(-1);
    }
    
    if((p_node->port[0].direction!=IN)||(p_node->port[0].type!=INTERFACE_SHM))
    {
        printc(MSG_E, "Data save(id=%d): the port information is not matching!\n",node_id);
        free(p_node);
        gdaq_exit(-1);
    }
    if(ln_interface_open(p_node->port[0].parameter, &p_input_shm_interface)!=0)
    {
        printc(MSG_E, "Data save(id=%d): Open SHM interface error!\n",node_id);
        free(p_node);
        gdaq_exit(-1);
    }
    	


      online_interface_destroy("online_shm", p_output_online_shm_interface);


    if(online_interface_create("online_shm", &p_output_online_shm_interface)!=0)
    {
        printc(MSG_E, "Data save(id=%d): Open SHM interface error!\n",node_id);
        free(p_node);
        gdaq_exit(-1);
    }
    
    if(online_interface_open("online_shm", &p_output_online_shm_interface)!=0)
    {
        printc(MSG_E, "Data save(id=%d): Open SHM interface error!\n",node_id);
        free(p_node);
        gdaq_exit(-1);
    }
    
	int buffer_page_size, buffer_page_num;
    buffer_page_size = DEFAULT_GLOBAL_BUFFER_PAGE_SIZE;
    buffer_page_num = DEFAULT_GLOBAL_BUFFER_PAGE_NUM;
    if(gdaq_global_buf_open(DEFAULT_GLOBAL_BUFFER_NAME, buffer_page_size, buffer_page_num, &global_buffer)!=0)
    {
        printc(MSG_E, "Data save(id=%d): Global buffer open error!\n",node_id);
        free(p_node);
        gdaq_exit(-1);
    }
    free(p_node);
    //initial done
    DIR *dir;
	if((dir = opendir(DEFAULT_SAVE_DATA_PATH)) == NULL)
	{
        if(mkdir(DEFAULT_SAVE_DATA_PATH, 0777)!=0)
        {
            printc(MSG_E,"Data save(id=%d): mkdir error!(%m)\n", node_id,errno);
            gdaq_exit(-1);
        }
	}
    closedir(dir);
	
	time_t now;   
    struct tm *timenow;   
    char strtemp[MAX_TMP_STRING_LENGTH];   
    time(&now);   
    timenow = localtime(&now);   
   // strftime(strtemp,MAX_TMP_STRING_LENGTH,"%m_%d_%H_%M",timenow);
    strftime(strtemp,MAX_TMP_STRING_LENGTH,"%Y%m%d_%H%M",timenow);
	char save_file_name[MAX_TMP_STRING_LENGTH];
	//sprintf(save_file_name,"%s/ln%d_%s.dat", DEFAULT_SAVE_DATA_PATH,node_id,strtemp);
	//sprintf(save_file_name,"%s/ln%d_%s.dat", DEFAULT_SAVE_DATA_PATH,node_id,strtemp);
	sprintf(save_file_name,"%s/%s.dat", DEFAULT_SAVE_DATA_PATH,strtemp);
	FILE *fout;
    if((fout=fopen(save_file_name,"wb"))==NULL)
    {
		printc(MSG_E,"Data save(id=%d): open output file error!(%m)\n", node_id,errno);
        gdaq_exit(-1);
    }
	//finish open the file
	int ret;
    LIST_HEAD(task_page_list);
    LIST_HEAD(data_out_list);

    ln_page_addr    input_task_addr;
    shm_interface_page_descriptor* p_interface;
    int             total_data_length;

    struct list_head    *pos, *q;
    data_out_section* p_data_section;
    	
    int output_online_data[DEFAULT_INTERFACE_ONLINE_LEN]={300};

  	
	while(1)
    {
       // printc(MSG_E,"before save get task\n");
        if(ln_interface_get_task(p_input_shm_interface,&input_task_addr)!=0)
        {
            printc(MSG_E,"Data save(id=%d): ln_interface_get_task error!\n", node_id);
	    fclose(fout);
            gdaq_exit(-1);
        }



     //   printc(MSG_E,"save get task(page id%d)\n",input_task_addr.ln_page.page_id);
        if((generate_data_out_list(&global_buffer, input_task_addr,&data_out_list,&total_data_length,&task_page_list))!=0)
        {
            free_page_ready_list(&global_buffer, &task_page_list, 0);
            free_data_out_list(&data_out_list);
			fclose(fout);
            printc(MSG_E,"Data save(id=%d):generate_data_out_list error!\n", node_id);
        	gdaq_exit(-1);
        }



         int pos_of_addr = 1;
		//save to file
		list_for_each_safe(pos,q, &data_out_list)
    	{
    		p_data_section=list_entry(pos, data_out_section, node);
                    /*
			if(fwrite(p_data_section->addr,sizeof(char),p_data_section->length,fout)!=p_data_section->length)
			{
				printc(MSG_E,"fwrite error\n");
				free_page_ready_list(&global_buffer, &task_page_list, 0);
            	                free_data_out_list(&data_out_list);
				fclose(fout);
    			        gdaq_exit(-1);
			} 
                      */
 

                if(pos_of_addr+p_data_section->length/4<=DEFAULT_INTERFACE_ONLINE_LEN )
                memcpy(&output_online_data[pos_of_addr], p_data_section->addr, p_data_section->length);

                pos_of_addr+=p_data_section->length/4;


        }
                pos_of_addr-=1; // after minus one, pos_of_addr becomes the data length of an event.
                memcpy(&output_online_data[0], &pos_of_addr, sizeof(int));

		fflush(fout);
      int nempty=-1;
      sem_getvalue(&p_output_online_shm_interface->nempty,&nempty);
      if(nempty!=0)

        if(online_interface_add_task(&output_online_data[0],p_output_online_shm_interface)!=0)
        {
            printc(MSG_E,"Data save(id=%d): ln_interface_get_task error!\n", node_id);
	    fclose(fout);
            gdaq_exit(-1); 
        }



        if((p_interface = (shm_interface_page_descriptor*)gdaq_global_buffer_get_addr(&global_buffer, input_task_addr,&task_page_list)) ==NULL)
        {
        	free_page_ready_list(&global_buffer, &task_page_list, 0);
            free_data_out_list(&data_out_list);
            printc(MSG_E,"Data save(id=%d): gdaq_global_buffer_get_addr get_addr error!\n", node_id);
			fclose(fout);
    		gdaq_exit(-1);
        }
	#ifdef DAQ_DEBUG

            if(pos_of_addr >= DEFAULT_INTERFACE_ONLINE_LEN*0.5)
            // if(p_interface->section_head.index%2000==0)
            printc(MSG_E,"Event length:  %d :(Index:%d)\n", pos_of_addr,p_interface->section_head.index);

            if(p_interface->section_head.index%1000==0)
            printd("Data save(id=%d): index: %d  Event Len: %d Words.\n", node_id,p_interface->section_head.index,total_data_length/4);
/*
            if(p_interface->section_head.index%1000==0)
            printd("Data save(id=%d): index: %d  Event Len: %d Words.\n", node_id,p_interface->section_head.index,pos_of_addr);
*/
     	#endif
		//begin free relevent page
        int if_free_data_page;
        pthread_mutex_lock(&p_interface->mutex);
        p_interface->map_factor--;
        if_free_data_page = (p_interface->map_factor== 0)?1:0;
        pthread_mutex_unlock(&p_interface->mutex);
		if(if_free_data_page)
			pthread_mutex_destroy (&p_interface->mutex);
        
        if(free_page_ready_list(&global_buffer,&task_page_list,if_free_data_page)!=0)
        {
			free_data_out_list(&data_out_list);
			printc(MSG_E,"Data save(id=%d): free_page_ready_list error!\n", node_id);
			fclose(fout);
            gdaq_exit(-1);
        }
        
        free_data_out_list(&data_out_list); 
		
   }
    
    
    
    fclose(fout);
    ln_interface_destroy(p_input_shm_interface->name, p_input_shm_interface);

    #ifdef DAQ_DEBUG
	printc(MSG_N, "Data save(id=%d): Data save Node exited!\n", node_id);
    #endif
	return 0;
}

#endif

