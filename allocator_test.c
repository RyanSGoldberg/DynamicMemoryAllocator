#include <stdio.h>
#include <string.h>
#include "allocator.h"

#define a "HelloWorld"
#define b "FooBar"
#define c "ShalomOlam"



int main(int argc, char **argv){
    char *x = cust_malloc(strlen(a)+1);
    strncpy(x, a, strlen(a)+1);
    printf("%s\n", x);
    char *y = cust_malloc(strlen(b)+1);
    strncpy(y, b, strlen(b)+1);
    printf("%s\n", y);
    char *z = cust_malloc(strlen(c)+1);
    strncpy(z, c, strlen(c)+1);
    printf("%s\n", z);
    cust_free(x);
    cust_free(y);
}