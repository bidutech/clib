//
// Created by root on 17-1-13.
//

#ifndef CLIB_CLIBSTRING_H
#define CLIB_CLIBSTRING_H

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * the function strsplit comefrom internet the  author can email me
 * */
int strsplit(char ***dest, int *count, char *s_str, char **separator, int number_separators, int compress_separator, int keep_separator);

/*
 * 6 进制打印start
 有时候会需要将字符串转化为16进制字符串给用户，因为ASCII中有些字符，
当我们使用printf("%s",p_ch);输出时会杂乱无章，如果采用16进制，代码如下：
 this code come from internet please author email me.
 * */
int strToHex(char *ch, char *hex);
int hexToStr(char *hex, char *ch);
int hexCharToValue(const char ch);
char valueToHexCh(const int value);
//如果字符含有不可打印字符 则把该字符打印16进制
void printhex(unsigned char *src,int srclen,char * des,int deslen);

/*
 16 进制打印end
 * */


#endif //CLIB_CLIBSTRING_H
