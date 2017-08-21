/*************************************************************************
	> File Name: clib_socket.h
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年08月19日 星期六 18时14分59秒
 ************************************************************************/

#ifndef CLIB_SOCKET_H
#define CLIB_SOCKET_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#define REQ_DATA_BUF   4096


#define printf_if(r, ...) if(r) {printf(__VA_ARGS__); printf("error no: %d error msg %s\n", errno, strerror(errno));}   


#define kReadEvent = 1;
#define kWriteEvent = 2;


typedef  int (*Clib_CallbackFuction)(void *req_arg);

typedef int (*Clib_Socket_Update_Event_Func)(void * efdarg, int fd, int events, int modify);

typedef enum {
    SOCKET_NON = 0,
    SOCKET_SELECT,
    SOKCET_EPOLL,
    SOKCET_KQUEUE
}CLIB_SOCKET_TYPE;


typedef enum {
        CMD_UNKNOW = 1,
        CMD_STARTV1,
        CMD_START,
        CMD_STOP,
        CMD_READ,
        CMD_SEEK,
        CMD_SPEED,
        CMD_PLAYEND,
        CMD_GETPID,
        CMD_GETVER,
        CMD_EXIT,
        CMD_INFO,
        CMD_PLAYBGN,
        CMD_LOGON,
        CMD_LOGOFF,
        CMD_INFORM,
        CMD_GETERROR,
        CMD_TRANSTYPE
} Clib_req_cmd;

typedef struct clib_pthread_arg {
        int  sockfd;
        char readbuf[REQ_DATA_BUF];
        int  readlen;
        Clib_req_cmd cmd; 
        struct clib_pthread_arg *next;

} Clib_req_arg;

typedef struct  socket_handle_data{
    int epollfd;
    CLIB_SOCKET_TYPE  socket_type;
    Clib_Socket_Update_Event_Func  event_update_callback;
    Clib_CallbackFuction data_get_callback;
    Clib_CallbackFuction data_post_callback;
} CLib_socket_handle_data;


int clib_test_callbackfunc(void *req_arg);


Clib_CallbackFuction  clib_common_get_callbackfunc(char op);


int clib_socket_create_listened_socket(char *ip,int port);

void clib_socket_set_non_block(int fd);

int  clib_socket_handle_accept(void *efdarg, int fd);

void clib_socket_handle_read(void * efdarg, int fd);

#ifdef __cplusplus
}
#endif

#endif

