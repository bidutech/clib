#ifndef _CLIB_MPOOL_H
#define _CLIB_MPOOL_H

#ifdef  __cplusplus                                                                                                                                                                            
extern "C" {
#endif  


#include <stdint.h>
#include <pthread.h>

//#define MP_STAT_DBG

#define MAXINDEXSIZE    10000

typedef struct {
      uint64_t size;
      int64_t count;
      int next;
} mpstat_t ;

//Memory pool object.
typedef struct {
  
  uint64_t sum;
  
  int64_t count; /* Amount of elements in index array */
  int stat_idx[MAXINDEXSIZE];
  mpstat_t stat[MAXINDEXSIZE];
  
  pthread_mutex_t lock; //To support Multi-Thread access.
} mempool;

//Memory Pool Initialization Method.
int  mpinit (int maxpages, int maxindex);

//Cleanup Method.
void mpcleanup ();

//Allocate dynamic memory from pool.
void* mpcalloc (size_t size);

void  clib_mem_use_test();//how to use
void * clib_mem_malloc(long size,char * file,int line);
void   clib_mem_free(void * ptr,char * file,int line);
//Free memory.
void mpfree (void *addr);

#define malloc_proxy mpcalloc
#define free_proxy   mpfree

void mpstat_print ();

/*
 *mpstat_print  please repalce your log function to printlog
 * */
#define clib_mem_print() mpstat_print()
#define clib_mem_clean() mpcleanup()

#define clib_mem_init()  mpinit(0,0)
#define clib_malloc(size) clib_mem_malloc((size),__FILE__,__LINE__)
#define clib_free(ptr)   clib_mem_free((ptr),__FILE__,__LINE__)

#ifdef __cplusplus                                                                                                                                                                             
}                                                                                                                                                                                              
#endif 


#endif//_CLIB_MPOOL_H
