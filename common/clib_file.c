#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clib_file.h>

int clib_file_current_path(char ** ppath)
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

int
clib_file_can_wr(char *path)
{
  char  file[256] = "";
  FILE *fp = NULL;
  if (!path) return 0;

  printf("test file mod:%s\n", path);
  memcpy(file, path, strlen(path));
  strcat(file, "xxxxtesttmp");
  fp = fopen(file, "ab+");
  if (!fp)
    return 0;
  remove(file);
  return 1;
}

