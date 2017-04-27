#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clib_file.h>
#include <sys/vfs.h>

int 
clib_file_current_path(char * ppath,int pathlen)
{
		if( pathlen <=  0){
			return 0;	
		}
		char *lpath=NULL;
		char link[1024]={0};
		char path[1024]={0};
		if(ppath == NULL)
			return 0;
		sprintf(link, "/proc/%d/exe", getpid());
		pathlen=readlink(link, path, sizeof(path));
		/*if(pathlen===错误处理===strlen(path))*/
		lpath=calloc(1,strlen(path)+1);
		strncpy(lpath,path,strlen(path));
		*ppath = lpath;
		
		strncpy(ppath,lpath,pathlen);
		ppath[pathlen]='\0';
		free(lpath);
		lpath = NULL;	
		return strlen(ppath);
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

unsigned long long
clib_file_disk_freespace(char *filepath)
{
		if(filepath == NULL){
				return 0;
		}
		struct statfs diskInfo;

		memset(&diskInfo, 0, sizeof(diskInfo));
		statfs(filepath, &diskInfo);
		unsigned long long blocksize = diskInfo.f_bsize;
		unsigned long long totalsize = blocksize * diskInfo.f_blocks;

		unsigned long long freesize = diskInfo.f_bfree * blocksize;
		unsigned long long availablesize = diskInfo.f_bavail * blocksize;
		return availablesize;

}

unsigned long long 
clib_file_disk_totalspace(char *filepath)
{
		if(filepath == NULL){
				return 0;
		}
		struct statfs diskInfo;

		memset(&diskInfo, 0, sizeof(diskInfo));
		statfs(filepath, &diskInfo);
		unsigned long long blocksize = diskInfo.f_bsize;
		unsigned long long totalsize = blocksize * diskInfo.f_blocks;

		unsigned long long freesize = diskInfo.f_bfree * blocksize;
		unsigned long long availablesize = diskInfo.f_bavail * blocksize;
		return totalsize;
}
