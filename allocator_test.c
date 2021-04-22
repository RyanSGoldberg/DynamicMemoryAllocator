#include <stdio.h>
#include <string.h>
#include "allocator.h"

int main(int argc, char **argv){
    char *x = cust_malloc(sizeof("Hello World"));
    strncpy(x, "Hello World", strlen("Hello World")+1);
    printf("%s\n", x);
    cust_free(x);
}