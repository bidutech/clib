/*************************************************************************
	> File Name: clib_conf.c
	> Created Time: 2017年02月06日 星期一 11时16分49秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "clib_conf.h"

int clib_conf_ini_writeconf(const char *confpath, const char *key, const char *value,int fflag/*1: key="value",0:key=value*/)
{
#define CONF_LINE_MAX 1024
    char  tmppath[512], line[CONF_LINE_MAX], *begin;
    FILE *conf_fp, *tmp_fp;
    char  flag = 0;

    conf_fp = fopen(confpath, "r");
    if (!conf_fp) {
        return -1;
    }
    snprintf(tmppath, sizeof(tmppath), "%s.tmp", confpath);
    tmp_fp = fopen(tmppath, "w+");
    if (!tmp_fp) {
        fclose(conf_fp);
        return -2;
    }
    while (fgets(line, sizeof(line), conf_fp)) {
        begin = strstr(line, key);
        if (strstr(line, key)) {
            if(fflag){
                fprintf(tmp_fp, "%s = \"%s\"\n", key, value);
            }else{
                fprintf(tmp_fp, "%s = %s\n", key, value);
            }

            flag = 1;
        } else {
            fprintf(tmp_fp, "%s", line);
        }
    }
    if (0 == flag) {
        if(fflag){
            fprintf(tmp_fp, "\n%s = \"%s\"\n", key, value);
        }else{
            fprintf(tmp_fp, "\n%s = %s\n", key, value);
        }

    }
    fclose(conf_fp);
    fclose(tmp_fp);
    rename(tmppath, confpath);

    return 0;

}
char* clib_conf_parse_ini_param(const char* FileName, char *VariableName)
{
  #define LINE_DIM 256
    static char Str[LINE_DIM];
    char *VarName, *Comment=NULL, *Equal=NULL;
    char *FirstQuote, *LastQuote, *P1, *P2;
    int Line=0, Len=0, Pos=0;
    FILE *file=fopen(FileName, "r");

    if (file==NULL) {
        fprintf(stderr, "Error: Could not find file %s\n\n", FileName);
        return NULL;
    }

    while (fgets(Str, LINE_DIM-1, file) != NULL) {
        Line++;
        Len=strlen(Str);
        if (Len==0) goto Next;
        if (Str[Len-1]=='\n' || Str[Len-1]=='\r') Str[--Len]='\0';
        Equal = strchr (Str, '=');          // search for equal sign
        Pos = strcspn (Str, ";#!");         // search for comment
        Comment = (Pos==Len) ? NULL : Str+Pos;
        if (Equal==NULL || ( Comment!=NULL && Comment<=Equal)) goto Next;   // Only comment
        *Equal++ = '\0';
        if (Comment!=NULL) *Comment='\0';

        // String
        FirstQuote=strchr (Equal, '"');     // search for double quote char
        LastQuote=strrchr (Equal, '"');
        if (FirstQuote!=NULL) {
            if (LastQuote==NULL) {
                fprintf(stderr, "Error reading parameter file %s line %d - Missing end quote.\n", FileName, Line);
                goto Next;
            }
            *FirstQuote=*LastQuote='\0';
            Equal=FirstQuote+1;
        }

        // removes leading/trailing spaces
        Pos=strspn (Str, " \t");
        if (Pos==strlen(Str)) {
            fprintf(stderr, "Error reading parameter file %s line %d - Missing variable name.\n", FileName, Line);
            goto Next;      // No function name
        }
        while ((P1=strrchr(Str, ' '))!=NULL || (P2=strrchr(Str, '\t'))!=NULL)
            if (P1!=NULL) *P1='\0';
            else if (P2!=NULL) *P2='\0';
            VarName=Str+Pos;
            //while (strspn(VarName, " \t")==strlen(VarName)) VarName++;

            Pos=strspn (Equal, " \t");
            if (Pos==strlen(Equal)) {
                fprintf(stderr, "Error reading parameter file %s line %d - Missing value.\n", FileName, Line);
                goto Next;      // No function name
            }
            Equal+=Pos;

            if (strcmp(VarName, VariableName)==0) {     // Found it
                fclose(file);
                return Equal;
            }
Next:;
    }

    //fprintf(stderr, "Error reading parameter file %s - Variable %s not found.", FileName, VariableName);

    fclose(file);

    return NULL;
}
