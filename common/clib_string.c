//
// Created by root on 17-1-13.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "clib_string.h"

/*** 
 * *int atoi(char *nptr) - Convert string to long 
 * * 
 * *Purpose: 
 * * Converts ASCII string pointed to by nptr to binary. 
 * * Overflow is not detected. Because of this, we can just use 
 * * atol(). 
 * * 
 * *Entry: 
 * * nptr = ptr to string to convert 
 * * 
 * *Exit: 
 * * return int value of the string 
 * * 
 * *Exceptions: 
 * * None - overflow is not detected. 
 * * 
 * *******************************************************************************/

__int64 clib_string_atoi64(  const char *nptr)  
{  
    int c; /* current char */  
    __int64 total; /* current total */  
    int sign; /* if ''-'', then negative, otherwise positive */  

    /* skip whitespace */  
    while ( isspace((int)(unsigned char)*nptr) )  
        ++nptr;  

    c = (int)(unsigned char)*nptr++;  
    sign = c; /* save sign indication */  
    if (c == '-' || c == '+')  
        c = (int)(unsigned char)*nptr++; /* skip sign */  

    total = 0;  

    while (isdigit(c)) {  
        total = 10 * total + (c - '0'); /* accumulate digit */  
        c = (int)(unsigned char)*nptr++; /* get next char */  
    }  

    if (sign == '-')  
        return -total;  
    else  
        return total; /* return result, negated if necessary */  
}  

/*** 
 * *int atoi(char *nptr) - Convert string to long 
 * * 
 * *Purpose: 
 * * Converts ASCII string pointed to by nptr to binary. 
 * * Overflow is not detected. Because of this, we can just use 
 * * atol(). 
 * * 
 * *Entry: 
 * * nptr = ptr to string to convert 
 * * 
 * *Exit: 
 * * return int value of the string 
 * * 
 * *Exceptions: 
 * * None - overflow is not detected. 
 * * 
 * *******************************************************************************/  
  
int clib_string_atoi(const char *nptr)  
{  
    return (int)clib_string_atol(nptr);  
}  

/*** 
 * *long atol(char *nptr) - Convert string to long 
 * * 
 * *Purpose: 
 * * Converts ASCII string pointed to by nptr to binary. 
 * * Overflow is not detected. 
 * * 
 * *Entry: 
 * * nptr = ptr to string to convert 
 * * 
 * *Exit: 
 * * return long int value of the string 
 * * 
 * *Exceptions: 
 * * None - overflow is not detected. 
 * * 
 * *******************************************************************************/  
  
long  clib_string_atol(  const char *nptr )  
{  
    int c; /* current char */  
    long total; /* current total */  
    int sign; /* if ''-'', then negative, otherwise positive */  

    /* skip whitespace */  
    while ( isspace((int)(unsigned char)*nptr) )  
        ++nptr;  

    c = (int)(unsigned char)*nptr++;  
    sign = c; /* save sign indication */  
    if (c == '-' || c == '+')  
        c = (int)(unsigned char)*nptr++; /* skip sign */  

    total = 0;  

    while (isdigit(c)) {  
        total = 10 * total + (c - '0'); /* accumulate digit */  
        c = (int)(unsigned char)*nptr++; /* get next char */  
    }  

    if (sign == '-')  
        return -total;  
    else  
        return total; /* return result, negated if necessary */  
}  

char* clib_string_itoa(int value, char* string, int radix)
{  
    char tmp[33];  
    char* tp = tmp;  
    int i;  
    unsigned v;  
    int sign;  
    char* sp;  
    if (radix > 36 || radix <= 1)  
    {  

        return 0;  
    }  
    sign = (radix == 10 && value < 0);  
    if (sign)  
        v = -value;  
    else  
        v = (unsigned)value;  
    while (v || tp == tmp)  
    {  
        i = v % radix;  
        v = v / radix;  
        if (i < 10)  
            *tp++ = i+'0';  
        else  
            *tp++ = i + 'a' - 10;  
    }  
    if (string == 0)  
        string = (char*)malloc((tp-tmp)+sign+1);  
    sp = string;  
    if (sign)  
        *sp++ = '-';  
    while (tp > tmp)  
        *sp++ = *--tp;  
    *sp = 0;  
    return string;  
}  
int
string_up_to_lower(char *buf, int len)
{
  int i = 0;
  char dif = 'a' - 'A';

  for(; i < len; ++i)
      if(buf[i] >= 'A' && buf[i] <= 'Z')
          buf[i] += dif;
  return 0;
}

