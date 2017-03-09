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
