/*************************************************************************
	> File Name: clib_socket.c
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年08月19日 星期六 18时32分39秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>


#include "clib_socket.h"


int clib_test_callbackfunc(void *req_arg){

    if(req_arg == NULL) 
        return 0;
}

Clib_CallbackFuction  clib_common_get_callbackfunc(char op){

    switch (op)
    {
        case 't': 
            return clib_test_callbackfunc;
        default:
            break;
    }
    return NULL;
}

void clib_socket_handle_read(void * efdarg, int fd)
{
    CLib_socket_handle_data * opdata  = (CLib_socket_handle_data *)efdarg;
    if(opdata == NULL) return;
    int efd = opdata->epollfd;

    int readret = 0;
    Clib_req_arg * data_ok = NULL;
    data_ok = calloc(1, sizeof(Clib_req_arg));
    if(data_ok == NULL){
        printf("calloc error");
        return ;
    }
    data_ok->sockfd =fd;
    int i =0;
    int r = 0;
    while (readret = read(data_ok->sockfd, data_ok->readbuf + data_ok->readlen, sizeof(data_ok->readbuf) - data_ok->readlen - 1) > 0) {

        i =readret;
        data_ok->readlen += readret;
        if(opdata->data_get_callback != NULL)
        {
            r = opdata->data_get_callback(opdata);
        }
        if(r <=0) break;

        //pstrncasecmp(url, "/Service.do?", strlen("/Service.do?"))) {rintf("read %d bytes\n", n);
        //int r = write(fd, buf, n); //写出读取的数据  process EAGAIN
        ////实际应用中，写出数据可能会返回EAGAIN，此时应当监听可写事件，当可写时再把数据写出
        // printf_if(r<=0, "write error");
    }
    if (readret<0 && (errno == EAGAIN || errno == EWOULDBLOCK)){
        printf("readret <0 errno:%d",errno);
        if(i ==0 ){
            printf("i == 0,readret <0 errno:%d",errno);
            goto exit;
        }
    }
    return;
exit:
    close(data_ok->sockfd);
    free(data_ok);
    data_ok = NULL;
    /*
     *  actual should check errno :EINTR eg
    */
    printf("readret:%d -------------errno:%d",readret,errno);


}


int clib_socket_handle_accept(void *efdarg, int fd) 
{

    CLib_socket_handle_data * opdata  = (CLib_socket_handle_data *) efdarg;
    if(opdata == NULL) return 0;
    int efd = opdata->epollfd;

    struct sockaddr_in raddr;
    socklen_t rsz = sizeof(raddr);
    int cfd = accept(fd,(struct sockaddr *)&raddr,&rsz);
    printf_if(cfd<0, "accept failed");

    //struct sockaddr_in peer, local;
    //socklen_t alen = sizeof(peer);
    //int r= 0;
    //r = getpeername(cfd, (sockaddr*)&peer, &alen);
    //printf_if(r<0, "getpeername failed");
    //printf("accept a connection from %s\n", inet_ntoa(raddr.sin_addr));
    //clib_socket_set_non_block(cfd);
    //


    return cfd;
}


void clib_socket_set_non_block(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    printf_if(flags<0, "fcntl failed");
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    printf_if(r<0, "fcntl failed");
}

int clib_socket_create_listened_socket(char *ipstr,int port)
{
    // ip参数值为INADDR_ANY表示任意地址。
        int listenfd;
    int reuseaddr = 1;
    int keepalive = 1;
    struct sockaddr_in servaddr;

    if(0 > (listenfd = socket(AF_INET, SOCK_STREAM, 0))){
        printf("socket err!\n");
        return -1; 
    }   
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseaddr, sizeof(reuseaddr));
    setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepalive, sizeof(keepalive));

#ifdef OS_IOS
    int set = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
#endif

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if(ipstr != NULL){
        servaddr.sin_addr.s_addr = inet_addr(ipstr);
    }else{
        servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
        //servaddr.sin_addr.s_addr = INADDR_ANY;

    } 
    servaddr.sin_port = htons(port);
    if(0 > bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))){

        close(listenfd);
        return -3; 
    }
    if(0 > listen(listenfd, 5)){
        close(listenfd);
        return -4;
    }

    return listenfd;
}
