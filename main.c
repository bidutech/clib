#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
//#include <uuid/uuid.h>
#include "stringtest.h"
#include "clib_uuid_wraper.h"



int main() {
    //cout << "Hello, World!" << endl;
    
    clib_test_uuid();

    printf("Hello, World!\n");
    teststrsplit();
    teststrprinthx();
    printhextest();
    timenow();
    test_file();
    test_conf();

    return 0;
}
