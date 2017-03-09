/*************************************************************************
	> File Name: clib_http_url.h
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年02月22日 星期三 17时12分29秒
 ************************************************************************/

#ifndef CLIB_HTTP_URL_H
#define CLIB_HTTP_URL_H
#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void clib_http_url_string_append(char *str, unsigned int len, const char *fmt, ...);

int  clib_http_url_get_value(const char *src, const char *varname, char *buf, int blen);

int clib_http_url_get_int(const char *src, const char *key, int *val);

int clib_http_url_get_uint(const char *src, const char *key, unsigned int *val);

int clib_http_url_uint64(const char *src, const char *key, uint64_t *val);

int clib_http_url_set_value(const char *varname, const char *varvalue, char *obj, int olen);

int clib_http_url_parse_xml(const char *xml, char *key, char *val, int size);

char *clib_http_url_parse_ip(uint32_t uip, char* buf, int len);

int clib_http_url_local_ip(char *buf, int len);

char *clib_http_url_strrstr(char *s1, char *s2);

char *clib_http_url_encode(const char *src, char *dest, size_t ulen);

char *clib_http_url_decode(char *str);

void clib_http_url_trim(char *buf);

int clib_http_url_from_header(char * httpheader, char * buf, size_t bufsize);

int clib_http_url_get_host_port(char *url,char *host,char *port);//host is ip or domain

#ifdef __cplusplus
}
#endif

#endif
