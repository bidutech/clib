
#ifndef CLIB_UUID32_WRAPPER
#define CLIB_UUID32_WRAPPER

#ifdef  __cplusplus
extern "C" {
#endif
#include "clib_uuid32.h"

char* clib_uuid_MD5_sign (const unsigned char *str, unsigned int len);
void clib_uuid_create(uuid_t* uuid);
char *clib_uuid_to_string(const uuid_t* uuid);
char *clib_uuid_create_string(void);
void clib_uuid_create_external(const char *external, uuid_t* uuid);
void clib_uuid_to_timestamp(const uuid_t* uuid, timestamp_t* time);
char* clib_timestamp_to_string(const timestamp_t* time);
int clib_test_uuid();

#ifdef __cplusplus
}
#endif
#endif

 
