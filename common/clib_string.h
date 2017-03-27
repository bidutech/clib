//
// Created by root on 17-1-13.
//

#ifndef CLIB_CLIBSTRING_H
#define CLIB_CLIBSTRING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "clib_btype.h"




int string_up_to_lower(char *buf, int len);

/**
 * the function strsplit comefrom internet the  author can email me
 * */
/**
 * Split a string into some strings according to a list of separators.
 *
 * @Param dest                      out: storage the strings has be split.
 * @Param count                     out: the number of strings has be split successfully, 0 for failed to split.
 * @Param s_str                     in:  the strings for split.
 * @Param separator                 in:  the list of split separators.
 * @Param number_separator          in:  the numbers of separators.
 * @Param compress_separator        in:  will be create a empty string when two split adjacent
 *                                       if compress_separator > 0 and not for compress_separator == 0
 * @Param keep_separator            in:  the separators will be put into parameter 'dest' if keep_separator > 0
 */
int string_split(char ***dest, int *count, char *s_str, char **separator, int number_separators, int compress_separator,
                 int keep_separator);

/*
 * 16 进制打印start
 有时候会需要将字符串转化为16进制字符串给用户，因为ASCII中有些字符，
当我们使用printf("%s",p_ch);输出时会杂乱无章，如果采用16进制，代码如下：
 this code come from internet please author email me.
 * */
int string_to_hex(char *ch, char *hex);

int string_hex_to_string(char *hex, char *ch);

int string_hexchar_tovalue(const char ch);

char string_value_to_hexchar(const int value);

void string_print_hex(unsigned char *src, int srclen, char *des, int deslen);//如果字符含有不可打印字符 则把该字符打印16进制


char *string_strstr(char * str, char *substr);//找出子串第一次出现的位置

char * string_malloc(char ** str,int n);//这只是一个函数内部修改指针本身的一个例子

int string_ascii_to_bin(uint8 *pascii, int asciilen, uint8 *pbin, int *binlen);

int string_bin_to_ascii(char *pbin, int binlen, uint8 *pascii, int *asclen);

int string_get_midstr_between_space(char *srcstr ,char * dstbuf, size_t dstbufsize);

#ifdef __cplusplus
}
#endif

#endif //CLIB_CLIBSTRING_H
