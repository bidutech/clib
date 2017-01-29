#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "clib_time.h"




//使用比特位记录每年的情况
//0~4 共5bit 春节日份
//5~6 共2bit 春节月份
//7~19 共13bit 13个月的大小月情况(如果无闰月，最后位无效)，大月为1,小月为0
//20~23 共4bit 记录闰月的月份，如果没有闰月为0
static const int BEGIN_YEAR = 1901;
static const int NUMBER_YEAR = 199;
static const unsigned int LUNAR_YEARS[199] = {
        0x3a953, 0x752c8, 0x5b253d, 0x325d0, 0x54dc4,    //1901-1905
        0x4aab39, 0x2b54d, 0x5acc2, 0x2ba936, 0x3a94a,   //1906-1910
        0x6d92be, 0x592d2, 0x525c6, 0x5a55ba, 0x156ce,   //1911-1915
        0x2b5c3, 0x25b4b7, 0x6d4cb, 0x7ec941, 0x74954,   //1916-1920
        0x692c8, 0x5d26bc, 0x52b50, 0x15b45, 0x4adab8,   //1921-1925
        0x36a4d, 0x754c2, 0x2f4937, 0x7494a, 0x66933e,   //1926-1930
        0x295d1, 0x52bc6, 0x596b3a, 0x5ad4e, 0x36a44,    //1931-1935
        0x3764b8, 0x3a4cb, 0x7b49bf, 0x549d3, 0x295c8,   //1936-1940
        0x652dbb, 0x556cf, 0x2b545, 0x4daab9, 0x5d24d,   //1941-1945
        0x5a4c2, 0x2d49b6, 0x54aca, 0x7a96bd, 0x29b51,   //1946-1950
        0x556c6, 0x5ad53b, 0x2d94e, 0x5d2c3, 0x3ea538,   //1951-1955
        0x5a54c, 0x854abf, 0x54bd2, 0x2ab48, 0x655abc,   //1956-1960
        0x56acf, 0x36945, 0x4752b9, 0x752cd, 0x32542,    //1961-1965
        0x364bb5, 0x54dc9, 0x7aad3e, 0x2b551, 0x5b4c6,   //1966-1970
        0x5ba93b, 0x5a94f, 0x592c3, 0x4b25b7, 0x525cb,   //1971-1975
        0x8a55bf, 0x156d2, 0x2d6c7, 0x65b4bc, 0x6d4d0,   //1976-1980
        0x6c945, 0x4e92b9, 0x692cd, 0xad26c2, 0x52b54,   //1981-1985
        0x15b49, 0x62dabd, 0x36ad1, 0x754c6, 0x5f493b,   //1986-1990
        0x7494f, 0x69344, 0x352b37, 0x52bca, 0x8a6b3f,   //1991-1995
        0x1ad53, 0x36ac7, 0x5b64bc, 0x3a4d0, 0x349c5,    //1996-2000
        0x4a95b8, 0x295cc, 0x52dc1, 0x2aad36, 0x2b549,   //2001-2005
        0x7daabd, 0x5d252, 0x5a4c7, 0x5d49ba, 0x54ace,   //2006-2010
        0x296c3, 0x4556b7, 0x55aca, 0x9ad53f, 0x2e953,   //2011-2015
        0x6d2c8, 0x6ea53c, 0x6a550, 0x64ac5, 0x4a9739,   //2016-2020
        0x2ab4c, 0x55ac1, 0x2ad936, 0x3694a, 0x6752bd,   //2021-2025
        0x392d1, 0x325c6, 0x564bba, 0x655cd, 0x2ad43,    //2026-2030
        0x356b37, 0x5b4cb, 0xbba93f, 0x5a953, 0x592c8,   //2031-2035
        0x6d25bc, 0x525cf, 0x255c4, 0x52adb8, 0x2d6cc,   //2036-2040
        0x5b541, 0x2da936, 0x6c94a, 0x7e92be, 0x692d1,   //2041-2045
        0x52ac6, 0x5a56ba, 0x25b4e, 0x2dac2,0x355B37,    //2041-2050
        0x0764cb,0x8497C1,0x049753,0x064B48,0x66A53C,
        0x052bcf,0x06B244,0x4AB638,0x03aacc,0x03a4c2,   //2051-2060
        0x3C9735,0x0349c9,0x7D4ABD,0x0295d1,0x052dc5,
        0x55AABA,0x056A4E,0x05b2c3,0x452EB7,0x052D4B,   //2061-2070
        0x8A95BF,0x054ad3,0x0296c7,0x6B553B,0x055acf,
        0x055A45,0x4A5D38,0x06d2cc,0x052B42,0x3A93B6,   //2071-2080
        0x069349,0x7729BD,0x06AA51,0x055ac6,0x54DABA,
        0x04B64E,0x0752c3,0x452738,0x0325ca,0x8E933E,   //2081-2090
        0x0255d2,0x02adc7,0x66B53B,0x056D4F,0x04AE45,
        0x4A4EB9,0x0592cc,0x0545c1,0x2D92B5             //2091-2099
};

