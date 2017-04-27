#ifndef  CLIB_FILES_
#define  CLIB_FILES_
#ifdef __cplusplus
extern "C" {
#endif


int clib_file_current_path(char ** ppath);
int clib_file_can_wr(char *path);//test rigfht
unsigned long long clib_file_disk_freespace(char *filepath);
unsigned long long clib_file_disk_totalspace(char *filepath);
#ifdef __cplusplus
}
#endif

#endif
