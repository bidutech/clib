//
// Created by root on 17-1-13.
//
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "clib_string.h"
#include "stringtest.h"
#include "clib_time.h"
#include "clib_conf.h"
#include "clib_file.h"

void test_conf(){
 char  *path="test.ini";
  char *p = clib_conf_parse_ini_param(path, "processname");

  if (!p) return ;
  char *proname = calloc(1,strlen(p) + 1);
  strncpy(proname, p, strlen(p));

  int port = 0;
  CLIB_INI_ParseParamInt(path, "httpport", port);
  if (port <= 0) {
      port = 3300;
  }
    printf("processname:%s,httpport:%d\n",proname,port);

    clib_conf_ini_writeconf(path,"updateurl","www.baidu.com",1);
    clib_conf_ini_writeconf(path,"intertime","3000",0);
    clib_conf_ini_writeconf(path,"httpport","8080",0);



    
    char *p1 = clib_conf_parse_ini_param(path, "updateurl");

    if (!p1) return ;
    char *updateurl = calloc(1,strlen(p1) + 1);
    strncpy(updateurl, p1, strlen(p1));

    int intertime = 0;
    CLIB_INI_ParseParamInt(path, "intertime", intertime);
    if (intertime <= 0) {
        intertime = 3300;
    }
    printf("updateurl:%s,intertime:%d\n",updateurl,intertime);
    free(updateurl);
    free(proname);

}
void test_file(){
  char *path=NULL;
  int pathlen=0;
  pathlen=clib_file_current_mallocfullpath(&path);
  printf("pathlen:%d,path:%s\n",pathlen,path);
  free(path);
  path =NULL;

}
void timenow(){


    struct timeval now,future;
    now =time_now_tv();
    sleep(1);
    future=time_now_tv();
    printf("time run :%lld\n",time_diff_ms(future,now));
    printf("now:%ld\n",time_now_s());
    printf("****************\n");
    char *today =time_today_callocstring();
    if(today){
        printf("today:%s\n",today);
    }

    lunardate  lunar_date =time_now_lunardate();

    char * buf = time_lunardate_to_callocstr(lunar_date);
    printf("now_lunar_date:%s\n",buf);


}
int teststrsplit(){
    char *str=NULL;
    char *str1=NULL;
    str1=string_malloc(&str,100);

    strcpy(str1,"hello---------------- word!\n");
    printf(str1);
    return 0;

    if(1){
        char *separator[] = {"\n","-"};
        char *str = "a\nababa\nbbaabab--aaab";
        char **result = NULL;
        int n_str = 0;
        int i = string_split(&result, &n_str, str, separator, 2, 0, 1);
        for (i = 0; i < n_str; i++){
            char tem[1024];
            //printf("%d\n",sizeof(result[i]));
            // printf("%d\n",strlen(result[i]));
            // strncpy(tem,result[i],sizeof(result[i]);
            printf("%s\n", result[i]);
        }

        for (i = 0; i < n_str; i++)
            free(result[i]);
        free(result);
    }


    char * s="012345678901234567890123456789";
    //012345678901234567890123456789
    char *p;
    p= strstr(s,"0123");
    printf("%d\n",p);

    return 0;
}

int teststrprinthx(){

    char ch[1024]={"hello word $#@ morning"};
    char hex[1024];
    char result[1024];
    char *p_ch = ch;
    char *p_hex = hex;
    char *p_result = result;
    printf("the src string:%s\n",ch);


    string_to_hex(p_ch, p_hex);
    printf("the hex is:%s\n",p_hex);
    string_hex_to_string(p_hex, p_result);
    printf("the src string:%s\n", p_result);
    return 0;
}
int printhextest(){

    char *p= "hello D0 word";
    char *des= (char*)malloc(strlen(p));

    string_print_hex((u_char *) p, strlen(p), des, strlen(p));
    printf("src string is:%s\n",p);
    printf("des string is:%s\n",des);
}

