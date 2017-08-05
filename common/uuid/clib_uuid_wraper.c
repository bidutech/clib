#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clib_uuid_md5.h"
#include "clib_uuid32.h"

char* clib_uuid_MD5_sign (const unsigned char *str, unsigned int len)
{
	
	return MD5_sign(str,len);
}

void clib_uuid_create(uuid_t* uuid)
{
	return uuid_create(uuid);
}

char *clib_uuid_to_string(const uuid_t* uuid)
{
	
	return uuid_to_string(uuid);
}

char *clib_uuid_create_string(void)
{
	return uuid_create_string();
}

void clib_uuid_create_external(const char *external, uuid_t* uuid)
{
	return clib_uuid_create_external(external,uuid);
}

void clib_uuid_to_timestamp(const uuid_t* uuid, timestamp_t* time)
{
	return uuid_to_timestamp(uuid,time);
}

char* clib_timestamp_to_string(const timestamp_t* time)
{
	
	return timestamp_to_string(time);
}

int clib_test_uuid()
{
    char *sign, *uid;
    uuid_t u, v, x;
    timestamp_t  t;

    sign = MD5_sign("hello world", (unsigned int)strlen("hello world"));
    printf("md5 string digit:%s\n", sign);

    uuid_create(&u);
    uid = uuid_to_string(&u);
    printf("uuid U to string:{%s}\n", uid);

    uuid_create(&v);
    uid = uuid_to_string(&v);
    printf("uuid V to string:{%s}\n", uid);

    printf("uuid compare U with V lexically:%d\n", uuid_compare(&u, &v));

    printf("uuid compare V with U temporally:%d\n", uuid_compare_time(&v, &u));


    uid = uuid_create_string();
    printf("new uuid string:{%s}\n", uid);

    uuid_create_external("cheungmine\n", &x);
    uid = uuid_to_string(&x);
    printf("new external uuid to string:{%s}\n", uid);

    uuid_to_timestamp(&u, &t);
    printf("timestamp_to_string:%s\n", timestamp_to_string(&t));
}