int
 clib_string_get_midstr_between_space(char *srcstr, char *dstbuf, size_t dstbufsize)
{

//char *httpheader="GET /Service.do?Action=startv1&srvlst=(null)&
//url='http://cdn.voole.com:3580/?data='test'' HTTP/1.1
//\r\nHost: 127.0.0.1:5658\r\nAccept: */*
//  \r\nRange: bytes=0-\r\nConnection: close";

    char * p, * p1;
    p = srcstr;
    while(*p == ' ' || *p == '\t') p++;
    while(*p != ' ' && *p != '\t') p++;
    while(*p == ' ' || *p == '\t') p++;
    p1 = p;
    while(*p1 != ' ' && *p1 != '\t') p1++;
    if(p1 - p + 1 < dstbufsize){
        memcpy(dstbuf, p, p1 - p);
        dstbuf[p1 - p] = '\0';
        return 0;
    }else{
        return -1;
    }
}
int
clib_string_bin_to_ascii(char *pbin, int binlen, uint8 *pascii, int *asclen)
{
    int i = 0, covind = 0;
    uint8   byte = 0;

    if (!pbin || binlen < 1) return -1;
    if (!pascii) return -2;

    for (i=0, covind=0; i<binlen; i++) {
        byte = ((pbin[i] >> 4) & 0x0F);
        if (byte < 10) pascii[covind++] = byte + '0';
        else if (byte < 16) pascii[covind++] = byte + 'A' - 10;
        else pascii[covind++] = '.';

        byte = (pbin[i] & 0x0F);
        if (byte < 10) pascii[covind++] = byte + '0';
        else if (byte < 16) pascii[covind++] = byte + 'A' - 10;
        else pascii[covind++] = '.';
    }

    if (asclen) *asclen = covind;
    return covind;
}