static const char *CH_NUMBER_V[] ={"零","一","二","三","四","五",
                                   "六","七","八","九","十"};

static  const char *CH_MONTH_V[] ={"*","正","二","三","四","五","六",
                                  "七","八","九","十","十一","腊"};

static  const char *CH_DAY_V[] ={"*","初一","初二","初三","初四","初五",
                                "初六","初七","初八","初九","初十",
                                "十一","十二","十三","十四","十五",
                                "十六","十七","十八","十九","二十",
                                "廿一","廿二","廿三","廿四","廿五",
                                "廿六","廿七","廿八","廿九","三十"};

static const char *CH_YEAR = "年";
static const char *CH_MONTH = "月";
static const char *CH_DAY ="日";
static const char *CH_SP = "";
static const char *CH_LEAP = "闰";

static char *
time_lunardateto_ch_str_r(lunardate lunar_date,char *buf);


/*
 struct tm
{ 　
int tm_sec;         // 秒–取值区间为[0,59]
int tm_min;         // 分 - 取值区间为[0,59]
int tm_hour;        // 时 - 取值区间为[0,23]
int tm_mday;        // 一个月中的日期 - 取值区间为[1,31]
int tm_mon;         //月份（从一月开始，0代表一月） - <span style="color:#cc0000;">取值区间为[0,11]</span>
int tm_year;        // 年份，其值从1900开始 　
int tm_wday;        // 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 　
int tm_yday;        //从每年的1月1日开始的天数–取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 　
int tm_isdst;       // 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。　
long int tm_gmtoff;  //指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数 　　
const char *tm_zone; //当前时区的名字(与环境变量TZ有关)　
};
*/
/***************************************************************************************************
C++代码实现的农历算法（1901 - 2099）
历法基本知识总结，不废话。
太阳公转周期，一年时间约365.2425天。

公历为了对齐公转周期，4年一闰，100年一停闰，400年加一闰。 400年共97闰， （365×400+97）/400 = 365.2425

农历，一个月相周期为一月，月相周期大约为29.53,   有大小月之分，大月30天，小月29天。月相变化受到地球公转
和月亮围绕地球转的共同影响，两个都是椭圆轨道，且不在一个平面上，非常复杂。所以，大小月变化规律不是一个简单的规律。

农历为了对齐公转周期，每19年加7个闰月，

因为，农历的推算比较复杂， 大多使用查表法进行计算，包括我这里提供的算法。

我一直想实现一个计算轨道的推算方法，因为人懒，现在还没有什么进展。我会继续关注。

现在看到网上没有找到满意的C++查表农历算法法实现，所以自己实现了一个。
luanrday - 使用查表发实现公历转换为农历

文件说明
luanrday.h     头文件，声明了公历到农历转换的函数
luanrday.cpp   实现文件,实现了公历到农历的转换函数
chdate.cpp     例子程序，使用luanrday实现功能：使用汉字显示当前农历日期.
Makefile       编译chdate使用

接口说明
请阅读文件clib_time.h

例子
lunardate ldate =time_lunardate(2008,8,2);
//ldate即保存农历日期, 其中:
//     年:    ldate.year
//     月:    ldate.month
//     日:    ldate.day
//   是否润月: ldate.leap

************************************************************************/

solardate
time_now_solardate()
{
    solardate solar_date ;
    struct tm *newtime;
    time_t lt1;
    time( &lt1 );
    newtime=localtime(&lt1);
    solar_date.year = newtime->tm_year +1900;
    solar_date.month =newtime->tm_mon +1;
    solar_date.day = newtime->tm_mday;
    return solar_date;
}

