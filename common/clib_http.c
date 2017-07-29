/*************************************************************************
	> File Name: clib_http.c
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年02月22日 星期三 16时47分55秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netdb.h>

//#include <sys/sockio.h>
#include <net/if.h>
#include <errno.h>
//#include <net/if_dl.h>
#include <net/ethernet.h>


#include "clib_http.h"

static int clib_s_free_addresses(void * ipinfo);
static int clib_s_http_ip_init(void * ipinfo);


static int  clib_s_http_ip_init(void * ipinfo)
{

    clib_ip_type *p_ip = (clib_ip_type *)ipinfo;
    if(!p_ip ){
        return 0;
    }
    clib_s_free_addresses(p_ip);
    int i = 0;
    for (i=0; i<MAXADDRS; ++i)
    {
        (*p_ip).ifa_names[i] = (*p_ip).ip_names[i] = (*p_ip).hw_addrs[i] = NULL;
        (*p_ip).ip_addrs[i] = 0;
        (*p_ip).ip_addrs_v6[i] = 0;
    }
    (*p_ip).ip_count = 0;
    (*p_ip).ip6_count = 0;    
    return 1;
}


static int clib_s_free_addresses(void * ipinfo)
{
    clib_ip_type *p_ip = (clib_ip_type *)ipinfo; 
    if(!p_ip ){
        return 0;
    }
    if((*p_ip).pInit == NULL){
        //return 0;
    }

    int i;
    
    for (i=0; i<MAXADDRS; ++i)
    {
        if ((*p_ip).ifa_names[i] != 0) 
        {
            free((void *)((*p_ip).ifa_names[i]));
            (*p_ip).ifa_names[i] = NULL;
        }
        if ((*p_ip).ip_names[i] != 0)
        {
            free((void  *)((*p_ip).ip_names[i]));
            (*p_ip).ip_names[i] = NULL;
        }

        if ((*p_ip).hw_addrs[i] != 0)
        {
            free((void *)((*p_ip).hw_addrs[i]));
            (*p_ip).hw_addrs[i] = NULL;
        }
        (*p_ip).ip_addrs[i] = 0;
        (*p_ip).ip_addrs_v6[i] = 0;
        
    }
    (*p_ip).ip_count = 0;
    (*p_ip).ip6_count = 0;  
    return 1;
}

int clib_http_init_ip(void *ipinfo)
{
    clib_ip_type *p_ip = (clib_ip_type *)ipinfo; 
    if(!p_ip ){
        return 0;
    }
    (*p_ip).pInit = clib_s_http_ip_init;
    (*p_ip).pfree = clib_s_free_addresses;
    (*p_ip).pInit(p_ip);
    return 1;
}

int clib_http_free_ip(void * ipinfo)
{

    clib_ip_type *p_ip = (clib_ip_type *)ipinfo; 
    if(!p_ip ){
        return 0;
    }
    if((*p_ip).pfree)
    {
        (*p_ip).pfree(p_ip);
    }else{
        return 0;
    }
    return 1;
}

int clib_http_get_ip_v2(void * ipinfo) 
{
    clib_ip_type *p_ip = (clib_ip_type *)ipinfo; 
    if(!p_ip )
    {
        return 0;
    }
    if((*p_ip).pfree == NULL || (*p_ip).pInit == NULL){
        clib_http_init_ip(p_ip);    
    }
    int i=0;
    int  ipnum = 0;

    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) 
    {
        char temp[80] = {0}; 
   
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { 
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("ifa_name:%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
            if(0 == strcmp(addressBuffer, "127.0.0.1"))
            {
                ifAddrStruct=ifAddrStruct->ifa_next;
                continue;
            }
            (*p_ip).ip_addrs[ipnum] =inet_addr(addressBuffer);
            (*p_ip).ifa_names[ipnum] = (char *)malloc(strlen(ifAddrStruct->ifa_name)+1);
            if((*p_ip).ifa_names[ipnum] == NULL)
            {
                (*p_ip).ip_count = ipnum;
                return ipnum;
            }
            memset((*p_ip).ifa_names[ipnum],0,strlen(ifAddrStruct->ifa_name)+1);
            strcpy((*p_ip).ifa_names[ipnum], ifAddrStruct->ifa_name);

            struct in_addr irp;
            irp.s_addr = inet_addr(addressBuffer);// for use to use
            strcpy(temp, inet_ntoa(irp));
            (*p_ip).ip_names[ipnum] = (char *)malloc(strlen(temp)+1);
            if(NULL == (*p_ip).ip_names[ipnum])
            {
                (*p_ip).ip_count = ipnum;
                return ipnum;
            }
            strcpy((*p_ip).ip_names[ipnum], temp);

            printf("local addr =%s, [%s], iplong:%lu\n",addressBuffer,inet_ntoa(irp),(*p_ip).ip_addrs[ipnum]);
            ipnum++;
        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { 

            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("ifa_name:%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
        }

        ifAddrStruct=ifAddrStruct->ifa_next;
    }

    (*p_ip).ip_count = ipnum;

    return ipnum;

}

int clib_http_get_ip_v1(void * ipinfo)
{
    clib_ip_type *p_ip = (clib_ip_type *)ipinfo; 
    if(!p_ip )
    {
        return 0;
    }
    if((*p_ip).pfree == NULL || (*p_ip).pInit == NULL){
        clib_http_init_ip(p_ip);    
    }
    int i=0;
    int  ipnum =0;

    int sockfd;
    struct ifconf ifconf;
    unsigned char buf[512];
    struct ifreq *ifreq;


    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        return ipnum;
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);   


    ifreq = (struct ifreq*)buf;
    for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)
    {

        printf("ifr_name = [%s]\n", ifreq->ifr_name);
        char *p =inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
        printf("localaddr =%s, sin_addr:[%s]\n",p,inet_ntoa((((struct sockaddr_in*)&(ifreq->ifr_addr)))->sin_addr));
        ifreq++;

        if(0 == strcmp(p, "127.0.0.1"))
        {
            continue;
        }
        (*p_ip).ip_addrs[ipnum] =inet_addr(p);
        struct in_addr irp;
        irp.s_addr = inet_addr(p);
        printf("local addr =%s, [%s]\n",p,inet_ntoa(irp));
        ipnum++;
    }
    p_ip->ip_count = ipnum;
    return ipnum;

}



char *
clib_http_long2ip(u_long in)
{
    u_char *p;
    p = (u_char*)(&in);
    static char output[16];
    snprintf(output,16,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
    return output;
}

u_long 
clib_http_ip2long(char *ip)
{
    u_char addr[16];
    sscanf(ip,"%d.%d.%d.%d",addr,addr+1,addr+2,addr+3);
    u_long *vl = (u_long*)(&addr);
    return *vl;
}

int
clib_http_socket_available_1(int fd)
{
    int error = 0;
    socklen_t len;

    len = sizeof(error);
    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0) {
        return 1;
    }

    return 0;
}

int
clib_http_socket_available(int fd)
{
    struct tcp_info info;
    int    len;
    len = sizeof(info);
    if (0 == getsockopt(fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len))
    {
        if (TCP_ESTABLISHED == info.tcpi_state) {
            return 1;
        }
    }
    return 0;
}
