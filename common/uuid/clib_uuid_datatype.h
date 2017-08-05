/*************************************************************************
	> File Name: cdatatype.h
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年08月04日 星期五 09时25分50秒
 ************************************************************************/

/* cdatatype.h 
   2008-09-15 Last created by cheungmine.
   All rights reserved by cheungmine.
*/
#ifndef CLIB_UUID_CDATATYPE_H__
#define CLIB_UUID_CDATATYPE_H__

#ifdef  __cplusplus
extern "C" {
#endif


/*============================================================================*/
typedef unsigned char uchar, byte, BYTE;

typedef unsigned short uint16, word_t, ushort;


//typedef unsigned int uint, uint32, dword_t size_t;
typedef unsigned int uint, uint32, dword_t ;

typedef unsigned long ulong;

//typedef __int64 int64;
typedef long long int64;
typedef unsigned long long  uint64, qword_t;


#ifndef BOOL
    #define BOOL  int
    #define TRUE  1
    #define FALSE 0
#endif

#ifndef RESULT
    #define RESULT  long
    #define SUCCESS        0
    #define ERROR        -1
#endif

#define SIZE_BYTE    1
#define SIZE_SHORT    2
#define SIZE_INT    4
#define SIZE_FLT    4
#define SIZE_DBL    8
#define SIZE_WORD    2
#define SIZE_DWORD    4
#define SIZE_QWORD    8
#define SIZE_LINT    8
#define SIZE_INT64    8
#define SIZE_UUID    16

/*============================================================================*/

#ifdef __cplusplus
}
#endif

#endif    /*CLIB_UUID_CDATATYPE_H__*/
