/*************************************************************************
	> File Name: clib_kqueue.c
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年08月21日 星期一 11时42分03秒
 ************************************************************************/

#include <stdio.h>

#include "clib_socket.h"
#include "clib_kqueue.h"
#include "clib_http_url.h"

#ifdef OS_FREE_BSD

#define kReadEvent = 1;
#define kWriteEvent = 2;

static int clib_socket_kqueue_handle_accept(void *efdarg, int fd) 
{

    CLib_socket_handle_data * opdata  = (CLib_socket_handle_data *)efdarg;
    if(opdata == NULL) return 0;
    
    int efd = opdata->epollfd;
    if(opdata->sokcet_type!= SOKCET_KQUEUE)  
        return -1;
    int cfd = clib_socket_handle_accept(efdarg,fd);
    opdata->event_update_callback(efd, cfd, kReadEvent|kWriteEvent, 0);
    printf_if(cfd<0, "accept failed");
    return cfd;
}

static int clib_socket_kqueue_handle_read(void *efdarg, int fd) 
{

    CLib_socket_handle_data * opdata  = (CLib_socket_handle_data *)efdarg; 
    if(opdata == NULL) return 0;

    if(opdata->sokcet_type!= SOKCET_KQUEUE)  

        return 0;

    clib_socket_handle_read(efdarg,fd);

}
static void clib_kqueue_loop_work_once(void* efdarg, int lfd, int waitms) {

    CLib_socket_handle_data * opdata  = (CLib_socket_handle_data *)efdarg;
    if(opdata == NULL) return;
    int efd = opdata->epollfd;
    if(opdata->sokcet_type!= SOKCET_KQUEUE)  
        return;

    struct timespec timeout;
    timeout.tv_sec = waitms / 1000;
    timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;
    const int kMaxEvents = 20;
    struct kevent activeEvs[kMaxEvents];
    int n = kevent(efd, NULL, 0, activeEvs, kMaxEvents, &timeout);
    if(n <=0){
        printf("epoll_wait return %d\n", n);
        return;
    }
    for (int i = 0; i < n; i ++) {
        int fd = (int)(intptr_t)activeEvs[i].ident;

        int events = activeEvs[i].filter;
        if (events == EVFILT_READ) {
            if (fd == lfd) {
                clib_socket_kqueue_handle_accept(opdata, fd);
            } else {
                clib_socket_kqueue_handle_read(opdata, fd);
            }
        } else if (events == EVFILT_WRITE) {
            //clib_socket_handleWrite(opdata, fd);
        } else {
            printf_if(1, "unknown event");
            printf"unknown event");
        }
    }
}

void clib_socket_kqueue_update_events(void * efdarg, int fd, int events, int modify) {

    CLib_socket_handle_data * opdata  = (CLib_socket_handle_data *)efdarg;
    if(opdata == NULL) return;
    int efd = opdata->epollfd;
    if(opdata->sokcet_type!= SOKCET_KQUEUE)
        return;

    struct kevent ev[2];
    int n = 0;
    if (events & kReadEvent) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    } else if (modify){
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    }
    if (events & kWriteEvent) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    } else if (modify){
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    }
    printf("%s fd %d events read %d write %d\n",
            modify ? "mod" : "add", fd, events & kReadEvent, events & kWriteEvent);
    int r = kevent(efd, ev, n, NULL, 0, NULL);
    printf_if(r, "kevent failed ");

}


static void clib_kqueue_HandleRequest_exmple(void *req_arg)
{
    if(req_arg == NULL) return ;
    char  url[1024] = {0};
    CLib_socket_handle_data * arg  = (CLib_socket_handle_data *)req_arg; 

    if(opdata->sokcet_type!= SOKCET_KQUEUE)  
        return ;

    clib_http_url_from_header(arg->readbuf, url, sizeof(url));
    //if (0 == strncasecmp(url, "/Service.do?", strlen("/Service.do?"))) {
    clib_http_url_get_value(url, "Action", Action, sizeof(Action));
    if (!strcmp(Action, "startv1")) {             
        arg->cmd = CMD_STARTV1;
    } else if (!strcmp(Action, "start")) {
        arg->cmd = CMD_START;
    } else {
        arg->cmd = CMD_UNKNOW;
    }    
    //DoRequest(arg); //real work
    //
    char writebuf[1024] = {0};
    write(arg->sockfd,writebuf, 1024);
    close(arg->sockfd);
    free(arg);
    // }
}

