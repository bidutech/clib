/*************************************************************************
	> File Name: clib_kqueue.h
	> Author: shanhai2015
	> Mail: sshanhai2015
	> Created Time: 2017年02月22日 星期三 16时47分48秒
 ************************************************************************/


#ifndef CLIB_KQUEUE_H
#define CLIB_KQIEUE_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef OS_FREE_BSD

void clib_socket_kqueue_update_events(void * efdarg, int fd, int events, int modify);

int clib_handle_request_kqueue(int listensocket,Clib_CallbackFuction callbackfunc);

#endif

#ifdef __cplusplus
}
#endif

#endif