int
clib_string_ascii_to_bin(uint8 *pascii, int asciilen, uint8 *pbin, int *binlen)
{
    int i, conind;
    uint8  byte = 0;

    if (!pascii || asciilen < 1) return -1;

    for (i=0, conind=0; i<asciilen; i++) {
        byte <<= 4;
        if (pascii[i] >= 'a' && pascii[i] <= 'f')
            byte |= pascii[i] - 'a' + 10;
        else if (pascii[i] >= 'A' && pascii[i] <= 'F')
            byte |= pascii[i] - 'A' + 10;
        else if (pascii[i] >= '0' && pascii[i] <= '9')
            byte |= pascii[i] - '0';
        else
            return -100;

        if ((i+1)%2 == 0) {
            pbin[conind++] = byte;
            byte = 0;
        }
    }

    if (binlen) *binlen = conind;

    return conind;
}




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
int
clib_string_split(char ***dest, int *count, char *s_str, char **separator, int number_separators,
                  int compress_separator,
                  int keep_separator)
{
    int i = 0;
    char **result = NULL;
    char **temp_result = NULL;
    unsigned int curt_size = 0;
    unsigned int new_size = 0;
    char *look_ahead = NULL;
    char *most_front_separator_start = NULL;
    char *most_front_separator_end = NULL;
    char *separator_start = NULL;
    int find_a_separator = 0;
    int find_a_string = 0;

    *count = 0;
    *dest = NULL;

    /* check parameters */
    if (
            dest == NULL
            || s_str == NULL || *s_str == '\0'
            || separator == NULL
            || number_separators <= 0
            || compress_separator < 0
            || keep_separator < 0
            )
        return -1;

    for (i = 0; i < number_separators; i++)
        if (separator[i] == NULL || *separator[i] == '\0')
            return -1;

    for (look_ahead = s_str; *look_ahead != '\0'; look_ahead = most_front_separator_end)
    {
        most_front_separator_start = look_ahead + strlen(look_ahead);
        most_front_separator_end = look_ahead + strlen(look_ahead);
        find_a_separator = 0;

        /* find the next separator. */
        for (i = 0; i < number_separators; i++)
        {
            separator_start = strstr(look_ahead, separator[i]);
            if (separator_start == NULL)
                continue;

            find_a_separator = 1;
            /* update the most front separator. */
            if (separator_start < most_front_separator_start)
            {
                most_front_separator_start = separator_start;
                most_front_separator_end = most_front_separator_start + strlen(separator[i]);
            }
        }

        find_a_string = (look_ahead == most_front_separator_start) ? 0 : 1;

        /* allow put the new string into result if need. */
        new_size = (find_a_string > 0) ? (curt_size + 1) : ((compress_separator > 0) ? curt_size : (curt_size + 1));
        /* allow put the separator into result if need. */
        new_size = (keep_separator > 0) ? (new_size + 1) : new_size;
        if (new_size == curt_size)
            continue;

        temp_result = (char **)malloc((new_size) * sizeof(char *));
        if (temp_result == NULL)
        {
            if (result != NULL)
            {
                for (i = 0; i < curt_size; i++)
                    if (result[i] != NULL)
                        free(result[i]);
                free(result);
                result = NULL;
            }

            return -2;
        }

        /* copy the pointers of string find early. */
        memset(temp_result, 0, new_size);
        for (i = 0; i < curt_size; i++)
            temp_result[i] = result[i];

        if (find_a_string == 0)
        {
            if (compress_separator == 0)
            {
                temp_result[curt_size] = (char *)malloc(sizeof(char));
                if (temp_result[curt_size] == NULL)
                {
                    if (temp_result != NULL)
                    {
                        for (i = 0; i < curt_size; i++)
                            if (temp_result[i] != NULL)
                                free(temp_result[i]);
                        free(temp_result);
                        temp_result = NULL;
                    }

                    return -2;
                }
                memset(temp_result[curt_size], 0, 1);
            }
        }
        else
        {
            /* put the new string into result. */
            temp_result[curt_size] = (char *)malloc((most_front_separator_start - look_ahead + 1) * sizeof(char));
            if (temp_result[curt_size] == NULL)
            {
                if (temp_result != NULL)
                {
                    for (i = 0; i < curt_size; i++)
                        if (temp_result[i] != NULL)
                            free(temp_result[i]);
                    free(temp_result);
                    temp_result = NULL;
                }

                return -2;
            }
            memset(temp_result[curt_size], 0, most_front_separator_start - look_ahead + 1);
            strncpy(temp_result[curt_size], look_ahead, most_front_separator_start - look_ahead);
            temp_result[curt_size][most_front_separator_start - look_ahead] = '\0';
        }

        if (keep_separator > 0)
        {
            /* put the separator into result. */
            temp_result[new_size - 1] = (char *)malloc(most_front_separator_end - most_front_separator_start + 1);
            if (temp_result[new_size - 1] == NULL)
            {
                if (temp_result != NULL)
                {
                    for (i = 0; i < new_size - 1; i++)
                        if (temp_result[i] != NULL)
                            free(temp_result[i]);
                    free(temp_result);
                    temp_result = NULL;
                }

                return -2;
            }
            memset(temp_result[new_size - 1], 0, most_front_separator_end - most_front_separator_start + 1);
            strncpy(temp_result[new_size - 1], most_front_separator_start, most_front_separator_end - most_front_separator_start);
            temp_result[new_size - 1][most_front_separator_end - most_front_separator_start] = '\0';
        }

        /* update result. */
        free(result);
        result = temp_result;
        temp_result = NULL;
        curt_size = new_size;
    }

    *dest = result;
    *count = curt_size;

    return 0;
}

