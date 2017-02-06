/*************************************************************************
	> File Name: clib_conf.h
	> Created Time: 2017年02月06日 星期一 11时16分38秒
 ************************************************************************/

#ifndef  CLIB_CONF_
#define  CLIB_CONF_
#ifdef  __cplusplus
extern "C" {
#endif


static char *TempPP = NULL;

int clib_conf_ini_writeconf(const char *confpath, const char *key, const char *value,int fflag/*1: key="value",0:key=value*/);

char* clib_conf_parse_ini_param(const char* conf_name, char *variable_name);//解析key="value"配置文件

#define CLIB_INI_ParseParamString(ParamFileName, ParamName, Str)  \
    if ((TempPP=clib_conf_parse_ini_param((ParamFileName), (ParamName)))!=NULL) \
          {memset(Str,0,sizeof(Str)); strcpy(Str, TempPP); }
//  else Str[0]='\0';


#define CLIB_INI_ParseParamInt(ParamFileName, ParamName, Int)  \
    if ((TempPP=clib_conf_parse_ini_param((ParamFileName), (ParamName)))!=NULL)    \
        Int=atoi(TempPP);


#define CLIB_INI_ParseParamHex(ParamFileName, ParamName, Int)\
    if ((TempPP=clib_conf_parse_ini_param((ParamFileName), (ParamName)))!=NULL) \
    Int=strtol(TempPP, NULL, 16);

#define CLIB_INI_ParseParamFloat(ParamFileName, ParamName, Flt) \
    if ((TempPP=clib_conf_parse_ini_param((ParamFileName), (ParamName)))!=NULL) \
    Flt=atof(TempPP);


#define CLIB_INI_ParseParamBool(ParamFileName, ParamName, B) \
    if ((TempPP=clib_conf_parse_ini_param((ParamFileName), (ParamName)))!=NULL) {\
    B=(toupper(TempPP[0])=='Y' || toupper(TempPP[0])=='T'|| TempPP[0]=='1'); else B=0; }

#ifdef __cplusplus
}
#endif
#endif