static int clib_kqueue_test_get_callback_exmple(void *req_arg)
{

    int readret = 0;
    char url[2048]={0};
    char * p = NULL;
    char * p1= NULL;
    char c_0x[3] ={0};
    
    if(req_arg == NULL)
        return -1;
    CLib_socket_handle_data * data_ok  = (CLib_socket_handle_data *)req_arg;

    if(opdata->sokcet_type!= SOKCET_KQUEUE)  
        return -1;

    p = strstr(data_ok->readbuf, "\r\n\r\n");
    if(p == NULL) return 1;

    APP_LOG(LOG_DEBUG, "Socketfd:%d, request:%s", data_ok->sockfd, data_ok->readbuf);
    p = data_ok->readbuf;
    while(*p == ' ' || *p == '\t') p++;
    while(*p != ' ' && *p != '\t') p++;
    while(*p == ' ' || *p == '\t') p++;
    p1 = p;
    while(*p1 != ' ' && *p1 != '\t') p1++;
    if(p1 - p >= sizeof url){
        APP_LOG(LOG_DEBUG,"error goto exit");
        goto exit;
    }
    memcpy(url, p, p1 - p);
    url[p1-p] = '\0';
    while(NULL != (p = strstr(url, "%"))){
        c_0x[0] = *(p+1);
        c_0x[1] = *(p+2);
        c_0x[2] = '\0';
        *p = strtol(c_0x, NULL, 16);
        p++;
        while('\0' != *(p+2)){
            *p = *(p+2);
            p++;
        }
        *p = ' ';
        *(p+1) = ' ';
    }
    p = data_ok->readbuf;
    while(*p == ' ' || *p == '\t') p++;
    while(*p != ' ' && *p != '\t') p++;
    while(*p == ' ' || *p == '\t') p++;
    p1 = url;
    while(*p1 == '/' && *(p1+1) == '/') p1++;
    if(p1 != url){
        memmove(url, p1, strlen(p1));
        memset(url + strlen(p1), ' ', p1 - url);
    }
    memcpy(p, url, strlen(url));
    if(NULL != (p = strstr(url, " "))) *p = '\0';
    p = data_ok->readbuf;
    clib_kqueue_HandleRequest_exmple(data_ok);
    return -1;

exit: 
    close(data_ok->sockfd);
    free(data_ok);
    data_ok = NULL;

    return -1;
}


int clib_handle_request_kqueue(int listensocket,Clib_CallbackFuction getcallbackfunc,Clib_CallbackFuction  postcallbackfunc){

    CLib_socket_handle_data * opdata = calloc(1,sizeof(CLib_socket_handle_data));
    if(opdata  == NULL)
        return 0;
    opdata->epollfd = kqueue();
    opdata->sokcet_type = SOKCET_KQUEUE;
    opdata->event_update_callback = clib_socket_kqueue_update_events;
    if(getcallbackfunc)
    {
        opdata->data_get_callbackfunc = getcallbackfunc; 
    }else{
        opdata->data_get_callbackfunc = clib_kqueue_test_get_callback_exmple;
    }
    opdata->data_post_callbackfunc = postcallbackfunc;
    //clib_socket_set_non_block(listensocket);
    opdata->event_update_callback(opdata, listensocket, kReadEvent, 0);
    for (;;) { // actual  should register singn process func  when quit clear resourcer 
        clib_kqueue_loop_work_once(opdata, listensocket, 10000);
    }

    return 1;
}

#endif
