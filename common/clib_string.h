//
// Created by root on 17-1-13.
//

#ifndef CLIB_CLIBSTRING_H
#define CLIB_CLIBSTRING_H

#include <stdio.h>


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
 * 6 进制打印start
 有时候会需要将字符串转化为16进制字符串给用户，因为ASCII中有些字符，
当我们使用printf("%s",p_ch);输出时会杂乱无章，如果采用16进制，代码如下：
 this code come from internet please author email me.
 * */
int string_to_hex(char *ch, char *hex);
int string_hex_to_string(char *hex, char *ch);
int string_hexchar_tovalue(const char ch);
char string_value_to_hexchar(const int value);
//如果字符含有不可打印字符 则把该字符打印16进制
void string_print_hex(unsigned char *src, int srclen, char *des, int deslen);

/*
 16 进制打印end
 * */


#endif //CLIB_CLIBSTRING_H
