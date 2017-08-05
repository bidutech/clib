#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#include "clib_mpool.h"

#define MP_STAT_DBG

mempool *mp = NULL;

//Memory Pool Initialization Method.
int mpinit (int maxpages, int maxindex)
{
    int i = 0;
    mp = (mempool *) calloc(sizeof(mempool), sizeof(char));
    if (mp == NULL) {
        //LOG(LOG_LEVEL_ERROR, "Memory Pool Initialization - insufficient memory.\n");
       // tolog("Memory Pool Initialization - insufficient memory.\n");
        return -1;
    }
    
    pthread_mutex_init(&mp->lock, 0);
    for (i = 0; i < MAXINDEXSIZE - 1; i++)
          mp->stat[i].next = i + 1;
    return 0;
}

//Cleanup Method.
void mpcleanup ()
{
    if (mp == NULL)
        return;

    pthread_mutex_destroy(&mp->lock);
    free(mp);
    mp = NULL;
}

void mpstat(int size, int inc)
{
    if (size > 104857600)
    //    LOG(LOG_LEVEL_WARN, "bigsize - %lu:%d\n", size, inc);
    	printf("bigsize - %lu:%d\n", size, inc);
    if (inc > 0)
        __sync_add_and_fetch(&mp->sum, size);
    else
        __sync_sub_and_fetch(&mp->sum, size);

    //if (!log_enabled(LOG_LEVEL_DEBUG))
       // return;

    pthread_mutex_lock(&mp->lock);
    int k = size % MAXINDEXSIZE;
    int i = mp->stat_idx[k];
    int j = 0;

    if (i == 0)
    {
        i = mp->stat[0].next;
        if (i > 0)
        {
            mp->stat[0].next = mp->stat[i].next;
            mp->stat[i].next = 0;
            mp->stat_idx[k] = i;
        }
    }
    else
    {
        while (i > 0 && mp->stat[i].size != size)
        {
            j = i;
            i = mp->stat[i].next;
        }
        if (i == 0)
        {
            i = mp->stat[0].next;
            if (i > 0)
            {
                mp->stat[0].next = mp->stat[i].next;
                mp->stat[i].next = mp->stat[j].next;
                mp->stat[j].next = i;
            }
        }
    }
    if (i > 0)
    {
        mp->stat[i].size = size;
        if (inc > 0)
            mp->stat[i].count ++;
        else
            mp->stat[i].count --;
        if (mp->stat[i].count == 0)
        {
            mp->stat[i].size = 0;
            if (j == 0)
                mp->stat_idx[k] = mp->stat[i].next;
            else
                mp->stat[j].next = mp->stat[i].next;
            mp->stat[i].next = mp->stat[0].next;
            mp->stat[0].next = i;
        }
    }

    if (inc > 0)
        mp->count ++;
    else
        mp->count --;
    pthread_mutex_unlock(&mp->lock);
}

//Allocate dynamic memory from pool.
void* mpcalloc (size_t size)
{
#ifdef MP_STAT_DBG
    void *p = calloc(1, size + sizeof(int));
    if (p == NULL)
    {
        //LOG(LOG_LEVEL_ERROR, "mpalloc p NULL size %lu, sum %lu\n", size, mp->sum);
        //tolog("mpalloc p NULL size %lu, sum %lu\n", size, mp->sum);
        return p;
    }
    *(int *)p = (int)size;
    mpstat((int)size, 1);
    return ((char*)p + sizeof(int));
#else
    return calloc(1, size);
#endif
}

//Free memory.
void mpfree (void *addr)
{
#ifdef MP_STAT_DBG
    void *p = (char*)addr - sizeof(int);
    int size = *((int *)p);
    free(p);
    mpstat(size, -1);
#else
    free(addr);
#endif
    return;
}

void mpstat_print ()
{
#ifdef MP_STAT_DBG
    //LOG(LOG_LEVEL_NOTICE, "[mpstat] sum %llu \n", mp->sum);

    //if (!log_enabled(LOG_LEVEL_DEBUG))
        //return;

    mpstat_t stat2[MAXINDEXSIZE];
    int count = 0;
    uint64_t sum = 0;

    pthread_mutex_lock(&mp->lock);
    memcpy(stat2, mp->stat, sizeof(mp->stat));
    count = mp->count;
    sum = mp->sum;
    pthread_mutex_unlock(&mp->lock);

    int i = 0;
    for (i = 0; i < MAXINDEXSIZE; ++i)
    {
        if (stat2[i].size == 0 || stat2[i].count == 0)
            continue;
       // LOG(LOG_LEVEL_DEBUG, "[mpstat] i %d, size %d count %ld next %d\n", i, stat2[i].size, stat2[i].count, stat2[i].next);
    }
    //LOG(LOG_LEVEL_DEBUG, "[mpstat] count %d, sum %llu \n", count, sum);
#endif
}

void * clib_mem_malloc(long size,char * file,int line)
{
    void * ptr = NULL;

    if (size <= 0) {
        return NULL;
    }

#ifdef MP_STAT_DBG
    printf("size:%ld, line:%d, file:%s", size, line, file);//log
#endif
    return  mpcalloc(size);
}


void clib_mem_free(void * ptr,char * file,int line)
{
    if (ptr != NULL) {
#ifdef MP_STAT_DBG
 printf("ptr:%p, line:%d, file:%s", ptr, line, file);//log  
#endif
    mpfree(ptr);
    }
   return;
}

void  clib_mem_use_test()
{
#define MP_STAT_DBG

#ifdef MP_STAT_DBG
    clib_mem_init();
#endif

    char * pstr = NULL;
    pstr = clib_malloc(100);

    if(pstr){
        strcpy(pstr,"test_use_mem_tool");
        printf("p:%p,%s",pstr);
    }

    clib_free(pstr);
    pstr = NULL;
#ifdef MP_STAT_DBG
    clib_mem_clean();
#endif
}
