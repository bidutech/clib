//
// Created by root on 17-1-13.
//

#include "clibstring.h"

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
strsplit(char ***dest, int *count, char *s_str, char **separator, int number_separators, int compress_separator, int keep_separator)
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
int strToHex(char *ch, char *hex)
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
        *hex++ = valueToHexCh(high); //先写高字节
        *hex++ = valueToHexCh(low); //其次写低字节
        ch++;
    }
    *hex = '\0';
    return 0;
}

int hexToStr(char *hex, char *ch)
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
        high = hexCharToValue(*hex);
        if(high < 0){
            *ch = '\0';
            return -3;
        }
        hex++; //指针移动到下一个字符上
        low = hexCharToValue(*hex);
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

int hexCharToValue(const char ch){
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

char valueToHexCh(const int value)
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
void printhex(unsigned char *src,int srclen,char * des,int deslen)
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

/*
 16 进制打印end
 * */
