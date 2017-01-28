#include <unistd.h>
#include <time.h>
#include <sys/time.h>


#include "clib_time.h"

struct timeval time_now_tv(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

long time_now_s(void){

    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  tv.tv_sec;
}

long long time_now_ms(void){

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec;
    return ms * 1000 + tv.tv_usec / 1000;
}

long long  time_diff_ms(struct timeval newer, struct timeval older){

    return (newer.tv_sec-older.tv_sec)*1000 +  (newer.tv_usec-older.tv_usec)/1000;
}