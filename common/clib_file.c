#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clib_file.h>

int clib_file_current_mallocfullpath(char ** ppath)
{
  int pathlen=0;
  char *lpath=NULL;
  char link[1024]={0};
  char path[1024]={0};
  sprintf(link, "/proc/%d/exe", getpid());
  pathlen=readlink(link, path, sizeof(path));
  /*if(pathlen===错误处理===strlen(path))*/
  lpath=calloc(1,strlen(path)+1);
  strncpy(lpath,path,strlen(path));
  *ppath = lpath;
  return pathlen;
}