lunardate
time_now_lunardate()
{
    solardate solar_date;
    lunardate lunar_date;
    solar_date = time_now_solardate();
    lunar_date = time_lunardate(solar_date.year,solar_date.month,solar_date.day);
    return  lunar_date;
}
//计算这个公历日期是一年中的第几天
int
time_day_of_solaryear(int year, int month, int day )
{
    //为了提高效率，记录每月一日是一年中的第几天
    static const int NORMAL_YDAY[12] = {1,32,60,91,121,152,
                                        182,213,244,274,305,335};
    //闰年的情况
    static const int LEAP_YDAY[12] = {1,32,61,92,122,153,
                                      183,214,245,275,306,336};
    const int *t_year_yday_ = NORMAL_YDAY;

    //判断是否是公历闰年
    if( year % 4 ==0 )
    {
        if(year%100 != 0)
            t_year_yday_ = LEAP_YDAY;
        if(year%400 == 0)
            t_year_yday_ = LEAP_YDAY;
    }
    return t_year_yday_[month -1] + (day -1);
}


char *
time_lunardate_to_callocstr(lunardate lunar_date)
{
    char *buf=calloc(1,MAX_CH_DATE_LEN+1);
    return time_lunardateto_ch_str_r(lunar_date,buf);
}



lunardate time_lunardate(int solar_year,int solar_month,int solar_day)
{
    lunardate luanr_date ;
    luanr_date.year = solar_year;
    luanr_date.month = 0;
    luanr_date.day = 0;
    luanr_date.leap = false;

    //越界检查，如果越界，返回无效日期
    if(solar_year <= BEGIN_YEAR || solar_year > BEGIN_YEAR + NUMBER_YEAR - 1 )
        return luanr_date;

    int year_index = solar_year - BEGIN_YEAR;

    //计算春节的公历日期
    int spring_ny_month = ( LUNAR_YEARS[year_index] & 0x60 ) >> 5;
    int spring_ny_day = ( LUNAR_YEARS[year_index] & 0x1f);

    //计算今天是公历年的第几天
    int today_solar_yd = time_day_of_solaryear(solar_year,solar_month,solar_day);
    //计算春节是公历年的第几天
    int spring_ny_yd = time_day_of_solaryear(solar_year,spring_ny_month,spring_ny_day);
    //计算今天是农历年的第几天
    int today_luanr_yd = today_solar_yd - spring_ny_yd + 1;
    //如果今天在春节的前面，重新计算today_luanr_yd
    if ( today_luanr_yd <= 0 )
    {
        //农历年比当前公历年小1
        year_index --;
        luanr_date.year --;
        //越界，返回无效日期
        if(year_index <0)
            return luanr_date;
        spring_ny_month = ( LUNAR_YEARS[year_index] & 0x60 ) >> 5;
        spring_ny_day = ( LUNAR_YEARS[year_index] & 0x1f);
        spring_ny_yd = time_day_of_solaryear(solar_year,spring_ny_month,spring_ny_day);

        int year_total_day = time_day_of_solaryear(luanr_date.year,12,31);
        today_luanr_yd = today_solar_yd + year_total_day - spring_ny_yd + 1;
    }

    int luanr_month = 1;
    //计算月份和日期
    for(;luanr_month<=13;luanr_month++)
    {
        int month_day  = 29;
        if( (LUNAR_YEARS[year_index] >> (6 + luanr_month)) & 0x1 )
            month_day = 30;
        if( today_luanr_yd <= month_day )
            break;
        else
            today_luanr_yd -= month_day;
    }
    luanr_date.day = today_luanr_yd;
    //处理闰月
    int leap_month = (LUNAR_YEARS[year_index] >>20)  & 0xf;
    if(leap_month > 0 && leap_month < luanr_month )
    {
        luanr_month --;
        //如果当前月为闰月，设置闰月标志
        if( luanr_month == leap_month )
            luanr_date.leap = true;
    }
    assert(leap_month <= 12);//TODO:delete this code releasre
    luanr_date.month = luanr_month;
    return luanr_date;
}


