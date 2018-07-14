#ifndef GDAQ_LIB_MODULE_H
#define GDAQ_LIB_MODULE_H

#include "gdaq_object.h"
#include "gdaq_general.h"
#include "gdaq_typedef.h"
#include "gdaq_driver_module.h"
#include "dlfcn.h"

#define MODULE_PATH "./lib/"                //动态链接库存放的路径。该路径是相对于编译后的build目录，即build/lib
#define MODULE_CONFIG_APTH "./device_card"  //动态链接库配置文件存放路径。该路径是相对于编译后的build目录，即build/device_card


typedef struct driver_module_t
{

    int (*start_dma)(); //开始采集数据
    int (*stop_dma)();  //停止采集数据
    void (*close)();    //关闭相关资源
    int (*init)(struct driver_module_t *_driver_module);  //初始换动态链接库
    int (*device_read)(char* buffer,int num);             //读取数据，读取num个数据才返回。
    int (*read)(char* buf,int length);                    //单次读取，最多读取num个数据
    int (*get_dev_info)(struct driver_module_t *_driver_module); //得到动态链接库相关信息
        
	int fd;   //驱动设备描述符           
	char module_name[LEN_32]; //动态链接库的名称
    char dev_name[LEN_128];//动态链接库中打开的设备的路径
    char card_type[LEN_32];//动态链接库中打开的设备的种类
    int channel_num;      //动态链接库中打开的设备的通道数
    int start_channel;    //动态链接库中打开的设备的起始通道号
    int card_id;          //动态连接库的id
	int  id;  //



 
}driver_module;


void print_module_info(driver_module *module)
{
    printf("module name:%s\n" ,module->module_name);
    printf("dev name:%s\n" ,module->dev_name);
    printf("card type:%s\n" ,module->card_type);
    printf("start channel :%d\n" ,module->start_channel);
    printf("channel num :%d\n" ,module->channel_num);
    printf("card id :%d\n" ,module->card_id);

}


/**
 * @brief is_driver_dev
 * @param name
 * @param prefix
 * @return
 */
int is_driver_dev(char* name ,char *prefix )
{
    char* str=prefix;
    if(memcmp(name,str ,strlen(str))!=0)
        return 0;
    char* p = name+strlen(str);
    if(strlen(p)==0)
        return 0;
    int is_digit=1;
    int i;
    for(i=0;i<strlen(p);i++)
    {
        if(isdigit(p[i])==0)
        {
            is_digit=0;
            break;
        }
    }
    return is_digit;
}

/**
 * @brief get_device_num ,
 * @param prefix:driver type,such as : altera_fe
 * @return
 */

int get_device_num(char *prefix)
{
    if(prefix==NULL)
    {
        return -1;
    }
    DIR *dir;
    struct dirent *ptr;
    int card_num=0;
    dir = opendir("/dev/");
    if(dir == NULL)
    {
        printf("directory /dev open error!\n");
        return -1;
    }
    while((ptr = readdir(dir)) != NULL)
    {
        if(is_driver_dev(ptr->d_name , prefix)==1)
        {
            printd("find device %s\n",ptr->d_name);
            card_num++;
        }
    }
    closedir(dir);
    return card_num;
}

int get_device_list(char name_list[][MAX_TMP_STRING_LENGTH] , int max_count , char *prefix)
{
    if(prefix==NULL||name_list==NULL)
    {
        return -1;
    }
    DIR *dir;
    int length = max_count;
    if(length<=0)
    {
        return 0;
    }

#ifdef DAQ_DEBUG
    printd("len  %d\n" , length);
#endif

    struct dirent *ptr;
    int card_num=0;
    dir = opendir("/dev/");
    if(dir == NULL)
    {
        printd("directory /dev open error!\n");
        return -1;
    }
    while((ptr = readdir(dir)) != NULL)
    {
        if(is_driver_dev(ptr->d_name , prefix)==1)
        {
            printd("find device %s\n",ptr->d_name);

            strcpy(name_list[card_num] , ptr->d_name);
            card_num++;
            if(card_num>=length)
            {
                break;
            }
        }
    }
    closedir(dir);
    return card_num;
}



int find_modules()
{



}

int check_module_exist()
{

}


/**
 * @brief get_device_module_name,由设备驱动名称得到module name
 * @param device_name 要得到的设备名称
 * @param module_name 返回module name
 */

void get_device_module_name(char *device_name , char *module_name)
{
    if(strncmp(device_name,"altera_fe" , sizeof("altera_fe")))
    {
        sprintf(module_name , "libaltera_fe.so");
    }
}

int load_module(char *module_name , driver_module *module)
{
    void *handle;
    char *error;
    void (*register_module )(driver_module *module);
    handle = dlopen(module_name, RTLD_LAZY);
    char module_path_name[LEN_512];
    sprintf(module_path_name , "%s%s" , MODULE_PATH , module_name);
    if(!handle){
        printc(MSG_E , "open lib (%s)  error :%s\n" ,module_path_name , dlerror());
        return -1;
    }
    else
    {
        printd(  "open lib success\n");
    }

    register_module = dlsym(handle, "register_module");
    if((error = dlerror()) != NULL){

        printc(MSG_E , "register module error %s\n" , error);
        return -1;
    }
    register_module(module);
    return 0;
}

#endif // GDAQ_LIB_MODULE_H
