/*************************************************************************
	> File Name: clib_http.h
	> Author: shanhai2015
	> Mail: sshanhai2015
	> Created Time: 2017年02月22日 星期三 16时47分48秒
 ************************************************************************/


#ifndef CLIB_HTTP_H
#define CLIB_HTTP_H
#ifdef __cplusplus
extern "C" {
#endif

/********************************
* MAXADDRS is max ip item num
********************************/
#define MAXADDRS  32 

#define IP_BUFFERSIZE 4000

#define clib_min(a,b) ((a) < (b) ? (a) : (b))
#define clib_max(a,b) ((a) > (b) ? (a) : (b))


typedef int (*pfun_ipinit)(void *);

typedef struct CLIBIPINFO{
    char *ifa_names[MAXADDRS];
    char *ip_names[MAXADDRS];
    char *hw_addrs[MAXADDRS];
    int ip_count;
    int ip6_count;
    unsigned long ip_addrs[MAXADDRS];
    unsigned long ip_addrs_v6[MAXADDRS];
    pfun_ipinit pInit;
    pfun_ipinit pfree;
    
    

}CLIB_IP_INFO,clib_ip_type;



int clib_http_init_ip(void *ipinfo);
int clib_http_free_ip(void * ipinfo);
int clib_http_get_ip_v2(void * ipinfo);
int clib_http_get_ip_v1(void * ipinfo);


/*
 ip2long long toip used by :used ip->long sa hash key
 * */
u_long 
clib_http_ip2long(char *ip);

char * 
clib_http_long2ip(u_long in);

int 
clib_http_socket_available_1(int fd);

int 
clib_http_socket_available(int fd);

#ifdef __cplusplus
}
#endif

#endif
