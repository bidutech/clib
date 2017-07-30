/*********************************************************
 *  des.h
 *  �û�ʹ��des�㷨ͷ�ļ�
 *    
 *********************************************************/
 
 /*************************************************************************
	> File Name: clib_des.h
	> Author: li
	> Mail: li
	> Created Time: 2017��04��27�� ������ 11ʱ05��42��
 ************************************************************************/
#ifndef  CLIB_DES1_
#define  CLIB_DES1_

#ifdef __cplusplus
extern "C" {
#endif


int clib_des_file_decode(const char *pfile1, const char *pfile2);//pfile1 input �����ļ��� decode func

int clib_des_file_encode(const char *pfile1, const char *pfile2);//pfile1 input�����ļ��� encode func


//ab\0defg

//�û�ʹ�õĺ���
int DesEnc(
        unsigned char *pInData,//����
        int            nInDataLen,
        unsigned char *pOutData,//����
        int           *pOutDataLen);

//�û�ʹ�ú���des����
int DesDec(
       unsigned char *pInData,//����
       int            nInDataLen,
       unsigned char *pOutData,//����
       int           *pOutDataLen);

int DesEnc_raw(
    unsigned char *pInData,
    int            nInDataLen,
    unsigned char *pOutData,
    int           *pOutDataLen);
//�û�ʹ�ú���des����
int DesDec_raw(
    unsigned char *pInData,
    int            nInDataLen,
    unsigned char *pOutData,
    int           *pOutDataLen);


#ifdef __cplusplus
}
#endif

#endif