/*
 * 6 进制打印start
 有时候会需要将字符串转化为16进制字符串给用户，因为ASCII中有些字符，
当我们使用printf("%s",p_ch);输出时会杂乱无章，如果采用16进制，代码如下：
 this code come from internet please author email me.
 * */
int clib_string_to_hex(char *ch, char *hex)
{
    int high,low;
    int tmp = 0;
    if(ch == NULL || hex == NULL){
        return -1;
    }

    if(strlen(ch) == 0){
        return -2;
    }

    while(*ch){
        tmp = (int)*ch;
        high = tmp >> 4;
        low = tmp & 15;
        *hex++ = clib_string_value_to_hexchar(high); //先写高字节
        *hex++ = clib_string_value_to_hexchar(low); //其次写低字节
        ch++;
    }
    *hex = '\0';
    return 0;
}

int clib_string_hex_to_string(char *hex, char *ch)
{
    int high,low;
    int tmp = 0;
    if(hex == NULL || ch == NULL){
        return -1;
    }

    if(strlen(hex) %2 == 1){
        return -2;
    }

    while(*hex){
        high = clib_string_hexchar_tovalue(*hex);
        if(high < 0){
            *ch = '\0';
            return -3;
        }
        hex++; //指针移动到下一个字符上
        low = clib_string_hexchar_tovalue(*hex);
        if(low < 0){
            *ch = '\0';
            return -3;
        }
        tmp = (high << 4) + low;
        *ch++ = (char)tmp;
        hex++;
    }
    *ch = '\0';
    return 0;
}

int clib_string_hexchar_tovalue(const char ch){
    int result = 0;
    //获取16进制的高字节位数据
    if(ch >= '0' && ch <= '9'){
        result = (int)(ch - '0');
    }
    else if(ch >= 'a' && ch <= 'z'){
        result = (int)(ch - 'a') + 10;
    }
    else if(ch >= 'A' && ch <= 'Z'){
        result = (int)(ch - 'A') + 10;
    }
    else{
        result = -1;
    }
    return result;
}

char clib_string_value_to_hexchar(const int value)
{
    char result = '\0';
    if(value >= 0 && value <= 9){
        result = (char)(value + 48); //48为ascii编码的‘0’字符编码值
    }
    else if(value >= 10 && value <= 15){
        result = (char)(value - 10 + 65); //减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
    }
    else{
        ;
    }

    return result;
}

//如果字符含有不可打印字符 则把该字符打印16进制
void clib_string_print_hex(unsigned char *src, int srclen, char *des, int deslen)
{
    if(src==NULL || des ==NULL)
    {
        return;
    }
    if(srclen>(1024*1024*3-1))
    {
        return;
    }
    if(deslen > srclen){
        return;
    }
    char x[1024*1024*3]={0};
    int i=0;
    for(i=0;i<srclen;i++)
    {
        char tmp[10]={0};
        if(isprint(src[i]))
        {
            snprintf(tmp,8,"%c",src[i]);
            strcat(x,tmp);
        }else
        {
            snprintf(tmp,8,"(%X)",src[i]);
            strcat(x,tmp);
        }
    }
    printf("%s",x);
    memset(des,0,deslen);
    memcpy(des,x,strlen(x));
    return;
}

//找出子串第一次出现的位置
char *clib_string_strstr(char *str, char *substr) {
    char *p0;
    char *p1;
    char *p2;
    p1 = p0 = str;
    p2 = substr;
    int i = 0;
    int j = 0;
    int lensubstr = strlen(substr);
    int lensrc = strlen(str);
    while (1) {
        if (*p0 == *p2) {
            j++;
            p0++;
            p2++;
            if (j == lensubstr) {
                p0 = p1 + i;
                break;
            }
        } else {
            j = 0;
            i++;
            p0 = p1 + i;
            p2 = substr;
        }
        if (i == lensrc) {
            p0 = (char *) NULL;
            break;
        }
    }
    return p0;
}

char *clib_string_malloc(char **str, int n)//这只是一个函数内部修改指针本身的一个例子
{
    *str=(char*)malloc(n);
    return  *str;
}
