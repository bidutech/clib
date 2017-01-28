#ifndef CLIB_TIMER_H
#define CLIB_TIMER_H
#include <stdio.h>

/**********************************************************
 1s=1000ms=1000*1000微秒=1000*1000*1000纳秒=1000000000000皮秒

 struct timeval
{
__time_t tv_sec;        //Seconds.
__suseconds_t tv_usec;  //Microseconds.
};
其中，tv_sec为Epoch到创建struct timeval时的秒数，tv_usec为微秒数
*************************************************************/

long time_now_s(void);

long long  time_now_ms(void);/*long long 防止在32位设备溢出 gettimeofday取毫秒的溢出*/

struct timeval time_now_tv(void);

//返回时间间隔单位毫秒
long long  time_diff_ms(struct timeval newer, struct timeval older);

#endif