/*************************************************************************
	> File Name: clib_http.c
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年02月22日 星期三 16时47分55秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "clib_http.h"

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
