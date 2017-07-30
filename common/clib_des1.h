/*********************************************************
 *  des.h
 *  用户使用des算法头文件
 *    
 *********************************************************/
 
 /*************************************************************************
	> File Name: clib_des.h
	> Author: li
	> Mail: li
	> Created Time: 2017年04月27日 星期四 11时05分42秒
 ************************************************************************/
#ifndef  CLIB_DES1_
#define  CLIB_DES1_

#ifdef __cplusplus
extern "C" {
#endif


int clib_des_file_decode(const char *pfile1, const char *pfile2);//pfile1 input 输入文件， decode func

int clib_des_file_encode(const char *pfile1, const char *pfile2);//pfile1 input输入文件， encode func


//ab\0defg

//用户使用的函数
int DesEnc(
        unsigned char *pInData,//明文
        int            nInDataLen,
        unsigned char *pOutData,//密文
        int           *pOutDataLen);

//用户使用函数des解密
int DesDec(
       unsigned char *pInData,//密文
       int            nInDataLen,
       unsigned char *pOutData,//明文
       int           *pOutDataLen);

int DesEnc_raw(
    unsigned char *pInData,
    int            nInDataLen,
    unsigned char *pOutData,
    int           *pOutDataLen);
//用户使用函数des解密
int DesDec_raw(
    unsigned char *pInData,
    int            nInDataLen,
    unsigned char *pOutData,
    int           *pOutDataLen);


#ifdef __cplusplus
}
#endif

#endif
