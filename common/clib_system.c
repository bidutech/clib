/*************************************************************************
  > File Name: clib_sytem.c
  > Author: shanhai2015
  > Mail: shanhai2015
  > Created Time: 2017年05月02日 星期二 14时11分43秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clib_system.h"



/**********************************************************************************
 * FROM:http://blog.csdn.net/cscmaker/article/details/7396631
 字节序有两种表示方法：大端法（big ending），小端法（little  ending）。
网络字节序采用的是大端法。
主机字节序不同的CPU采用的方法不一样，可以通过代码来查看自己主机的字节序。
大端法：高位字节排放在内存低地址端，低位字节排放在内存的高地址端。
小端法：低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。
看一个unsigned short 数据，它占2个字节，给它赋值0x1234。
若采用的大端法，则其低地址端应该存放的是0x12；
若采用的小端法，则其低地址端应该存放的是0x34；
可以通过联合体来获得其高低地址的数据。
测试主机字节序的代码：
********************************************************************************/
int clib_is_bigend()
{
	typedef union{  
    		unsigned short value;  
    		unsigned char bytes[2];  
	}Test; 
	   Test test_value;  
    test_value.value = 0x1234;  
  
    if(test_value.bytes[0] == 0x12 && test_value.bytes[1] == 0x34)  
    {   
	printf("big ending");  
	return 1;
    
    }else if(test_value.bytes[0] == 0x34 && test_value.bytes[1] == 0x12)  
   { 
       printf("little ending");  
	return 0;	
   }   
   else  
        printf("use test_value error");  
    return 0; 
}


int 
clib_check_live_process(char * live_process_name)
{
    if(live_process_name == NULL || live_process_name[0]==0)    
        return -3;
    FILE *fp = NULL;
    char *command = NULL;
    char buf[512] = "";
    int count = 0;


    command = malloc(512);        
    if(!command) return -2;                                                                                                                                                                                                 
    memset(command,0,512);

#ifdef ANDROID
    sprintf(command, "ps | grep -v grep | grep -E '%s'", live_process_name);                                                                                                                                          
#else  
    sprintf(command, "ps -ef | grep -v grep | grep -E '%s'", live_process_name);                                                                                                                                      
#endif 
    printf("check live process command:%s", command);                                                                                                                                                           
    fp = popen(command, "r");      
    if (!fp) {
        if(command)     
            free(command);
        command = NULL;
        return 0;
    }

    while (fgets(buf, sizeof(buf), fp)) {
        if (strstr(buf, "ps") && strstr(buf, "grep")) {
            continue;
        }
        count++;
    }

    pclose(fp);

    free(command);
    command = NULL;
    printf("check live process, count:%d", count);

    return count;
}

