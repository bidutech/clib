#ifndef CLIB_TIMER_H
#define CLIB_TIMER_H
#include <stdio.h>
#include <stdbool.h>


#define  MAX_CH_DATE_LEN  26

typedef struct time_lunar_date  lunardate;
typedef struct time_lunar_date  solardate;


struct time_lunar_date
{
    //保存一个农历日期
    //年
    int year;
    //月
    int month;
    //日
    int day;
    //是否闰月
    bool leap;
};

solardate time_now_solardate();//年 月 日

lunardate time_now_lunardate();//当前农历年月日

char * time_lunardate_to_callocstr(lunardate lunar_date);//获取农历字符串 “二零一七年二月初二”


lunardate time_lunardate(int solar_year,int solar_month,int solar_day);//公历年月日转农历

//计算这个公历日期是一年中的第几天
int time_day_of_solaryear(int year, int month, int day );


long time_now_s(void);

long long  time_now_ms(void);/*long long 防止在32位设备溢出 gettimeofday取毫秒的溢出*/

struct timeval time_now_tv(void);


long long  time_diff_ms(struct timeval newer, struct timeval older);//返回时间间隔单位毫秒


long time_elapsed_time();//返回进程运行时间

char * time_today_full_string();//Today is Sunday, day 29 of January in the year 2017



#endif