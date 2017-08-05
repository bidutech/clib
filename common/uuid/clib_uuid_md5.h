/*************************************************************************
	> File Name: md5.h
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年08月04日 星期五 09时26分35秒
 ************************************************************************/

#ifndef CLIB_UUID_MD5_H__
#define CLIB_UUID_MD5_H__

#ifdef  __cplusplus
extern "C" {
#endif


/* MD5.H - header file for MD5C.C */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.

   2007-09-15 Last modified by cheungmine.
 */

/* MD5 context. */
typedef struct {
    unsigned int state[4];                /* state (ABCD) */
    unsigned int count[2];                /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];    /* input buffer */
} MD5_CTX;

void  MD5_init (MD5_CTX *);
void  MD5_update (MD5_CTX *, const unsigned char *str, unsigned int len);
void  MD5_fini (unsigned char[16], MD5_CTX *);

char* MD5_sign (const unsigned char *str, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif    /* CLIB_UUID_MD5_H__ */