static char *
time_lunardateto_ch_str_r(lunardate lunar_date,char *buf)
{
    if(lunar_date.year >= 1000);
    assert(lunar_date.year <= 9999);

    assert(lunar_date.month >=1);
    assert(lunar_date.month <=12);

    assert(lunar_date.day >=1);
    assert(lunar_date.day <=30);

    char *pbuf = buf;

    int t_years = lunar_date.year;

    int t_idx = t_years /1000;
    t_years -= t_idx*1000;
    strcpy(pbuf,CH_NUMBER_V[t_idx]);
    pbuf += strlen(CH_NUMBER_V[t_idx]);

    t_idx = t_years /100;
    t_years -= t_idx*100;
    strcpy(pbuf,CH_NUMBER_V[t_idx]);
    pbuf += strlen(CH_NUMBER_V[t_idx]);

    t_idx = t_years /10;
    t_years -= t_idx*10;
    strcpy(pbuf,CH_NUMBER_V[t_idx]);
    pbuf += strlen(CH_NUMBER_V[t_idx]);

    t_idx = t_years;
    strcpy(pbuf,CH_NUMBER_V[t_idx]);
    pbuf += strlen(CH_NUMBER_V[t_idx]);

    strcpy(pbuf,CH_YEAR);
    pbuf += strlen(CH_YEAR);

    strcpy(pbuf,CH_SP);
    pbuf += strlen(CH_SP);

    if(lunar_date.leap)
    {
    strcpy(pbuf,CH_LEAP);
    pbuf += strlen(CH_LEAP);
    }

    t_idx = lunar_date.month;
    strcpy(pbuf,CH_MONTH_V[t_idx]);
    pbuf += strlen(CH_MONTH_V[t_idx]);

    strcpy(pbuf,CH_MONTH);
    pbuf += strlen(CH_MONTH);

    t_idx = lunar_date.day;
    strcpy(pbuf,CH_DAY_V[t_idx]);
    pbuf += strlen(CH_DAY_V[t_idx]);
    pbuf = '\0';
    assert(pbuf - buf <= MAX_CH_DATE_LEN);

    return buf;
}


/**********************************************************
 1s=1000ms=1000*1000微秒=1000*1000*1000纳秒=1000000000000皮秒

 struct timeval
{
__time_t tv_sec;        //Seconds.
__suseconds_t tv_usec;  //Microseconds.
};
其中，tv_sec为Epoch到创建struct timeval时的秒数，tv_usec为微秒数
 1秒＝1000毫秒，
1毫秒＝1000微秒，
1微妙＝1000纳秒，
1纳秒＝1000皮秒。
秒用s表现,毫秒用ms,微秒用μs表示，纳秒用ns表示，皮秒用ps表示
*************************************************************/

struct timeval
time_now_tv(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

long
time_now_s(void)
{

    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  tv.tv_sec;
}

long long
time_now_ms(void)
{

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec;
    return ms * 1000 + tv.tv_usec / 1000;
}

long long
time_diff_ms(struct timeval newer, struct timeval older)
{

    return (newer.tv_sec-older.tv_sec)*1000 +  (newer.tv_usec-older.tv_usec)/1000;
}

/*
 C/C++中的计时函数是clock()，而与其相关的数据类型是clock_t
在time.h文件中，还定义了一个常量CLOCKS_PER_SEC，它用来表示一秒钟会有多少个时钟计时单元，其定义如下：
#define CLOCKS_PER_SEC ((clock_t)1000)
可以看到每过千分之一秒（1毫秒）
 */
long
time_elapsed_time()
{

    return clock()/CLOCKS_PER_SEC;
}

/*
 gmtime()函数是将日历时间转化为世界标准时间（即格林尼治时间），
 并返回一个tm结构体来保存这个时间，而localtime()函数是将日历时间转化为本地时间。
 比如现在用gmtime()函数获得的世界标准时间是2017年1月29日7点18分30秒，
 那么我用localtime()函数在中国地区获得的本地时间会比世界标准时间晚8个小时，
 即2017年1月29日30日15点18分30秒
 * */
char *
time_today_full_string()
{
    struct tm *newtime;
    char *todaystr=calloc(1,128);
    if(!todaystr){
        return  NULL;
    }
    time_t lt1;
    time( &lt1 );
    newtime=localtime(&lt1);
    strftime(todaystr, 128,"Today is %A, day %d of %B in the year %Y", newtime);
    return todaystr;
}