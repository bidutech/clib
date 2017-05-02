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

