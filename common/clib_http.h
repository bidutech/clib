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


int clib_http_socket_available_1(int fd);
int clib_http_socket_available(int fd);

#ifdef __cplusplus
}
#endif

#endif
