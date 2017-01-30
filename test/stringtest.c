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

